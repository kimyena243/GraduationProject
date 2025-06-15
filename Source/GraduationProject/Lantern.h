// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchObserver.h"
#include "Lantern.generated.h"

UCLASS()
class GRADUATIONPROJECT_API ALantern : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALantern();

    // 랜턴 조명이 닿는 범위
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lantern")
    float LightRadius;

    void RegisterObserver(ITorchObserver* Observer);
    void UnregisterObserver(ITorchObserver* Observer);

    void ToggleLight();
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    // 빛을 나타낼 포인트 라이트
    UPROPERTY(VisibleAnywhere)
    class UPointLightComponent* PointLight;

    // 포스트 프로세스 머티리얼
    UPROPERTY()
    class UMaterialInstanceDynamic* DynamicLanternMaterial;

 
    // 랜턴이 켜져 있는지 여부
    bool bIsLightOn;

    UPROPERTY()
    TArray<ITorchObserver*> Observers;

    void NotifyObservers();

};
