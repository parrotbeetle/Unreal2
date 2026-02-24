#include "BTTask_FindDestinaton.h"
#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_FindDestinaton::UBTTask_FindDestinaton()
{
    NodeName = TEXT("Find Patrol Destination");
}

EBTNodeResult::Type UBTTask_FindDestinaton::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    auto Pawn = OwnerComp.GetAIOwner()->GetPawn();
    if (Pawn != nullptr)
    {
        auto NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
        if (NavSystem != nullptr)
        {
            FNavLocation RandomLocation;

            //GetRandomReachablePointInRadius(중심점, 반지름, 담을 위치)
            if (NavSystem->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), 500.f, RandomLocation))
            {
                //GetBlackboardComponent()->SetValueAsVector("블랙보드 키 이름", 넣어줄 값);
                OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName("Destination"), RandomLocation);
                return EBTNodeResult::Succeeded;
            }
        }
    }

    return EBTNodeResult::Failed;
}
