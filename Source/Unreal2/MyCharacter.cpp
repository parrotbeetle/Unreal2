#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyAnimInstance.h"

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
}

void AMyCharacter::KeyUpDown(float value)
{
	AddMovementInput(GetActorForwardVector(), value, false);
}

void AMyCharacter::KeyLeftRight(float value)
{
	AddMovementInput(GetActorRightVector(), value, false);
}
void AMyCharacter::KeyAttack()
{
	if (IsValid(AnimInstance))
	{
		AnimInstance->PlayAttackMontage();
	}
}

