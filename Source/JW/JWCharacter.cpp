// Copyright Epic Games, Inc. All Rights Reserved.

#include "JWCharacter.h"

#include "BaseEnemy.h"
#include "JWGameMode.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AJWCharacter

AJWCharacter::AJWCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	WeaponRoot = CreateDefaultSubobject<USceneComponent>(TEXT("WeaponSocket"));
	WeaponRoot->SetupAttachment(Mesh1P);
}

void AJWCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	WeaponRoot->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	Mesh1P->SetHiddenInGame(true, true);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AJWCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AJWCharacter::OnFire);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AJWCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AJWCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AJWCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AJWCharacter::LookUpAtRate);
}

void AJWCharacter::OnFire()
{
	if (!CurrentWeapon)
		return;

	CurrentWeapon->Fire();

	// try and play a firing animation if specified
	if (CurrentWeapon && CurrentWeapon->WeaponParams.VisualParams.FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(CurrentWeapon->WeaponParams.VisualParams.FireAnimation, 1.f);
		}
	}
}

void AJWCharacter::ScrollWeapon(bool bScrollForward)
{
	if (Weapons.Num() <= 0)
		return;

	if (CurrentWeapon)
	{
		CurrentWeapon->SetActorHiddenInGame(true);
	}

	CurrentWeaponIndex = (bScrollForward ? CurrentWeaponIndex + 1 : CurrentWeaponIndex == 0 ? Weapons.Num() - 1 : CurrentWeaponIndex - 1) % Weapons.Num();
	CurrentWeapon = Weapons[CurrentWeaponIndex];
	
	CurrentWeapon->SetActorHiddenInGame(false);
	Mesh1P->SetHiddenInGame(false, false);
}

bool AJWCharacter::HandleWeaponPickup(AJW_BaseWeapon* Weapon)
{
	Weapon->ToggleWeaponPhysics(false);

	if (!Weapons.Contains(Weapon))
	{
		Weapons.Add(Weapon);
		Weapon->AttachToComponent(WeaponRoot, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false), FName(""));
		Weapon->SetActorRelativeLocation(FVector::ZeroVector);
		Weapon->SetActorHiddenInGame(true);
		return true;
	}
	return false;
}

void AJWCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AJWCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AJWCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AJWCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AJWCharacter::HandleCharacterDeath_Implementation()
{
	CurrentWeapon = nullptr;
	Mesh1P->SetHiddenInGame(true, false);

	AJWGameMode* GameMode = GetWorld()->GetAuthGameMode<AJWGameMode>();
	const auto PlayerStart = GameMode->K2_FindPlayerStart(Controller);
	SetActorTransform(PlayerStart->GetTransform());

	OnDamageReceived.Clear();
	OnEntityDead.Clear();

	Health = MaxHealth;
}


float AJWCharacter::ReceiveDamage_Implementation(float DamageAmount)
{
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	if (OnDamageReceived.IsBound()) OnDamageReceived.Broadcast(Health, DamageAmount);

	if (Health == 0.0f)
	{
		if (OnEntityDead.IsBound())
			OnEntityDead.Broadcast(this);
		HandleCharacterDeath();
	}

	return Health;
}

float AJWCharacter::GetCurrentHealth_Implementation()
{
	return Health;
}

float AJWCharacter::GetMaximumHealth_Implementation()
{
	return MaxHealth;
}
