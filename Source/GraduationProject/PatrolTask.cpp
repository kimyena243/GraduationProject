// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolTask.h"
#include "AIController.h"
#include "EnemyAIController.h"

UPatrolTask::UPatrolTask()
{
    NodeName = "Patrol";
}

EBTNodeResult::Type UPatrolTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AEnemyAIController* AIController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner());
    if (AIController)
    {
        UE_LOG(LogTemp, Warning, TEXT("alalalPatrol")); // 추가된 로그
        AIController->Patrol(); // Patrol 함수 호출
        return EBTNodeResult::InProgress; // 순찰 진행 중으로 설정
    }
    return EBTNodeResult::Failed; // 실패
}
