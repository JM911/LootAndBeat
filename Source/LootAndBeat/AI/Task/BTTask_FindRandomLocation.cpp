// Copyright JM, Inc. All Rights Reserved.


#include "BTTask_FindRandomLocation.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindRandomLocation::UBTTask_FindRandomLocation()
{
	NodeName = "Find Random Location";
}

EBTNodeResult::Type UBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//if(!LocationKey.IsSet()) return EBTNodeResult::Failed;
	//UE_LOG(LogTemp, Warning, TEXT("Location Key Found"));
	
	const APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!OwnerPawn) return EBTNodeResult::Failed;
	
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;

	FNavLocation RandomLocation;
	if(NavSystem->GetRandomPointInNavigableRadius(OwnerPawn->GetActorLocation(), Radius,RandomLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKey.SelectedKeyName, RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;
}
