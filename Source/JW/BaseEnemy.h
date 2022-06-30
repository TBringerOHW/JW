// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Damageable.h"
#include "GameFramework/Actor.h"
#include "BaseEnemy.generated.h"

UCLASS(Blueprintable, BlueprintType)
class JW_API ABaseEnemy : public AActor, public IDamageable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable)
	FReceivedDamageDeleagate OnDamageReceived;
	UPROPERTY(BlueprintAssignable)
	FEntityDeadDelegate OnEntityDead;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth;

	virtual float ReceiveDamage_Implementation(float DamageAmount) override;

	virtual float GetCurrentHealth_Implementation();

	virtual float GetMaximumHealth_Implementation();

	FReceivedDamageDeleagate* GetOnDamageReceivedDelegate();

	FEntityDeadDelegate* GetOnDeadReceivedDelegate();
};
