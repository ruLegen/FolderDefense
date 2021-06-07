// Fill out your copyright notice in the Description page of Project Settings.


#include "Aninations/FDAnimNotify.h"

void UFDAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	OnNotify.Broadcast(MeshComp);
	Super::Notify(MeshComp, Animation);
}
