// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_FindDestinaton.generated.h"

/**
 * 
 */
UCLASS()
class UNREALCPLUSPLUS_API UBTTask_FindDestinaton : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_FindDestinaton();
public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
