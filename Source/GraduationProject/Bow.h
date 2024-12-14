// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Arrow.h"  // 화살 클래스 포함
#include "Bow.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API ABow : public AWeapon
{
	GENERATED_BODY()

public:
	// 기본 생성자
	ABow();

protected:
	// BeginPlay 오버라이드
	virtual void BeginPlay() override;

public:
	// Tick 오버라이드
		// 발사된 화살을 저장할 변수
	int32 LoadedArrows;

	// 최대 화살 수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MaxArrows;

	// 화살 발사 쿨타임 (초)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate;


	virtual void Tick(float DeltaTime) override;

	// 활 쏘기 함수 (화살을 발사)
	void FireArrow();

	// 화살 발사 시 호출될 함수
	void ShootArrow();

	// 화살 아이템과 관련된 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AArrow> ArrowClass;  // 화살 클래스

	// 발사 상태 체크
	bool bCanFire;

	// 쿨타임을 위한 타이머 핸들
	FTimerHandle FireRateTimerHandle;

	// 화살 발사 후 호출될 함수
	void DisplayFiredArrow();

	// 화살 장전
	void ReloadArrows();
	
};
