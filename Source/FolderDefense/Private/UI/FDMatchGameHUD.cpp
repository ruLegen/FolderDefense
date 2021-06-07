// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDMatchGameHUD.h"

#include "Blueprint/UserWidget.h"

void AFDMatchGameHUD::BeginPlay()
{
	Super::BeginPlay();
	
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
