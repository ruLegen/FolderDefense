// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widgets/FDMatchGameOverWidget.h"

#include "Components/TextBlock.h"

void UFDMatchGameOverWidget::SetText(FString Text)
{
	if(!GameResultInfo) return;

	GameResultInfo->SetText(FText::FromString(Text));
}
