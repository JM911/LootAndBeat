// Copyright JM, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "LABHit.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class ULABHit : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class LOOTANDBEAT_API ILABHit
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void LABApplyHit(AActor* Attacker, float DamageAmount, FVector ForceDirection);
};
