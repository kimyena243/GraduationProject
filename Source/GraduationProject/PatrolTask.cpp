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
        UE_LOG(LogTemp, Warning, TEXT("alalalPatrol")); // �߰��� �α�
        AIController->Patrol(); // Patrol �Լ� ȣ��
        return EBTNodeResult::InProgress; // ���� ���� ������ ����
    }
    return EBTNodeResult::Failed; // ����
}
