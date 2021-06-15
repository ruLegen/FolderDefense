// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDMatchGameHUD.h"

#include "FDGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "GameModes/FDMatchGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerStates/FDMatchPlayerState.h"
#include "UI/Widgets/FDMatchGameOverWidget.h"

void AFDMatchGameHUD::BeginPlay()
{
	Super::BeginPlay();
	GameOverWidget = CreateWidget<UFDMatchGameOverWidget>(GetWorld(), GameOverWidgetClass);
	if (GameOverWidget)
	{
		//GameOverWidget->ExitButton->SetVisibility(GetWorld()->GetAuthGameMode() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);		// if we are server; May be problem on listen server
		GameOverWidget->ExitButton->OnClicked.AddDynamic(this, &AFDMatchGameHUD::OnExitButtonPressed);
	}
	
	HUDWidgetMap.Add(EHUDState::GAMEOVER, GameOverWidget);
	InitAll();
	SetState(EHUDState::NONE);
}


void AFDMatchGameHUD::UpdatePlayerMatchResult(bool bIsDefeat)
{
	if(!bIsDefeat && GameOverWidget)
		GameOverWidget->SetText("You WIN");
	if(bIsDefeat && GameOverWidget)
		GameOverWidget->SetText("You LOOOOOOSE");
}

void AFDMatchGameHUD::SetState(EHUDState State)
{
	HUDState = State;
	switch (State)
	{
	case EHUDState::NONE:
		HideAll();
		break;
	case EHUDState::GAMEOVER:
		//UpdatePlayerMatchResult();
	default:
		SetStateVisibility(State);
		break;
	}
	SetStateVisibility(State);
}

void AFDMatchGameHUD::InitAll()
{
	for (auto MapPair : HUDWidgetMap)
	{
		UUserWidget* Widget = MapPair.Value;
		if (Widget)
		{
			Widget->AddToViewport();
		} 
	}
}

void AFDMatchGameHUD::HideAll()
{
	
	for (auto MapPair : HUDWidgetMap)
	{
		UUserWidget* Widget = MapPair.Value;
		if (Widget)
		{
			Widget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void AFDMatchGameHUD::SetStateVisibility(EHUDState State)
{
	if (HUDWidgetMap.Contains(State))
	{
		UUserWidget* Widget = HUDWidgetMap[State];
		if (Widget)
		{
			HideAll();
			Widget->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void AFDMatchGameHUD::OnExitButtonPressed()
{
	auto GameMode = GetWorld()->GetAuthGameMode<AFDMatchGameMode>();
	if (GameMode)
	{
		GameMode->FinishGame();
	}else
	{
		auto world = GetWorld();
		if(!world) return;
		UGameplayStatics::OpenLevel(world,FName(UFDGameInstance::MainMenuLevel));
	}
}

