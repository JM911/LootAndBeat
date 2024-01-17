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
	//EBTNodeResult::Type Result = EBTNodeResult::Failed;

	UE_LOG(LogTemp, Warning, TEXT("Task Start"));
	
	//if(!LocationKey.IsSet()) return EBTNodeResult::Failed;
	//UE_LOG(LogTemp, Warning, TEXT("Location Key Found"));
	
	const APawn* OwnerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!OwnerPawn) return EBTNodeResult::Failed;
	UE_LOG(LogTemp, Warning, TEXT("Owner Pawn Found"));
	
	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem == nullptr) return EBTNodeResult::Failed;
	UE_LOG(LogTemp, Warning, TEXT("Nav System Found"));

	FNavLocation RandomLocation;
	if(NavSystem->GetRandomPointInNavigableRadius(OwnerPawn->GetActorLocation(), Radius,RandomLocation))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(LocationKey.SelectedKeyName, RandomLocation.Location);
		return EBTNodeResult::Succeeded;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Random Location Failed"));
	return EBTNodeResult::Failed;
}
