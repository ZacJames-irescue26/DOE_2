// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SelectionActor.generated.h"

UCLASS()
class DOE_2_API ASelectionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ASelectionActor();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Tile")
	class UStaticMeshComponent* SelectionArea;

};
