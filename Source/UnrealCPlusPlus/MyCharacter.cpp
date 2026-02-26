// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyAnimInstance.h"
#include "Kismet/GameplayStatics.h"	  //Ãß°¡


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

	static ConstructorHelpers::FClassFinder<UAnimInstance> AI(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/ABP_Character.ABP_Character_C'"));

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
		//AActor* Target = HitResult.GetActor(); 
		auto Target = HitResult.GetActor();

		UGameplayStatics::ApplyDamage(Target, 10.f, nullptr, this, NULL);
	}
	

}

