// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LABCharacterBase.h"
#include "LootAndBeat/Interface/LABHit.h"
#include "LABCharacterEnemy.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDead);

/**
 * 
 */
UCLASS()
class LOOTANDBEAT_API ALABCharacterEnemy : public ALABCharacterBase, public ILABHit
{
	GENERATED_BODY()

public:
	ALABCharacterEnemy();

public:
	virtual void LABApplyHit(AActor* Attacker, float DamageAmount, FVector ForceDirection) override;

private:
	void Die();
	void DestoryByDead();
	FTimerHandle DieDestroyTimer;
	
	// TEMP
	float TempHP = 50.f;
	UPROPERTY(EditAnywhere)
	float DieForce = 10.f;
	
	// TODO: 배열로 변경하여 랜덤 재생 처리
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage> HitReactMontage;

public:
	FOnDead OnDead;
};
