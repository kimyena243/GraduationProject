// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnemyAIController.h"
#include "Animation/AnimInstance.h"
#include "Components/CapsuleComponent.h"
#include "AIController.h"
#include "TimerManager.h"
// Sets default values
AEnemy::AEnemy()
{
	// AI ��Ʈ�ѷ� �ʱ�ȭ
	AIControllerClass = AEnemyAIController::StaticClass();
	DefaultWalkSpeed = 200.0f;
   // GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Overlap);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    GetCapsuleComponent()->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
    
	//SetActorRotation(FRotator(0.0f, 0.0f, 0.0f)); // �ʱ� ȸ�� ����
	//GetCharacterMovement()->GravityScale = 1.0f; // �߷� ����
	// AI ��Ʈ�ѷ� ����
	AIController = Cast<AEnemyAIController>(GetController());
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

AEnemyAIController* AEnemy::GetAIController() const
{
	return AIController;
}

void AEnemy::OnHit(float Damage)
{
    if (bIsHit) return; // �̹� �°� �ִٸ� ����
    UE_LOG(LogTemp, Warning, TEXT("Current Health: %f"), Health);
    Health -= Damage; // ������ ����
    bIsHit = true; // �´� ���·� ����
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
 
    // �´� �ִϸ��̼� ���
    if (HitMontage)
    {
       
        if (AnimInstance)
        {
            //AnimInstance->Montage_Play(HitMontage);
            AnimInstance->Montage_Play(HitMontage);
            AnimInstance->Montage_JumpToSection(FName("Hit"), HitMontage);
        }
    }

    // UI ������Ʈ - ü�� ��
    UpdateHealthBar();

    // ��� �̵��� ���߰� �ϰų� �߰� ������ �����մϴ�.
    GetCharacterMovement()->DisableMovement();

    // ���� �ð� �Ŀ� �̵� �����ϵ��� �����ϴ� ����
    GetWorld()->GetTimerManager().SetTimer(HitTimerHandle, this, &AEnemy::ResetHitState, 1.0f, false); // 1�� �Ŀ� �̵� ����

    // HP�� 0 ������ ���
    if (Health <= 0.0f)
    {
        
        Die();
    }

    UE_LOG(LogTemp, Log, TEXT("Health: %d"), Health);
}

void AEnemy::ResetHitState()
{
    bIsHit = false; // �´� ���� ����
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // �̵� ���� ���·� ����
}

void AEnemy::UpdateHealthBar()
{
}
void AEnemy::TakeDamage(float DamageAmount)
{
    Health -= DamageAmount;

    // HP�� 0 ������ ���
    if (Health <= 0.0f)
    {
        Die();
    }
}

void AEnemy::Die()
{
    GetCharacterMovement()->DisableMovement();
  

    if (HitMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            // ������ ��Ÿ�� ���¸� ������ ������Ű�� �ʱ�ȭ
            AnimInstance->Montage_Stop(0.2f, HitMontage);

            UE_LOG(LogTemp, Warning, TEXT("Playing Die Animation"));
            AnimInstance->Montage_Play(HitMontage);
           
        }
    }

    // �ִϸ��̼��� ���� �� ĳ���� ����
    // ���� �ð�(�ִϸ��̼� �ð�) �� ���� ó��
    GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemy::OnDeathFinished, 1.0f, false); // 3�� �� ����
}
void AEnemy::OnDeathFinished()
{
    // �� ĳ���͸� ���忡�� ����
    Destroy();
}
