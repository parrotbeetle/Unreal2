#include "BTService_SearchTarget.h"
#include "EnemyAIController.h"	
#include "Engine/OverlapResult.h"
#include "MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTService_SearchTarget::UBTService_SearchTarget()
{
	NodeName = TEXT("Search Target");
	Interval = 1.f;
}

void UBTService_SearchTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
	if (Pawn != nullptr)
	{
		FVector Center = Pawn->GetActorLocation();	//중심점
		float SearchDistance = 500.f;				//탐색범위
		TArray<FOverlapResult> OverlapResults;		//충돌체들 담을 배열
		FCollisionQueryParams QueryParams(NAME_Name, false, Pawn);//기본 매개변수 옵션들

		bool Result = GetWorld()->OverlapMultiByChannel
		(
			OverlapResults,
			Center,
			FQuat::Identity, //회전값
			ECollisionChannel::ECC_GameTraceChannel1, //충돌채널
			FCollisionShape::MakeSphere(SearchDistance),//충돌모양
			QueryParams
		);

		if (Result)
		{
			for (auto& OverlapResult : OverlapResults)
			{
				auto Player = Cast<AMyCharacter>(OverlapResult.GetActor());
				if (Player)
				{
					DrawDebugSphere(GetWorld(), Center, SearchDistance, 10, FColor::Green, false, 0.5f);
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Target"), Player);
					return;
				}
			}
		}

		DrawDebugSphere(GetWorld(), Center, SearchDistance, 10, FColor::Red, false, 0.5f);
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName("Target"), nullptr);

	}
}
