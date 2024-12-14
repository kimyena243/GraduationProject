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
      
        // ���� Ÿ���� �̹��� �귯�ÿ� ����
        FSlateBrush Brush;
        Brush.SetResourceObject(MiniMapRenderTarget); // Render Target ����
        MiniMapImage->SetBrush(Brush);
      
    }
}
