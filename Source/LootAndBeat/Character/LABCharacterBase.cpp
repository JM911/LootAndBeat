// Fill out your copyright notice in the Description page of Project Settings.


#include "LABCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LootAndBeat/Interface/LABHit.h"

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
	
	if(bHitTrace)
	{
		TraceHitSphere();
	}
}

void ALABCharacterBase::TraceHitSphere()
{
	const FVector CurHitSocketLocation = GetMesh()->GetSocketLocation(HitSocketName);

	TArray<FHitResult> OutHitResults;
	const ETraceTypeQuery HitTraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_GameTraceChannel1);
	const EDrawDebugTrace::Type DebugTrace = bDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	
	UKismetSystemLibrary::SphereTraceMulti(this, LastHitSocketLocation, CurHitSocketLocation, HitTraceRadius,
		HitTraceType, false, TraceIgnoreActors, DebugTrace, OutHitResults, true);

	for(auto HitResult : OutHitResults)
	{
		if(AActor* CurActor = HitResult.GetActor())
		{
			// TOOD: 원래 트레이스 함수에서 적용되어야 함... 오작동 원인 찾고 제거
			if(TraceIgnoreActors.Contains(CurActor)) continue;
			
			if(ILABHit* CurHitActor = Cast<ILABHit>(CurActor))
			{
				// TODO: 데미지 전달
				CurHitActor->LABApplyHit(this, 0.f, (CurHitSocketLocation - LastHitSocketLocation).GetSafeNormal());
			}
			TraceIgnoreActors.Add(CurActor);
		}
	}

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
