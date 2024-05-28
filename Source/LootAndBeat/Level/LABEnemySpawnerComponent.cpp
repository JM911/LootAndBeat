// Copyright JM, Inc. All Rights Reserved.


#include "LABEnemySpawnerComponent.h"

#include "../Character/LABCharacterEnemy.h"
#include "Components/CapsuleComponent.h"

// Sets default values for this component's properties
ULABEnemySpawnerComponent::ULABEnemySpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	
}

void ULABEnemySpawnerComponent::SpawnEnemy()
{
	if(!EnemyClass) return;

	ALABCharacterEnemy* EnemyCDO = EnemyClass->GetDefaultObject<ALABCharacterEnemy>();
	if(!EnemyCDO) return;

	float SpawnHeight = EnemyCDO->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FTransform SpawnTransform = GetComponentTransform();
	SpawnTransform.SetLocation(SpawnTransform.GetLocation() + FVector(0.f, 0.f, SpawnHeight));

	FActorSpawnParameters Param;
	Param.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	ALABCharacterEnemy* SpawnedEnemy = GetWorld()->SpawnActor<ALABCharacterEnemy>(EnemyClass, SpawnTransform, Param);
	
	if(SpawnedEnemy)
		SpawnedEnemy->OnDead.AddUObject(this, &ULABEnemySpawnerComponent::OnEnemyDeadEvent);	// TODO: 람다로 변경
}

void ULABEnemySpawnerComponent::OnEnemyDeadEvent()
{
	OnEnemyDead.Broadcast();
}


