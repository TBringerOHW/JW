// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ABaseEnemy::ReceiveDamage_Implementation(float DamageAmount)
{
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	if (OnDamageReceived.IsBound()) OnDamageReceived.Broadcast(Health, DamageAmount);

	if (Health == 0.0f && OnEntityDead.IsBound())
	{
		OnEntityDead.Broadcast(this);
	}
	return Health;
}

float ABaseEnemy::GetCurrentHealth_Implementation()
{
	return Health;
}

float ABaseEnemy::GetMaximumHealth_Implementation()
{
	return MaxHealth;
}

