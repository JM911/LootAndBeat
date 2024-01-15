// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "LABCharacterBase.h"
#include "LABCharacterPlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputAction;

/**
 * 
 */
UCLASS()
class LOOTANDBEAT_API ALABCharacterPlayer : public ALABCharacterBase
{
	GENERATED_BODY()

public:
	ALABCharacterPlayer();
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Move(const FInputActionValue& InputActionValue);

private:
	// Camera
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> FollowCamera;

	// Input
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> MoveAction;
	
};
