// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelManager.generated.h"

UCLASS()
class GRADUATIONPROJECT_API ALevelManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    // 적을 추적할 배열
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
    TArray<AActor*> Enemies;

    // 열쇠 액터
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
    TSubclassOf<AActor> KeyActorClass;

    // 모든 적을 처치했는지 확인
    void CheckAllEnemiesDefeated();

    // 적을 죽였을 때 호출
    void OnEnemyDefeated(AActor* DefeatedEnemy);

private:
    // 처치된 적의 수
    int32 DefeatedEnemiesCount;
};
