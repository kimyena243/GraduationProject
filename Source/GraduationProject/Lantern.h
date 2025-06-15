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

    // ���� ������ ��� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lantern")
    float LightRadius;

    void RegisterObserver(ITorchObserver* Observer);
    void UnregisterObserver(ITorchObserver* Observer);

    void ToggleLight();
protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

private:
    // ���� ��Ÿ�� ����Ʈ ����Ʈ
    UPROPERTY(VisibleAnywhere)
    class UPointLightComponent* PointLight;

    // ����Ʈ ���μ��� ��Ƽ����
    UPROPERTY()
    class UMaterialInstanceDynamic* DynamicLanternMaterial;

 
    // ������ ���� �ִ��� ����
    bool bIsLightOn;

    UPROPERTY()
    TArray<ITorchObserver*> Observers;

    void NotifyObservers();

};
