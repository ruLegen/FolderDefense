// Fill out your copyright notice in the Description page of Project Settings.


#include "Classes/Folder.h"
#include "Classes/File.h"
#include "FileUtils.h"

void UFolder::Init(const FString& FolderPath, UFolder* ParentFolder, int CurrentDepth)
{
	this->Path = FolderPath;
	this->Name = FPaths::GetBaseFilename(this->Path,true);
	this->Depth = CurrentDepth;
	this->Parent = TWeakObjectPtr<UFolder>(ParentFolder);
}

UFolder* UFolder::CreateInstace(FString FolderPath,int DepthLimit)
{
	int CurrentDepth = 0;
	UFolder* Folder = NewObject<UFolder>();
	Folder->Init(FolderPath,nullptr,CurrentDepth);
	Folder->InitializeDirectories();
	Folder->InitializeFiles();
	if(DepthLimit == 0)
		return Folder;
	TQueue<UFolder*> SubFolders;
	// Init Queue with 1-st level directories
	for (auto Directory : Folder->GetDirectories())
	{
		SubFolders.Enqueue(Directory);
	}
	while (!SubFolders.IsEmpty())
	{
		UFolder* CurrentSubFolder;
		SubFolders.Dequeue(CurrentSubFolder);
		if(!CurrentSubFolder)
			continue;
		if(CurrentSubFolder->Depth < DepthLimit)
		{
			CurrentSubFolder->InitializeDirectories();
			CurrentSubFolder->InitializeFiles();
			for (auto Directory : CurrentSubFolder->GetDirectories())
			{
				SubFolders.Enqueue(Directory);
			}
		}
	}
	return Folder;
}

bool UFolder::InitializeDirectories()
{
	auto DirectoryNames = UFileUtils::FindDirectories(GetPath(),true);
	for (auto Directory : DirectoryNames)
	{
		UFolder* SubFolder = NewObject<UFolder>();
		SubFolder->Init(Directory,this,this->Depth+1);
		if(SubFolder->Name.Len() <= 1)
		{
			SubFolder->ConditionalBeginDestroy();
			SubFolder = nullptr;
			continue;
		}
		GetDirectories().Push(SubFolder);
	}
	bIsDirectoriesInitialized = true;
	return bIsDirectoriesInitialized;
}

bool UFolder::InitializeFiles()
{
	auto FileNames = UFileUtils::FindFiles(GetPath(),false);
	for (auto FileName : FileNames)
	{
		UFile* File = NewObject<UFile>();
		File->Init(FileName,this);
		Files.Push(File);
	}
	bIsFilesInitialized = true;
	return bIsFilesInitialized;
}
