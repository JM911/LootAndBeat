// Copyright Epic Games, Inc. All Rights Reserved.

#include "LootAndBeatGameMode.h"
#include "LootAndBeatCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALootAndBeatGameMode::ALootAndBeatGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
