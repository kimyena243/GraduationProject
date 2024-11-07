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
    // MagicRange ���� ���� (���� ���� ������)
    MagicRange = 300.0f;

    MagicEffectDistance = 500.0f;
   // PrimaryActorTick.bCanEverTick = true;
}


void AStaff::MagicAttack()
{
    if (bIsEffectActive) return; // �̹� ����Ʈ�� ���� ���̸� ����

    bIsEffectActive = true; // ����Ʈ ���� ������ ����
    bIsMagic = false;
    // �÷��̾��� ��ġ�� ������ �����ɴϴ�.
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    APawn* PlayerPawn = PlayerController->GetPawn();

    if (!PlayerPawn)
    {
        UE_LOG(LogTemp, Warning, TEXT("PlayerPawn is null"));
        return;
    }

    FVector PlayerLocation = PlayerPawn->GetActorLocation();
    FVector PlayerForwardVector = PlayerPawn->GetActorForwardVector();

    // �÷��̾ ���� �ִ� �������� ���� �Ÿ���ŭ �տ� ���� ����Ʈ�� �����մϴ�.
    FVector MagicEffectLocation = PlayerLocation + (PlayerForwardVector * MagicEffectDistance);

    // Z���� 1�� ���� (�⺻���� �ٴ� ��ġ�� ����)
    MagicEffectLocation.Z = 6.0f;

    // ���̾ư��� ����Ʈ ����
    UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(), MagicEffect, MagicEffectLocation
    );

    // ����Ʈ ũ�⸦ ���� ������ �°� ����
    if (NiagaraComponent)
    {
        NiagaraComponent->SetWorldScale3D(FVector(MagicRange / 100.0f));  // MagicRange�� ���� ũ�� ����
    }

    // ���� ���� ���� �ִ� ���鿡�� �������� �ִ� ����
    TArray<AActor*> OverlappingActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), OverlappingActors);

    for (AActor* Actor : OverlappingActors)
    {
        AEnemy* Enemy = Cast<AEnemy>(Actor);
        if (Enemy && IsActorInMagicRange(Enemy, MagicEffectLocation))
        {
            // ���� ���� ���� ���� ������ �������� �ݴϴ�.
            Enemy->OnHit(Damage);

            UE_LOG(LogTemp, Log, TEXT("ohhhh"));
        }
    }

    GetWorld()->GetTimerManager().SetTimer(EffectTimerHandle, this, &AStaff::ResetEffectState, 5.0f, false);//5�ʸ���
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
    // ���� ���� ����Ʈ�� ��ġ ���� �Ÿ��� ����Ͽ� ���� ���� ������ true�� ��ȯ
    float Distance = FVector::Dist(Enemy->GetActorLocation(), MagicEffectLocation);
    return Distance <= MagicRange;
}
void AStaff::ResetEffectState()
{
    bIsEffectActive = false; // ����Ʈ�� ���� �� �ٽ� ���� �����ϵ��� ����
    bIsMagic = true;
}