// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"  // BoxComponent 헤더 추가
// Sets default values
AWeapon::AWeapon()
{
	// 무기 메시 생성
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	RootComponent = WeaponMesh;

	// BoxComponent 생성 및 설정
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	//BoxComponent->SetCollisionProfileName(TEXT("OverlapAll")); // 충돌 프로파일 설정
	BoxComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnComponentBeginOverlap);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AEnemy* Enemy = Cast<AEnemy>(OtherActor);
	if (Enemy)
	{
		Enemy->OnHit(Damage); // 적에게 데미지를 주는 함수 호출

		UE_LOG(LogTemp, Warning, TEXT("bb")); // 추가된 로그
	}
}



