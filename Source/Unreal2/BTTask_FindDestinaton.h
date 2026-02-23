#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindDestinaton.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL2_API UBTTask_FindDestinaton : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FindDestinaton();
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
