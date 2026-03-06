// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "MyAnimInstance.h"		   
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Arrow.h"
#include "Kismet/GameplayStatics.h"	
#include "Components/WidgetComponent.h"
#include "HpUserWidget.h"
#include "HPActorComponent.h"

AMyPlayer::AMyPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM(TEXT("/Game/ParagonSparrow/Characters/Heroes/Sparrow/Meshes/Sparrow.Sparrow"));

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
	SpringArm->SocketOffset = FVector(0.f, 120.f, 0.f);

	static ConstructorHelpers::FClassFinder<UMyAnimInstance> AI(TEXT("/Script/Engine.AnimBlueprint'/Game/Animation/ABP_Player.ABP_Player_C'"));

	if (AI.Succeeded())
	{
		GetMesh()->SetAnimClass(AI.Class);
	}
	HpBar = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.f, 0.f, 140.f));
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
void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();

	AnimInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());

	auto HpWidget = Cast<UHpUserWidget>(HpBar->GetUserWidgetObject());
	if (HpWidget)
	{
		HpWidget->BindHp(HPActorComponent);
	}
}

// Called every frame
void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayer::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayer::Look);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Started, this, &AMyPlayer::Fire);

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyPlayer::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyPlayer::StopJumping);
	}

}

void AMyPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		//컨트롤 회전값 받기
		const FRotator Rotation = Controller->GetControlRotation();	
		//어느 방향의 회전축인지 받기 위해서
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//앞쪽 벡터
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		//오른쪽 벡터
		const FVector RightDirecdtion = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//움직임 추가
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirecdtion, MovementVector.X);
	}
	
}

void AMyPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMyPlayer::Fire(const FInputActionValue& Value)
{
	if (IsValid(AnimInstance))
	{
		AnimInstance->PlayAttackMontage();

		float AttackRange = 10000.f;

		FHitResult HitResult;

		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(this, 0);

		FVector AimLocation = CameraManager->GetCameraLocation();
		FVector TargetLocation = AimLocation + CameraManager->GetActorForwardVector() * AttackRange;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		bool Result = GetWorld()->LineTraceSingleByChannel
		(
			OUT HitResult,
			AimLocation,
			TargetLocation,
			ECollisionChannel::ECC_Visibility,
			Params
		);

		if (Result)
			TargetLocation = HitResult.ImpactPoint;

		FColor Color = Result ? FColor::Green : FColor::Red;

		DrawDebugLine(GetWorld(), AimLocation, TargetLocation, Color, true);

		FTransform SocketTransform = GetMesh()->GetSocketTransform(FName("ArrowSocket"));
		SocketLocation = SocketTransform.GetLocation();
		FVector DeltaVector = TargetLocation - SocketLocation;
		SocketRotation = FRotationMatrix::MakeFromX(DeltaVector).Rotator();



	}

}

void AMyPlayer::PlayerAttack()
{
	FActorSpawnParameters Params;
	Params.Owner = this;
	Params.Instigator = this;

	auto MyArrow = GetWorld()->SpawnActor<AArrow>(SocketLocation, SocketRotation, Params);
}

float AMyPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	HPActorComponent->OnDamaged(Damage);
	return Damage;
}