// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h" 
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense_Sight.h"
#include "Perception/AISenseConfig_Sight.h"
#include "NavigationSystem.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:
    virtual void BeginPlay() override;
   
    AEnemyAIController();
public:
    //// AI의 행동 트리와 블랙보드
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    //UBehaviorTree* BehaviorTree;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    //UBlackboardData* BlackboardData; // AI 상태를 저장하는 Blackboard
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    //UBlackboardComponent* EBlackboard;

private:
    UPROPERTY(VisibleAnywhere, Category = "AI")
    UAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    UAISenseConfig_Sight* AIPerceptionSight;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTree; // BehaviorTree를 하나로 수정

    FTimerHandle LineOfSightTimerHandle;

    UFUNCTION()
    void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);



    // 현재 순찰 포인트 인덱스
    int32 CurrentPatrolIndex;
public:
    // 몬스터가 순찰하도록 하는 함수
    void Patrol();


    UFUNCTION(BlueprintCallable, Category = "AI")
    FVector GetRandomPatrolPoint(); // 랜덤 순찰 포인트를 반환하는 함수
private:
    FTimerHandle PatrolTimerHandle; // 순찰 타이머 핸들
	
};
