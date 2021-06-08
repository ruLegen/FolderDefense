#include "GameEntities/FDEntityActorBase.h"
#include "Components/FDHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

AFDEntityActorBase::AFDEntityActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility,ECollisionResponse::ECR_Block);
	SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	RootComponent = SkeletalMeshComponent;

	TextComponent = CreateDefaultSubobject<UTextRenderComponent>("Text");
	TextComponent->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);
	
	HealthComponent = CreateDefaultSubobject<UFDHealthComponent>("Health");
	HealthComponent->SetHealth(Data.DefaultHealth);
}

void AFDEntityActorBase::BeginPlay()
{
	Super::BeginPlay();
	this->TextComponent->SetText(FText::FromString(EntityName));
	if(HasAuthority())
		HealthComponent->OnDeathWithController.AddUObject(this, &AFDEntityActorBase::OnDie);
}

void AFDEntityActorBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AFDEntityActorBase, HealthComponent);
}

void AFDEntityActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFDEntityActorBase::OnDie(AController* KilledBy)
{
	SetLifeSpan(1);
	UE_LOG(LogTemp,Warning,TEXT("Killed by %s"),*KilledBy->GetName());
}

void AFDEntityActorBase::SetEntityName(const FString& EName)
{
	this->EntityName = EName;
}


