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
    // ���� ������ �迭
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
    TArray<AActor*> Enemies;

    // ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
    TSubclassOf<AActor> KeyActorClass;

    // ��� ���� óġ�ߴ��� Ȯ��
    void CheckAllEnemiesDefeated();

    // ���� �׿��� �� ȣ��
    void OnEnemyDefeated(AActor* DefeatedEnemy);

private:
    // óġ�� ���� ��
    int32 DefeatedEnemiesCount;
};
