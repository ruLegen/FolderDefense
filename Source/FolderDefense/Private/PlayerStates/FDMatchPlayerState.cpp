// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStates/FDMatchPlayerState.h"

#include "JsonObjectConverter.h"
#include "GameStates/FDMatchGameState.h"
#include "Net/UnrealNetwork.h"

void AFDMatchPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFDMatchPlayerState, Kills);
	DOREPLIFETIME(AFDMatchPlayerState, Deaths);
	DOREPLIFETIME(AFDMatchPlayerState, Folder);
}

void AFDMatchPlayerState::On_FolderChanges()
{
	;
}

bool AFDMatchPlayerState::IsDefeat()
{
	return  Folder.GetFiles().Num() == 0 && Folder.GetDirectories().Num() == 0;
}

void AFDMatchPlayerState::NotifyKill_Implementation(AController* KilledBy, EEntityType Type, const FString& Name)
{
	auto& Files = Folder.GetFiles();
	auto& Directories = Folder.GetDirectories();

	switch (Type)
	{
		case EEntityType::FILE:
		
			for(int i=0; i< Files.Num(); i++)
			{
				auto File = Files[i];
				if(Name.Equals(File.GetPath()+File.Name))
				{
					UE_LOG(LogTemp,Warning,TEXT("FOund File to delete"));
					Files.RemoveAt(i);
					UE_LOG(LogTemp,Warning,TEXT("Left %d Files"),Files.Num());

					break;;
				}
			}
		break;
		case EEntityType::FOLDER:
			for(int j=0; j< Directories.Num(); j++)
			{
				auto Directory = Directories[j];
				if(Name.Equals(Folder.GetPath()+ Directory))
				{
					UE_LOG(LogTemp,Warning,TEXT("FOund Directory to delete"));
					Directories.RemoveAt(j);
					UE_LOG(LogTemp,Warning,TEXT("Left %d Directories"),Directories.Num());

					break;;
				}
			}
		break;
	}
	
}

void AFDMatchPlayerState::UpdatePlayerFolder_Implementation(const FString& FolderJSON)
{
	FFolder NewFolder;
	FJsonObjectConverter::JsonObjectStringToUStruct(FolderJSON, &NewFolder, 0, 0);
	this->Folder = NewFolder;
	On_FolderChanges();
	
	if(!GetWorld())return;

	auto MatchGameState = GetWorld()->GetGameState<AFDMatchGameState>();
	if(!MatchGameState) return;

	MatchGameState->Ready();
}

void AFDMatchPlayerState::AddKill_Implementation(int Num)
{
	Kills += Num;
}

void AFDMatchPlayerState::AddDeath_Implementation(int Num)
{
	Deaths += Num;
}

void AFDMatchPlayerState::ResetScore_Implementation()
{
	Kills = 0;
	Deaths = 0;
}

void AFDMatchPlayerState::ResetBonus_Implementation()
{
	// TODO ResetBonus
}
