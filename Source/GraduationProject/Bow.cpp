// Fill out your copyright notice in the Description page of Project Settings.


#include "Bow.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#include "TimerManager.h"
ABow::ABow()
{
	// 기본 값 설정
	LoadedArrows = 10;  // 기본 장전된 화살 수
	MaxArrows = 10;     // 최대 화살 수
	FireRate = 1.0f;    // 기본 쿨타임 1초
	bCanFire = true;    // 발사 가능 여부 초기화

	// BoxComponent 설정
	//BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	//BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ABow::OnComponentBeginOverlap);
	// 화살 클래스 설정
	ArrowClass = AArrow::StaticClass();  // AArrow 클래스로 설정
}

void ABow::BeginPlay()
{
	Super::BeginPlay();

	// 장전된 화살 수가 최대치 이상이 되지 않도록 제한
	LoadedArrows = FMath::Clamp(LoadedArrows, 0, MaxArrows);
}

void ABow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABow::FireArrow()
{
	// 쿨타임이 끝났는지 확인
	if (!bCanFire || LoadedArrows <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot fire: Out of arrows or cooldown."));
		return;
	}

	// 화살 발사
	ShootArrow();

	// 화살 발사 후 쿨타임 시작
	bCanFire = false;
	GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &ABow::DisplayFiredArrow, FireRate, false);
}

void ABow::ShootArrow()
{// 발사할 화살을 생성합니다
	if (ArrowClass)
	{
		FVector ShootLocation = WeaponMesh->GetSocketLocation("ArrowSocket");  // 화살 발사 위치 (소켓 위치)
		FRotator ShootRotation = WeaponMesh->GetSocketRotation("ArrowSocket"); // 발사 방향 (소켓 회전)

		// 발사 방향을 벡터로 설정 (회전값을 벡터로 변환)
		FVector ShootDirection = ShootRotation.Vector();

		// 화살을 생성
		AArrow* FiredArrow = GetWorld()->SpawnActor<AArrow>(ArrowClass, ShootLocation, ShootRotation);
		if (FiredArrow)
		{
			// 화살의 발사 방향 설정
			FiredArrow->ShootDirection = ShootDirection;

			// 화살 발사 후 장전된 화살 수 감소
			LoadedArrows--;
		}
	}

	// 발사된 화살을 화면에 표시하는 함수 호출
	DisplayFiredArrow();
}

void ABow::DisplayFiredArrow()
{
	// 쿨타임 후 발사 가능
	bCanFire = true;

	// 화살 다 떨어지면 장전
	if (LoadedArrows <= 0)
	{
		ReloadArrows();
	}
}

void ABow::ReloadArrows()
{
	// 화살 다발을 다시 장전
	UE_LOG(LogTemp, Log, TEXT("Reloading arrows."));

	// 10발로 장전 (원하는 수로 설정 가능)
	LoadedArrows = MaxArrows;
}
