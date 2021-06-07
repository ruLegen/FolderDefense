// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/FDLobbyRowUserWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"



void UFDLobbyRowUserWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	if (ServerButton)
	{
		ServerButton->OnClicked.AddDynamic(this, &UFDLobbyRowUserWidget::OnSelect);
	}
}


void UFDLobbyRowUserWidget::OnSelect()
{
	OnClicked.Broadcast(Index);
}

void UFDLobbyRowUserWidget::SetServerName(FString Name)
{
	ServerName->SetText(FText::FromString(Name));
}
