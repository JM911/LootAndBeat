// Copyright JM, Inc. All Rights Reserved.


#include "LABHitTraceNotifyState.h"

#include "LootAndBeat/Character/LABCharacterBase.h"


void ULABHitTraceNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if(ALABCharacterBase* Player = Cast<ALABCharacterBase>(MeshComp->GetOwner()))
	{
		Player->SetHitTrace(true, HitSocketName, Radius);
	}
}

void ULABHitTraceNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if(ALABCharacterBase* Player = Cast<ALABCharacterBase>(MeshComp->GetOwner()))
	{
		Player->SetHitTrace(false, HitSocketName, Radius);
	}
}
