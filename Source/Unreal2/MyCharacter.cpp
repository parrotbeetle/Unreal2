#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyAnimInstance.h"
#include "Kismet/GameplayStatics.h"

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("FollowSpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));

	SpringArm->SetupAttachment(RootComponent);
	Camera->SetupAttachment(SpringArm);

	SpringArm->TargetArmLength = 400.f;
	SpringArm->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 100.f), FRotator(-25.f, 0.f, 0.f));
	SpringArm->bUsePawnControlRotation = true;

	static ConstructorHelpers::FClassFinder<UAnimInstance> AI(TEXT("/Game/Animations/ABP_Character.ABP_Character_C"));

	if (AI.Succeeded())
	{
		GetMesh()->SetAnimClass(AI.Class);
	}
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForwardBackward"), this, &AMyCharacter::KeyUpDown);
	PlayerInputComponent->BindAxis(TEXT("MoveLeftRight"), this, &AMyCharacter::KeyLeftRight);

	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::KeyAttack);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AMyCharacter::Jump);

	PlayerInputComponent->BindAxis(TEXT("LookUpDown"), this, &AMyCharacter::LookUpDown);
	PlayerInputComponent->BindAxis(TEXT("LookLeftRight"), this, &AMyCharacter::LookLeftRight);
}

void AMyCharacter::KeyUpDown(float value)
{
	AddMovementInput(GetActorForwardVector(), value, false);
}

void AMyCharacter::KeyLeftRight(float value)
{
	AddMovementInput(GetActorRightVector(), value, false);
}

void AMyCharacter::LookLeftRight(float Value)
{
	AddControllerYawInput(Value);
}

void AMyCharacter::LookUpDown(float Value)
{
	AddControllerPitchInput(Value);
}

void AMyCharacter::KeyAttack()
{
	if (IsValid(AnimInstance))
	{
		AnimInstance->PlayAttackMontage();
	}
}
void AMyCharacter::PlayerAttack()
{
	FHitResult HitResult;
	FCollisionQueryParams Params(NAME_None, false, this);

	float AttackRange = 100.f;
	float AttackRadius = 40.f; //캡슐의 반지름
	float AttackHalfHeight = 90.f; // 캡슐의 전체 절반 높이
	FVector StartPos = GetActorLocation();
	FVector EndPos = GetActorLocation() + GetActorForwardVector() * AttackRange;


	bool Result = GetWorld()->SweepSingleByChannel
	(
		OUT HitResult,													//충돌 결과를 저장하는 변수
		StartPos,														//시작 지점
		EndPos,															//끝 지점
		FQuat::Identity,												//회전 (기본값)
		//ECC_Visibility,												//충돌 채널(Visibility)
		ECC_GameTraceChannel1,											//Attack
		FCollisionShape::MakeCapsule(AttackRadius, AttackHalfHeight),	//형태 : Sphere(구) => MakeSphere(반지름) => Capsule(캡슐) 
		Params															//충돌 쿼리 파라미터들.
	);

	FVector Vec = GetActorForwardVector() * AttackRange;

	//Fvector Center = StartPos + (EndPos - StartPos) * 0.5f;
	FVector Center = StartPos + Vec * 0.5f;

	//공격 회전값
	FQuat AttackRotation = FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat();

	//Result == true  -> DebugColor = FColor::Green;
	//Result == false -> DebugColor = FColor::Red;
	FColor DebugColor = Result ? FColor::Green : FColor::Red;

	DrawDebugCapsule
	(
		GetWorld(),			//World값
		Center,			//중앙위치
		AttackHalfHeight,	//캡슐 전체높이의 절반
		AttackRadius,		//캡슐의 반지름
		AttackRotation,		//캡슐의 회전값
		DebugColor,			//색깔
		false,				//지속여부
		2.0f				//지속시간


	);

	if (Result && HitResult.GetActor())
	{
		//AActor* Target = HitResult.GetActor(); 
		auto Target = HitResult.GetActor();

		UGameplayStatics::ApplyDamage(Target, 10.f, nullptr, this, NULL);
	}
}
