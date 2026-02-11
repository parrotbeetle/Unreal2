// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"

// 생성자
// Sets default values
AMyActor::AMyActor()
{
	//true를 해야지 Tick이 돌아감
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Tick돌기전 한번 호출
// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	//Super : 부모
	//부모에 있는 BeginPlay 함수 호출
	Super::BeginPlay();
	
}

// 프레임 단위당 호출
//FPS : frame per second 1초에 몇번 호출
//|----|-----|--|----| 1초
//     [delta] - 몇초 걸렸는지 ex) 0.0123 초 == DeltaTime
// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	//Super : AActor
	//부모에 있는 Tick 함수 호출
	Super::Tick(DeltaTime);

}

