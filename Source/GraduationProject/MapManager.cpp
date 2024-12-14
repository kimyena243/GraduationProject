// Fill out your copyright notice in the Description page of Project Settings.


#include "MapManager.h"
#include "Engine/World.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

AMapManager::AMapManager()
{
    PrimaryActorTick.bCanEverTick = true;

 
    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    RootComponent = SceneCapture;

    // Configure SceneCaptureComponent2D
    SceneCapture->ProjectionType = ECameraProjectionMode::Orthographic;
    SceneCapture->OrthoWidth = 5000.0f; // Adjust the size of the minimap
    SceneCapture->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
    SceneCapture->ShowFlags.SetPostProcessing(false);
    SceneCapture->ShowFlags.SetSkyLighting(false);
    
   
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

    UpdatePlayerIconPosition();
}

void AMapManager::UpdatePlayerIconPosition()
{
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (PlayerPawn)
    {
        // Convert the player's world position to minimap space
        FVector PlayerLocation = PlayerPawn->GetActorLocation();
        UE_LOG(LogTemp, Log, TEXT("Player Location: %s"), *PlayerLocation.ToString());

        // Example: Adjust icon material position here
    }
}




