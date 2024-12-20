// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"
// Sets default values
ALevelManager::ALevelManager()
{
	// 이 액터가 tick을 받도록 설정
	PrimaryActorTick.bCanEverTick = true;

	DefeatedEnemiesCount = 0;

}

// Called when the game starts or when spawned
void ALevelManager::BeginPlay()
{
	Super::BeginPlay();

	// 현재 레벨의 모든 적을 추적하여 Enemies 배열에 추가
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
		// 열쇠를 스폰
		FVector SpawnLocation = GetActorLocation() + FVector(0, 0, 100);  // 예시: 현재 액터 위치에서 위로 100만큼 떨어진 위치에 스폰
		FActorSpawnParameters SpawnParams;
		GetWorld()->SpawnActor<AActor>(KeyActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
	}
}

void ALevelManager::OnEnemyDefeated(AActor* DefeatedEnemy)
{
	// 적이 처치되면 해당 적을 리스트에서 제거하고 카운트를 증가
	if (Enemies.Contains(DefeatedEnemy))
	{
		Enemies.Remove(DefeatedEnemy);
		DefeatedEnemiesCount++;
	}
}

