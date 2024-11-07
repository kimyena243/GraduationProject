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
	// AI 컨트롤러 초기화
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
    
	//SetActorRotation(FRotator(0.0f, 0.0f, 0.0f)); // 초기 회전 설정
	//GetCharacterMovement()->GravityScale = 1.0f; // 중력 적용
	// AI 컨트롤러 설정
	AIController = Cast<AEnemyAIController>(GetController());
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
}

AEnemyAIController* AEnemy::GetAIController() const
{
	return AIController;
}

void AEnemy::OnHit(float Damage)
{
    if (bIsHit) return; // 이미 맞고 있다면 무시
    UE_LOG(LogTemp, Warning, TEXT("Current Health: %f"), Health);
    Health -= Damage; // 데미지 적용
    bIsHit = true; // 맞는 상태로 변경
    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
 
    // 맞는 애니메이션 재생
    if (HitMontage)
    {
       
        if (AnimInstance)
        {
            //AnimInstance->Montage_Play(HitMontage);
            AnimInstance->Montage_Play(HitMontage);
            AnimInstance->Montage_JumpToSection(FName("Hit"), HitMontage);
        }
    }

    // UI 업데이트 - 체력 바
    UpdateHealthBar();

    // 잠시 이동을 멈추게 하거나 추가 로직을 구현합니다.
    GetCharacterMovement()->DisableMovement();

    // 일정 시간 후에 이동 가능하도록 복구하는 로직
    GetWorld()->GetTimerManager().SetTimer(HitTimerHandle, this, &AEnemy::ResetHitState, 1.0f, false); // 1초 후에 이동 가능

    // HP가 0 이하일 경우
    if (Health <= 0.0f)
    {
        
        Die();
    }

    UE_LOG(LogTemp, Log, TEXT("Health: %d"), Health);
}

void AEnemy::ResetHitState()
{
    bIsHit = false; // 맞는 상태 해제
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking); // 이동 가능 상태로 변경
}

void AEnemy::UpdateHealthBar()
{
}
void AEnemy::TakeDamage(float DamageAmount)
{
    Health -= DamageAmount;

    // HP가 0 이하일 경우
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
            // 마지막 몽타주 상태를 완전히 정지시키고 초기화
            AnimInstance->Montage_Stop(0.2f, HitMontage);

            UE_LOG(LogTemp, Warning, TEXT("Playing Die Animation"));
            AnimInstance->Montage_Play(HitMontage);
           
        }
    }

    // 애니메이션이 끝난 후 캐릭터 삭제
    // 일정 시간(애니메이션 시간) 후 삭제 처리
    GetWorldTimerManager().SetTimer(DeathTimerHandle, this, &AEnemy::OnDeathFinished, 1.0f, false); // 3초 후 삭제
}
void AEnemy::OnDeathFinished()
{
    // 적 캐릭터를 월드에서 제거
    Destroy();
}
