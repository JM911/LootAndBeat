// Fill out your copyright notice in the Description page of Project Settings.


#include "LABComboNotifyState.h"

#include "LootAndBeat/Character/LABCharacterPlayer.h"

void ULABComboNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       float TotalDuration)
{
	ALABCharacterPlayer* Player = Cast<ALABCharacterPlayer>(MeshComp->GetOwner());
	if(Player)
	{
		Player->SetComboEnable(true);
	}
}

void ULABComboNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	ALABCharacterPlayer* Player = Cast<ALABCharacterPlayer>(MeshComp->GetOwner());
	if(Player)
	{
		Player->SetComboEnable(false);
	}
}
