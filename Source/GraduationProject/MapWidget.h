
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

    // �̴ϸ� UI ������Ʈ
    void UpdateMinimapActors();

    // ���� ��ǥ�� �̴ϸ� ��ǥ�� ��ȯ
    FVector2D WorldToMinimapCoordinates(FVector WorldLocation);

    // ������ ��ġ�� �̴ϸʿ� ǥ��
    void DrawActorOnMinimap(FVector2D MinimapPosition, FLinearColor Color);

protected:
    // UI ��ҵ�
    UPROPERTY(meta = (BindWidget))
    UImage* MinimapImage; // �̴ϸ� �̹���

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* MinimapCanvas; // �̴ϸ��� �׸� ĵ����

    // ���͵��� ��ġ�� �ǽð����� �����ϱ� ���� Tick �Լ� ȣ��
    virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime);
};
