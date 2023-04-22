// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicUnit.generated.h"

UCLASS()
class DOE_2_API ABasicUnit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicUnit();

private:
	void MoveUnit(FVector2D TargetLocation);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	class AHexGridManager* HexManager;
	FVector2D UnitLocation;

};
