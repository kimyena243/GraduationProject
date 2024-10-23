// Fill out your copyright notice in the Description page of Project Settings.


#include "EnableNextComboNotify.h"
#include "PlayerCharacter.h"
void UEnableNextComboNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{


    // 캐릭터에 콤보 가능 상태로 전환
    APlayerCharacter* Character = Cast<APlayerCharacter>(MeshComp->GetOwner());
    if (Character)
    {
        UE_LOG(LogTemp, Warning, TEXT("notify "));
        Character->EnableNextCombo();  // 다음 콤보 가능 상태로 설정


    }
}
