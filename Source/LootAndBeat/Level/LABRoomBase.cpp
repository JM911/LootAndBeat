// Copyright JM, Inc. All Rights Reserved.


#include "LABRoomBase.h"

#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
ALABRoomBase::ALABRoomBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bRunConstructionScriptOnDrag = false;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(RootScene);
	
	InstancedFloor = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedFloor"));
	InstancedFloor->SetupAttachment(GetRootComponent());
	
	InstancedWall = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedWall"));
	InstancedWall->SetupAttachment(GetRootComponent());

	const auto DefaultFloorMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("/Script/Engine.StaticMesh'/Game/_Project/Asset/Level/Mesh/SM_Floor_100x100.SM_Floor_100x100'"));
	if(DefaultFloorMesh.Succeeded())
	{
		FloorMesh = DefaultFloorMesh.Object;
	}
}

// Called when the game starts or when spawned
void ALABRoomBase::BeginPlay()
{
	Super::BeginPlay();

	InitRoom();
}

void ALABRoomBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	RefreshFloor();
	RefreshWall();
}

// Called every frame
void ALABRoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALABRoomBase::InitRoom()
{
	// Property Setting
	InitDirectionMap();
}

void ALABRoomBase::InitDirectionMap()
{
	OccupiedDirectionMap.Empty();
	OccupiedDirectionMap.Add(EAdjacentDirection::LEFT, 0);
	OccupiedDirectionMap.Add(EAdjacentDirection::UP, 0);
	OccupiedDirectionMap.Add(EAdjacentDirection::RIGHT, 0);
	OccupiedDirectionMap.Add(EAdjacentDirection::DOWN, 0);
}

void ALABRoomBase::RefreshFloor()
{
	InstancedFloor->ClearInstances();
	InstancedFloor->SetStaticMesh(FloorMesh);

	if(FloorMesh)
	{
		for(int i=0; i<FloorNumX; i++)
		{
			for(int j=0; j<FloorNumY; j++)
			{
				FTransform InstanceTransform = FTransform::Identity;
				InstanceTransform.SetLocation(FVector(i*FloorSize, j*FloorSize, 0.f));
				InstancedFloor->AddInstance(InstanceTransform);
			}
		}
	}
}

void ALABRoomBase::RefreshWall()
{
	InstancedWall->ClearInstances();
	InstancedWall->SetStaticMesh(WallMesh);

	if(WallMesh)
	{
		for(int dir=0; dir<4; dir++)
		{
			const int WallLength = (dir % 2) == 0 ? FloorNumX : FloorNumY;
			for(int i=0; i<WallLength; i++)
			{
				for(int j=0; j<WallHeight; j++)
				{
					FVector InstanceLocation(0.f, 0.f, (j+1)*WallSize);
					switch(dir)
					{
					case 0:
						InstanceLocation.X = i * WallSize;
						InstanceLocation.Y = -1.f * WallSize;
						break;
					case 1:
						InstanceLocation.X = FloorNumX * WallSize;
						InstanceLocation.Y = i * WallSize;
						break;
					case 2:
						InstanceLocation.X = i * WallSize;
						InstanceLocation.Y = FloorNumY * WallSize;
						break;
					case 3:
						InstanceLocation.X = -1.f * WallSize;
						InstanceLocation.Y = i * WallSize;
						break;
					}
					
					FTransform InstanceTransform = FTransform::Identity;
					InstanceTransform.SetLocation(InstanceLocation);
					InstancedWall->AddInstance(InstanceTransform);
				}
			}
		}
	}
}

float ALABRoomBase::GetWidthLength(bool bWithWall)
{
	float ret = (float)FloorNumX * FloorSize;
	if(bWithWall)
		ret += WallSize;
	return ret;
}

float ALABRoomBase::GetHeightLength(bool bWithWall)
{
	float ret = (float)FloorNumY * FloorSize;
	if(bWithWall)
		ret += WallSize;
	return ret;
}

FVector ALABRoomBase::GetCenterLocation() const
{
	return CenterLocation;
}

void ALABRoomBase::SetCenterLocation(FVector Location)
{
	CenterLocation = Location;
	float x = Location.X - ((float)FloorNumX * FloorSize * 0.5f);
	float y = Location.Y - ((float)FloorNumY * FloorSize * 0.5f);

	SetActorLocation(FVector(x, y, Location.Z));
}

bool ALABRoomBase::IsCollideWith(ALABRoomBase* OtherRoom)
{
	const float diffX = FMath::Abs(CenterLocation.X - OtherRoom->CenterLocation.X);
	const float diffY = FMath::Abs(CenterLocation.Y - OtherRoom->CenterLocation.Y);
	// TODO: 최소 간격만큼 벌어져있지 않으면 직접 닿지 않아도 충돌로 판정 => 값을 멤버 변수로 변경하거나 등...
	const float eps = 100.f;

	const float myX = (float)FloorNumX * FloorSize * 0.5f + WallSize;
	const float myY = (float)FloorNumY * FloorSize * 0.5f + WallSize;
	const float otherX = (float)(OtherRoom->FloorNumX) * (OtherRoom->FloorSize) * 0.5f + OtherRoom->WallSize;
	const float otherY = (float)(OtherRoom->FloorNumY) * (OtherRoom->FloorSize) * 0.5f + OtherRoom->WallSize;
	
	const bool bCollideX = diffX < (myX + otherX + eps);
	const bool bCollideY = diffY < (myY + otherY + eps);

	return bCollideX && bCollideY;
}

EAdjacentDirection ALABRoomBase::GetNextAdjacentDirection() const
{
	TArray<EAdjacentDirection> MinOccupiedDirections;
	EAdjacentDirection MaxOccupiedDirection = EAdjacentDirection::UP;

	int minDirectionNum = 99;
	int maxDirectionNum = -1;
	for(auto elem : OccupiedDirectionMap)
	{
		if(elem.Value == minDirectionNum)
			MinOccupiedDirections.Add(elem.Key);
		if(elem.Value < minDirectionNum)
		{
			minDirectionNum = elem.Value;
			MinOccupiedDirections.Empty();
			MinOccupiedDirections.Add(elem.Key);
		}
		if(elem.Value > maxDirectionNum)
		{
			maxDirectionNum = elem.Value;
			MaxOccupiedDirection = elem.Key;
		}
	}

	if(MinOccupiedDirections.Num() == 4 || MinOccupiedDirections.Num() == 2)
	{
		int randIdx = FMath::RandRange(0, MinOccupiedDirections.Num() - 1);
		return MinOccupiedDirections[randIdx];
	}

	if(MinOccupiedDirections.Num() == 3)
		return (EAdjacentDirection)(3 - (int)MaxOccupiedDirection);

	return MinOccupiedDirections.IsEmpty() ? EAdjacentDirection::UP : MinOccupiedDirections[0];
}

void ALABRoomBase::SetAdjecentDirection(EAdjacentDirection Direction, bool bOccupy)
{
	if(OccupiedDirectionMap.IsEmpty())
		InitDirectionMap();
	
	if(bOccupy)
		OccupiedDirectionMap[Direction]++;
	else
		OccupiedDirectionMap[Direction]--;
}

