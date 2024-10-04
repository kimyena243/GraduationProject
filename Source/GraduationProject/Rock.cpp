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

    // ���� �ùķ��̼� �ʱ� ����
    MeshComponent->SetSimulatePhysics(true); // ������ ��ġ �� ���� �ùķ��̼� Ȱ��ȭ
    SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
    // �浹 ó��
    MeshComponent->OnComponentHit.AddDynamic(this, &ARock::OnHit);

}

// Called when the game starts or when spawned
void ARock::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARock::Throw(const FVector& LaunchVelocity)
{
    // ���� ���� �ӵ��� ����
    ProjectileMovement->Velocity = LaunchVelocity;
}

void ARock::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    //// ���Ϳ� �浹�ߴ��� Ȯ��
    //AMonster* Monster = Cast<AMonster>(OtherActor);
    //if (Monster)
    //{
    //    // ������ ü���� ���ҽ�Ŵ
    //    Monster->TakeDamage(10.0f);  // ���� ���, 10�� �������� ����

    //    // �浹 �� ������Ʈ ����
    //    Destroy();
    //}
}



