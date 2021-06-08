// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FDHealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDeathWithController,AController*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHealthChanged,float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class FOLDERDEFENSE_API UFDHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFDHealthComponent();
	float GetHealth(){return Health;}
	
	UFUNCTION(BlueprintCallable)
	bool IsDead(){return Health <=0;}
	UFUNCTION(BlueprintCallable,Category="Health")
	float GetHealthPercent() const { return Health / MaxHealth;}

	bool TryAddHealth(int32 Amount);
	FOnDeath OnDeath;
	FOnDeathWithController OnDeathWithController;
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(EditAnywhere,Category="AutoHeal")
	float HealDelay;		// Delay between damage and heal

	UPROPERTY(EditAnywhere, Category = "AutoHeal")
	float HealRate;			// Rate of executing function Heal

	UPROPERTY(EditAnywhere, Category = "AutoHeal")
	float HealAmount;

	UPROPERTY(EditAnywhere, Category = "AutoHeal")
	bool IsAutoHeal = true;
	//UPROPERTY(EditAnywhere, Category = "VFX")
	//TSubclassOf<UCameraShakeBase> CameraShake;
	UPROPERTY()
	FTimerHandle HealTimerHanlde;

	UFUNCTION(BlueprintCallable,Server, Reliable)
	void Heal();

	UFUNCTION(BlueprintCallable)
	void SetHealth(float NewHealth);
	
	protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(ClampMin="0",ClampMax="1000"))
	float MaxHealth = 100.0f;


	private:
	UPROPERTY(EditAnywhere,Replicated)
	float Health = 0.0f;
	//void PlayCameraShake();
	UFUNCTION()
	void OnAnyDamage(AActor *DamagedActor, float Damage, const class UDamageType *DamageType,
					class AController *InstigatedBy, AActor *DamageCauser);

};
