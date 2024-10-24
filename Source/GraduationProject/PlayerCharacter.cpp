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

	// // Spring Arm ������Ʈ ���� �� ����
	//CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	//CameraBoom->SetupAttachment(RootComponent);  // ĳ���Ϳ� ����
	//CameraBoom->TargetArmLength = 600.0f;        // ĳ���Ϳ� ī�޶� ������ �Ÿ� (���� ��Ÿ�Ͽ� �°� ����)
	//CameraBoom->bUsePawnControlRotation = false; // ĳ������ ȸ���� ������� ������ ������ ����

	//// ī�޶� Ư�� ������ �����ǵ��� �ϱ� ���� ī�޶��� ȸ���� ����
	//CameraBoom->SetRelativeRotation(FRotator(-60.0f, 0.0f, 0.0f)); // 60�� ������ ���� (������ �Ʒ��� �ٶ󺸰� ����)

	//// ī�޶� ������Ʈ ���� �� Spring Arm�� ����
	//FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	//FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	//FollowCamera->bUsePawnControlRotation = false;    // ī�޶� ĳ������ ȸ������ ������ ����

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

	//CurrentCombo = 0; // ���� �޺��� 0
	bCanCombo = true; // ���� ���� ���·� ����
	bIsRunning = false;
	bIsAiming = false;
	bIsMove = true;
	// �⺻ �ӵ� �� �޸��� �ӵ� �ʱ�ȭ
	DefaultWalkSpeed = 600.0f;
	RunSpeed = 1200.0f;
	AimingSpeed = 200.0f;
	Rock = nullptr;
	ThrowSpeed = 1000.0f;

	// �ݸ��� ���� (Capsule ������Ʈ���� ����)
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	// �ݸ��� ���� ����
	PickupRange = CreateDefaultSubobject<USphereComponent>(TEXT("PickupRange"));
	PickupRange->SetupAttachment(RootComponent);
	PickupRange->SetSphereRadius(80.0f);
	PickupRange->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapBegin);
	PickupRange->OnComponentEndOverlap.AddDynamic(this, &APlayerCharacter::OnOverlapEnd);
	// ���� �ð�ȭ ���� (����� ��忡���� ����)
	PickupRange->SetHiddenInGame(false);  // ���� ������ ������ ����
	PickupRange->SetVisibility(true);     // ���̵��� ����

	// ���� �ʱ�ȭ
	EquippedWeapon = nullptr;
	CurrentWeapon = nullptr;
	bIsWeaponEquipped = false;

	// �ٸ� ������Ʈ�� �ʱ�ȭ
	MagicArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MagicArea"));
	MagicArea->SetupAttachment(RootComponent);
	MagicArea->SetVisibility(false); // ó������ ������ �ʰ� ����

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/Asset/ceiling_tile.ceiling_tile'"));
	if (MeshAsset.Succeeded())
	{
		MagicArea->SetStaticMesh(MeshAsset.Object);
		MagicArea->SetWorldScale3D(FVector(1.0f)); // ũ�� ����
		UE_LOG(LogTemp, Warning, TEXT("Mesh loaded successfully."));
	}
}

void APlayerCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlapped with: %s"), *OtherActor->GetName()); // � ��ü�� �浹�ߴ��� Ȯ��

	//
	// //�浹�� ���Ͱ� Rock���� Ȯ��
	//ARock* _Rock = Cast<ARock>(OtherActor);
	//if (_Rock)
	//{
	//	// �ֿ� ���� HeldRock ������ ����
	//	Rock = _Rock;
	//	// ���� �÷��̾� �Ӹ� ���� ����
	//	Rock->SetActorLocation(GetActorLocation() + FVector(0, 0, 100)); // �Ӹ� �� ��ġ ����
	//	Rock->SetActorRotation(GetActorRotation()); // �÷��̾�� ���� �������� ȸ��
	//	Rock->MeshComponent->SetSimulatePhysics(false); // ���� �ùķ��̼� ��Ȱ��ȭ
	//	Rock->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform); // �÷��̾ ����
	//	UE_LOG(LogTemp, Warning, TEXT("Rock picked up and fixed!"));
	//}
	   // ����� �浹 ����
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OtherActor);
	if (OverlappingWeapon && OverlappingWeapon != EquippedWeapon)
	{
		// ���⸦ �������� �� ���� ó��
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
		if (!bIsWeaponEquipped)  // ���Ⱑ �������� ���� ��쿡�� ���� �ʱ�ȭ
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
		MagicArea->SetWorldScale3D(FVector(1.0f)); // ũ�� ����
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

					// ���� ������ ��������� ����
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
			// �޺��� ������ �� ó������ �ٽ� �����ϵ��� ��Ÿ�� ���¸� �ʱ�ȭ
			UE_LOG(LogTemp, Warning, TEXT("Resetting attack combo."));

			// ������ ��Ÿ�� ���¸� ������ ������Ű�� �ʱ�ȭ
			AnimInstance->Montage_Stop(0.2f, AttackMontage);

			// ó�� ������ ���� (Combo1���� ����)
			AnimInstance->Montage_Play(AttackMontage);
			AnimInstance->Montage_JumpToSection(FName("Combo1"), AttackMontage);

			bCanCombo = false;  // �ʱ� �޺� ���� ���·� ����
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
		// �÷��̾ �ٶ󺸴� ���� ���
		FVector ForwardVector = GetActorForwardVector();
		FVector SpawnLocation = GetActorLocation() + ForwardVector * 1000.0f; // �÷��̾� ���ʿ� ��ġ

		// MagicArea�� ��ġ �� ȸ�� ����
		MagicArea->SetWorldLocation(SpawnLocation);
		//MagicArea->SetWorldRotation(FRotator(90.0f, 0.0f, 0.0f)); // �ٴڿ� �����ϵ��� ȸ��
		MagicArea->SetVisibility(true); // ���� ���̰� ����
	}
}
void APlayerCharacter::EnableNextCombo()
{
	UE_LOG(LogTemp, Warning, TEXT("next "));
	bCanCombo = true;  // ���� �޺��� �Ѿ �� �ִ� ���·� ��ȯ
}

void APlayerCharacter::OnPickupMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsMove = true; // bIsMove�� true�� ����
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
	// Combo4 ���� �޺��� ������ �ʱ�ȭ�ϰų� �ٸ� ������ �߰�
	return FName(); // �⺻������ ��ȿ���� ���� �̸� ��ȯ
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
 // �Է°��� ���ͷ� �޾ƿ�
	if (bIsMove)
	{
		FVector2D MovementVector = Value.Get<FVector2D>();

		// �Է� ���� 0�̸� ȸ������ �ʵ��� ���� ó��
		if (MovementVector.IsZero())
		{
			return;
		}

		// Forward �� Right ���� ���� ���
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // �� ����
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);    // ������ ����

		// ĳ���� �̵� ó��
		AddMovementInput(ForwardDirection, MovementVector.Y); // W/S�� �ش�
		AddMovementInput(RightDirection, MovementVector.X);    // A/D�� �ش�

		// ĳ���Ͱ� �̵��ϴ� ���� ���
		FVector MovementDirection = (ForwardDirection * MovementVector.Y) + (RightDirection * MovementVector.X);

		// ���Ͱ� 0�� �ƴ϶�� ȸ�� ó��
		if (!MovementDirection.IsNearlyZero())
		{
			// �̵� �������� ȸ��
			FRotator TargetRotation = MovementDirection.Rotation();  // �̵� ������ ȸ����
			TargetRotation.Pitch = 0; // Pitch�� ����
			TargetRotation.Roll = 0;  // Roll�� ����

			// �ε巯�� ȸ���� ���� Lerp ���
			FRotator CurrentRotation = GetActorRotation();
			FRotator NewRotation = FMath::Lerp(CurrentRotation, TargetRotation, 0.1f); // 0.1f�� ȸ�� �ӵ� ����

			// ĳ������ ȸ�� ����
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


	//if (CurrentWeapon)  // ���� ���� ���Ⱑ �ְ�, ���⸦ �����ϰ� ���� ���� ��쿡�� ���� ����
	//{
	//	EquipWeapon(CurrentWeapon);  // ���� ���� ���⸦ ����
	//	UE_LOG(LogTemp, Warning, TEXT("Weapon equipped!"));
	//}
	//else   // �̹� ���⸦ �����ϰ� ���� ��
	//{
	//	if (bIsWeaponEquipped)
	//	{//sDropWeapon();  // �տ� �� ���⸦ ��������
	//		UE_LOG(LogTemp, Warning, TEXT("Weapon dropped!"));
	//	}
	//}
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (CurrentWeapon )  // ���� ���� ���Ⱑ �ְ�, ���⸦ �����ϰ� ���� ���� ���
	{
		bIsMove = false;
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("PickUp"), AttackMontage);
		// ��Ÿ�� ���� �� ȣ��� �Լ� ���ε�
		FOnMontageEnded MontageEndedDelegate;
		MontageEndedDelegate.BindUObject(this, &APlayerCharacter::OnPickupMontageEnded);
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate, AttackMontage);

		
		EquipWeapon(CurrentWeapon);  // ���� ���� ���⸦ ����
		UE_LOG(LogTemp, Warning, TEXT("Weapon equipped!"));
	}
	//else
	//{
	//	if (bIsWeaponEquipped)
	//	{
	//		DropWeapon();  // �տ� �� ���⸦ ��������
	//	}
	//}

}
void APlayerCharacter::DropWeapon()
{
	if (EquippedWeapon)  // ���� ���Ⱑ �ִ� ���
	{
		// �տ� �� ���⸦ �ٴڿ� ����
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 150.0f; // ĳ���� ���ʿ� ���� ��ġ
		FRotator DropRotation = FRotator::ZeroRotator;  // ȸ�� �ʱ�ȭ

		// ���⸦ �ش� ��ġ�� ��ġ
		EquippedWeapon->SetActorLocation(DropLocation);
		EquippedWeapon->SetActorRotation(DropRotation);

		// ���� ���� ����
		bIsWeaponEquipped = false;
		EquippedWeapon = nullptr;  // �տ� �� ���� �ʱ�ȭ
		UE_LOG(LogTemp, Warning, TEXT("Weapon dropped!"));
		
	}
}

void APlayerCharacter::EquipWeapon(AWeapon* Weapon)
{

	if (Weapon && Weapon != EquippedWeapon)
	{
		// ������ ������ ���Ⱑ ������ ��������
		if (EquippedWeapon)
		{
			DropWeapon();
		}
		
		// �� ���⸦ ����
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
		Weapon->SetOwner(this);
		
		bIsWeaponEquipped = true;  // ���� ���� ���� ����
		EquippedWeapon = Weapon;   // ���� ���� ������Ʈ

		// ���ο� ���Ⱑ ������ �� CurrentWeapon �ʱ�ȭ
		CurrentWeapon = nullptr;
		
	
		UE_LOG(LogTemp, Warning, TEXT("Equipped weapon: %s"), *Weapon->GetName());
	}
}
void APlayerCharacter::Throw(const FInputActionValue& Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Rock !"));
	if (Rock)
	{
		// ���� ������ ����
		Rock->MeshComponent->SetSimulatePhysics(true); // ���� �ùķ��̼� Ȱ��ȭ

		// ������ ���� ���
		FVector ThrowDirection = GetActorForwardVector(); // �÷��̾ ���� �ִ� ����
		Rock->MeshComponent->AddImpulse(ThrowDirection * 1000.0f); // ���� ���� ����

		// ���� ����߸� �� HeldRock�� nullptr�� ����
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
	//���� �߿��� ���޸���
	if (bIsAiming)
	{
		return;
	}

	// �޸��� ����
	bIsRunning = true;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
}

void APlayerCharacter::StopRunning()
{
	//���� �߿��� ���޸���
	if (bIsAiming)
	{
		return;
	}
	// �޸��� ����
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



