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

    // 포인트 라이트 추가
    PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
    RootComponent = PointLight;
   
    bIsLightOn = true; // 기본적으로 켜진 상태
    LightRadius = 500.0f;
}

// Called when the game starts or when spawned
void ALantern::BeginPlay()
{
    Super::BeginPlay();

    PointLight->SetCastShadows(false);

    // 포스트 프로세스 볼륨 찾기
    APostProcessVolume* PostProcessVolume = nullptr;
    for (TActorIterator<APostProcessVolume> It(GetWorld()); It; ++It)
    {
        PostProcessVolume = *It;
        break;
    }

    if (PostProcessVolume)
    {
        // 머티리얼 동적 생성
        UMaterialInterface* LanternMaterial = LoadObject<UMaterialInterface>(nullptr, TEXT("MaterialInstanceConstant'/Game/Materials/M_LanternLighting_Inst.M_LanternLighting_Inst'"));
        if (LanternMaterial)
        {
            DynamicLanternMaterial = UMaterialInstanceDynamic::Create(LanternMaterial, this);
            PostProcessVolume->AddOrUpdateBlendable(DynamicLanternMaterial);
        }
    }

    //횃불 등록
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

        // 포스트 프로세스 머티리얼에 랜턴 위치 전달
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

    // 머티리얼 반응 끄기/켜기
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
            // 빛 반경을 0으로 만들어 포스트 프로세스 효과 제거
            DynamicLanternMaterial->SetScalarParameterValue("LightRadius", 0.0f);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("ToggleLight() 호출됨, 상태: %s"), bIsLightOn ? TEXT("켜짐") : TEXT("꺼짐"));

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