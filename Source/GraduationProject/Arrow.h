// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Arrow.generated.h"

UCLASS()
class GRADUATIONPROJECT_API AArrow : public AActor
{
	GENERATED_BODY()

public:
	// 기본 생성자
	AArrow();

protected:
	// BeginPlay 오버라이드
	virtual void BeginPlay() override;

public:
	// ProjectileMovementComponent 추가
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
	// Tick 오버라이드
	virtual void Tick(float DeltaTime) override;

	// SphereComponent로 충돌 영역 설정
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	// 화살 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow")
	UStaticMeshComponent* ArrowMesh;

	// 발사된 방향
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	FVector ShootDirection;

	// 화살 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	float ArrowSpeed;

	// 화살 충돌 처리
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
