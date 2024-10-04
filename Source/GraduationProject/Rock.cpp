// Fill out your copyright notice in the Description page of Project Settings.


#include "Rock.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
// Sets default values
ARock::ARock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    RootComponent = SphereComponent;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);

    // 물리 시뮬레이션 초기 설정
    MeshComponent->SetSimulatePhysics(true); // 레벨에 배치 시 물리 시뮬레이션 활성화
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
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



