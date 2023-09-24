// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_ThirdPersonCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "CharactersComp/ActionCombat.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"
#include "WOTF/Items/ItemBase.h"
#include "WOTF/Items/ItemInterface.h"
#include "WOTF/Items/Weapons/WeaponBase.h"
#include "WOTF/utils/FLogUtil.h"


//////////////////////////////////////////////////////////////////////////
// ATP_ThirdPersonCharacter


void ATP_ThirdPersonCharacter::CalculateAimOffset(float DeltaSeconds)
{
	if (GetCharacterMovement()->IsMovingOnGround() && GetCharacterMovement()->Velocity.Size2D() > 3.0f)
	{
		FRotator StartingAimRotator = FRotator(0, GetActorRotation().Yaw, 0);
		FRotator CurrentAimRotator = FRotator(0, GetBaseAimRotation().Yaw, 0);
		FRotator DeltaAimRotator =
			UKismetMathLibrary::NormalizedDeltaRotator(CurrentAimRotator, StartingAimRotator);
		AO_Yaw = DeltaAimRotator.Yaw;
	}
	else
	{
		AO_Yaw = 0.f;
	}
	AO_Pitch = GetBaseAimRotation().Pitch;
	if (AO_Pitch > 90.f && !IsLocallyControlled())
	{
		FVector2D InRange(270.f, 360.f);
		FVector2D OutRange(-90.f, 0.f);
		AO_Pitch = FMath::GetMappedRangeValueClamped(InRange, OutRange, AO_Pitch);
	}
	// FLogUtil::Warning(FString::Printf(TEXT("Yaw Rotation %f & Pitch Rotation %f"), AO_Yaw, AO_Pitch));
}

ATP_ThirdPersonCharacter::ATP_ThirdPersonCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	ActionCombat = CreateDefaultSubobject<UActionCombat>(TEXT("Action"));
	ActionCombat->SetIsReplicated(true);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}


void ATP_ThirdPersonCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	StartLineTraceForItems();
}

void ATP_ThirdPersonCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	DefaultCameraFov = GetFollowCamera()->FieldOfView;
	//Add Input Mapping Context
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ATP_ThirdPersonCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::Look);

		// Picking
		EnhancedInputComponent->BindAction(PickAction, ETriggerEvent::Triggered, this, &ATP_ThirdPersonCharacter::Pick);

		//Crouch
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this,
		                                   &ATP_ThirdPersonCharacter::CrouchPressed);

		// Aiming
		EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Triggered, this,
		                                   &ATP_ThirdPersonCharacter::AimingPressed);
	}
}

void ATP_ThirdPersonCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATP_ThirdPersonCharacter::Pick()
{
	if (ActionCombat)
	{
		if (OverlappedItemBase && OverlappedItemBase->GetWidgetVisibility())
		{
			ActionCombat->PickupItem(this, OverlappedItemBase);
		}
	}
}

void ATP_ThirdPersonCharacter::CrouchPressed()
{
	if (!GetCharacterMovement()->IsFalling())
	{
		if (bIsCrouched)
		{
			UnCrouch();
		}
		else
		{
			Crouch();
		}
	}
}

void ATP_ThirdPersonCharacter::AimingPressed()
{
	if (ActionCombat)
	{
		if (ActionCombat->GetIsAiming())
		{
			if (ActionCombat->GetEquipWeapon() && ActionCombat->GetEquipWeapon()->WeaponType != EWeaponType::Melee)
			{
				FollowCamera->SetFieldOfView(DefaultCameraFov);
			}
			ActionCombat->SetAiming(false);
		}
		else
		{
			if (ActionCombat->GetEquipWeapon() && ActionCombat->GetEquipWeapon()->WeaponType != EWeaponType::Melee)
			{
				FollowCamera->SetFieldOfView(ZoomCameraFov);
			}
			ActionCombat->SetAiming(true);
		}
	}
}

void ATP_ThirdPersonCharacter::EquippedWeapon(ACharacter* Character, AWeaponBase* Weapon)
{
}

void ATP_ThirdPersonCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}


void ATP_ThirdPersonCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ATP_ThirdPersonCharacter, OverlappedItemBase, COND_OwnerOnly);
}

void ATP_ThirdPersonCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (ActionCombat)
	{
		ActionCombat->OnEquipWeapon.AddDynamic(this, &ATP_ThirdPersonCharacter::EquippedWeapon);
	}
}

void ATP_ThirdPersonCharacter::StartLineTraceForItems()
{
	if (OverlappedItemBase && OverlappedItemBase->bCharacterCanStartLineTrace)
	{
		FHitResult OutHitResult;
		TraceUnderCrosshairs(OutHitResult);
		if (OutHitResult.bBlockingHit)
		{
			if (OutHitResult.GetActor()->GetClass()->ImplementsInterface(UItemInterface::StaticClass()))
			{
				// Show the widget if it's a valid item
				if (LastHitItemBase != nullptr && LastHitItemBase != OutHitResult.GetActor())
				{
					// Hide the previous widget if it exists
					if (LastHitItemBase != nullptr)
					{
						IItemInterface::Execute_ToggleVisibilityOfItemPickupWidget(LastHitItemBase, false);
					}
					// Show the widget for the new item
					LastHitItemBase = OutHitResult.GetActor();
					IItemInterface::Execute_ToggleVisibilityOfItemPickupWidget(OutHitResult.GetActor(), true);
				}
				else
				{
					LastHitItemBase = OutHitResult.GetActor();
					IItemInterface::Execute_ToggleVisibilityOfItemPickupWidget(OutHitResult.GetActor(), true);
				}
			}
			else
			{
				if (LastHitItemBase != nullptr)
				{
					IItemInterface::Execute_ToggleVisibilityOfItemPickupWidget(LastHitItemBase, false);
					LastHitItemBase = nullptr;
				}
			}
		}
		else
		{
			if (LastHitItemBase != nullptr)
			{
				IItemInterface::Execute_ToggleVisibilityOfItemPickupWidget(LastHitItemBase, false);
				LastHitItemBase = nullptr;
			}
		}
	}
	else
	{
		if (LastHitItemBase != nullptr)
		{
			IItemInterface::Execute_ToggleVisibilityOfItemPickupWidget(LastHitItemBase, false);
			LastHitItemBase = nullptr;
		}
	}
}

void ATP_ThirdPersonCharacter::SetOverlappedItemBase_Implementation(AItemBase* ItemBase)
{
	/* Clear any data previously*/
	if (OverlappedItemBase && IsLocallyControlled() && OverlappedItemBase->bCharacterCanStartLineTrace)
	{
		OverlappedItemBase->bCharacterCanStartLineTrace = false;
	}
	this->OverlappedItemBase = ItemBase;
	if (OverlappedItemBase && IsLocallyControlled())
	{
		OverlappedItemBase->bCharacterCanStartLineTrace = true;
	}
}

bool ATP_ThirdPersonCharacter::TraceUnderCrosshairs(FHitResult& OutHitResult)
{
	/* Get ViewPort Size */
	FVector2D ViewPortSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewPortSize);
	}

	/* Get Screen space location of cross hairs which is usually a center of screen for most of the games*/
	const FVector2D CrosshairLocation(ViewPortSize.X / 2.f, ViewPortSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	/* Get World position and direction of cross hairs */
	if (UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this, 0),
	                                             CrosshairLocation,
	                                             CrosshairWorldPosition,
	                                             CrosshairWorldDirection))
	{
		/* Trace from cross hairs world location outward */
		const FVector Start = {CrosshairWorldPosition};
		const FVector End = {Start + CrosshairWorldDirection * 50000.f};
		GetWorld()->LineTraceSingleByChannel(OutHitResult, Start, End, ECC_Visibility);
		if (OutHitResult.bBlockingHit)
		{
			// If hit occurred, draw the line in green
			// DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);
			return true;
		}
	}
	return false;
}

void ATP_ThirdPersonCharacter::OnRep_OverlappedItemBase(AItemBase* PrevValue) const
{
	if (PrevValue && PrevValue->bCharacterCanStartLineTrace)
	{
		PrevValue->bCharacterCanStartLineTrace = false;
	}
	if (OverlappedItemBase)
	{
		OverlappedItemBase->bCharacterCanStartLineTrace = true;
	}
}
