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
#include "Animation/AnimInstance.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);
	//CameraBoom->TargetArmLength = 400.f;
	//CameraBoom->bUsePawnControlRotation = true;

	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//FollowCamera->bUsePawnControlRotation = false;

	// // Spring Arm 컴포넌트 생성 및 설정
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);  // 캐릭터에 부착
	//CameraBoom->TargetArmLength = 600.0f;        // 캐릭터와 카메라 사이의 거리 (게임 스타일에 맞게 설정)
	//CameraBoom->bUsePawnControlRotation = false; // 캐릭터의 회전과 관계없이 고정된 시점을 유지

	//// 카메라가 특정 각도로 고정되도록 하기 위해 카메라의 회전도 설정
	//CameraBoom->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f)); // 60도 각도로 고정 (위에서 아래를 바라보게 설정)

	//// 카메라 컴포넌트 생성 및 Spring Arm에 부착
	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//FollowCamera->bUsePawnControlRotation = false;    // 카메라가 캐릭터의 회전값을 따르지 않음

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
	static ConstructorHelpers::FObjectFinder<UInputAction> AttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Attack.IA_Attack'"));
	if (AttackActionRef.Object)
	{
		AttackAction = AttackActionRef.Object;
	}

	//CurrentCombo = 0; // 시작 콤보는 0
	bCanCombo = true; // 공격 가능 상태로 시작
	bIsRunning = false;
	bIsAiming = false;
	bIsMove = true;
	// 기본 속도 및 달리기 속도 초기화
	DefaultWalkSpeed = 600.0f;
	RunSpeed = 1200.0f;
	AimingSpeed = 200.0f;
	Rock = nullptr;
	ThrowSpeed = 1000.0f;

	// 콜리전 설정 (Capsule 컴포넌트에서 설정)
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	// 콜리전 범위 설정
	PickupRange = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRange"));
	PickupRange->SetupAttachment(RootComponent);
	PickupRange->SetSphereRadius(80.0f);
	PickupRange->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	PickupRange->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);
	// 범위 시각화 설정 (디버그 모드에서만 보임)
	PickupRange->SetHiddenInGame(false);  // 게임 내에서 숨기지 않음
	PickupRange->SetVisibility(true);     // 보이도록 설정

	// 무기 초기화
	EquippedWeapon = nullptr;
	CurrentWeapon = nullptr;
	bIsWeaponEquipped = false;

	// 다른 컴포넌트들 초기화
	MagicArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagicArea"));
	MagicArea->SetupAttachment(RootComponent);
	MagicArea->SetVisibility(false); // 처음에는 보이지 않게 설정

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Asset/ceiling_tile.ceiling_tile'"));
	if (MeshAsset.Succeeded())
	{
		MagicArea->SetStaticMesh(MeshAsset.Object);
		MagicArea->SetWorldScale3D(FVector(1.0f)); // 크기 조정
		UE_LOG(LogTemp, Warning, TEXT("Mesh loaded successfully."));
	}
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlapped with: %s"), *OtherActor->GetName()); // 어떤 객체와 충돌했는지 확인

	//
	// //충돌한 액터가 Rock인지 확인
	//ARock* _Rock = Cast<ARock>(OtherActor);
	//if (_Rock)
	//{
	//	// 주운 돌을 HeldRock 변수에 저장
	//	Rock = _Rock;
	//	// 돌을 플레이어 머리 위에 고정
	//	Rock->SetActorLocation(GetActorLocation() + FVector(0, 0, 100)); // 머리 위 위치 조정
	//	Rock->SetActorRotation(GetActorRotation()); // 플레이어와 같은 방향으로 회전
	//	Rock->MeshComponent->SetSimulatePhysics(false); // 물리 시뮬레이션 비활성화
	//	Rock->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform); // 플레이어에 고정
	//	UE_LOG(LogTemp, Warning, TEXT("Rock picked up and fixed!"));
	//}
	   // 무기와 충돌 감지
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OtherActor);
	if (OverlappingWeapon && OverlappingWeapon != EquippedWeapon)
	{
		// 무기를 감지했을 때 로직 처리
		CurrentWeapon = OverlappingWeapon;
		UE_LOG(LogTemp, Warning, TEXT("Weapon in range!"));
		UE_LOG(LogTemp, Warning, TEXT("Equipped weapon: %s"), *CurrentWeapon->GetName());
	}
}

void APlayerCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OtherActor);
	if (OverlappingWeapon && CurrentWeapon == OverlappingWeapon)
	{
		if (!bIsWeaponEquipped)  // 무기가 장착되지 않은 경우에만 무기 초기화
		{
			CurrentWeapon = nullptr;
			UE_LOG(LogTemp, Warning, TEXT("Weapon out of range!"));
		}
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

	if (MagicAreaMesh)
	{
		MagicArea->SetStaticMesh(MagicAreaMesh);
		MagicArea->SetWorldScale3D(FVector(1.0f)); // 크기 조정
		UE_LOG(LogTemp, Warning, TEXT("Mesh loaded from variable."));
	}


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
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
}

void APlayerCharacter::AnimCommand()
{
	//UAnimInstance* animInstace = GetMesh()->GetAnimInstance();
	//animInstace->Montage_Play(animMontage);

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();



	if (AnimInstance && AttackMontage)
	{	
		if (AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			if (bCanCombo)
			{
				FName NextSectionName = GetNextComboSection();
				if (!NextSectionName.IsNone())
				{
					UE_LOG(LogTemp, Warning, TEXT("Next section name: %s"), *NextSectionName.ToString());

					// 다음 섹션을 명시적으로 설정
					AnimInstance->Montage_SetNextSection(AnimInstance->Montage_GetCurrentSection(AttackMontage), NextSectionName, AttackMontage);

					bCanCombo = false;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Invalid section name for combo!"));
				}
			}
		}
		else
		{
			// 콤보가 끝났을 때 처음부터 다시 시작하도록 몽타주 상태를 초기화
			UE_LOG(LogTemp, Warning, TEXT("Resetting attack combo."));

			// 마지막 몽타주 상태를 완전히 정지시키고 초기화
			AnimInstance->Montage_Stop(0.2f, AttackMontage);

			// 처음 공격을 시작 (Combo1으로 시작)
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("Combo1"), AttackMontage);

			bCanCombo = false;  // 초기 콤보 가능 상태로 설정
		}

	}
}

void APlayerCharacter::Attack(const FInputActionValue& Value)
{
	if (bIsWeaponEquipped)
	{
		bool v = Value.Get<bool>();
		AnimCommand();
	}
}
//
//void APlayerCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
//{
//	if (Montage == AttackMontage)
//	{
//		//CurrentCombo = 0;
//		bCanCombo = false;
//	}
//}

void APlayerCharacter::SetMagicArea()
{
	if (bIsAiming)
	{
		MagicArea->SetActive(true);
		// 플레이어가 바라보는 방향 계산
		FVector ForwardVector = GetActorForwardVector();
		FVector SpawnLocation = GetActorLocation() + ForwardVector * 1000.0f; // 플레이어 앞쪽에 위치

		// MagicArea의 위치 및 회전 설정
		MagicArea->SetWorldLocation(SpawnLocation);
		//MagicArea->SetWorldRotation(FRotator(90.0f, 0.0f, 0.0f)); // 바닥에 평행하도록 회전
		MagicArea->SetVisibility(true); // 범위 보이게 설정
	}
}
void APlayerCharacter::EnableNextCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("next "));
	bCanCombo = true;  // 다음 콤보로 넘어갈 수 있는 상태로 전환
}

void APlayerCharacter::OnPickupMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsMove = true; // bIsMove를 true로 설정
}

FName APlayerCharacter::GetNextComboSection()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	FName CurrentSection = AnimInstance->Montage_GetCurrentSection(AttackMontage);
	UE_LOG(LogTemp, Warning, TEXT("now section name: %s"), *CurrentSection.ToString());

	if (CurrentSection == "Combo1")
	{
		UE_LOG(LogTemp, Warning, TEXT("1"));
		return FName("Combo2");
	}
	else if (CurrentSection == "Combo2")
	{
		UE_LOG(LogTemp, Warning, TEXT("2"));
		return FName("Combo3");
	}
	else if (CurrentSection == "Combo3")
	{
		UE_LOG(LogTemp, Warning, TEXT("3"));
		return FName("Combo4");
	}
	// Combo4 이후 콤보가 끝나면 초기화하거나 다른 로직을 추가
	return FName(); // 기본적으로 유효하지 않은 이름 반환
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	//FVector2D MovementVector = Value.Get<FVector2D>();

	//const FRotator Rotation = Controller->GetControlRotation();
	//const FRotator YawRotation(0, Rotation.Yaw, 0);

	//
	//const FVector ForwardDiraction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	//const FVector RightDiraction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//
	//const FVector ForwardDirection = GetActorForwardVector();
	//const FVector RightDirection = GetActorRightVector();

	//AddMovementInput(ForwardDiraction, MovementVector.Y);
	//AddMovementInput(RightDiraction, MovementVector.X);
 // 입력값을 벡터로 받아옴
	if (bIsMove)
	{
		FVector2D MovementVector = Value.Get<FVector2D>();

		// 입력 값이 0이면 회전하지 않도록 예외 처리
		if (MovementVector.IsZero())
		{
			return;
		}

		// Forward 및 Right 방향 벡터 계산
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // 앞 방향
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);    // 오른쪽 방향

		// 캐릭터 이동 처리
		AddMovementInput(ForwardDirection, MovementVector.Y); // W/S에 해당
		AddMovementInput(RightDirection, MovementVector.X);    // A/D에 해당

		// 캐릭터가 이동하는 방향 계산
		FVector MovementDirection = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);

		// 벡터가 0이 아니라면 회전 처리
		if (!MovementDirection.IsNearlyZero())
		{
			// 이동 방향으로 회전
			FRotator TargetRotation = MovementDirection.Rotation();  // 이동 방향의 회전값
			TargetRotation.Pitch = 0; // Pitch는 고정
			TargetRotation.Roll = 0;  // Roll은 고정

			// 부드러운 회전을 위해 Lerp 사용
			FRotator CurrentRotation = GetActorRotation();
			FRotator NewRotation = FMath::Lerp(CurrentRotation, TargetRotation, 0.1f); // 0.1f는 회전 속도 조정

			// 캐릭터의 회전 설정
			SetActorRotation(NewRotation);
		}
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
//	FVector2D LookAxisVector = Value.Get<FVector2D>();
//
//	AddControllerYawInput(LookAxisVector.X);
//	AddControllerPitchInput(LookAxisVector.Y);
}

void APlayerCharacter::Get(const FInputActionValue& Value)
{


	//if (CurrentWeapon)  // 범위 내에 무기가 있고, 무기를 장착하고 있지 않은 경우에만 무기 장착
	//{
	//	EquipWeapon(CurrentWeapon);  // 범위 내의 무기를 장착
	//	UE_LOG(LogTemp, Warning, TEXT("Weapon equipped!"));
	//}
	//else   // 이미 무기를 장착하고 있을 때
	//{
	//	if (bIsWeaponEquipped)
	//	{//sDropWeapon();  // 손에 든 무기를 내려놓기
	//		UE_LOG(LogTemp, Warning, TEXT("Weapon dropped!"));
	//	}
	//}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (CurrentWeapon )  // 범위 내에 무기가 있고, 무기를 장착하고 있지 않은 경우
	{
		bIsMove = false;
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("PickUp"), AttackMontage);
		// 몽타주 종료 시 호출될 함수 바인딩
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &APlayerCharacter::OnPickupMontageEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);

		
		EquipWeapon(CurrentWeapon);  // 범위 내의 무기를 장착
		UE_LOG(LogTemp, Warning, TEXT("Weapon equipped!"));
	}
	//else
	//{
	//	if (bIsWeaponEquipped)
	//	{
	//		DropWeapon();  // 손에 든 무기를 내려놓기
	//	}
	//}

}
void APlayerCharacter::DropWeapon()
{
	if (EquippedWeapon)  // 현재 무기가 있는 경우
	{
		// 손에 든 무기를 바닥에 놓기
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 150.0f; // 캐릭터 앞쪽에 무기 배치
		FRotator DropRotation = FRotator::ZeroRotator;  // 회전 초기화

		// 무기를 해당 위치에 배치
		EquippedWeapon->SetActorLocation(DropLocation);
		EquippedWeapon->SetActorRotation(DropRotation);

		// 장착 상태 해제
		bIsWeaponEquipped = false;
		EquippedWeapon = nullptr;  // 손에 든 무기 초기화
		UE_LOG(LogTemp, Warning, TEXT("Weapon dropped!"));
		
	}
}

void APlayerCharacter::EquipWeapon(AWeapon* Weapon)
{

	if (Weapon && Weapon != EquippedWeapon)
	{
		// 기존에 장착한 무기가 있으면 내려놓음
		if (EquippedWeapon)
		{
			DropWeapon();
		}
		
		// 새 무기를 장착
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
		Weapon->SetOwner(this);
		
		bIsWeaponEquipped = true;  // 무기 장착 상태 설정
		EquippedWeapon = Weapon;   // 현재 무기 업데이트

		// 새로운 무기가 장착된 후 CurrentWeapon 초기화
		CurrentWeapon = nullptr;
		
	
		UE_LOG(LogTemp, Warning, TEXT("Equipped weapon: %s"), *Weapon->GetName());
	}
}
void APlayerCharacter::Throw(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Rock !"));
	if (Rock)
	{
		// 돌을 던지는 로직
		Rock->MeshComponent->SetSimulatePhysics(true); // 물리 시뮬레이션 활성화

		// 던지는 방향 계산
		FVector ThrowDirection = GetActorForwardVector(); // 플레이어가 보고 있는 방향
		Rock->MeshComponent->AddImpulse(ThrowDirection * 1000.0f); // 힘을 가해 던짐

		// 돌을 떨어뜨린 후 HeldRock을 nullptr로 설정
		Rock = nullptr;
		UE_LOG(LogTemp, Warning, TEXT("Rock thrown!"));
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
	//CameraBoom->TargetArmLength = 600.f;
	GetCharacterMovement()->MaxWalkSpeed = AimingSpeed;
	SetMagicArea();
}

void APlayerCharacter::StopAiming()
{
	bIsAiming = false;
	//CameraBoom->TargetArmLength = 400.f;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;

	MagicArea->SetActive(false);
}



