// Fill out your copyright notice in the Description page of Project Settings.


#include "Arrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"

#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
AArrow::AArrow()
{

	// SphereComponent로 충돌 영역 설정 (화살의 충돌체)
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->InitSphereRadius(10.0f);  // 충돌 반경 설정
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &AArrow::OnHit);

	// ProjectileMovementComponent를 초기화
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bShouldBounce = false;  // 튕기지 않도록 설정
	ProjectileMovement->bInitialVelocityInLocalSpace = true;  // 로컬 공간에서의 초기 속도
	ProjectileMovement->ProjectileGravityScale = 0.0f;  // 중력 적용 안 함
	// Tick을 활성화하여 매 프레임마다 업데이트 가능
	PrimaryActorTick.bCanEverTick = true;
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();

	// ShootDirection을 이용하여 화살의 초기 속도를 설정합니다.
	if (ProjectileMovement)
	{
		// ShootDirection이 설정되어 있어야 한다는 조건을 추가합니다.
		if (ShootDirection.IsZero())
		{
			UE_LOG(LogTemp, Warning, TEXT("ShootDirection is zero!"));
			return;
		}

		// 발사 속도 설정 (1000.0f는 예시값으로, 원하는 속도로 조정하세요)
		ProjectileMovement->Velocity = ShootDirection * 1000.0f; // 초기 속도 설정
	}
}
void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AArrow::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 적이 맞은 경우 데미지 처리
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		Enemy->OnHit(10.0f);  // 화살 데미지 예시로 10.0f 사용
		Destroy();  // 화살이 적에 맞으면 삭제
	}
}
