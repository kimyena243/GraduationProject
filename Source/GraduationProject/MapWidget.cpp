// Fill out your copyright notice in the Description page of Project Settings.


#include "MapWidget.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Engine/World.h"
#include "GameFramework/Character.h"
#include "Math/Vector.h"
#include "Math/Vector2D.h"
#include "Blueprint/UserWidget.h"

#include "Components/Image.h"
#include "Components/Button.h"
void UMapWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 머티리얼 생성 및 이미지 연결
    if (BrushMaterial)
    {
        BrushRef = UMaterialInstanceDynamic::Create(BrushMaterial, this);
        if (MinimapImage)
        {
            MinimapImage->SetBrushFromMaterial(BrushRef);
        }
    }

    // 기본층(예: 0층) 초기화
    if (FloorRenderTargets.Num() > 0)
    {
        CurrentRenderTarget = FloorRenderTargets[0];
        UKismetRenderingLibrary::ClearRenderTarget2D(this, CurrentRenderTarget, FLinearColor::Black);

        DrawAtLocation();
    }

    //// 버튼 이벤트 바인딩
    //if (Floor1Button)
    //    Floor1Button->OnClicked.AddDynamic(this, &UMapWidget::OnFloor1Clicked);
    //if (Floor2Button)
    //    Floor2Button->OnClicked.AddDynamic(this, &UMapWidget::OnFloor2Clicked);
    //if (Floor3Button)
    //    Floor3Button->OnClicked.AddDynamic(this, &UMapWidget::OnFloor3Clicked);
}

void UMapWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);
    DrawAtLocation();
}

void UMapWidget::DrawAtLocation()
{

    if (!BrushRef || !CurrentRenderTarget) return;

    FVector PlayerLocation;

    if (UWorld* World = GetWorld())
    {
        ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(World, 0);
        if (PlayerCharacter)
        {
            PlayerLocation = PlayerCharacter->GetActorLocation();
        }
        else
        {
            return; // 플레이어 없으면 그리지 않음
        }

        // 위치 계산
        FVector2D MapSize = FVector2D(MapSizeX, MapSizeY);
        FVector2D Offset = FVector2D(MapSizeX * 0.5f, MapSizeY * 0.5f);

        FVector2D Pos = FVector2D(PlayerLocation.X, PlayerLocation.Y) + Offset;
        FVector2D UV = FVector2D(Pos.Y / MapSize.Y, 1.f - (Pos.X / MapSize.X));
        BrushRef->SetVectorParameterValue(FName("Location"), FVector(UV.X, UV.Y, 0.f));
        BrushRef->SetScalarParameterValue(FName("Size"), RevealSize / MapSize.X);
        BrushRef->SetScalarParameterValue(FName("Strength"), 20.f);

        // Draw to render target
        UKismetRenderingLibrary::DrawMaterialToRenderTarget(World, CurrentRenderTarget, BrushRef);
    }
}

void UMapWidget::OnFloorButtonClicked(int32 FloorIndex)
{
    if (FloorRenderTargets.IsValidIndex(FloorIndex))
    {
        CurrentRenderTarget = FloorRenderTargets[FloorIndex];

    
        // 브러쉬에 적용
        if (BrushRef && MinimapImage)
        {
            BrushRef->SetTextureParameterValue(TEXT("Brush"), CurrentRenderTarget);
        }
    }
}

void UMapWidget::OnFloor1Clicked()
{
    OnFloorButtonClicked(0);
}

void UMapWidget::OnFloor2Clicked()
{
    OnFloorButtonClicked(1);
}

void UMapWidget::OnFloor3Clicked()
{
    OnFloorButtonClicked(2);
}
