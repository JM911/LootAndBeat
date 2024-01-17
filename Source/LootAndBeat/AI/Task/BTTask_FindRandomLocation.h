// Copyright JM, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindRandomLocation.generated.h"

/**
 * 
 */
UCLASS()
class LOOTANDBEAT_API UBTTask_FindRandomLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_FindRandomLocation();
	
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(Category = Node, EditAnywhere)
	FBlackboardKeySelector LocationKey;

	UPROPERTY(Category = Node, EditAnywhere)
	float Radius = 500.f;

	// TODO: 중심점을 Vector 또는 Object 로 고를 수 있게 변경 (현재는 주인 폰 기준)
	
};
