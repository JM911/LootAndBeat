// Copyright JM, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LABRoomBase.generated.h"

UCLASS()
class LOOTANDBEAT_API ALABRoomBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALABRoomBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	void RefreshFloor();
	void RefreshWall();
	
	

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> RootScene;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInstancedStaticMeshComponent> InstancedFloor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInstancedStaticMeshComponent> InstancedWall;
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	float FloorSize = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	int FloorNumX = 5;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	int FloorNumY = 5;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMesh> FloorMesh;

	
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	float WallSize = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	int WallHeight = 5;
	
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMesh> WallMesh;
	
};
