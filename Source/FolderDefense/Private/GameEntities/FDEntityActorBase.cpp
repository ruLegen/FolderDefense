#include "GameEntities/FDEntityActorBase.h"
#include "Components/FDHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

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
}

void AFDEntityActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFDEntityActorBase::OnDie(AController* KilledBy)
{
	Destroy();
	switch (Type)
	{
		case EEnityType::FILE: UE_LOG(LogTemp,Warning,TEXT("Killed FILE")); break;
		case EEnityType::FOLDER: UE_LOG(LogTemp,Warning,TEXT("Killed FOLDER")); break;
	default: ;
	}
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

