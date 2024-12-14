// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMapWidget.h"
#include "Components/Image.h"
#include "Engine/TextureRenderTarget2D.h"

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
