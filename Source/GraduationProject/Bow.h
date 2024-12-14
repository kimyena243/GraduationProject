// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Arrow.h"  // ȭ�� Ŭ���� ����
#include "Bow.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API ABow : public AWeapon
{
	GENERATED_BODY()

public:
	// �⺻ ������
	ABow();

protected:
	// BeginPlay �������̵�
	virtual void BeginPlay() override;

public:
	// Tick �������̵�
		// �߻�� ȭ���� ������ ����
	int32 LoadedArrows;

	// �ִ� ȭ�� ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 MaxArrows;

	// ȭ�� �߻� ��Ÿ�� (��)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate;


	virtual void Tick(float DeltaTime) override;

	// Ȱ ��� �Լ� (ȭ���� �߻�)
	void FireArrow();

	// ȭ�� �߻� �� ȣ��� �Լ�
	void ShootArrow();

	// ȭ�� �����۰� ���õ� Ŭ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<AArrow> ArrowClass;  // ȭ�� Ŭ����

	// �߻� ���� üũ
	bool bCanFire;

	// ��Ÿ���� ���� Ÿ�̸� �ڵ�
	FTimerHandle FireRateTimerHandle;

	// ȭ�� �߻� �� ȣ��� �Լ�
	void DisplayFiredArrow();

	// ȭ�� ����
	void ReloadArrows();
	
};
