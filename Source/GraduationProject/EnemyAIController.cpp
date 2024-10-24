// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "PlayerCharacter.h" // �÷��̾� ĳ���� ��� ������ �����մϴ�.
#include "Kismet/GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy.h"



AEnemyAIController::AEnemyAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    AIPerceptionSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

    AIPerceptionComponent->ConfigureSense(*AIPerceptionSight);
    AIPerceptionComponent->SetDominantSense(AIPerceptionSight->GetSenseImplementation());

    AIPerceptionSight->SightRadius = 1500.0f; // �þ� �Ÿ�
    AIPerceptionSight->LoseSightRadius = 2000.0f; // �þ߿��� �Ҵ� �Ÿ�
    AIPerceptionSight->PeripheralVisionAngleDegrees = 90.0f; // �ֺ� �þ߰�
    AIPerceptionSight->DetectionByAffiliation.bDetectEnemies = true; // �� ����
    AIPerceptionSight->DetectionByAffiliation.bDetectNeutrals = true; // �߸� ����
    AIPerceptionSight->DetectionByAffiliation.bDetectFriendlies = false; // �Ʊ� ����

    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::TargetPerceptionUpdated);
}
void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    // Behavior Tree ����
    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);
    }


}



void AEnemyAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Actor && Actor->ActorHasTag("Player") && Stimulus.WasSuccessfullySensed())
    {
        UE_LOG(LogTemp, Warning, TEXT("Player Detected!")); // �α� �߰�
        // �÷��̾� ���� �� ó��
        GetBlackboardComponent()->SetValueAsBool("bPlayerInRange", true);
        GetBlackboardComponent()->SetValueAsObject("TargetActor", Actor);

        // Ÿ�̸� Ŭ����
        GetWorld()->GetTimerManager().ClearTimer(LineOfSightTimerHandle);

        // �Ÿ� ���
        FVector EnemyLocation = GetPawn()->GetActorLocation();
        float DistanceToPlayer = FVector::Dist(EnemyLocation, Actor->GetActorLocation());
        GetBlackboardComponent()->SetValueAsFloat("DistanceToPlayer", DistanceToPlayer);
    }
    else if (Actor && Actor->ActorHasTag("Player"))
    {
        // �������� ���� ���
        GetWorld()->GetTimerManager().ClearTimer(LineOfSightTimerHandle);
        GetWorld()->GetTimerManager().SetTimer(LineOfSightTimerHandle, FTimerDelegate::CreateLambda([this]()
            {
                GetBlackboardComponent()->SetValueAsBool("bPlayerInRange", false);
                GetBlackboardComponent()->SetValueAsObject("TargetActor", nullptr);
            }), 2.0f, false); // 2�� �Ŀ� False�� ����
    }
}
void AEnemyAIController::Patrol()
{
    FVector TargetLocation = GetRandomPatrolPoint();

    // ��ǥ ��ġ�� �̵�
    MoveToLocation(TargetLocation);

    // �̵� ������ �ٶ󺸵��� ����
    AEnemy* Enemy = Cast<AEnemy>(GetPawn());
    if (Enemy)
    {
        FRotator NewRotation = FRotationMatrix::MakeFromX(TargetLocation - Enemy->GetActorLocation()).Rotator();
        Enemy->SetActorRotation(NewRotation);
    }

    // ���� �ð� �Ŀ� �ٽ� ȣ���Ͽ� ���ο� ���� ��ġ�� �̵��ϵ��� ����
    GetWorld()->GetTimerManager().SetTimer(PatrolTimerHandle, this, &AEnemyAIController::Patrol, 5.0f, false); // 5�ʸ��� ���ο� ��ġ�� �̵�
}

FVector AEnemyAIController::GetRandomPatrolPoint()
{
    FVector Origin = GetPawn()->GetActorLocation();
    FNavLocation RandomPoint; // FNavLocation���� ����
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(this);
    if (NavSystem)
    {
        // NavSystem���� ���� ����Ʈ�� ã���ϴ�.
        if (NavSystem->GetRandomPointInNavigableRadius(Origin, 1000.0f, RandomPoint))
        {
            return RandomPoint.Location; // FNavLocation���� Location�� ��ȯ
        }
    }

    return FVector::ZeroVector; // ������ ��� ���� ���� ��ȯ
}

//AEnemyAIController::AEnemyAIController()
//{
//    // �⺻�� ����
//    CurrentPatrolIndex = 0;
//
//
//    // ������ ������Ʈ ����
//    EBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
//    EPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EPerceptionComponent")); // ������Ʈ �ʱ�ȭ
//
//    // �þ� ���� ����
//    UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
//
//    EPerceptionComponent->ConfigureSense(*SightConfig);
//    EPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
//
//    SightConfig->SightRadius = 1500.0f;  // �þ� �Ÿ� ����
//    SightConfig->LoseSightRadius = 2000.0f;  // �þ߿��� ������� �� �����Ǵ� �Ÿ�
//    SightConfig->PeripheralVisionAngleDegrees = 90.0f;  // �þ߰� ����
//    SightConfig->SetMaxAge(5.0f);  // ������ ������ �󸶳� ���� �����Ǵ���
//    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
//    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
//    SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
//
//    //// �þ� ������ AIPerceptionComponent�� �߰�
//    //EPerceptionComponent->ConfigureSense(*SightConfig);
//    //EPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
//    // ���� �ý����� ��������Ʈ ���ε�
//    //EPerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnPerceptionUpdated);
//    EPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::TargetPerceptionUpdated);
//
//}



//void AEnemyAIController::OnPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
//{
//
//    for (AActor* Actor : UpdatedActors)
//    {
//        if (Actor->IsA(APlayerCharacter::StaticClass()))
//        {
//            EBlackboard->SetValueAsBool("bPlayerInRange", true);
//            // �÷��̾� ��ġ���� ������ �߰�
//            FVector PlayerLocation = Actor->GetActorLocation();
//            FVector Offset = Actor->GetActorForwardVector() * -50.0f; // �÷��̾� �������� �̵��ϴ� ������
//            EBlackboard->SetValueAsVector("TargetLocation", PlayerLocation + Offset);
//
//            UE_LOG(LogTemp, Warning, TEXT("Player detected at: %s"), *PlayerLocation.ToString());
//            UE_LOG(LogTemp, Warning, TEXT("bPlayerInRange set to: true"));
//
//            // �̵� �ӵ� ����
//            GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 600.0f; // �ٴ� �ӵ� ����
//
//            // MoveToLocation ȣ�� �߰�
//            MoveToLocation(EBlackboard->GetValueAsVector("TargetLocation")); // �÷��̾� �ѱ�
//            return;
//        }
//    }
//    EBlackboard->SetValueAsBool("bPlayerInRange", false);
//    UE_LOG(LogTemp, Warning, TEXT("bPlayerInRange set to: false"));
//    GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 200.0f; // ���� �ӵ� ����
//    //bool bFoundPlayer = false; // �÷��̾ �����Ǿ����� ���θ� �����ϴ� ����
//
//}

