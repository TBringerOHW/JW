// Fill out your copyright notice in the Description page of Project Settings.


#include "JW_BaseWeapon.h"

#include "JWProjectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AJW_BaseWeapon::AJW_BaseWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	this->SetRootComponent(WeaponMesh);
	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
}

// Called when the game starts or when spawned
void AJW_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AJW_BaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJW_BaseWeapon::SpawnProjectile()
{
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);//
			const FRotator SpawnRotation = PC->PlayerCameraManager->GetCameraRotation() + FRotator(FMath::FRandRange(-Spread, Spread), FMath::FRandRange(-Spread, Spread), FMath::FRandRange(-Spread, Spread));

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			// spawn the projectile at the muzzle
			World->SpawnActor<AJWProjectile>(ProjectileClass, FP_MuzzleLocation->GetComponentLocation(), SpawnRotation, ActorSpawnParams);
		}
	}
}

void AJW_BaseWeapon::Fire()
{
	// try and fire a projectile
	for (int ProjectileNumber = 0; ProjectileNumber < ProjectileAmount; ProjectileNumber++)
		SpawnProjectile();

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}
}

void AJW_BaseWeapon::ToggleWeaponPhysics(bool bEnabled)
{
	WeaponMesh->SetSimulatePhysics(bEnabled);
	WeaponMesh->SetCollisionEnabled(bEnabled ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}
