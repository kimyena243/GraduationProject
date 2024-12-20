// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"
#include "MiniMapWidget.h"
#include "Engine/World.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/Image.h"
#include "GameFramework/Pawn.h"
#include "MyPlayerController.h"


AMapManager::AMapManager()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 0.016f;
 
    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    RootComponent = SceneCapture;

    // Configure SceneCaptureComponent2D
    SceneCapture->ProjectionType = ECameraProjectionMode::Orthographic;
    SceneCapture->OrthoWidth = MinimapZoom; // Adjust the size of the minimap
    SceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
    SceneCapture->ShowFlags.SetPostProcessing(false);
    SceneCapture->ShowFlags.SetSkyLighting(false);
    SceneCapture->ShowFlags.SetLighting(false);
    //SceneCapture->ShowFlags.SetMaterials(false);      // 머티리얼 비활성화 (바닥 제거)
    SceneCapture->ShowFlags.SetFog(false);
}

void AMapManager::BeginPlay()
{
    Super::BeginPlay();

    // Assign the Render Target
    if (UTextureRenderTarget2D* RenderTarget = Cast<UTextureRenderTarget2D>(SceneCapture->TextureTarget))
    {
        SceneCapture->TextureTarget = RenderTarget;
    }
 
}

void AMapManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn && SceneCapture)
    {
        FVector PlayerLocation = PlayerPawn->GetActorLocation();

        // SceneCapture가 플레이어를 따라 움직임
        FVector SceneCaptureLocation = FVector(PlayerLocation.X, PlayerLocation.Y, SceneCapture->GetComponentLocation().Z);
        SceneCapture->SetWorldLocation(SceneCaptureLocation);

       
    }
    //UpdatePlayerIconPosition();
}

void AMapManager::UpdatePlayerIconPosition()
{
    AMyPlayerController* PlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (PlayerController && PlayerController->MiniMapWidgetInstance)
    {
        // Convert the player's world position to minimap space
        FVector PlayerLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();
        FVector2D MinimapPosition = WorldToMinimapPosition(PlayerLocation);
     
        UE_LOG(LogTemp, Log, TEXT("Player Location: %s"), *PlayerLocation.ToString());
       
        PlayerController->MiniMapWidgetInstance->UpdatePlayerIconPosition(MinimapPosition);
        
        // Example: Adjust icon material position here
    }
}

FVector2D AMapManager::WorldToMinimapPosition(const FVector& PlayerLocation)
{
    //float WorldMinX = -1000.0f;  // 월드 최소 X
    //float WorldMaxX = 1000.0f;   // 월드 최대 X
    //float WorldMinY = -1000.0f;  // 월드 최소 Y
    //float WorldMaxY = 1000.0f;   // 월드 최대 Y

    //float MinimapWidth = 500.0f;  // 미니맵 가로 길이
    //float MinimapHeight = 500.0f; // 미니맵 세로 길이
 
    //FVector2D MinimapOffset = FVector2D(-500.0f, -500.0f);  // 미니맵의 위치

    //// X, Y 좌표를 미니맵 영역으로 변환
    //float NormalizedX = (PlayerLocation.Y - WorldMinY) / (WorldMaxY - WorldMinY);
    //   
    //float NormalizedY = (PlayerLocation.X - WorldMinX) / (WorldMaxX - WorldMinX);

    //// 미니맵 좌표 변환
    //float MinimapX = NormalizedX * MinimapWidth;
    //float MinimapY = (1.0f - NormalizedY) * MinimapHeight; // Y축 반전 추가


    //FVector2D MinimapPosition = FVector2D(MinimapX, MinimapY) + MinimapOffset;
 
  
   
   
    float MinimapWidth = 500.0f;  // 미니맵 가로 크기
    float MinimapHeight = 500.0f; // 미니맵 세로 크기

    FVector2D MinimapPosition = FVector2D(-MinimapWidth * 0.5f, -MinimapHeight * 0.5f);

 
    return MinimapPosition;
}




