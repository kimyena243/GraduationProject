
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"


UCLASS()
class GRADUATIONPROJECT_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UFUNCTION()
    void DrawAtLocation();

    UFUNCTION()
    void OnFloorButtonClicked(int32 FloorIndex);

    // �̴ϸ� �̹���
    UPROPERTY(meta = (BindWidget))
    class UImage* MinimapImage;

    // ���� RenderTarget �迭 (�����̳ʰ� ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
    TArray<UTextureRenderTarget2D*> FloorRenderTargets;

    // ���� ��� ���� RenderTarget (FloorRenderTargets���� ���õ�)
    UPROPERTY()
    UTextureRenderTarget2D* CurrentRenderTarget;

    //// ��ư�� (���� ���ε� �ʿ�)
    //UPROPERTY(meta = (BindWidget))
    //class UButton* Floor1Button;
    //UPROPERTY(meta = (BindWidget))
    //class UButton* Floor2Button;
    //UPROPERTY(meta = (BindWidget))
    //class UButton* Floor3Button;

    UFUNCTION()
    void OnFloor1Clicked();

    UFUNCTION()
    void OnFloor2Clicked();

    UFUNCTION()
    void OnFloor3Clicked();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
    UMaterialInterface* BrushMaterial;

 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
  
    UMaterialInstanceDynamic* BrushRef;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
    float MapSizeX = 3500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
    float MapSizeY = 3500.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
    float RevealSize = 200.f;

    class APlayerCharacter* Player;
};
