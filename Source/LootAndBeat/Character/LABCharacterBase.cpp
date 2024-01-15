// Fill out your copyright notice in the Description page of Project Settings.


#include "LABCharacterBase.h"

#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY(LogLABCharacter);

ALABCharacterBase::ALABCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// TODO: 캐릭터 메쉬 Location & Rotation 초기 설정 추가
	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
}
