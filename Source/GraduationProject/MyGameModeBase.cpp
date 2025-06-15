// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
void AMyGameModeBase::BeginPlay()
{
	Super::BeginPlay();


    static bool bHasShownStory = false;

    if (bHasShownStory)
    {
    
        return;
    }
    bHasShownStory = true; 

    UE_LOG(LogTemp, Warning, TEXT("start!"));
    // Story 위젯 클래스 로드
    StoryWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Game/UI/BP_StoryWidget.BP_StoryWidget_C"));
    if (!StoryWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("StoryWidgetClass"));
        return;
    }

    // DataTable 로드
    StoryDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/Font/DT_Story.DT_Story"));
    if (!StoryDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("StoryDataTable"));
        return;
    }

    // 위젯 생성 및 화면에 추가
    if (StoryWidgetClass)
    {
        StoryWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), StoryWidgetClass);
        if (StoryWidgetInstance)
        {
            StoryWidgetInstance->AddToViewport();
            UE_LOG(LogTemp, Warning, TEXT("gogo"));
            APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
            FInputModeUIOnly InputMode;
            InputMode.SetWidgetToFocus(StoryWidgetInstance->TakeWidget());
            InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
            PC->SetInputMode(InputMode);
            PC->SetShowMouseCursor(true);
            StoryWidgetInstance->SetKeyboardFocus();
        }
    }
}

AMyGameModeBase::AMyGameModeBase()
{
	bUseSeamlessTravel = true;
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Game/Blueprint/BP_PlayerCharacter.BP_PlayerCharacter_C"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	/*static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Game/Blueprint/BP_MyPlayerController.BP_MyPlayerController_C"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}*/

}
