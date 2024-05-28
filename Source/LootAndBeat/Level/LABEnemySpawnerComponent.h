// Copyright JM, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LABEnemySpawnerComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnEnemyDead);

class ALABCharacterEnemy;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LOOTANDBEAT_API ULABEnemySpawnerComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	ULABEnemySpawnerComponent();

private:

public:
	void SpawnEnemy();
	void OnEnemyDeadEvent();

private:
	// TArray<TSubclassOf<ALABCharacterEnemy>> EnemyClassList;
	UPROPERTY(EditDefaultsOnly, meta=(AllowPrivateAccess = "true"))
	TSubclassOf<ALABCharacterEnemy> EnemyClass;

	// TODO: 빌보드 변경
	
public:
	FOnEnemyDead OnEnemyDead;

	
		
};
