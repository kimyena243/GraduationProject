// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "PlayerCharacter.h" // 플레이어 캐릭터 헤더 파일을 포함합니다.
#include "Kismet/GameplayStatics.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Enemy.h"



AEnemyAIController::AEnemyAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    AIPerceptionSight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight"));

    AIPerceptionComponent->ConfigureSense(*AIPerceptionSight);
    AIPerceptionComponent->SetDominantSense(AIPerceptionSight->GetSenseImplementation());

    AIPerceptionSight->SightRadius = 1500.0f; // 시야 거리
    AIPerceptionSight->LoseSightRadius = 2000.0f; // 시야에서 잃는 거리
    AIPerceptionSight->PeripheralVisionAngleDegrees = 90.0f; // 주변 시야각
    AIPerceptionSight->DetectionByAffiliation.bDetectEnemies = true; // 적 감지
    AIPerceptionSight->DetectionByAffiliation.bDetectNeutrals = true; // 중립 감지
    AIPerceptionSight->DetectionByAffiliation.bDetectFriendlies = false; // 아군 감지

    AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::TargetPerceptionUpdated);
}
void AEnemyAIController::BeginPlay()
{
    Super::BeginPlay();

    // Behavior Tree 실행
    if (BehaviorTree)
    {
        RunBehaviorTree(BehaviorTree);
    }


}



void AEnemyAIController::TargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (Actor && Actor->ActorHasTag("Player") && Stimulus.WasSuccessfullySensed())
    {
        UE_LOG(LogTemp, Warning, TEXT("Player Detected!")); // 로그 추가
        // 플레이어 감지 시 처리
        GetBlackboardComponent()->SetValueAsBool("bPlayerInRange", true);
        GetBlackboardComponent()->SetValueAsObject("TargetActor", Actor);

        // 타이머 클리어
        GetWorld()->GetTimerManager().ClearTimer(LineOfSightTimerHandle);

        // 거리 계산
        FVector EnemyLocation = GetPawn()->GetActorLocation();
        float DistanceToPlayer = FVector::Dist(EnemyLocation, Actor->GetActorLocation());
        GetBlackboardComponent()->SetValueAsFloat("DistanceToPlayer", DistanceToPlayer);
    }
    else if (Actor && Actor->ActorHasTag("Player"))
    {
        // 감지하지 못할 경우
        GetWorld()->GetTimerManager().ClearTimer(LineOfSightTimerHandle);
        GetWorld()->GetTimerManager().SetTimer(LineOfSightTimerHandle, FTimerDelegate::CreateLambda([this]()
            {
                GetBlackboardComponent()->SetValueAsBool("bPlayerInRange", false);
                GetBlackboardComponent()->SetValueAsObject("TargetActor", nullptr);
            }), 2.0f, false); // 2초 후에 False로 설정
    }
}
void AEnemyAIController::Patrol()
{
    FVector TargetLocation = GetRandomPatrolPoint();

    // 목표 위치로 이동
    MoveToLocation(TargetLocation);

    // 이동 방향을 바라보도록 설정
    AEnemy* Enemy = Cast<AEnemy>(GetPawn());
    if (Enemy)
    {
        FRotator NewRotation = FRotationMatrix::MakeFromX(TargetLocation - Enemy->GetActorLocation()).Rotator();
        Enemy->SetActorRotation(NewRotation);
    }

    // 일정 시간 후에 다시 호출하여 새로운 랜덤 위치로 이동하도록 설정
    GetWorld()->GetTimerManager().SetTimer(PatrolTimerHandle, this, &AEnemyAIController::Patrol, 5.0f, false); // 5초마다 새로운 위치로 이동
}

FVector AEnemyAIController::GetRandomPatrolPoint()
{
    FVector Origin = GetPawn()->GetActorLocation();
    FNavLocation RandomPoint; // FNavLocation으로 변경
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(this);
    if (NavSystem)
    {
        // NavSystem에서 랜덤 포인트를 찾습니다.
        if (NavSystem->GetRandomPointInNavigableRadius(Origin, 1000.0f, RandomPoint))
        {
            return RandomPoint.Location; // FNavLocation에서 Location을 반환
        }
    }

    return FVector::ZeroVector; // 실패할 경우 제로 벡터 반환
}

//AEnemyAIController::AEnemyAIController()
//{
//    // 기본값 설정
//    CurrentPatrolIndex = 0;
//
//
//    // 블랙보드 컴포넌트 생성
//    EBlackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
//    EPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EPerceptionComponent")); // 컴포넌트 초기화
//
//    // 시야 감지 설정
//    UAISenseConfig_Sight* SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
//
//    EPerceptionComponent->ConfigureSense(*SightConfig);
//    EPerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());
//
//    SightConfig->SightRadius = 1500.0f;  // 시야 거리 설정
//    SightConfig->LoseSightRadius = 2000.0f;  // 시야에서 사라졌을 때 감지되는 거리
//    SightConfig->PeripheralVisionAngleDegrees = 90.0f;  // 시야각 설정
//    SightConfig->SetMaxAge(5.0f);  // 감지된 정보가 얼마나 오래 유지되는지
//    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
//    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
//    SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
//
//    //// 시야 설정을 AIPerceptionComponent에 추가
//    //EPerceptionComponent->ConfigureSense(*SightConfig);
//    //EPerceptionComponent->SetDominantSense(UAISense_Sight::StaticClass());
//    // 감지 시스템의 델리게이트 바인딩
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
//            // 플레이어 위치에서 오프셋 추가
//            FVector PlayerLocation = Actor->GetActorLocation();
//            FVector Offset = Actor->GetActorForwardVector() * -50.0f; // 플레이어 앞쪽으로 이동하는 오프셋
//            EBlackboard->SetValueAsVector("TargetLocation", PlayerLocation + Offset);
//
//            UE_LOG(LogTemp, Warning, TEXT("Player detected at: %s"), *PlayerLocation.ToString());
//            UE_LOG(LogTemp, Warning, TEXT("bPlayerInRange set to: true"));
//
//            // 이동 속도 조정
//            GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 600.0f; // 뛰는 속도 설정
//
//            // MoveToLocation 호출 추가
//            MoveToLocation(EBlackboard->GetValueAsVector("TargetLocation")); // 플레이어 쫓기
//            return;
//        }
//    }
//    EBlackboard->SetValueAsBool("bPlayerInRange", false);
//    UE_LOG(LogTemp, Warning, TEXT("bPlayerInRange set to: false"));
//    GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 200.0f; // 느린 속도 설정
//    //bool bFoundPlayer = false; // 플레이어가 감지되었는지 여부를 추적하는 변수
//
//}

