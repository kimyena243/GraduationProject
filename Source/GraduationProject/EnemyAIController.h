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
    //// AI�� �ൿ Ʈ���� ������
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    //UBehaviorTree* BehaviorTree;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    //UBlackboardData* BlackboardData; // AI ���¸� �����ϴ� Blackboard
    //UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    //UBlackboardComponent* EBlackboard;

private:
    UPROPERTY(VisibleAnywhere, Category = "AI")
    UAIPerceptionComponent* AIPerceptionComponent;

    UPROPERTY(VisibleAnywhere, Category = "AI")
    UAISenseConfig_Sight* AIPerceptionSight;

    UPROPERTY(EditDefaultsOnly, Category = "AI")
    UBehaviorTree* BehaviorTree; // BehaviorTree�� �ϳ��� ����

    FTimerHandle LineOfSightTimerHandle;

    UFUNCTION()
    void TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);



    // ���� ���� ����Ʈ �ε���
    int32 CurrentPatrolIndex;
public:
    // ���Ͱ� �����ϵ��� �ϴ� �Լ�
    void Patrol();


    UFUNCTION(BlueprintCallable, Category = "AI")
    FVector GetRandomPatrolPoint(); // ���� ���� ����Ʈ�� ��ȯ�ϴ� �Լ�
private:
    FTimerHandle PatrolTimerHandle; // ���� Ÿ�̸� �ڵ�
	
};
