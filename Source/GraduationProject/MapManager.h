// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapManager.generated.h"

USTRUCT(BlueprintType)
struct FDungeonMap
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector2D MapSize;  // ���� ũ��

	UPROPERTY(BlueprintReadWrite)
	FVector2D PlayerPosition;  // �÷��̾� ��ġ

	UPROPERTY(BlueprintReadWrite)
	TArray<FVector2D> ObjectPositions;  // ������Ʈ ��ġ��
};

UCLASS()
class GRADUATIONPROJECT_API AMapManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMapManager();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// SceneCaptureComponent2D for capturing the map
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "MiniMap")
	class USceneCaptureComponent2D* SceneCapture;

	// Player icon material instance
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MiniMap")
	class UImage* PlayerIcon;

	// Update player icon position on the minimap
	void UpdatePlayerIconPosition();

	FVector2D WorldToMinimapPosition(const FVector& WorldLocation);

	UPROPERTY(EditAnywhere)
	float MinimapZoom = 1000.0f; // �̴ϸ� Ȯ�� ũ��
	
};
