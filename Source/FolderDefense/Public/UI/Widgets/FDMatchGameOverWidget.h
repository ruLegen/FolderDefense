// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"

#include "FDMatchGameOverWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class FOLDERDEFENSE_API UFDMatchGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
public:


	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* GameResultInfo;


	public:
	void SetText(FString Text);
};
