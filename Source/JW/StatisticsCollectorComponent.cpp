// Fill out your copyright notice in the Description page of Project Settings.


#include "StatisticsCollectorComponent.h"

#include "BaseEnemy.h"
#include "Damageable.h"

// Sets default values for this component's properties
UStatisticsCollectorComponent::UStatisticsCollectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatisticsCollectorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatisticsCollectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatisticsCollectorComponent::Initialize(TArray<AActor*> ActorsToTrack)
{
	for (AActor* Actor : ActorsToTrack)
	{
		ABaseEnemy* Damageable = Cast<ABaseEnemy>(Actor);
		if (!Damageable) continue;
		Damageable->OnDamageReceived.AddDynamic(this, &UStatisticsCollectorComponent::HandleDamageDealt);
		Damageable->OnEntityDead.AddDynamic(this, &UStatisticsCollectorComponent::HandleEntityDeath);
	}
}

void UStatisticsCollectorComponent::HandleDamageDealt_Implementation(float CurrentHealth, float DamageAmount)
{
	TotalDealtDamage += DamageAmount;
}

void UStatisticsCollectorComponent::HandleEntityDeath_Implementation(AActor* DeadActor)
{
	KilledEnemies++;
}

