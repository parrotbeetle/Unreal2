// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyAIController.h"
#include "EnemyAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "HpUserWidget.h"
#include "HPActorComponent.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Game/ParagonGreystone/Characters/Heroes/Greystone/Skins/Dragonlord/Meshes/Greystone_Dragonlord.Greystone_Dragonlord"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -90.0), FRotator(0.0, -90.0, 0.0));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AI(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/ABP_Enemy.ABP_Enemy_C'"));
	if (AI.Succeeded())
	{
		GetMesh()->SetAnimClass(AI.Class);
	}

	AIControllerClass = AEnemyAIController::StaticClass();

	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	HpBar->SetDrawSize(FVector2D(200.f, 20.f));
	static ConstructorHelpers::FClassFinder<UHpUserWidget> UW(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/WBP_HpBar.WBP_HpBar_C'"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
	}

	HPActorComponent = CreateDefaultSubobject<UHPActorComponent>(TEXT("HP Actor Component"));

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyAnimInstance->OnMontageEnded.AddDynamic(this, &AEnemy::OnAttackMontageEnded);


	auto HpWidget = Cast<UHpUserWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHp(HPActorComponent);
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HPActorComponent->OnDamaged(Damage);

	return Damage;
}

void AEnemy::EnemyAttack()
{
	if (IsValid(EnemyAnimInstance))
	{
		if (!isAttacking)
		{
			EnemyAnimInstance->PlayAttackMontage();
			isAttacking = true;
		}
	}
}

void AEnemy::EnemyHit()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 200.f;
	float AttackRadius = 40.f;
	float AttackHalfHeight = 90.f;
	FVector StartPos = GetActorLocation();
	FVector EndPos = GetActorLocation() + GetActorForwardVector() * AttackRange;


	bool Result = GetWorld()->SweepSingleByChannel
	(
		OUT HitResult,
		StartPos,
		EndPos,
		FQuat::Identity,
		ECC_GameTraceChannel1,
		FCollisionShape::MakeCapsule(AttackRadius, AttackHalfHeight),
		Params
	);

	FVector Vec = GetActorForwardVector() * AttackRange;
	FVector Center = StartPos + Vec * 0.5f;

	FQuat AttackRotation = FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat();
	FColor DebugColor = Result ? FColor::Green : FColor::Red;

	DrawDebugCapsule
	(
		GetWorld(),
		Center,
		AttackHalfHeight,
		AttackRadius,
		AttackRotation,
		DebugColor,
		false,
		2.0f


	);

	if (Result && HitResult.GetActor())
	{
		UE_LOG(LogTemp, Log, TEXT("Hit : %s"), *HitResult.GetActor()->GetName());
	}
}

void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterupted)
{
	isAttacking = false;
}

