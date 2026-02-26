// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "GameFramework/Character.h"

AMyGameModeBase::AMyGameModeBase()
{
	static ConstructorHelpers::FClassFinder<ACharacter> BP_Character(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_MyCharacter.BP_MyCharacter_C'"));

	if (BP_Character.Succeeded())
	{
		DefaultPawnClass = BP_Character.Class;
	}
	
}
