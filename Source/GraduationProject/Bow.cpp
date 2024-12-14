// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#include "TimerManager.h"
ABow::ABow()
{
	// �⺻ �� ����
	LoadedArrows = 10;  // �⺻ ������ ȭ�� ��
	MaxArrows = 10;     // �ִ� ȭ�� ��
	FireRate = 1.0f;    // �⺻ ��Ÿ�� 1��
	bCanFire = true;    // �߻� ���� ���� �ʱ�ȭ

	// BoxComponent ����
	//BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABow::OnComponentBeginOverlap);
	// ȭ�� Ŭ���� ����
	ArrowClass = AArrow::StaticClass();  // AArrow Ŭ������ ����
}

void ABow::BeginPlay()
{
	Super::BeginPlay();

	// ������ ȭ�� ���� �ִ�ġ �̻��� ���� �ʵ��� ����
	LoadedArrows = FMath::Clamp(LoadedArrows, 0, MaxArrows);
}

void ABow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABow::FireArrow()
{
	// ��Ÿ���� �������� Ȯ��
	if (!bCanFire || LoadedArrows <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot fire: Out of arrows or cooldown."));
		return;
	}

	// ȭ�� �߻�
	ShootArrow();

	// ȭ�� �߻� �� ��Ÿ�� ����
	bCanFire = false;
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ABow::DisplayFiredArrow, FireRate, false);
}

void ABow::ShootArrow()
{// �߻��� ȭ���� �����մϴ�
	if (ArrowClass)
	{
		FVector ShootLocation = WeaponMesh->GetSocketLocation("ArrowSocket");  // ȭ�� �߻� ��ġ (���� ��ġ)
		FRotator ShootRotation = WeaponMesh->GetSocketRotation("ArrowSocket"); // �߻� ���� (���� ȸ��)

		// �߻� ������ ���ͷ� ���� (ȸ������ ���ͷ� ��ȯ)
		FVector ShootDirection = ShootRotation.Vector();

		// ȭ���� ����
		AArrow* FiredArrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, ShootLocation, ShootRotation);
		if (FiredArrow)
		{
			// ȭ���� �߻� ���� ����
			FiredArrow->ShootDirection = ShootDirection;

			// ȭ�� �߻� �� ������ ȭ�� �� ����
			LoadedArrows--;
		}
	}

	// �߻�� ȭ���� ȭ�鿡 ǥ���ϴ� �Լ� ȣ��
	DisplayFiredArrow();
}

void ABow::DisplayFiredArrow()
{
	// ��Ÿ�� �� �߻� ����
	bCanFire = true;

	// ȭ�� �� �������� ����
	if (LoadedArrows <= 0)
	{
		ReloadArrows();
	}
}

void ABow::ReloadArrows()
{
	// ȭ�� �ٹ��� �ٽ� ����
	UE_LOG(LogTemp, Log, TEXT("Reloading arrows."));

	// 10�߷� ���� (���ϴ� ���� ���� ����)
	LoadedArrows = MaxArrows;
}
