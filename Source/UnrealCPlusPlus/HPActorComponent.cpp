// Fill out your copyright notice in the Description page of Project Settings.


#include "HPActorComponent.h"

// Sets default values for this component's properties
UHPActorComponent::UHPActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	MaxHp = 100.f;
}

void UHPActorComponent::BeginPlay()
{
	Super::BeginPlay();
	Hp = MaxHp;
	
}

void UHPActorComponent::OnDamaged(float DamagedAmount)
{
	float NewHp = Hp - DamagedAmount;
	SetHp(NewHp);
}

void UHPActorComponent::SetHp(float NewHp)
{
	Hp = NewHp;
	if (Hp <= 0)
	{
		Hp = 0;
	}

	OnHpChanaged.Broadcast();
}

float UHPActorComponent::GetHpRatio()
{
	if (MaxHp <= 0.f || Hp <= 0.f)
	{
		return 0.f;
	}

	return Hp / MaxHp;
}