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
        // À§Á¬ »ý¼º
        UMiniMapWidget* MiniMapWidget = CreateWidget<UMiniMapWidget>(this, MiniMapWidgetClass);

        if (MiniMapWidget)
        {
            // ·»´õ Å¸°Ù ¼³Á¤
            MiniMapWidget->MiniMapRenderTarget = RenderTarget;

            // È­¸é¿¡ Ãß°¡
            MiniMapWidget->AddToViewport();
        }
    }
}
