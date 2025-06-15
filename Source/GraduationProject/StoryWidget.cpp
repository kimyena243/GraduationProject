// Fill out your copyright notice in the Description page of Project Settings.


#include "StoryWidget.h"
#include "Components/TextBlock.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

void UStoryWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // 데이터 불러오기
    if (StoryDataTable)
    {
        TArray<FStoryRow*> AllRows;
        StoryDataTable->GetAllRows<FStoryRow>(TEXT("Load Story"), AllRows);

        for (const FStoryRow* Row : AllRows)
        {
            StoryLines.Add(Row->Text);
        }
    }

    CurrentLineIndex = 0;
    ShowNextLine();
   
    SetKeyboardFocus();
    bIsFocusable = true;

}

FReply UStoryWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
    if (InKeyEvent.GetKey() == EKeys::Enter)
    {
        UE_LOG(LogTemp, Warning, TEXT("enter"));
 
        // 타이핑 중이면 바로 전체 문장 출력
        if (GetWorld()->GetTimerManager().IsTimerActive(TypingTimerHandle))
        {
            GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
            StoryText->SetText(FText::FromString(CurrentFullLine));
        }
        else
        {
            ShowNextLine();
        }

        return FReply::Handled();
    }

    return FReply::Unhandled();
}

void UStoryWidget::ShowNextLine()
{
    if (CurrentLineIndex < StoryLines.Num())
    {
        CurrentFullLine = StoryLines[CurrentLineIndex];
        CurrentTypingText = "";
        TypingCharIndex = 0;
        StoryText->SetText(FText::FromString(""));

        ++CurrentLineIndex;

        // 타이핑 시작
        GetWorld()->GetTimerManager().SetTimer(
            TypingTimerHandle, this, &UStoryWidget::TypeNextCharacter, TypingSpeed, true
        );
    }
    else
    {
        RemoveFromParent(); // 종료

        APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
        if (PC)
        {
            // 입력 모드를 게임 전용으로 되돌림
            FInputModeGameOnly InputMode;
            PC->SetInputMode(InputMode);
            PC->SetShowMouseCursor(false);
        }
        bIsStory = false;
    }
}

void UStoryWidget::TypeNextCharacter()
{
    if (TypingCharIndex < CurrentFullLine.Len())
    {
        CurrentTypingText.AppendChar(CurrentFullLine[TypingCharIndex]);
        ++TypingCharIndex;
        StoryText->SetText(FText::FromString(CurrentTypingText));
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(TypingTimerHandle);
    }
}