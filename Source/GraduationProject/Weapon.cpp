// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
	// ���� �޽� ����
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}



