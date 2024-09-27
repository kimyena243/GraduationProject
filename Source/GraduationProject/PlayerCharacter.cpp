#include "PlayerCharacter.h"
// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "Rock.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC.IMC'"));
	if (InputMappingContextRef.Object)
	{
		InputMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));
	if (MoveActionRef.Object)
	{
		MoveAction = MoveActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> JumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
	if (JumpActionRef.Object)
	{
		JumpAction = JumpActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> LookActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Look.IA_Look'"));
	if (LookActionRef.Object)
	{
		LookAction = LookActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> GetActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Get.IA_Get'"));
	if (GetActionRef.Object)
	{
		GetAction = GetActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> RunActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Run.IA_Run'"));
	if (RunActionRef.Object)
	{
		RunAction = RunActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> ThrowActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Throw.IA_Throw'"));
	if (ThrowActionRef.Object)
	{
		ThrowAction = ThrowActionRef.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> AimingActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Aiming.IA_Aiming'"));
	if (AimingActionRef.Object)
	{
		AimingAction = AimingActionRef.Object;
	}

	bIsRunning = false;
	bIsAiming = false;

	// 기본 속도 및 달리기 속도 초기화
	DefaultWalkSpeed = 600.0f;
	RunSpeed = 1200.0f;
	AimingSpeed = 200.0f;
	Rock = nullptr;
	ThrowSpeed = 1000.0f;

	// 콜리전 설정 (Capsule 컴포넌트에서 설정)
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped with: %s"), *OtherActor->GetName()); // 어떤 객체와 충돌했는지 확인

	
	 //충돌한 액터가 Rock인지 확인
	ARock* _Rock = Cast<ARock>(OtherActor);
	if (_Rock)
	{
		// 주운 돌을 HeldRock 변수에 저장
		Rock = _Rock;
		_Rock->MeshComponent->SetSimulatePhysics(false);
		
		UE_LOG(LogTemp, Warning, TEXT("Rock picked up!"));

		// 주운 돌을 화면에 표시하기 위해 위치 조정 (예시)
		_Rock->SetActorLocation(GetActorLocation() + FVector(0, 0, 50)); // 예: 플레이어 머리 위로 위치 조정
	}
	
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem
		= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	EnhancedInputComponent->BindAction(GetAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Get);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Started, this, &APlayerCharacter::StartRunning);
	EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopRunning);
	EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Throw);
	EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Started, this, &APlayerCharacter::StartAiming);
	EnhancedInputComponent->BindAction(AimingAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopAiming);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	
	const FVector ForwardDiraction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDiraction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDiraction, MovementVector.X);
	AddMovementInput(RightDiraction, MovementVector.Y);
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void APlayerCharacter::Get(const FInputActionValue& Value)
{
	
}

void APlayerCharacter::Throw(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Rock !"));
	if (Rock)
	{
		UE_LOG(LogTemp, Warning, TEXT("tessssRock !"));
		// 돌을 던지는 로직
		Rock->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		FVector ForwardVector = GetControlRotation().Vector();
		FVector LaunchVelocity = ForwardVector * ThrowSpeed;
		Rock->Throw(LaunchVelocity);

		Rock = nullptr;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("nooooo Rock"));
	}
}

void APlayerCharacter::StartRunning()
{
	UE_LOG(LogTemp, Warning, TEXT("run"));
	//조준 중에는 못달리게
	if (bIsAiming)
	{
		return;
	}

	// 달리기 시작
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void APlayerCharacter::StopRunning()
{
	//조준 중에는 못달리게
	if (bIsAiming)
	{
		return;
	}
	// 달리기 멈춤
	bIsRunning = false;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

void APlayerCharacter::StartAiming()
{
	bIsAiming = true;
	CameraBoom->TargetArmLength = 600.f;
	GetCharacterMovement()->MaxWalkSpeed = AimingSpeed;

}

void APlayerCharacter::StopAiming()
{
	bIsAiming = false;
	CameraBoom->TargetArmLength = 400.f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

