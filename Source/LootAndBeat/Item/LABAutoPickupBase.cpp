// Copyright JM, Inc. All Rights Reserved.


#include "LABAutoPickupBase.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LootAndBeat/Character/LABCharacterPlayer.h"

ALABAutoPickupBase::ALABAutoPickupBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>("Root Scene");
	SetRootComponent(RootScene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh");
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(StaticMesh);
	Sphere->SetAbsolute(false, false, true);
	
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Sphere->SetSphereRadius(PickupRange);
}

void ALABAutoPickupBase::BeginPlay()
{
	Super::BeginPlay();

	StaticMesh->SetSimulatePhysics(bSimulatePhysics);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ALABAutoPickupBase::OnSphereBeginOverlap);
}

void ALABAutoPickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bPicking)
		UpdatePickup(DeltaTime);
}

void ALABAutoPickupBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	
	Sphere->SetSphereRadius(PickupRange);
}

void ALABAutoPickupBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALABCharacterPlayer* PlayerCharacter = Cast<ALABCharacterPlayer>(OtherActor);
	if(!PlayerCharacter) return;
	
	bPicking = true;
	StaticMesh->SetSimulatePhysics(false);
}

void ALABAutoPickupBase::UpdatePickup(float DeltaTime)
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if(!PlayerPawn) return;
	
	FVector CurLocation = StaticMesh->GetComponentLocation();
	FVector TargetLocation = PlayerPawn->GetActorLocation();
	float CurDistance = (TargetLocation - CurLocation).Size();
	float TargetDistance = CurPickupSpeed * DeltaTime;
	if(TargetDistance >= CurDistance)
		EndPickup();

	FVector TargetDirection = (TargetLocation - CurLocation).GetSafeNormal();
	FVector NewLocation = CurLocation + TargetDirection * TargetDistance;
	StaticMesh->SetWorldLocation(NewLocation);

	CurPickupSpeed += AddPickupSpeed * DeltaTime;
}

void ALABAutoPickupBase::EndPickup()
{
	bPicking = false;
	this->Destroy();
}

