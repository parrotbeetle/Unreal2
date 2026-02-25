#include "BTTask_Attack.h"
#include "EnemyAIController.h"
#include "Enemy.h"

UBTTask_Attack::UBTTask_Attack()
{
    NodeName = TEXT("Attack");
    bNotifyTick = true;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    auto Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
    if (Enemy == nullptr)
        return EBTNodeResult::Failed;

    if (!Enemy->IsAttacking())
    {
        Enemy->EnemyAttack();
    }


    return Result;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    auto Enemy = Cast<AEnemy>(OwnerComp.GetAIOwner()->GetPawn());
    if (Enemy == nullptr)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    if (Enemy->IsAttacking() == false)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}