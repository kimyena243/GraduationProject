
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapWidget.generated.h"


class UImage;
class UCanvasPanel;


UCLASS()
class GRADUATIONPROJECT_API UMapWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

    // 미니맵 UI 업데이트
    void UpdateMinimapActors();

    // 월드 좌표를 미니맵 좌표로 변환
    FVector2D WorldToMinimapCoordinates(FVector WorldLocation);

    // 액터의 위치를 미니맵에 표시
    void DrawActorOnMinimap(FVector2D MinimapPosition, FLinearColor Color);

protected:
    // UI 요소들
    UPROPERTY(meta = (BindWidget))
    UImage* MinimapImage; // 미니맵 이미지

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* MinimapCanvas; // 미니맵을 그릴 캔버스

    // 액터들의 위치를 실시간으로 갱신하기 위해 Tick 함수 호출
    virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime);
};
