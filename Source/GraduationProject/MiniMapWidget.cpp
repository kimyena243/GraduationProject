// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "MapManager.h"
#include "Engine/TextureRenderTarget2D.h"

void UMiniMapWidget::UpdatePlayerIconPosition(FVector2D MinimapPosition)
{
    if (PlayerIcon)
    {
        UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(PlayerIcon->Slot);
        if (CanvasSlot)
        {
            FVector2D CurrentPosition = CanvasSlot->GetPosition();
            FVector2D SmoothPosition = FMath::Lerp(CurrentPosition, MinimapPosition, 0.1f); // 0.1은 보간 속도
            CanvasSlot->SetPosition(SmoothPosition);
            //CanvasSlot->SetPosition(MinimapPosition);
            UE_LOG(LogTemp, Log, TEXT("Updated PlayerIcon Position: %s"), *MinimapPosition.ToString());
        }
    }
}

void UMiniMapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // Assign the Render Target texture to the Image widget
    if (MiniMapImage && MiniMapRenderTarget)
    {
      
        // 렌더 타겟을 이미지 브러시에 연결
        FSlateBrush Brush;
        Brush.SetResourceObject(MiniMapRenderTarget); // Render Target 연결
        MiniMapImage->SetBrush(Brush);
      
    }
}

void UMiniMapWidget::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
    Super::NativeTick(MyGeometry, DeltaTime);

    if (MapManager)
    {
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerPawn)
        {
            FVector PlayerLocation = PlayerPawn->GetActorLocation();
           
            FVector2D MinimapPosition = MapManager->WorldToMinimapPosition(PlayerLocation);

            UpdatePlayerIconPosition(MinimapPosition);
        }
    }
}
