// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.h"
#include "MapManager.h"

void AMyPlayerController::BeginPlay()
{

	Super::BeginPlay();

	FInputModeGameOnly GameInputMode;
	SetInputMode(GameInputMode);
    //// �� �Ŵ��� ��������
    //MapManagerInstance = Cast<AMapManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMapManager::StaticClass()));


    //if (MiniMapWidgetClass)
    //{
    //    // ���� ����
    //    MiniMapWidgetInstance = CreateWidget<UMiniMapWidget>(this, MiniMapWidgetClass);

    //    if (MiniMapWidgetInstance)
    //    {
    //        // ���� Ÿ�� ����
    //        MiniMapWidgetInstance->MiniMapRenderTarget = RenderTarget;
    //        MiniMapWidgetInstance->MapManager = MapManagerInstance;

    //        // ȭ�鿡 �߰�
    //        MiniMapWidgetInstance->AddToViewport();
    //    }
    //  
    //}
}
