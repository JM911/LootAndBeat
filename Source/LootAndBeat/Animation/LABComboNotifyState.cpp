// Fill out your copyright notice in the Description page of Project Settings.


#include "LABComboNotifyState.h"

#include "LootAndBeat/Character/LABCharacterPlayer.h"

void ULABComboNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if(ALABCharacterPlayer* Player = Cast<ALABCharacterPlayer>(MeshComp->GetOwner()))
	{
		Player->SetComboEnable(true);
	}
}

void ULABComboNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if(ALABCharacterPlayer* Player = Cast<ALABCharacterPlayer>(MeshComp->GetOwner()))
	{
		Player->SetComboEnable(false);
	}
}
