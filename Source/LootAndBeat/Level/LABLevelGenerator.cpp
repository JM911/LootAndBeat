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
	/* TODO
	 * - 첫 방 생성 (기본 위치 0, 0, 0 ?)
	 * - 트리 타면서 다음 방 생성
	 *		- 이전 방 기준 4방향 중 위치 정해서 적당한 거리 두고 위치 설정
	 *		- 적당한 거리 = 방향, 방크기에 따라 달라야 하므로 메소드 따로 제작 (방 or 여기)
	 *		- 기존 방들 저장해놓고 겹치는지 확인 (확인 메서드는 방 클래스에?)
	 *		- 겹치지 않을 때까지 조금씩 더 멀리 잡아서 생성
	 */
	
	TArray<int> RoomIndexStack, GeneratedRoomIndexStack;
	RoomIndexStack.Push(0);

	while(!RoomIndexStack.IsEmpty())
	{
		int curIndex = RoomIndexStack.Top();
		RoomIndexStack.Pop();
		int ParentRoomIndex = RoomTree[curIndex].ParentRoomIndex;

		// 방 생성
		ALABRoomBase* CurRoom = Cast<ALABRoomBase>(GetWorld()->SpawnActor(RoomTree[curIndex].RoomBP));
		FVector GenerateLocation(0.f, 0.f, 0.f);

		// 위치 잡기
		if(ParentRoomIndex >= 0)	// 부모 방 존재할 때
		{
			// TODO: 이전 방을 트리에서 Parent 얻는 걸로 변경
			ALABRoomBase* ParentRoom = GeneratedRooms[ParentRoomIndex];

			// 이전 방 대비 
			EAdjacentDirection GenerateDirection = ParentRoom->GetNextAdjacentDirection();
			ParentRoom->SetAdjecentDirection(GenerateDirection, true);
			CurRoom->SetAdjecentDirection((EAdjacentDirection)(3 - (int)GenerateDirection), true);	// TODO: RoomBase 함수로 바꾸는 것 고려
			FVector AddDirectionVector;
			float InitialAddDistance = 0.f;
			float InitialRoomDistance = 50.f;
			
			switch(GenerateDirection)
			{
			case EAdjacentDirection::LEFT:
				AddDirectionVector = FVector(-1.f, 0.f, 0.f);
				InitialAddDistance = CurRoom->GetWidthLength(true) + ParentRoom->GetWidthLength(true) + InitialRoomDistance; 
				break;
			case EAdjacentDirection::UP:
				AddDirectionVector = FVector(0.f, 1.f, 0.f);
				InitialAddDistance = CurRoom->GetHeightLength(true) + ParentRoom->GetHeightLength(true) + InitialRoomDistance;
				break;
			case EAdjacentDirection::RIGHT:
				AddDirectionVector = FVector(1.f, 0.f, 0.f);
				InitialAddDistance = CurRoom->GetWidthLength(true) + ParentRoom->GetWidthLength(true) + InitialRoomDistance;
				break;
			case EAdjacentDirection::DOWN:
				AddDirectionVector = FVector(0.f, -1.f, 0.f);
				InitialAddDistance = CurRoom->GetHeightLength(true) + ParentRoom->GetHeightLength(true) + InitialRoomDistance;
				break;
			}

			GenerateLocation = ParentRoom->GetCenterLocation() + AddDirectionVector * InitialAddDistance;

			// 무한 루프 주의
			while(true)
			{
				CurRoom->SetCenterLocation(GenerateLocation);
				bool bPass = true;
				for(auto room : GeneratedRooms)
					bPass = bPass && !(room.Value->IsCollideWith(CurRoom));

				if(bPass) break;
				GenerateLocation += AddDirectionVector * 50.f;	// TODO: 단위 길이 조정 (변수로 빼거나 등)
			}
		}
		else	// 부모 방 없을 때
		{
			// 일단 가만 두기
			CurRoom->SetCenterLocation(GenerateLocation);
		}
		
		GeneratedRooms.Add(curIndex, CurRoom);

		// TEST - 디버그 프린트 용 함수 (블루프린트 구현)
		CurRoom->SetRoomDebugTest(curIndex);
		if(ParentRoomIndex >= 0)
			DrawDebugLine(GetWorld(), GeneratedRooms[ParentRoomIndex]->GetCenterLocation(), CurRoom->GetCenterLocation(), FColor::Red, true);
		
		for(auto elem : RoomTree[curIndex].ChildRoomIndices)
		{
			RoomIndexStack.Push(elem);
		}
	}
}

void ULABLevelGenerator::InitRoomTree()
{
	// TODO: 현재 Test 용 => 추후 데이터 에셋에서 가져와 초기회하는 것으로 변경
	const int RoomNum = 5;
	RoomTree.SetNum(RoomNum);
	const auto TestRoomFinder = ConstructorHelpers::FClassFinder<ALABRoomBase>(TEXT("/Script/Engine.Blueprint'/Game/_Project/Blueprint/Level/Room/BP_RoomTest.BP_RoomTest_C'"));
	
	for(int i=0; i<RoomNum; i++)
	{
		RoomTree[i].RoomBP = TestRoomFinder.Class;
	}

	RoomTree[0].ChildRoomIndices.Push(1);
	RoomTree[0].ChildRoomIndices.Push(2);

	RoomTree[1].ParentRoomIndex = 0;

	RoomTree[2].ParentRoomIndex = 0;
	RoomTree[2].ChildRoomIndices.Push(3);
	RoomTree[2].ChildRoomIndices.Push(4);

	RoomTree[3].ParentRoomIndex = 2;
	
	RoomTree[4].ParentRoomIndex = 2;
}
