// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Enemy.h"

#include "Staff.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API AStaff : public AWeapon
{
	GENERATED_BODY()


public:
	AStaff();
    bool bIsMagic = true;
   
	UFUNCTION(BlueprintCallable, Category = "Magic")
	void MagicAttack();
protected:
	virtual void BeginPlay() override;

private:
    // ���� ������ ��Ÿ���� ���� ǥ�ø� ���� ����
    UPROPERTY(EditDefaultsOnly, Category = "Magic")
    float MagicRange;
    bool bIsEffectActive = false;
    FTimerHandle EffectTimerHandle; // Ÿ�̸� �ڵ�
    // ���� ���� ���ʿ� ����Ʈ�� ������ �Ÿ�

    UPROPERTY(EditDefaultsOnly, Category = "Magic")
    float MagicEffectDistance; 


    // ���� ������ �����ϴ� ���� �Լ�
   // void ExecuteMagicAttack();
    void ResetEffectState();
    // ���̾ư��� ����Ʈ�� ǥ���ϱ� ���� �ý���
    UPROPERTY(EditDefaultsOnly, Category = "Magic")
    class UNiagaraSystem* MagicEffect;

    void DamageMagic();
    FVector MagicLocation;
    // ���� �� ���� �ִ��� Ȯ���ϴ� �Լ�
    bool IsActorInMagicRange(AEnemy* Enemy, FVector MagicEffectLocation);
};
