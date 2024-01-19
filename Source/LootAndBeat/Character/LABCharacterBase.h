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

protected:
	virtual void Tick(float DeltaSeconds) override;

private:
	void TraceHitSphere();

	bool bHitTrace = false;
	FName HitSocketName = "";
	FVector LastHitSocketLocation = FVector(0.f, 0.f, 0.f);
	float HitTraceRadius = 100.f;
	TArray<AActor*> TraceIgnoreActors;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	bool bDebugTrace = false;

public:
	// Trace On/Off
	void SetHitTrace(bool bActive, FName TargetSocketName, float Radius);
	
	
	
};
