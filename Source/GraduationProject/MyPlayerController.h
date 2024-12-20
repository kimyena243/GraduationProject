#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
class UMiniMapWidget;
class AMapManager;

UCLASS()
class GRADUATIONPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UMiniMapWidget* MiniMapWidgetInstance;
	// �� �Ŵ��� ����
	UPROPERTY(BlueprintReadWrite, Category = "World")
	AMapManager* MapManagerInstance;

	// �̴ϸ� ���� Ŭ����
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UUserWidget> MiniMapWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	UTextureRenderTarget2D* RenderTarget;
	
};
