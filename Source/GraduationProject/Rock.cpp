// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock.h"
#include "GameFramework/ProjectileMovementComponent.h"
// Sets default values
ARock::ARock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    // 물리적 이동 설정
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->bRotationFollowsVelocity = true;

    // 충돌 처리
    MeshComponent->OnComponentHit.AddDynamic(this, &ARock::OnHit);

}

// Called when the game starts or when spawned
void ARock::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARock::Throw(const FVector& LaunchVelocity)
{
    // 던질 때의 속도를 설정
    ProjectileMovement->Velocity = LaunchVelocity;
}

void ARock::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    //// 몬스터와 충돌했는지 확인
    //AMonster* Monster = Cast<AMonster>(OtherActor);
    //if (Monster)
    //{
    //    // 몬스터의 체력을 감소시킴
    //    Monster->TakeDamage(10.0f);  // 예를 들어, 10의 데미지를 가함

    //    // 충돌 후 오브젝트 삭제
    //    Destroy();
    //}
}



