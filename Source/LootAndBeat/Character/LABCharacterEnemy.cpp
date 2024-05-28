// Fill out your copyright notice in the Description page of Project Settings.


#include "LABCharacterEnemy.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ALABCharacterEnemy::ALABCharacterEnemy()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 800.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void ALABCharacterEnemy::LABApplyHit(AActor* Attacker, float DamageAmount, FVector ForceDirection)
{
	// 1. 맞은 방향 바라보기
	const FVector LookAttackerDir = (Attacker->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	const FRotator LookAttackerRot = FRotationMatrix::MakeFromX(LookAttackerDir).Rotator();
	SetActorRotation(LookAttackerRot, ETeleportType::TeleportPhysics);

	// 2. Hit React 몽타주 재생
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if(HitReactMontage && AnimInstance)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		// TODO: 몽타주 동안 움직임 막았다 풀기 (AI or Character Movement)
	}	

	// TODO: 3. 데미지 적용
	// 임시
	TempHP -= 10.f;
	//UE_LOG(LogTemp, Warning, TEXT("Cur HP: %f"), TempHP);
	if(TempHP <= 0.f)
	{
		Die();
		// TODO: 랙돌 메쉬 날리기 => Add Force 등 잘 안됨... 조사 필요

		// FVector Force = ForceDirection * DieForce;
		// if(Force.IsNearlyZero())
		// 	Force = (Attacker->GetActorLocation() - GetActorLocation()).GetSafeNormal() * DieForce;
		// GetMesh()->AddImpulse(Force, "pelvis");
		//AddForce(Force, "pelvis");

		// Temp debug
		//UE_LOG(LogTemp, Warning, TEXT("%f - %f - %f"), Force.X, Force.Y, Force.Z);
		//DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation()+Force, 2.f, FColor::Red);
	}
}

void ALABCharacterEnemy::Die()
{
	// AI off
	AAIController* MyAIController = Cast<AAIController>(GetController());
	if(MyAIController)
	{
		MyAIController->BrainComponent->StopLogic("Dead");
	}

	// 랙돌
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->GravityScale = 0.f;

	// Destroy 타이머
	GetWorldTimerManager().SetTimer(DieDestroyTimer, this, &ALABCharacterEnemy::DestoryByDead, 5.f);

	// Delegate
	OnDead.Broadcast();
}

void ALABCharacterEnemy::DestoryByDead()
{
	Destroy();
}
