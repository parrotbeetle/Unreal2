#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL2_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
private:
	UPROPERTY()
	class UBehaviorTree* BehaviorTree;
	UPROPERTY()
	class UBlackboardData* BlackboardData;
public:
	AEnemyAIController();
public:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
};
