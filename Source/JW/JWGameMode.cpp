// Copyright Epic Games, Inc. All Rights Reserved.

#include "JWGameMode.h"
#include "JWHUD.h"
#include "JWCharacter.h"
#include "UObject/ConstructorHelpers.h"

AJWGameMode::AJWGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AJWHUD::StaticClass();
}
