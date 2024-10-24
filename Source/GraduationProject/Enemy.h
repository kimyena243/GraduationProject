// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class GRADUATIONPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
    virtual void BeginPlay() override;

public:
    // 몬스터 AI 컨트롤러에 대한 레퍼런스
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    class AEnemyAIController* AIController;

    // AI 컨트롤러를 가져오는 함수
    UFUNCTION(BlueprintCallable, Category = "AI")
    AEnemyAIController* GetAIController() const;
    float DefaultWalkSpeed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float Health = 50.0f; // 적의 초기 체력


    bool bIsHit = false; // 맞는 상태 체크
   
   void OnHit(float Damage);
   void ResetHitState();
   void UpdateHealthBar();
   FTimerHandle HitTimerHandle;
   FTimerHandle DeathTimerHandle;
   UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
   TObjectPtr<UAnimMontage> HitMontage;

   void TakeDamage(float DamageAmount);
   void Die();
   void OnDeathFinished();
};
