// Fill out your copyright notice in the Description page of Project Settings.


#include "JWPlayerController.h"

#include "Damageable.h"

void AJWPlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*IDamageable* DamageablePawn = Cast<IDamageable>(GetPawn());
	DamageablePawn->GetOnDeadReceivedDelegate().AddDynamic();*/
}
