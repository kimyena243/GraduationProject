// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TorchObserver.h"
#include "Torch.generated.h"

UCLASS()
class GRADUATIONPROJECT_API ATorch : public AActor, public ITorchObserver
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATorch();


   
    virtual void OnLanternNearby(FVector LanternLocation) override;

protected:
    virtual void BeginPlay() override;
private:
    UPROPERTY(VisibleAnywhere)
    class UPointLightComponent* TorchLight;

    UPROPERTY(VisibleAnywhere)
    class UPointLightComponent* Light;

    UPROPERTY(VisibleAnywhere)
    class UParticleSystemComponent* FireEffect;

};
