#include "GameEntities/FDEntityActorBase.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/FDHealthComponent.h"
#include "GameFramework/Actor.h"
#include "GameStates/FDMatchGameState.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PlayerStates/FDMatchPlayerState.h"

AFDEntityActorBase::AFDEntityActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("123");
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComponent->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);

	TextComponent = CreateDefaultSubobject<UTextRenderComponent>("Text");
	TextComponent->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
	TextComponent->SetIsReplicated(true);
	
	HealthComponent = CreateDefaultSubobject<UFDHealthComponent>("Health");
	HealthComponent->SetHealth(Data.DefaultHealth);
}

void AFDEntityActorBase::BeginPlay()
{
	Super::BeginPlay();
	UpdateEnityName();
	if(HasAuthority())
		HealthComponent->OnDeathWithController.AddUObject(this, &AFDEntityActorBase::OnDie);
}

void AFDEntityActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFDEntityActorBase, HealthComponent);
	DOREPLIFETIME(AFDEntityActorBase, TextComponent);
	DOREPLIFETIME(AFDEntityActorBase, Id);
	DOREPLIFETIME(AFDEntityActorBase, ParentFolderClass);
	
}

void AFDEntityActorBase::SetId_Implementation(int32 NewId)
{
	this->Id = NewId;
}

void AFDEntityActorBase::SetParentFolderClass_Implementation(const FFolder& Folder)
{
	this->ParentFolderClass = Folder;
}

void AFDEntityActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFDEntityActorBase::SpawnNiagaraEffect_Implementation(UNiagaraSystem* NiagaraSystem,FLinearColor Color)
{
	UE_LOG(LogTemp,Warning,TEXT("Niagara tryed"));
	if(!NiagaraSystem || !GetWorld()) return;
	UE_LOG(LogTemp,Warning,TEXT("Niagara Spawned"));
	auto Effect = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(),NiagaraSystem,GetActorLocation(),GetActorRotation());
	Effect->SetColorParameter("Color",Color);
}

void AFDEntityActorBase::OnDie(AController* KilledBy)
{
	switch (Type)
	{
		case EEntityType::FILE:
			SpawnNiagaraEffect(NiagaraDieEffect,FLinearColor::MakeRandomColor());

		break;
		case EEntityType::FOLDER:
			SpawnNiagaraEffect(NiagaraDieEffect,FLinearColor::Blue);
			break;
	default: ;
	}
	if(!GetWorld()) return;
		
	auto MatchGameState = GetWorld()->GetGameState<AFDMatchGameState>();
	if(!MatchGameState) return;

	MatchGameState->NofifyKill(ParentFolderClass,KilledBy,Type,EntityName);
	DeleteEntity(Type,EntityName);
	Destroy();
}

void AFDEntityActorBase::DeleteEntity_Implementation(EEntityType EntityType, const FString& Path)
{
	FString NewPath = Path.Replace(TEXT("/"),TEXT("\\"));
	FString cmd;
	switch (EntityType) {
		case EEntityType::FOLDER: cmd.Append("rmdir "); break;
		case EEntityType::FILE: cmd.Append("del "); break;
	}
	cmd.Append("\"");
	cmd.Append(NewPath);
	cmd.Append("\" /S /Q");
	;
	system(TCHAR_TO_ANSI(*cmd));	// TODO uncomment on release
}

void AFDEntityActorBase::SetMaterialInstance_Implementation(UMaterialInstance* MaterialInstance)
{
	if(!MaterialInstance || !MaterialInstance ) return;
	MeshComponent->SetMaterial(0,MaterialInstance);
}

void AFDEntityActorBase::UpdateEnityName_Implementation()
{
	this->TextComponent->SetText(FText::FromString(EntityName));
}

void AFDEntityActorBase::SetEntityName_Implementation(const FString& EName)
{
	this->EntityName = EName;

}

