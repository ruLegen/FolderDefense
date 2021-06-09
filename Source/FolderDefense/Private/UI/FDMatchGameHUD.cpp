// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDMatchGameHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widgets/FDMatchGameOverWidget.h"

void AFDMatchGameHUD::BeginPlay()
{
	Super::BeginPlay();
	auto GameOverWidget = CreateWidget<UFDMatchGameOverWidget>(GetWorld(), GameOverWidgetClass);
	if (GameOverWidget)
	{
		GameOverWidget->RestartButton->SetVisibility(GetWorld()->GetAuthGameMode() ? ESlateVisibility::Visible : ESlateVisibility::Hidden);		// if we are server; May be problem on listen server
		//GameOverWidget->RestartButton->OnClicked.AddDynamic(this, &AFDMatchGameHUD::OnRestartMatchClicked);
	}
	
	HUDWidgetMap.Add(EHUDState::GAMEOVER, GameOverWidget);
	InitAll();
	SetState(EHUDState::NONE);
}

void AFDMatchGameHUD::UpdatePlayerInfo()
{
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
		UpdatePlayerInfo();
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

void AFDMatchGameHUD::OnMainMenuClicked()
{
}

void AFDMatchGameHUD::OnBackClicked()
{
}

void AFDMatchGameHUD::OnRestartMatchClicked()
{
}
