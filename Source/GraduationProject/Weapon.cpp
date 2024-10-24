// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"  // BoxComponent ��� �߰�
// Sets default values
AWeapon::AWeapon()
{
	// ���� �޽� ����
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));

	RootComponent = WeaponMesh;

	// BoxComponent ���� �� ����
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(RootComponent);

	//BoxComponent->SetCollisionProfileName(TEXT("OverlapAll")); // �浹 �������� ����
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
		Enemy->OnHit(Damage); // ������ �������� �ִ� �Լ� ȣ��

		UE_LOG(LogTemp, Warning, TEXT("bb")); // �߰��� �α�
	}
}



