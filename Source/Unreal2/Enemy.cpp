#include "Enemy.h"
#include "EnemyAIController.h"

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
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
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
	UE_LOG(LogTemp, Log, TEXT("Damaged : $f"), Damage);
	return 0.0f;
}

