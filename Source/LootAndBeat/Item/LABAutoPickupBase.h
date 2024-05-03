// Copyright JM, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LABAutoPickupBase.generated.h"

class USphereComponent;

UCLASS()
class LOOTANDBEAT_API ALABAutoPickupBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ALABAutoPickupBase();
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void OnConstruction(const FTransform& Transform) override;

private:
	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void UpdatePickup(float DeltaTime);
	void EndPickup();

public:
	
private:
	TObjectPtr<USceneComponent> RootScene;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess="true"))
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess="true"))
	TObjectPtr<USphereComponent> Sphere;

	bool bPicking = false;
	
	UPROPERTY(EditAnywhere, Category = "Pickup", meta = (AllowPrivateAccess="true"))
	bool bSimulatePhysics = false;
	
	UPROPERTY(EditAnywhere, Category = "Pickup", meta = (AllowPrivateAccess="true"))
	float PickupRange = 100.f;

	UPROPERTY(EditAnywhere, Category = "Pickup", meta = (AllowPrivateAccess="true"))
	float CurPickupSpeed = 500.f;

	UPROPERTY(EditAnywhere, Category = "Pickup", meta = (AllowPrivateAccess="true"))
	float AddPickupSpeed = 500.f;
};
