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
	
	InstancedPath = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedPath"));
	InstancedPath->SetupAttachment(GetRootComponent());

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
	InitPath();
	DoorDirectionSet.Empty();
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

	TArray<EAdjacentDirection> Directions = { EAdjacentDirection::UP, EAdjacentDirection::LEFT, EAdjacentDirection::DOWN, EAdjacentDirection::RIGHT };
	
	if(WallMesh)
	{
		for(auto dir : Directions)//int dir=0; dir<4; dir++)
		{
			// 문 생성 방향 (위치는 일단 가운데에 생성, 크기는 멤버 변수 고정 => 추후 변경 여지 있음)
			int DoorWidthStartIndex = -1;
			int DoorWidthEndIndex = -1;
			int DoorHeightEndIndex = -1;	// 항상 바닥부터 시작하므로 시작 인덱스는 필요 없음
			if(DoorDirectionSet.Contains(dir))
			{
				float DoorStartDist = (GetWidthLength(false) - DoorWidth) / 2.f;
				if(dir == EAdjacentDirection::LEFT || dir == EAdjacentDirection::RIGHT)
					DoorStartDist = (GetHeightLength(false) - DoorWidth) / 2.f;
				
				float DoorEndDist = DoorStartDist + DoorWidth;

				DoorWidthStartIndex = DoorStartDist / WallSize; 
				DoorWidthEndIndex = DoorEndDist / WallSize;
				DoorHeightEndIndex = DoorHeight / WallSize;
			}
			
			//const int WallLength = (dir % 2) == 0 ? FloorNumX : FloorNumY;
			int WallLength = FloorNumX;
			if(dir == EAdjacentDirection::UP || dir == EAdjacentDirection::DOWN)
				WallLength = FloorNumY;
			
			for(int i=0; i<WallLength; i++)
			{
				for(int j=0; j<WallHeight; j++)
				{
					// 문 위치라면 스킵 - TODO: 문 끝자락은 없애지 않고 스케일, 위치 오프셋 변경하도록 (문 크기 디테일하게 조정)
					if(i >= DoorWidthStartIndex && i <= DoorWidthEndIndex && j < DoorHeightEndIndex)
						continue;
					
					FVector InstanceLocation(0.f, 0.f, (j+1)*WallSize);
					switch(dir)
					{
					case EAdjacentDirection::LEFT:
						InstanceLocation.X = i * WallSize;
						InstanceLocation.Y = -1.f * WallSize;
						break;
					case EAdjacentDirection::UP:
						InstanceLocation.X = FloorNumX * WallSize;
						InstanceLocation.Y = i * WallSize;
						break;
					case EAdjacentDirection::RIGHT:
						InstanceLocation.X = i * WallSize;
						InstanceLocation.Y = FloorNumY * WallSize;
						break;
					case EAdjacentDirection::DOWN:
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

void ALABRoomBase::InitPath()
{
	InstancedPath->ClearInstances();
	InstancedPath->SetStaticMesh(PathMesh);
}

float ALABRoomBase::GetWidthLength(bool bWithWall)
{
	float ret = (float)FloorNumY * FloorSize;
	if(bWithWall)
		ret += WallSize;
	return ret;
}

float ALABRoomBase::GetHeightLength(bool bWithWall)
{
	float ret = (float)FloorNumX * FloorSize;
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

float ALABRoomBase::GetDistanceXWith(const ALABRoomBase* OtherRoom)
{
	const float diffX = FMath::Abs(CenterLocation.X - OtherRoom->CenterLocation.X);
	const float myX = (float)FloorNumX * FloorSize * 0.5f + WallSize;
	const float otherX = (float)(OtherRoom->FloorNumX) * (OtherRoom->FloorSize) * 0.5f + OtherRoom->WallSize;

	return diffX - myX - otherX;
}

float ALABRoomBase::GetDistanceYWith(const ALABRoomBase* OtherRoom)
{
	const float diffY = FMath::Abs(CenterLocation.Y - OtherRoom->CenterLocation.Y);
	const float myY = (float)FloorNumY * FloorSize * 0.5f + WallSize;
	const float otherY = (float)(OtherRoom->FloorNumY) * (OtherRoom->FloorSize) * 0.5f + OtherRoom->WallSize;

	return diffY - myY - otherY;
}

bool ALABRoomBase::IsCollideWith(const ALABRoomBase* OtherRoom)
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
		return (EAdjacentDirection)(((int)MaxOccupiedDirection + 2) % 4);

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

void ALABRoomBase::MakeDoor(EAdjacentDirection Direction)
{
	DoorDirectionSet.Add(Direction);
	RefreshWall();
}

void ALABRoomBase::MakePath(EAdjacentDirection Direction, float Length)
{
	if(Direction == EAdjacentDirection::NONE) return;
	
	float DistanceFromCenter = 0.f;
	FVector PathDirection(0.f);

	switch (Direction)
	{
	case EAdjacentDirection::UP:
		DistanceFromCenter = GetHeightLength(true) * 0.5f;
		PathDirection = FVector(1.f, 0.f, 0.f);
		break;
	case EAdjacentDirection::DOWN:
		DistanceFromCenter = GetHeightLength(true) * 0.5f;
		PathDirection = FVector(-1.f, 0.f, 0.f);
		break;
	case EAdjacentDirection::LEFT:
		DistanceFromCenter = GetWidthLength(true) * 0.5f;
		PathDirection = FVector(0.f, -1.f, 0.f);
		break;
	case EAdjacentDirection::RIGHT:
		DistanceFromCenter = GetWidthLength(true) * 0.5f;
		PathDirection = FVector(0.f, 1.f, 0.f);
		break;
	}

	FVector Offset = FVector(0.f, 0.f, -1.f);
	FVector PathStartLocation = CenterLocation + PathDirection * DistanceFromCenter + Offset;
	FVector PathEndLocation = PathStartLocation + PathDirection * Length;
	FVector PathRightDirection = FVector::CrossProduct(FVector::UpVector, PathDirection);
	

	// TODO: 정상구현 (현재 임시 표시용 바닥만 생성)
	if(PathSize <= 0.f) return;

	for(int i=-1; i<2; i++)
	{
		float CurLength = 0.f;
		FVector CurOffset = PathRightDirection * i * PathSize;
		UE_LOG(LogTemp, Warning, TEXT("CurOffset - %f, %f, %f"), CurOffset.X, CurOffset.Y, CurOffset.Z);
		while(CurLength < Length)
		{
			FVector InstanceLocation =  PathStartLocation + PathDirection * CurLength + CurOffset;
			InstanceLocation = GetActorTransform().InverseTransformPosition(InstanceLocation);
		
			FTransform InstanceTransform = FTransform::Identity;
			InstanceTransform.SetLocation(InstanceLocation);
			InstancedPath->AddInstance(InstanceTransform);
		
			CurLength += PathSize;
		}
	}
}

