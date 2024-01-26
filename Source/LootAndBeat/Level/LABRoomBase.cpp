// Copyright JM, Inc. All Rights Reserved.


#include "LABRoomBase.h"

#include "AsyncTreeDifferences.h"
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

