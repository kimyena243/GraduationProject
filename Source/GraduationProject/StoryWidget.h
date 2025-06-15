// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "FStoryRow.h"
#include "StoryWidget.generated.h"
/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API UStoryWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
    virtual void NativeConstruct() override;
    virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Story", meta = (ExposeOnSpawn = true))
    UDataTable* StoryDataTable;

protected:
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* StoryText;

private:
    TArray<FString> StoryLines;
    int32 CurrentLineIndex = 0;
    FString CurrentFullLine;
    FString CurrentTypingText;
    int32 TypingCharIndex = 0;

    FTimerHandle TypingTimerHandle;
    float TypingSpeed = 0.1f; // 한 글자당 출력 시간

    void ShowNextLine();
    void TypeNextCharacter();
    bool bIsStory;
};
