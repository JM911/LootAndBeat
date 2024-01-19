// Copyright JM, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "LABHitTraceNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class LOOTANDBEAT_API ULABHitTraceNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

private:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
private:	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	FName HitSocketName = "";
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float Radius = 100.f;
};
