#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_SearchTarget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL2_API UBTService_SearchTarget : public UBTService
{
	GENERATED_BODY()
public:
	UBTService_SearchTarget();
public:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
