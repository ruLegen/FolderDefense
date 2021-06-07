// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/FDLobbyWidget.h"

#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"

void UFDLobbyWidget::NativeOnInitialized() 
{
	Super::NativeOnInitialized();
	if (StartGameButton) {
		if (GetOwningPlayer()->HasAuthority()) {
			StartGameButton->OnClicked.AddDynamic(this, &UFDLobbyWidget::StartGame);
		}
		else {
			StartGameButton->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UFDLobbyWidget::StartGame()
{
	GetWorld()->ServerTravel("/Game/Levels/Game/GameWorld?game=/Game/Levels/Game/BP_FDMatchGameMode.BP_FDMatchGameMode_C?Listen");
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient)
	{
		ViewportClient->RemoveAllViewportWidgets();
	}
}

void UFDLobbyWidget::UpdatePlayers(TArray<FString> Players)
{
	
	if (TextPlayerNum)
	{
		FString NumText = FString::FromInt(Players.Num()) + " / 2";
		TextPlayerNum->SetText(FText::FromString(NumText));		// TODO Replace 2 with correct Max player value; But It is issue on client side
	}
	PlayerScrollBox->ClearChildren();
	for (auto PlayerName : Players) {
		auto PlayerRow = CreateWidget<UFDLobbyUserRowWidget>(GetWorld(),RowClass);
		if (!PlayerRow) continue;
		FText Name = FText::FromString(PlayerName.ToUpper());
		PlayerRow->SetName(Name);

		PlayerScrollBox->AddChild(PlayerRow);
	}
}

void UFDLobbyWidget::SetButtonVisibility(bool Visible)
{
	if (!StartGameButton) return;
	StartGameButton->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}
