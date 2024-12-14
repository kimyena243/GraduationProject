// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "MiniMapWidget.h"
void AMyPlayerController::BeginPlay()
{

	Super::BeginPlay();

	FInputModeGameOnly GameInputMode;
	SetInputMode(GameInputMode);

    if (MiniMapWidgetClass)
    {
        // ���� ����
        UMiniMapWidget* MiniMapWidget = CreateWidget<UMiniMapWidget>(this, MiniMapWidgetClass);

        if (MiniMapWidget)
        {
            // ���� Ÿ�� ����
            MiniMapWidget->MiniMapRenderTarget = RenderTarget;

            // ȭ�鿡 �߰�
            MiniMapWidget->AddToViewport();
        }
    }
}
