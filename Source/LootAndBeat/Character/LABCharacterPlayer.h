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
	void AttackPunch(const FInputActionValue& InputActionValue);
	void AttackKick(const FInputActionValue& InputActionValue);

	UFUNCTION()
	void OnAttackPunchEnd(UAnimMontage* Montage, bool bInterrupted);
	UFUNCTION()
	void OnAttackKickEnd(UAnimMontage* Montage, bool bInterrupted);

public:
	void SetComboEnable(bool bEnable) { bComboEnable = bEnable; }

private:
	// Camera
	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	TObjectPtr<UCameraComponent> FollowCamera;

	// Input
	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> AttackPunchAction;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	TObjectPtr<UInputAction> AttackKickAction;

	// Animation
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackPunchMontage;
	
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> AttackKickMontage;

private:
	bool bComboEnable = false;
	bool bAttackPunch = false;
	bool bAttackKick = false;
	int ComboNumPunch = 1;
	int ComboNumKick = 1;
	
};
