// Copyright JM, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LABLevelGenerator.generated.h"

class ALABRoomBase;

USTRUCT()
struct FRoomTreeNode
{
	GENERATED_BODY()

	int8 ParentRoomIndex = -1;
	TArray<int8> ChildRoomIndices;

	TSubclassOf<ALABRoomBase> RoomBP;
};

/**
 * 
 */
UCLASS()
class LOOTANDBEAT_API ULABLevelGenerator : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	ULABLevelGenerator();
	
	// USubsystem 시작
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	// USubsystem 끝

private:
	//void GenerateSingleRoom(int RoomIndex, );	// TODO: 추후 리팩토링

	UFUNCTION(BlueprintCallable)
	void GenerateRooms();

	UFUNCTION(BlueprintCallable)
	void ClearRooms();

	
private:
	// TODO: 방 트리 데이터
	TArray<FRoomTreeNode> RoomTree;

	void InitRoomTree();

	TMap<int, ALABRoomBase*> GeneratedRooms;

	// Test
	TArray<ConstructorHelpers::FClassFinder<ALABRoomBase>> TestRoomArray;
};
