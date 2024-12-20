// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
// Sets default values
ALevelManager::ALevelManager()
{
	// �� ���Ͱ� tick�� �޵��� ����
	PrimaryActorTick.bCanEverTick = true;

	DefeatedEnemiesCount = 0;

}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();

	// ���� ������ ��� ���� �����Ͽ� Enemies �迭�� �߰�
	TArray<AActor*> FoundEnemies;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), FoundEnemies);

	Enemies = FoundEnemies;
	
}

// Called every frame
void ALevelManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);  
	CheckAllEnemiesDefeated();

}

void ALevelManager::CheckAllEnemiesDefeated()
{
	if (DefeatedEnemiesCount >= Enemies.Num())
	{
		// ���踦 ����
		FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 100);  // ����: ���� ���� ��ġ���� ���� 100��ŭ ������ ��ġ�� ����
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AActor>(KeyActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	}
}

void ALevelManager::OnEnemyDefeated(AActor* DefeatedEnemy)
{
	// ���� óġ�Ǹ� �ش� ���� ����Ʈ���� �����ϰ� ī��Ʈ�� ����
	if (Enemies.Contains(DefeatedEnemy))
	{
		Enemies.Remove(DefeatedEnemy);
		DefeatedEnemiesCount++;
	}
}

