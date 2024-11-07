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
    // 마법 범위를 나타내는 원형 표시를 위한 변수
    UPROPERTY(EditDefaultsOnly, Category = "Magic")
    float MagicRange;
    bool bIsEffectActive = false;
    FTimerHandle EffectTimerHandle; // 타이머 핸들
    // 마법 범위 앞쪽에 이펙트가 생성될 거리

    UPROPERTY(EditDefaultsOnly, Category = "Magic")
    float MagicEffectDistance; 


    // 마법 공격을 수행하는 내부 함수
   // void ExecuteMagicAttack();
    void ResetEffectState();
    // 나이아가라 이펙트를 표시하기 위한 시스템
    UPROPERTY(EditDefaultsOnly, Category = "Magic")
    class UNiagaraSystem* MagicEffect;

    void DamageMagic();
    FVector MagicLocation;
    // 범위 내 적이 있는지 확인하는 함수
    bool IsActorInMagicRange(AEnemy* Enemy, FVector MagicEffectLocation);
};
