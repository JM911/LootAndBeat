// Fill out your copyright notice in the Description page of Project Settings.


#include "LABEnemyAIController.h"

#include "BehaviorTree/BehaviorTree.h"

void ALABEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if(BehaviorTree)
	{
		BlackboardData = BehaviorTree->GetBlackboardAsset();
		RunBehaviorTree(BehaviorTree);
	}
}
