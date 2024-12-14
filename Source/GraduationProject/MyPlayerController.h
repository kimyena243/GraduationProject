// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

    // ¹Ì´Ï¸Ê À§Á¬ Å¬·¡½º
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MiniMapWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UTextureRenderTarget2D* RenderTarget;
	
};
