// Copyright JM, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LABRoomBase.generated.h"

class UBoxComponent;

enum class EAdjacentDirection : uint8
{
	LEFT = 0,
	UP = 1,
	RIGHT = 2,
	DOWN = 3,
	NONE = 4,
};

UCLASS()
class LOOTANDBEAT_API ALABRoomBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ALABRoomBase();

protected:
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:	
	virtual void Tick(float DeltaTime) override;


private:
	void InitRoom();
	void InitDirectionMap();
	
	void RefreshFloor();
	void RefreshWall();

	void InitPath();

	void RefreshTrigger();
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnEnemyDead();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USceneComponent> RootScene;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInstancedStaticMeshComponent> InstancedFloor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInstancedStaticMeshComponent> InstancedWall;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UInstancedStaticMeshComponent> InstancedPath;
	
private:
	/* Setting */
	// Floor
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	float FloorSize = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	int FloorNumX = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	int FloorNumY = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMesh> FloorMesh;

	// Wall
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	float WallSize = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	int WallHeight = 5;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMesh> WallMesh;

	// Path
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	float PathSize = 100.f;
	UPROPERTY(EditDefaultsOnly, Category = "Setting", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMesh> PathMesh;

	// Trigger
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UBoxComponent> RoomTrigger;

	/* Property */
	// 방 바닥 중간지점의 월드 좌표 (해당 좌표 기반으로 위치 선정, 충돌 판정 등 수행)
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FVector CenterLocation = FVector(0.f, 0.f, 0.f);

	// 연결된 인접한 방이 있는 방향과 개수
	TMap<EAdjacentDirection, int> OccupiedDirectionMap;

	// 문 방향
	TSet<EAdjacentDirection> DoorDirectionSet;

	// 문 크기
	float DoorWidth = 250.f;
	float DoorHeight = 400.f;

	// 방 Enemy 개수
	int EnemyAliveCount = 0;

public:
	float GetWidthLength(bool bWithWall = false);
	float GetHeightLength(bool bWithWall = false);
	
	FVector GetCenterLocation() const;
	void SetCenterLocation(FVector Location);

	float GetDistanceXWith(const ALABRoomBase* OtherRoom);
	float GetDistanceYWith(const ALABRoomBase* OtherRoom);
	bool IsCollideWith(const ALABRoomBase* OtherRoom);

	EAdjacentDirection GetNextAdjacentDirection() const;
	void SetAdjecentDirection(EAdjacentDirection Direction, bool bOccupy);

	UFUNCTION(BlueprintImplementableEvent)
	void SetRoomDebugTest(int RoomIndex);

	void MakeDoor(EAdjacentDirection Direction);
	void MakePath(EAdjacentDirection Direction, float Length);
};
