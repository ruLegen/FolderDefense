// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/FDLobbyUserRowWidget.h"

#include "Components/TextBlock.h"

void UFDLobbyUserRowWidget::SetName(FText Name)
{
	if (PlayerNameText)
		PlayerNameText->SetText(Name);
}
