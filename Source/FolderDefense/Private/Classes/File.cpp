// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/File.h"
#include "Classes/Folder.h"
#include "Net/UnrealNetwork.h"


void FFile::Init(FString FileName,FString FolderPath)
{
	this->Name = FileName;
	this->Type = FPaths::GetExtension(this->Name);
	this->Path =Path;
}
