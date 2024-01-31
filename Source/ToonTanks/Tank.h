// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BasePawn.h"
#include "InputAction.h"
#include "Tank.generated.h"

/**
 * 
 */
UCLASS()
class TOONTANKS_API ATank : public ABasePawn
{
	GENERATED_BODY()

public:
	ATank();
	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* InputMove;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* InputTurn;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	UInputAction* InputFire;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enhanced Input")
	class UInputMappingContext* InputMapping;
	
private:
	UPROPERTY(VisibleAnywhere, Category="Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category="Components")
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, Category="Movement")
	float Speed = 400.f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float TurnRate = 100.f;

	void Move (const FInputActionValue& Value);

	void Turn (const FInputActionValue& Value);

	APlayerController* PlayerControllerRef;
};
