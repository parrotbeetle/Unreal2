// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL2_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
private:
	UPROPERTY(Category = "Ani", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool ShouldMove;
	UPROPERTY(Category = "Ani", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Horizontal;
	UPROPERTY(Category = "Ani", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	float Vertical;
	UPROPERTY(Category = "Ani", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	FVector Velocity;
	UPROPERTY(Category = "Ani", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = true))
	bool IsFalling;

public:
	UPROPERTY(VisibleAnywhere)
	class ACharacter* Character;
	UPROPERTY(VisibleAnywhere)
	class UCharacterMovementComponent* CharacterMovement;
	UPROPERTY(VisibleAnywhere)
	UAnimMontage* AttackMontage;
	public:
	UMyAnimInstance();
public:
	virtual void NativeBeginPlay() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	void PlayAttackMontage();
};
