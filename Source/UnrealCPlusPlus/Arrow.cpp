// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h" // 파티클
#include "Kismet/GameplayStatics.h"	  //Spawn

// Sets default values
AArrow::AArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}


	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM(TEXT("/Game/ParagonSparrow/FX/Meshes/Heroes/Sparrow/Abilities/SM_Sparrow_Arrow.SM_Sparrow_Arrow"));

	if (SM.Succeeded())
	{
		Mesh->SetStaticMesh(SM.Object);

		static ConstructorHelpers::FObjectFinder<UMaterial> MAT(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Materials/M_Sparrow_Arrow.M_Sparrow_Arrow"));
		if (MAT.Succeeded())
		{
			Mesh->SetMaterial(0, MAT.Object);
		}

		Mesh->SetRelativeLocationAndRotation(FVector(100.f, 0.f, 0.f), FRotator(90.f, 0.f, 0.f));
		Mesh->SetupAttachment(RootComponent);
		Mesh->SetCollisionProfileName(TEXT("NoCollision"));
	}

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(FName("Collision Box"));
	CollisionBox->SetupAttachment(Mesh);
	CollisionBox->SetRelativeLocation(FVector(0.0, 0.0, -55.0));
	CollisionBox->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));

	//충돌 바인딩
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AArrow::OnOverlapBegin);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->SetUpdatedComponent(RootComponent);
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;

	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS(TEXT("/Game/ParagonSparrow/FX/Particles/Sparrow/Abilities/Primary/FX/P_Sparrow_HitHero.P_Sparrow_HitHero"));
	if (PS.Succeeded())
	{
		HitParticle = PS.Object;
	}
}

void AArrow::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ProjectileMovement->StopMovementImmediately();
	ProjectileMovement->ProjectileGravityScale = 0.f;

	//충돌한 물체에다가 붙이기
	AttachToActor(OtherActor, FAttachmentTransformRules::KeepWorldTransform);

	//충돌처리 안되게
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, CollisionBox->GetComponentLocation(), FRotator::ZeroRotator, true);

	UGameplayStatics::ApplyDamage(OtherActor, 10.f, ProjectileMovement->GetOwner()->GetInstigatorController(), nullptr, NULL);

	FTimerHandle TimerHandler;
	GetWorld()->GetTimerManager().SetTimer(TimerHandler, [&]() {Destroy(); }, 3.f, false);
}
