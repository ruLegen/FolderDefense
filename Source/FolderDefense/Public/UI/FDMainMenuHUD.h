// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "FDMainMenuHUD.generated.h"

class UFDMainMenuWidget;
/**
 * 
 */
UCLASS()
class FOLDERDEFENSE_API AFDMainMenuHUD : public AHUD
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> MenuWidgetClass;

	UFDMainMenuWidget* MenuWidget;
};
