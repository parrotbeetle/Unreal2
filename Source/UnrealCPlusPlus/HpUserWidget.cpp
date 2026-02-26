// Fill out your copyright notice in the Description page of Project Settings.


#include "HpUserWidget.h"
#include "Components/ProgressBar.h"
#include "HPActorComponent.h"


void UHpUserWidget::BindHp(UHPActorComponent* ActorComp)
{
	HpActorComp = ActorComp;
	HpActorComp->OnHpChanaged.AddUObject(this, &UHpUserWidget::UpdateHP);
	UpdateHP();
}

void UHpUserWidget::UpdateHP()
{
	HP_ProgressBar->SetPercent(HpActorComp->GetHpRatio());
}
