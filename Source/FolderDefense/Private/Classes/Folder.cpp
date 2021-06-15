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


bool FFolder::operator==(const FFolder& Folder)
{
	return GetPath().Equals(Folder.GetPath()) && Name.Equals(Folder.Name);
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
	int MaxFolders = 20;
	auto DirectoryNames = UFileUtils::FindDirectories(GetPath(),true);
	for (auto Directory : DirectoryNames)
	{
		FFolder SubFolder=FFolder();
		SubFolder.Init(Directory,this->Depth+1);
		if(SubFolder.Name.Len() <= 1)
		{
			continue;
		}
		if( MaxFolders < 0)
			break;
		GetDirectories().Push(SubFolder.Name);
		MaxFolders--;
	}
	bIsDirectoriesInitialized = true;
	return bIsDirectoriesInitialized;
}

bool FFolder::InitializeFiles()
{
	int MaxFiles = 20;
	auto FileNames = UFileUtils::FindFiles(GetPath(),false);
	for (auto FileName : FileNames)
	{
		FFile File = FFile();
		File.Init(FileName,this->Path);
		Files.Push(File);
		if( MaxFiles < 0)
			break;
		MaxFiles--;
	}
	bIsFilesInitialized = true;
	return bIsFilesInitialized;
}