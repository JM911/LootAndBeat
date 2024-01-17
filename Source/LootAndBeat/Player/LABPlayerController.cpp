// Fill out your copyright notice in the Description page of Project Settings.


#include "LABPlayerController.h"

#include "EnhancedInputSubsystems.h"

void ALABPlayerController::BeginPlay()
{
	Super::BeginPlay();

	check(LABContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(LABContext, 0);
}
