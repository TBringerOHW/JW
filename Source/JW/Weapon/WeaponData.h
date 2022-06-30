// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponData.generated.h"


USTRUCT(BlueprintType)
struct FWeaponVisual
{
	GENERATED_USTRUCT_BODY()

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Visual)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	UAnimMontage* FireAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Visual)
	FLinearColor WeaponColor;

};

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowBurst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ProjectilesAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootDelay;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Spread;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Projectile)
	TSubclassOf<class AJWProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Projectile)
	FWeaponVisual VisualParams;
	
};