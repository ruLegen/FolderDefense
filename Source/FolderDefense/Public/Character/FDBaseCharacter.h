// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FDBaseCharacter.generated.h"

class UFDWeaponComponent;
class UCameraComponent;
class USpringArmComponent;
class UFDHealthComponent;

UCLASS()
class FOLDERDEFENSE_API AFDBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AFDBaseCharacter();
	UPROPERTY(EditAnywhere,Replicated)
	UFDHealthComponent *HealthComponent;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent *SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent *CameraComponent;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadWrite, Category = "Animation")
	UFDWeaponComponent* WeaponComponent;
	
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Custom Movement")
	float MaxRunSpeed;

	UPROPERTY(VisibleAnywhere, Replicated, BlueprintReadOnly, Category = "Custom Movement")
	float DefaultSpeed;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Custom Movement")
	float ImpulseMultiplier;

	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite, Category = "Custom Movement")
	bool IsWantToRun;

	UPROPERTY(Replicated)
	FRotator AimRotator;
	
	bool IsMovingForward;
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	bool IsRunning();
	
	// Return angle between ForwardVector and VelocityVector
	UFUNCTION(BlueprintCallable)
	float GetMovementDirection() const;
	
	UFUNCTION(BlueprintCallable)
	FRotator GetAimDirection();

	UFUNCTION(Server, unreliable)
	void UpdateAimDirection(FRotator ClientAimRotation);

	
	void MoveForward(float Input);
	void MoveRight(float Input);
	
	UFUNCTION(Server,Reliable)
	void StartRun();
	
	UFUNCTION(Server,Reliable)
	void StopRun();
	UFUNCTION(NetMulticast,Reliable)
	void Kill();
	void OnDeath();
	void OnHealthChanged(float health);
};
