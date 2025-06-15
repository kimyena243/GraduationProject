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
    //// 맵 매니저 가져오기
    //MapManagerInstance = Cast<AMapManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AMapManager::StaticClass()));


    //if (MiniMapWidgetClass)
    //{
    //    // 위젯 생성
    //    MiniMapWidgetInstance = CreateWidget<UMiniMapWidget>(this, MiniMapWidgetClass);

    //    if (MiniMapWidgetInstance)
    //    {
    //        // 렌더 타겟 설정
    //        MiniMapWidgetInstance->MiniMapRenderTarget = RenderTarget;
    //        MiniMapWidgetInstance->MapManager = MapManagerInstance;

    //        // 화면에 추가
    //        MiniMapWidgetInstance->AddToViewport();
    //    }
    //  
    //}
}
