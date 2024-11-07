// Fill out your copyright notice in the Description page of Project Settings.


#include "Staff.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"  
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

AStaff::AStaff()
{
    // MagicRange 값을 설정 (공격 범위 반지름)
    MagicRange = 300.0f;

    MagicEffectDistance = 500.0f;
   // PrimaryActorTick.bCanEverTick = true;
}


void AStaff::MagicAttack()
{
    if (bIsEffectActive) return; // 이미 이펙트가 실행 중이면 중지

    bIsEffectActive = true; // 이펙트 실행 중으로 설정
    bIsMagic = false;
    // 플레이어의 위치와 방향을 가져옵니다.
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    APawn* PlayerPawn = PlayerController->GetPawn();

    if (!PlayerPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is null"));
        return;
    }

    FVector PlayerLocation = PlayerPawn->GetActorLocation();
    FVector PlayerForwardVector = PlayerPawn->GetActorForwardVector();

    // 플레이어가 보고 있는 방향으로 일정 거리만큼 앞에 마법 이펙트를 생성합니다.
    FVector MagicEffectLocation = PlayerLocation + (PlayerForwardVector * MagicEffectDistance);

    // Z값을 1로 고정 (기본적인 바닥 위치로 설정)
    MagicEffectLocation.Z = 6.0f;

    // 나이아가라 이펙트 생성
    UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(), MagicEffect, MagicEffectLocation
    );

    // 이펙트 크기를 공격 범위에 맞게 설정
    if (NiagaraComponent)
    {
        NiagaraComponent->SetWorldScale3D(FVector(MagicRange / 100.0f));  // MagicRange에 맞춰 크기 조정
    }

    // 마법 범위 내에 있는 적들에게 데미지를 주는 로직
    TArray<AActor*> OverlappingActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        AEnemy* Enemy = Cast<AEnemy>(Actor);
        if (Enemy && IsActorInMagicRange(Enemy, MagicEffectLocation))
        {
            // 적이 마법 범위 내에 있으면 데미지를 줍니다.
            Enemy->OnHit(Damage);

            UE_LOG(LogTemp, Log, TEXT("ohhhh"));
        }
    }

    GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, this, &AStaff::ResetEffectState, 5.0f, false);//5초마다
}

void AStaff::BeginPlay()
{
    Super::BeginPlay();
}

void AStaff::DamageMagic()
{
}

bool AStaff::IsActorInMagicRange(AEnemy* Enemy, FVector MagicEffectLocation)
{
    // 적과 마법 이펙트의 위치 간의 거리를 계산하여 범위 내에 있으면 true를 반환
    float Distance = FVector::Dist(Enemy->GetActorLocation(), MagicEffectLocation);
    return Distance <= MagicRange;
}
void AStaff::ResetEffectState()
{
    bIsEffectActive = false; // 이펙트가 끝난 후 다시 공격 가능하도록 설정
    bIsMagic = true;
}