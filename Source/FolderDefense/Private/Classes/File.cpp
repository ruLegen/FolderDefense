// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/File.h"
#include "Classes/Folder.h"


void UFile::Init(FString FileName, UFolder* ParentFolder)
{
	this->Name = FileName;
	this->Type = FPaths::GetExtension(this->Name);
	this->Parent = TWeakObjectPtr<UFolder>(ParentFolder);
	if(this->Parent.IsValid())
	{
		this->Path = *Parent->GetPath()+FString("/");
	}
}
