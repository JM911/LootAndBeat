// Fill out your copyright notice in the Description page of Project Settings.


#include "LABCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"

DEFINE_LOG_CATEGORY(LogLABCharacter);

ALABCharacterBase::ALABCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터 공통 - 메쉬 Location & Rotation 초기 설정
	GetMesh()->SetRelativeLocation(FVector(0, 0, -GetCapsuleComponent()->GetUnscaledCapsuleHalfHeight()));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
}

void ALABCharacterBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// TODO: Trace 조건 체크 후 호출
}

void ALABCharacterBase::TraceHitSphere()
{
	FVector CurHitSocketLocation = GetMesh()->GetSocketLocation(HitSocketName);

	// TODO: Trace 채널 추가 후 완성
	//UKismetSystemLibrary::SphereTraceMulti(this, LastHitSocketLocation, CurHitSocketLocation, HitTraceRadius,
	//	)

	LastHitSocketLocation = CurHitSocketLocation;
}

void ALABCharacterBase::SetHitTrace(bool bActive, FName TargetSocketName, float Radius)
{
	// 소켓 없으면 종료
	if(!GetMesh()->DoesSocketExist(TargetSocketName)) return;

	bHitTrace = bActive;
	HitSocketName = TargetSocketName;
	HitTraceRadius = Radius;
	
	if(bHitTrace)
	{
		// On 시 이전 프레임 위치에 현재 위치 저장
		LastHitSocketLocation = GetMesh()->GetSocketLocation(HitSocketName);
	}
	else
	{
		// Off 시 이미 반응한 액터 리스트 초기화
		TraceIgnoreActors.Empty();
	}
}
