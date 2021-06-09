// Fill out your copyright notice in the Description page of Project Settings.




#include "FileUtils.h"

#include "GenericPlatform/GenericPlatformOutputDevices.h"
#include "Misc/OutputDeviceDebug.h"


bool UFileUtils::OpenFileDialog(FString& Directory)

{
	 FString AbsoluteGameContentDir = FPaths::ConvertRelativePathToFull(FPaths::ProjectContentDir());
	 IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	 if (!DesktopPlatform)
	 	return false;
	 if(!GEngine)
	 	return false;
	
	 FString StartDirectory = AbsoluteGameContentDir;
	 const void* ParentWindowHandle = GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle();
	for (;;)
	{
		 if (DesktopPlatform->OpenDirectoryDialog(ParentWindowHandle, FString(TEXT("Choose a directory")), StartDirectory, Directory))
		 {
		 	return true;					
		 } else
		 {
		 	return false;
		 }
		 break;
	}
}

TArray<FString> UFileUtils::FindDirectories(const FString SearchDirectory,bool bAbsolute)
{
	FString SearchDir = SearchDirectory;
	SearchDir.Append("/*");
	TArray<FString> output;
	
	FFileManagerGeneric::Get().FindFiles(output,*SearchDir,false,true);
	if(bAbsolute)
	{
		for (int i =0; i< output.Num();i++)
		{
			output[i] = SearchDirectory+"/"+output[i];
		}
	}
	return  output;
}

TArray<FString> UFileUtils::FindFiles(const FString SearchDirectory,bool bAbsolute)
{
	FString SearchDir = SearchDirectory;
	SearchDir.Append("/*");
	
	TArray<FString> output;
	FFileManagerGeneric::Get().FindFiles(output,*SearchDir,true,false);
	if(bAbsolute)
	{
		for (int i =0; i< output.Num();i++)
		{
			output[i] = SearchDirectory+"/"+output[i];
		}
	}
	return  output;
}

int UFileUtils::TestSomeFun(FString URL)
{
	auto device = FOutputDeviceDebug();
	return system(TCHAR_TO_ANSI(*URL));
}

