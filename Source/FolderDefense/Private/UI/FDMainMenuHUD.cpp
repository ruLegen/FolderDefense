// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/FDMainMenuHUD.h"

#include "Blueprint/UserWidget.h"
#include "UI/Widgets/FDMainMenuWidget.h"

void AFDMainMenuHUD::BeginPlay()
{
	if (MenuWidgetClass) {
		MenuWidget = CreateWidget<UFDMainMenuWidget>(GetWorld(), MenuWidgetClass);
		if (MenuWidget)
		{
			//MenuWidget->OnJoinRequest.AddUObject(this, &AWDMainMenuHUD::OnJoinRequest);
			MenuWidget->AddToViewport();
		}
	}
}
