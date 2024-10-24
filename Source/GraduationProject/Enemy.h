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
    // ���� AI ��Ʈ�ѷ��� ���� ���۷���
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
    class AEnemyAIController* AIController;

    // AI ��Ʈ�ѷ��� �������� �Լ�
    UFUNCTION(BlueprintCallable, Category = "AI")
    AEnemyAIController* GetAIController() const;
    float DefaultWalkSpeed;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
    float Health = 50.0f; // ���� �ʱ� ü��


    bool bIsHit = false; // �´� ���� üũ
   
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
