// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "GameFramework/Character.h"
#include "MyPlayerController.h"

AMyGameModeBase::AMyGameModeBase()
{
	static ConstructorHelpers::FClassFinder<ACharacter> BP_Character(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_MyPlayer.BP_MyPlayer_C'"));

	if (BP_Character.Succeeded())
	{
		DefaultPawnClass = BP_Character.Class;
	}

	PlayerControllerClass = AMyPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APlayerController> BP_PlayerController(TEXT("/Script/Engine.Blueprint'/Game/Blueprints/BP_MyPlayerController.BP_MyPlayerController_C'"));

	if (BP_PlayerController.Succeeded())
	{
		PlayerControllerClass = BP_PlayerController.Class;
	}
}
