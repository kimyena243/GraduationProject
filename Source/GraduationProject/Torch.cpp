// Fill out your copyright notice in the Description page of Project Settings.


#include "Torch.h"
#include "Components/PointLightComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATorch::ATorch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	//TorchLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("TorchLight"));
	//TorchLight->SetupAttachment(RootComponent);
	//TorchLight->SetIntensity(0.0f); // 초기에는 불이 꺼져 있음


	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	TorchLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("TorchLight"));
	TorchLight->SetupAttachment(RootComponent);
	TorchLight->SetVisibility(false);

	FireEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireEffect"));
	FireEffect->SetupAttachment(RootComponent);
	FireEffect->SetVisibility(false);
}

void ATorch::OnLanternNearby(FVector LanternLocation)
{
	const float Distance = FVector::Dist(GetActorLocation(), LanternLocation);
	if (Distance <= 300.0f)
	{
		TorchLight->SetVisibility(true);
		FireEffect->SetVisibility(true);
	}
}

// Called when the game starts or when spawned
void ATorch::BeginPlay()
{
	Super::BeginPlay();
	
}



