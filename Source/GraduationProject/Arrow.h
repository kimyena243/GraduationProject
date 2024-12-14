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
	// �⺻ ������
	AArrow();

protected:
	// BeginPlay �������̵�
	virtual void BeginPlay() override;

public:
	// ProjectileMovementComponent �߰�
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* ProjectileMovement;
	// Tick �������̵�
	virtual void Tick(float DeltaTime) override;

	// SphereComponent�� �浹 ���� ����
	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionComponent;

	// ȭ�� �޽�
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Arrow")
	UStaticMeshComponent* ArrowMesh;

	// �߻�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	FVector ShootDirection;

	// ȭ�� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrow")
	float ArrowSpeed;

	// ȭ�� �浹 ó��
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
