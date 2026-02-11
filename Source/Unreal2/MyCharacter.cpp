#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AMyCharacter::AMyCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Game/ParagonGreystone/Characters/Heroes/Greystone/Meshes/Greystone.Greystone"));

	if (SM.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SM.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));
	}
	//springArm이랑 Camera 만들기, text 안에 있는 글자에 따라 언리얼에서 보이는 글자도 바뀐다
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	//SpringArm은 Root 아래에 배치
	SpringArm->SetupAttachment(RootComponent);
	//Camera를 SpringArm 아래에 배치
	Camera->SetupAttachment(SpringArm);

	//SpringArm 거리를 400으로 설정
	SpringArm->TargetArmLength = 400.f;
	//SpringArm의 위치와 회전을 각각 z에 100.f, Pitch에 -25.f
	SpringArm->SetRelativeLocationAndRotation(FVector(0.f, 0.f, 100.f), FRotator(-25.f, 0.f, 0.f));
}


void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

