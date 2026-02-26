// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MyCharacter.generated.h"

UCLASS()
class UNREALCPLUSPLUS_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;
private:
	UPROPERTY(VisibleAnywhere)
	class UMyAnimInstance* AnimInstance;

public:
	AMyCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	void KeyUpDown(float Value);
	void KeyLeftRight(float Value);
public:
	void LookLeftRight(float Value);
	void LookUpDown(float Value);
public:
	void KeyAttack();
public:
	void PlayerAttack();
};
