// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/FDMainMenuWidget.h"

#include "FDGameInstance.h"
#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "UI/Widgets/FDLobbyRowUserWidget.h"

void UFDMainMenuWidget::NativeOnInitialized()
{
	if (LayoutSwitcher) {
		LayoutSwitcher->SetActiveWidgetIndex(0);
	}
	if (HostButton) {
		HostButton->OnClicked.AddDynamic(this, &UFDMainMenuWidget::OnHostGame);
	}
	if (JoinButton) {
		JoinButton->OnClicked.AddDynamic(this, &UFDMainMenuWidget::OnJoinGame);
	}
	if (BackToMainMenuButton) {
		BackToMainMenuButton->OnClicked.AddDynamic(this, &UFDMainMenuWidget::OnBackToMenu);
	}
	if (RefreshButton) {
		RefreshButton->OnClicked.AddDynamic(this, &UFDMainMenuWidget::OnRefreshClicked);
	}
	if (DisclaimerButton) {
		DisclaimerButton->OnClicked.AddDynamic(this, &UFDMainMenuWidget::OnDisclaimerClicked);
	}
	if (DisclaimerToMainMenuButton) {
		DisclaimerToMainMenuButton->OnClicked.AddDynamic(this, &UFDMainMenuWidget::OnBackToMenu);
	}
	auto GameInstace = GetGameInstance<UFDGameInstance>();
	if (GameInstace)
	{
		GameInstace->OnSessionFoundEvent.AddUObject(this, &UFDMainMenuWidget::OnRefreshed);
	};
}

void UFDMainMenuWidget::OnHostGame()
{
	UFDGameInstance* GameInstance = GetGameInstance<UFDGameInstance>();
	if (!GameInstance) return;
	GameInstance->DestroySession();
	HostGame();
}

void UFDMainMenuWidget::OnJoinGame()
{
	LayoutSwitcher->SetActiveWidgetIndex(1);
}

void UFDMainMenuWidget::OnBackToMenu()
{
	LayoutSwitcher->SetActiveWidgetIndex(0);
}


void UFDMainMenuWidget::OnRefreshClicked()
{
	auto GameInstace = GetGameInstance<UFDGameInstance>();
	if (!GameInstace) return;

	GameInstace->RequestSearchSession();
}

void UFDMainMenuWidget::OnDisclaimerClicked()
{
	LayoutSwitcher->SetActiveWidgetIndex(2);
}


void UFDMainMenuWidget::OnRefreshed(TArray<FOnlineSessionSearchResult>& Servers)
{
	UpdateServers(Servers);
}

void UFDMainMenuWidget::UpdateServers(TArray<FOnlineSessionSearchResult>& Servers)
{
	ServerListScrollBox->ClearChildren();
	int32 index = 0;
	for (auto Server : Servers) {

		auto ServerRow = CreateWidget<UFDLobbyRowUserWidget>(GetWorld(), ServerRowClass);
		if (!ServerRow) continue;
		ServerRow->SetServerName(Server.GetSessionIdStr());
		ServerRow->SetIndex(index);
		ServerRow->OnClicked.AddUObject(this, &UFDMainMenuWidget::OnServerClicked);
		ServerListScrollBox->AddChild(ServerRow);
		index++;
	}
}


void UFDMainMenuWidget::OnServerClicked(int32 Id)
{
	auto GameInstace = GetGameInstance<UFDGameInstance>();
	if (!GameInstace) return;
	GameInstace->Join(Id);
}

void UFDMainMenuWidget::HostGame()
{
	auto GameInstace = GetGameInstance<UFDGameInstance>();
	if (!GameInstace) return;
	
	GameInstace->Host(FName("Session"+GameInstace->GetPlayerName()+FString::FromInt(FMath::RandRange(-9999999,999999))));
}
