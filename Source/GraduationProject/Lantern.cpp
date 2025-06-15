// Fill out your copyright notice in the Description page of Project Settings.


#include "Lantern.h"
#include "Components/PointLightComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Actor.h"
#include "Engine/PostProcessVolume.h"
#include "Components/PostProcessComponent.h"
#include "EngineUtils.h"
#include "Torch.h"
// Sets default values
ALantern::ALantern()
{
    PrimaryActorTick.bCanEverTick = true;

    // ����Ʈ ����Ʈ �߰�
    PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
    RootComponent = PointLight;
   
    bIsLightOn = true; // �⺻������ ���� ����
    LightRadius = 500.0f;
}

// Called when the game starts or when spawned
void ALantern::BeginPlay()
{
    Super::BeginPlay();

    PointLight->SetCastShadows(false);

    // ����Ʈ ���μ��� ���� ã��
    APostProcessVolume* PostProcessVolume = nullptr;
    for (TActorIterator<APostProcessVolume> It(GetWorld()); It; ++It)
    {
        PostProcessVolume = *It;
        break;
    }

    if (PostProcessVolume)
    {
        // ��Ƽ���� ���� ����
        UMaterialInterface* LanternMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("MaterialInstanceConstant'/Game/Materials/M_LanternLighting_Inst.M_LanternLighting_Inst'"));
        if (LanternMaterial)
        {
            DynamicLanternMaterial = UMaterialInstanceDynamic::Create(LanternMaterial, this);
            PostProcessVolume->AddOrUpdateBlendable(DynamicLanternMaterial);
        }
    }

    //ȶ�� ���
    TArray<AActor*> TorchActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATorch::StaticClass(), TorchActors);

    for (AActor* Torch : TorchActors)
    {
        if (Torch->GetClass()->ImplementsInterface(UTorchObserver::StaticClass()))
        {
            RegisterObserver(Cast<ITorchObserver>(Torch));
        }
    }
	
}

// Called every frame
void ALantern::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (DynamicLanternMaterial && bIsLightOn)
    {
        FVector LanternLocation = GetActorLocation();

        // ����Ʈ ���μ��� ��Ƽ���� ���� ��ġ ����
        DynamicLanternMaterial->SetVectorParameterValue("LightCenter", FLinearColor(LanternLocation.X, LanternLocation.Y, LanternLocation.Z, 1.0f));
      //  DynamicLanternMaterial->SetScalarParameterValue("LightRadius", LightRadius);
    }


    NotifyObservers();
}

void ALantern::RegisterObserver(ITorchObserver* Observer)
{
    if (Observer && !Observers.Contains(Observer))
    {
        Observers.Add(Observer);
    }
}

void ALantern::UnregisterObserver(ITorchObserver* Observer)
{
    Observers.Remove(Observer);
}

void ALantern::ToggleLight()
{
    bIsLightOn = !bIsLightOn;
    if (PointLight)
    {
        PointLight->SetVisibility(bIsLightOn);
        PointLight->SetHiddenInGame(!bIsLightOn);
    }

    // ��Ƽ���� ���� ����/�ѱ�
    if (DynamicLanternMaterial)
    {
        if (bIsLightOn)
        {
            FVector LanternLocation = GetActorLocation();
            DynamicLanternMaterial->SetVectorParameterValue("LightCenter", FLinearColor(LanternLocation.X, LanternLocation.Y, LanternLocation.Z, 1.0f));
            DynamicLanternMaterial->SetScalarParameterValue("LightRadius", LightRadius);
        }
        else
        {
            // �� �ݰ��� 0���� ����� ����Ʈ ���μ��� ȿ�� ����
            DynamicLanternMaterial->SetScalarParameterValue("LightRadius", 0.0f);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("ToggleLight() ȣ���, ����: %s"), bIsLightOn ? TEXT("����") : TEXT("����"));

}

void ALantern::NotifyObservers()
{
    for (ITorchObserver* Observer : Observers)
    {
        if (Observer)
        {
            Observer->OnLanternNearby(GetActorLocation());
        }
    }
}