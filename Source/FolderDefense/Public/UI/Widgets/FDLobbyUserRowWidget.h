// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "FDLobbyUserRowWidget.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class FOLDERDEFENSE_API UFDLobbyUserRowWidget : public UUserWidget
{
	GENERATED_BODY()
	protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* PlayerNameText;

	public:
	void SetName(FText Name);
};
