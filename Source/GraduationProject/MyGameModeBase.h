// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Engine/DataTable.h"
#include "MyGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GRADUATIONPROJECT_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
public:
	AMyGameModeBase();
	
protected:
	TSubclassOf<class UUserWidget> StoryWidgetClass;
	class UDataTable* StoryDataTable;

	UPROPERTY()
	class UUserWidget* StoryWidgetInstance;
	
};
