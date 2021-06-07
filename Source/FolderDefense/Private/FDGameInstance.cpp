// Fill out your copyright notice in the Description page of Project Settings.


#include "FDGameInstance.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSessionSettings.h"

const  FString UFDGameInstance::LobbyLevel = "/Game/Levels/Lobby/Lobby?listen";
const  FString UFDGameInstance::MainMenuLevel = "/Game/Levels/MainMenu/MainMenu";

void UFDGameInstance::Init()
{
	Super::Init();
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) 
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UFDGameInstance::OnSessionCreated);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UFDGameInstance::OnSessionDestroyed);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UFDGameInstance::OnSessionSearchComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UFDGameInstance::OnJoinComplete);
			SessionSearch = MakeShareable(new FOnlineSessionSearch());
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
	}
}

void UFDGameInstance::CreateSession(FName SessionName)
{
	if (!SessionInterface) return;

	FOnlineSessionSetting compoundSessionName;
	compoundSessionName.AdvertisementType = EOnlineDataAdvertisementType::ViaOnlineService;
	FString s = SessionName.ToString();
	compoundSessionName.Data.SetValue(s);

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;	// Max players
	SessionSettings.Settings.Add("SESSION_NAME", compoundSessionName);

	SessionInterface->CreateSession(0, SessionName, SessionSettings);

}

void UFDGameInstance::OnSessionCreated(FName Name, bool Success)
{
	if (Success) 
	{
		HostedSessionName = Name;
		UE_LOG(LogTemp, Warning, TEXT("Session %s Created"), *Name.ToString());
		UWorld* World = GetWorld();
		if (!ensure(World != nullptr)) return;
		World->ServerTravel(UFDGameInstance::LobbyLevel);	// TODO Pass SessionName to Lobby world. Would be useful for destroying
	}
}

void UFDGameInstance::OnSessionDestroyed(FName Name, bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session %s Destroyed"), *Name.ToString());
		//CreateSession(Name);
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("Session %s Was not be able destroying"), *Name.ToString());
	}
}

void UFDGameInstance::OnSessionSearchComplete(bool Success)
{
	if (Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Finding Complete"));

		OnSessionFoundEvent.Broadcast(SessionSearch->SearchResults);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Session Finding Error"));
	}
}

void UFDGameInstance::OnJoinComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Type)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Error, TEXT("Could Not resolve Address"));
		return;
	}


	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UFDGameInstance::Host(FName SessionName)
{
	if (!SessionInterface.IsValid()) return;

	auto Session = SessionInterface->GetNamedSession(SessionName);
	if (Session != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Session %s Already exist"), *Session->SessionName.ToString());
		return;
	}
	else
	{
		CreateSession(SessionName);
	}
}

void UFDGameInstance::DestroySession()
{
	if (!SessionInterface.IsValid()) return;
	SessionInterface->DestroySession(HostedSessionName);
	HostedSessionName = "";
}

void UFDGameInstance::RequestSearchSession()
{
	if (SessionSearch.IsValid())
	{
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UFDGameInstance::Join(uint32 Index)
{
	if (!SessionInterface || !SessionSearch.IsValid()) 
	{
		UE_LOG(LogTemp, Error, TEXT("SessionInterface or SessionSearch doesn't set"));
		return;
	};

	const auto& SearchResult = SessionSearch->SearchResults[Index];
	FString SessionName;
	SearchResult.Session.SessionSettings.Settings.FindRef("SESSION_NAME").Data.GetValue(SessionName);
	//Session.Session.
	SessionInterface->JoinSession(0, FName(SessionName), SearchResult);

}
