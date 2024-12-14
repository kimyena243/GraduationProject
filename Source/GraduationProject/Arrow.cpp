// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
AArrow::AArrow()
{

	// SphereComponent�� �浹 ���� ���� (ȭ���� �浹ü)
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->InitSphereRadius(10.0f);  // �浹 �ݰ� ����
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AArrow::OnHit);

	// ProjectileMovementComponent�� �ʱ�ȭ
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bShouldBounce = false;  // ƨ���� �ʵ��� ����
	ProjectileMovement->bInitialVelocityInLocalSpace = true;  // ���� ���������� �ʱ� �ӵ�
	ProjectileMovement->ProjectileGravityScale = 0.0f;  // �߷� ���� �� ��
	// Tick�� Ȱ��ȭ�Ͽ� �� �����Ӹ��� ������Ʈ ����
	PrimaryActorTick.bCanEverTick = true;
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();

	// ShootDirection�� �̿��Ͽ� ȭ���� �ʱ� �ӵ��� �����մϴ�.
	if (ProjectileMovement)
	{
		// ShootDirection�� �����Ǿ� �־�� �Ѵٴ� ������ �߰��մϴ�.
		if (ShootDirection.IsZero())
		{
			UE_LOG(LogTemp, Warning, TEXT("ShootDirection is zero!"));
			return;
		}

		// �߻� �ӵ� ���� (1000.0f�� ���ð�����, ���ϴ� �ӵ��� �����ϼ���)
		ProjectileMovement->Velocity = ShootDirection * 1000.0f; // �ʱ� �ӵ� ����
	}
}
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArrow::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// ���� ���� ��� ������ ó��
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		Enemy->OnHit(10.0f);  // ȭ�� ������ ���÷� 10.0f ���
		Destroy();  // ȭ���� ���� ������ ����
	}
}
