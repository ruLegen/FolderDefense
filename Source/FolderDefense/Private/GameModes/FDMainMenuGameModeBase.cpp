// Fill out your copyright notice in the Description page of Project Settings.


#include "GameModes/FDMainMenuGameModeBase.h"

#include "PlayerControllers/FDMainMenuPlayerController.h"
#include "UI/FDMainMenuHUD.h"

AFDMainMenuGameModeBase::AFDMainMenuGameModeBase() {
	HUDClass = AFDMainMenuHUD::StaticClass();
	PlayerControllerClass = AFDMainMenuPlayerController::StaticClass();
}
