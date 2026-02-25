#include "Enemy.h"
#include "EnemyAIController.h"
#include "EnemyAnimInstance.h"
#include "Components/WidgetComponent.h"
#include "HpUserWidget.h"
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Game/ParagonGreystone/Characters/Heroes/Greystone/Skins/Dragonlord/Meshes/Greystone_Dragonlord.Greystone_Dragonlord"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -90.0), FRotator(0.0, -90.0, 0.0));
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AI(TEXT("/Game/Animations/ABP_Enemy.ABP_Enemy_C"));
	if (AI.Succeeded())
	{
		GetMesh()->SetAnimClass(AI.Class);
	}
	AIControllerClass = AEnemyAIController::StaticClass();

	AIControllerClass = AEnemyAIController::StaticClass();

	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetRootComponent());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	HpBar->SetWidgetSpace(EWidgetSpace::Screen);
	HpBar->SetDrawSize(FVector2D(200.f, 20.f));
	static ConstructorHelpers::FClassFinder<UHpUserWidget> UW(TEXT("/Game/UI/WBP_HpBar.WBP_HpBar_C"));
	if (UW.Succeeded())
	{
		HpBar->SetWidgetClass(UW.Class);
	}
	MaxHp = 100.f;
}


void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	EnemyAnimInstance = Cast<UEnemyAnimInstance>(GetMesh()->GetAnimInstance());
	EnemyAnimInstance->OnMontageEnded.AddDynamic(this, &AEnemy::OnAttackMontageEnded);

	Hp = MaxHp;
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
	Hp -= Damage;

	if (Hp <= 0)
	{
		float currentHP = Hp / MaxHp;
		UE_LOG(LogTemp, Log, TEXT("Damaged : %f"), currentHP);

	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Damaged : %f"), 0.f);
	}

	return 0.0f;
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

void AEnemy::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterupted)
{
	isAttacking = false;
}