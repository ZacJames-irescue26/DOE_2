// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "UnitController.generated.h"

/**
 * 
 */
UCLASS()
class DOE_2_API AUnitController : public AAIController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	
public:
	void MoveUnit(FVector Location);
	//class ADOEPlayerState* GetDOEPlayerState();
private:
	class AHexGridManager* HexManager;
	class ABasicCharacter* character;
	//class ADOEPlayerState* DOE_PlayerState;
};
