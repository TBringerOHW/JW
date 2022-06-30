// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Damageable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FReceivedDamageDeleagate, float, CurrentHealth, float, DamageAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEntityDeadDelegate, AActor*, Entity);


// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JW_API IDamageable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	float ReceiveDamage(float DamageAmount);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	float GetCurrentHealth();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	float GetMaximumHealth();

	FReceivedDamageDeleagate* GetOnDamageReceivedDelegate();

	FEntityDeadDelegate* GetOnDeadReceivedDelegate();
};
