// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class UNREAL2_API AEnemy : public ACharacter
{
	GENERATED_BODY()
private:
	UPROPERTY(VisibleAnywhere)
	float MaxHp;
	UPROPERTY(VisibleAnywhere)
	float Hp;
private:
	UPROPERTY(VisibleAnywhere)
	class UEnemyAnimInstance* EnemyAnimInstance;
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HpBar;
private:
	bool isAttacking = false;
public:
	bool IsAttacking() const { return isAttacking; }
public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
public:
	void EnemyAttack();
public:
	UFUNCTION()
	void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterupted);
};
