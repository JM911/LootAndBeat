// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LABCharacterBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogLABCharacter, Log, All);

UCLASS()
class LOOTANDBEAT_API ALABCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ALABCharacterBase();
};
