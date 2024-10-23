// Fill out your copyright notice in the Description page of Project Settings.


#include "EnableNextComboNotify.h"
#include "PlayerCharacter.h"
void UEnableNextComboNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{


    // ĳ���Ϳ� �޺� ���� ���·� ��ȯ
    APlayerCharacter* Character = Cast<APlayerCharacter>(MeshComp->GetOwner());
    if (Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("notify "));
        Character->EnableNextCombo();  // ���� �޺� ���� ���·� ����


    }
}
