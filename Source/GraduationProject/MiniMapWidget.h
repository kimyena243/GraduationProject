
#pragma once


#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Lantern.h"
#include "MiniMapWidget.generated.h"


class AMapManager;


UCLASS()
class GRADUATIONPROJECT_API UMiniMapWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    UPROPERTY(meta = (BindWidget))
    class UImage* MiniMapImage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MiniMap")
    UTextureRenderTarget2D* MiniMapRenderTarget;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UImage* PlayerIcon;

    void UpdatePlayerIconPosition(FVector2D MinimapPosition);

    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime);
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lantern")
    ALantern* Lantern;

    // MiniMapWidget.h
    UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Minimap")
    AMapManager* MapManager;

};
