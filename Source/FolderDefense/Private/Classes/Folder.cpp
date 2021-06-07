// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/Folder.h"
#include "Classes/File.h"
#include "FileUtils.h"
#include "Net/UnrealNetwork.h"

FFolder::FFolder()
{
}


void FFolder::Init(const FString& FPath,  int CurrentDepth)
{
	this->Path = FPath;
	this->Name = FPaths::GetBaseFilename(this->Path,true);
	this->Depth = CurrentDepth;
}


FFolder FFolder::CreateInstace(FString FPath, int DepthLimit)
{
		int CurrentDepth = 0;
		FFolder Folder = FFolder();
		Folder.Init(FPath,CurrentDepth);
		Folder.InitializeDirectories();
		Folder.InitializeFiles();
		if(DepthLimit == 0)
			return Folder;
		return Folder;
}



bool FFolder::InitializeDirectories()
{
	auto DirectoryNames = UFileUtils::FindDirectories(GetPath(),true);
	for (auto Directory : DirectoryNames)
	{
		FFolder SubFolder=FFolder();
		SubFolder.Init(Directory,this->Depth+1);
		if(SubFolder.Name.Len() <= 1)
		{
			continue;
		}
		GetDirectories().Push(SubFolder.Name);
	}
	bIsDirectoriesInitialized = true;
	return bIsDirectoriesInitialized;
}

bool FFolder::InitializeFiles()
{
	auto FileNames = UFileUtils::FindFiles(GetPath(),false);
	for (auto FileName : FileNames)
	{
		FFile File = FFile();
		File.Init(FileName,this->Path);
		Files.Push(File);
	}
	bIsFilesInitialized = true;
	return bIsFilesInitialized;
}