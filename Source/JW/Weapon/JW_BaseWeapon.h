// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.h"
#include "GameFramework/Actor.h"
#include "JW_BaseWeapon.generated.h"

UCLASS()
class JW_API AJW_BaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AJW_BaseWeapon();

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent* WeaponMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Params)
	FWeaponData WeaponParams;
	

protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile();

	/** Fires a projectile. */
	UFUNCTION(BlueprintCallable)
	void Fire();

	/** Fires a projectile. */
	UFUNCTION(BlueprintCallable)
	void ToggleWeaponPhysics(bool bEnabled);

	UFUNCTION(BlueprintCallable)
	void InitWeapon(FWeaponData Params);

};
