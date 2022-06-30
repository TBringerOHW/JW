// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JWPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class JW_API AJWPlayerController : public APlayerController
{
	GENERATED_BODY()

	virtual void BeginPlay() override;
};
