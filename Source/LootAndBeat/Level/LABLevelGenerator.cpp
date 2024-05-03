// Copyright JM, Inc. All Rights Reserved.


#include "LABLevelGenerator.h"
#include "LABRoomBase.h"

ULABLevelGenerator::ULABLevelGenerator()
{
	InitRoomTree();
}

void ULABLevelGenerator::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void ULABLevelGenerator::Deinitialize()
{
	Super::Deinitialize();
}

void ULABLevelGenerator::GenerateRooms()
{
	/* 
	 * - 첫 방 생성 (기본 위치 0, 0, 0 ?)
	 * - 트리 타면서 다음 방 생성
	 *		- 이전 방 기준 4방향 중 위치 정해서 적당한 거리 두고 위치 설정
	 *		- 적당한 거리 = 방향, 방크기에 따라 달라야 하므로 메소드 따로 제작 (방 or 여기)
	 *		- 기존 방들 저장해놓고 겹치는지 확인 (확인 메서드는 방 클래스에?)
	 *		- 겹치지 않을 때까지 조금씩 더 멀리 잡아서 생성
	 */
	
	TArray<int> RoomIndexStack;
	RoomIndexStack.Push(0);

	while(!RoomIndexStack.IsEmpty())
	{
		int curIndex = RoomIndexStack.Top();
		RoomIndexStack.Pop();
		int ParentRoomIndex = RoomTree[curIndex].ParentRoomIndex;

		// 방 생성
		//ALABRoomBase* CurRoom = Cast<ALABRoomBase>(GetWorld()->SpawnActor(RoomTree[curIndex].RoomBP));	// Real
		// Test
		int randIdx = FMath::RandRange(0, TestRoomArray.Num() - 1);
		ALABRoomBase* CurRoomPrototype = Cast<ALABRoomBase>(GetWorld()->SpawnActor(TestRoomArray[randIdx].Class));
		// ~Test
		FVector CurCenterLocation(0.f, 0.f, 0.f);
		EAdjacentDirection GenerateDirectionFromParent = EAdjacentDirection::NONE;
		float PathLength = 200.f;

		// 위치 잡기
		if(ParentRoomIndex >= 0)	// 부모 방 존재할 때
		{
			// TODO: 이전 방을 트리에서 Parent 얻는 걸로 변경
			ALABRoomBase* ParentRoom = GeneratedRooms[ParentRoomIndex];

			// 이전 방 대비 
			GenerateDirectionFromParent = ParentRoom->GetNextAdjacentDirection();
			ParentRoom->SetAdjecentDirection(GenerateDirectionFromParent, true);
			FVector AddDirectionVector;
			float InitialAddDistance = 0.f;
			float InitialRoomDistance = 500.f;
			
			switch(GenerateDirectionFromParent)
			{
			case EAdjacentDirection::LEFT:
				AddDirectionVector = FVector(0.f, -1.f, 0.f);
				InitialAddDistance = (CurRoomPrototype->GetWidthLength(true) + ParentRoom->GetWidthLength(true)) * 0.5f + InitialRoomDistance; 
				break;
			case EAdjacentDirection::UP:
				AddDirectionVector = FVector(1.f, 0.f, 0.f);
				InitialAddDistance = (CurRoomPrototype->GetHeightLength(true) + ParentRoom->GetHeightLength(true)) * 0.5f + InitialRoomDistance;
				break;
			case EAdjacentDirection::RIGHT:
				AddDirectionVector = FVector(0.f, 1.f, 0.f);
				InitialAddDistance = (CurRoomPrototype->GetWidthLength(true) + ParentRoom->GetWidthLength(true)) * 0.5f + InitialRoomDistance;
				break;
			case EAdjacentDirection::DOWN:
				AddDirectionVector = FVector(-1.f, 0.f, 0.f);
				InitialAddDistance = (CurRoomPrototype->GetHeightLength(true) + ParentRoom->GetHeightLength(true)) * 0.5f + InitialRoomDistance;
				break;
			}

			CurCenterLocation = ParentRoom->GetCenterLocation() + AddDirectionVector * InitialAddDistance;

			// 무한 루프 주의
			while(true)
			{
				CurRoomPrototype->SetCenterLocation(CurCenterLocation);
				bool bPass = true;
				for(auto room : GeneratedRooms)
					bPass = bPass && !(room.Value->IsCollideWith(CurRoomPrototype));

				if(bPass) break;
				CurCenterLocation += AddDirectionVector * 50.f;	// TODO: 단위 길이 조정 (변수로 빼거나 등)
			}

			// 부모 방과 이어지는 통로 길이 측정
			if(GenerateDirectionFromParent == EAdjacentDirection::UP || GenerateDirectionFromParent == EAdjacentDirection::DOWN)
				PathLength += CurRoomPrototype->GetDistanceXWith(ParentRoom);
			else
				PathLength += CurRoomPrototype->GetDistanceYWith(ParentRoom);

			// 부모 방 문 생성
			ParentRoom->MakeDoor(GenerateDirectionFromParent);
		}
		else	// 부모 방 없을 때
		{
			// 일단 가만 두기
			CurRoomPrototype->SetCenterLocation(CurCenterLocation);
		}

		// Prototype 과 동일한 CurRoom 생성하고 Prototype은 파괴, GeneratedRooms 에 CurRoom 등록으로 변경
		ALABRoomBase* CurRoom = Cast<ALABRoomBase>(GetWorld()->SpawnActor(CurRoomPrototype->GetClass(), &CurRoomPrototype->GetActorTransform()));
		CurRoom->SetCenterLocation(CurCenterLocation);	// TODO: 변경 (수치만 되도록 하거나 등)

		// 통로, 문 생성
		if(GenerateDirectionFromParent != EAdjacentDirection::NONE)
		{
			EAdjacentDirection ParentDirectionFromChild = (EAdjacentDirection)(((int)GenerateDirectionFromParent + 2) % 4);
			CurRoom->SetAdjecentDirection(ParentDirectionFromChild, true);	// TODO: RoomBase 함수로 바꾸는 것 고려
			CurRoom->MakePath(ParentDirectionFromChild, PathLength);
			CurRoom->MakeDoor(ParentDirectionFromChild);
		}
		CurRoomPrototype->Destroy();
		GeneratedRooms.Add(curIndex, CurRoom);

		// TEST - 디버그 프린트 용 함수 (블루프린트 구현)
		CurRoom->SetRoomDebugTest(curIndex);
		// TEST - 방 연결 디버그 선 표시
		// if(ParentRoomIndex >= 0)
		// 	DrawDebugLine(GetWorld(), GeneratedRooms[ParentRoomIndex]->GetCenterLocation(), CurRoom->GetCenterLocation(), FColor::Red, true);
		
		for(auto elem : RoomTree[curIndex].ChildRoomIndices)
		{
			RoomIndexStack.Push(elem);
		}
	}
}

void ULABLevelGenerator::ClearRooms()
{
	for(auto room : GeneratedRooms)
	{
		if(IsValid(room.Value))
		{
			room.Value->Destroy();
		}
	}

	GeneratedRooms.Empty();

	// TEST - 방 연결 디버그 선 제거
	// FlushPersistentDebugLines(GetWorld());
}

void ULABLevelGenerator::GenerateRoomsDelayed()
{
	RoomIndexStack_Delayed.Push(0);

	GetWorld()->GetTimerManager().SetTimer(DelayedGenerateTimer, this, &ULABLevelGenerator::GenerateSingleRoomDelayed, 1.f, false);
}

void ULABLevelGenerator::GenerateSingleRoomDelayed()
{
	if(RoomIndexStack_Delayed.IsEmpty()) return;

	int curIndex = RoomIndexStack_Delayed.Top();
	RoomIndexStack_Delayed.Pop();
	int ParentRoomIndex = RoomTree[curIndex].ParentRoomIndex;

	// 방 생성
	//ALABRoomBase* CurRoom = Cast<ALABRoomBase>(GetWorld()->SpawnActor(RoomTree[curIndex].RoomBP));	// Real
	// Test
	int randIdx = FMath::RandRange(0, TestRoomArray.Num() - 1);
	ALABRoomBase* CurRoomPrototype = Cast<ALABRoomBase>(GetWorld()->SpawnActor(TestRoomArray[randIdx].Class));
	// ~Test
	FVector CurCenterLocation(0.f, 0.f, 0.f);
	EAdjacentDirection GenerateDirectionFromParent = EAdjacentDirection::NONE;
	float PathLength = 200.f;

	// 위치 잡기
	if(ParentRoomIndex >= 0)	// 부모 방 존재할 때
	{
		// TODO: 이전 방을 트리에서 Parent 얻는 걸로 변경
		ALABRoomBase* ParentRoom = GeneratedRooms[ParentRoomIndex];

		// 이전 방 대비 
		GenerateDirectionFromParent = ParentRoom->GetNextAdjacentDirection();
		ParentRoom->SetAdjecentDirection(GenerateDirectionFromParent, true);
		FVector AddDirectionVector;
		float InitialAddDistance = 0.f;
		float InitialRoomDistance = 500.f;
		
		switch(GenerateDirectionFromParent)
		{
		case EAdjacentDirection::LEFT:
			AddDirectionVector = FVector(0.f, -1.f, 0.f);
			InitialAddDistance = (CurRoomPrototype->GetWidthLength(true) + ParentRoom->GetWidthLength(true)) * 0.5f + InitialRoomDistance; 
			break;
		case EAdjacentDirection::UP:
			AddDirectionVector = FVector(1.f, 0.f, 0.f);
			InitialAddDistance = (CurRoomPrototype->GetHeightLength(true) + ParentRoom->GetHeightLength(true)) * 0.5f + InitialRoomDistance;
			break;
		case EAdjacentDirection::RIGHT:
			AddDirectionVector = FVector(0.f, 1.f, 0.f);
			InitialAddDistance = (CurRoomPrototype->GetWidthLength(true) + ParentRoom->GetWidthLength(true)) * 0.5f + InitialRoomDistance;
			break;
		case EAdjacentDirection::DOWN:
			AddDirectionVector = FVector(-1.f, 0.f, 0.f);
			InitialAddDistance = (CurRoomPrototype->GetHeightLength(true) + ParentRoom->GetHeightLength(true)) * 0.5f + InitialRoomDistance;
			break;
		}

		CurCenterLocation = ParentRoom->GetCenterLocation() + AddDirectionVector * InitialAddDistance;

		// 무한 루프 주의
		while(true)
		{
			CurRoomPrototype->SetCenterLocation(CurCenterLocation);
			bool bPass = true;
			for(auto room : GeneratedRooms)
				bPass = bPass && !(room.Value->IsCollideWith(CurRoomPrototype));

			if(bPass) break;
			CurCenterLocation += AddDirectionVector * 50.f;	// TODO: 단위 길이 조정 (변수로 빼거나 등)
		}

		// 부모 방과 이어지는 통로 길이 측정
		if(GenerateDirectionFromParent == EAdjacentDirection::UP || GenerateDirectionFromParent == EAdjacentDirection::DOWN)
			PathLength += CurRoomPrototype->GetDistanceXWith(ParentRoom);
		else
			PathLength += CurRoomPrototype->GetDistanceYWith(ParentRoom);

		// 부모 방 문 생성
		ParentRoom->MakeDoor(GenerateDirectionFromParent);
	}
	else	// 부모 방 없을 때
	{
		// 일단 가만 두기
		CurRoomPrototype->SetCenterLocation(CurCenterLocation);
	}

	// Prototype 과 동일한 CurRoom 생성하고 Prototype은 파괴, GeneratedRooms 에 CurRoom 등록으로 변경
	ALABRoomBase* CurRoom = Cast<ALABRoomBase>(GetWorld()->SpawnActor(CurRoomPrototype->GetClass(), &CurRoomPrototype->GetActorTransform()));
	CurRoom->SetCenterLocation(CurCenterLocation);	// TODO: 변경 (수치만 되도록 하거나 등)

	// 통로, 문 생성
	if(GenerateDirectionFromParent != EAdjacentDirection::NONE)
	{
		EAdjacentDirection ParentDirectionFromChild = (EAdjacentDirection)(((int)GenerateDirectionFromParent + 2) % 4);
		CurRoom->SetAdjecentDirection(ParentDirectionFromChild, true);	// TODO: RoomBase 함수로 바꾸는 것 고려
		CurRoom->MakePath(ParentDirectionFromChild, PathLength);
		CurRoom->MakeDoor(ParentDirectionFromChild);
	}
	CurRoomPrototype->Destroy();
	GeneratedRooms.Add(curIndex, CurRoom);

	// TEST - 디버그 프린트 용 함수 (블루프린트 구현)
	CurRoom->SetRoomDebugTest(curIndex);
	// TEST - 방 연결 디버그 선 표시
	// if(ParentRoomIndex >= 0)
	// 	DrawDebugLine(GetWorld(), GeneratedRooms[ParentRoomIndex]->GetCenterLocation(), CurRoom->GetCenterLocation(), FColor::Red, true);
	
	for(auto elem : RoomTree[curIndex].ChildRoomIndices)
	{
		RoomIndexStack_Delayed.Push(elem);
	}
	
	GetWorld()->GetTimerManager().SetTimer(DelayedGenerateTimer, this, &ULABLevelGenerator::GenerateSingleRoomDelayed, 1.f, false);
}

void ULABLevelGenerator::InitRoomTree()
{
	// TODO: 현재 Test 용 => 추후 데이터 에셋에서 가져와 초기회하는 것으로 변경
	const int RoomNum = 10;
	RoomTree.SetNum(RoomNum);
	const auto TestRoomFinder = ConstructorHelpers::FClassFinder<ALABRoomBase>(TEXT("/Script/Engine.Blueprint'/Game/_Project/Blueprint/Level/Room/Test/BP_RoomTest.BP_RoomTest_C'"));

	TestRoomArray.Add(ConstructorHelpers::FClassFinder<ALABRoomBase>(TEXT("/Script/Engine.Blueprint'/Game/_Project/Blueprint/Level/Room/Test/BP_RoomTest_1.BP_RoomTest_1_C'")));
	TestRoomArray.Add(ConstructorHelpers::FClassFinder<ALABRoomBase>(TEXT("/Script/Engine.Blueprint'/Game/_Project/Blueprint/Level/Room/Test/BP_RoomTest_2.BP_RoomTest_2_C'")));
	TestRoomArray.Add(ConstructorHelpers::FClassFinder<ALABRoomBase>(TEXT("/Script/Engine.Blueprint'/Game/_Project/Blueprint/Level/Room/Test/BP_RoomTest_3.BP_RoomTest_3_C'")));
	TestRoomArray.Add(ConstructorHelpers::FClassFinder<ALABRoomBase>(TEXT("/Script/Engine.Blueprint'/Game/_Project/Blueprint/Level/Room/Test/BP_RoomTest_4.BP_RoomTest_4_C'")));
	
	for(int i=0; i<RoomNum; i++)
	{
		RoomTree[i].RoomBP = TestRoomFinder.Class;
		// int randIdx = FMath::RandRange(0, TestRoomArray.Num() - 1);
		// RoomTree[i].RoomBP = TestRoomArray[randIdx].Class;
	}

	RoomTree[0].ChildRoomIndices.Push(1);
	RoomTree[0].ChildRoomIndices.Push(2);

	RoomTree[1].ParentRoomIndex = 0;

	RoomTree[2].ParentRoomIndex = 0;
	RoomTree[2].ChildRoomIndices.Push(3);
	RoomTree[2].ChildRoomIndices.Push(4);

	RoomTree[3].ParentRoomIndex = 2;
	
	RoomTree[4].ParentRoomIndex = 2;


	// 추가
	RoomTree[0].ChildRoomIndices.Push(5);
	RoomTree[4].ChildRoomIndices.Push(8);
	
	RoomTree[5].ParentRoomIndex = 0;
	RoomTree[5].ChildRoomIndices.Push(6);
	RoomTree[5].ChildRoomIndices.Push(7);

	RoomTree[6].ParentRoomIndex = 5;

	RoomTree[7].ParentRoomIndex = 5;
	RoomTree[7].ChildRoomIndices.Push(9);

	RoomTree[8].ParentRoomIndex = 4;

	RoomTree[9].ParentRoomIndex = 7;
}
