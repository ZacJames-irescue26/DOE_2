// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexTile.generated.h"

UENUM()
enum class EHexTileType : uint8
{
	INVALID,
	GRASS,
	WATER,
	MAX UMETA(Hidden)
};
UCLASS()
class DOE_2_API AHexTile : public AActor
{
	GENERATED_BODY()

public:	
	AHexTile();

	UPROPERTY(VisibleInstanceOnly, Category = "Tile")
	FIntPoint TileIndex;
	UPROPERTY(VisibleInstanceOnly, Category = "Tile")
	int q; 
	UPROPERTY(VisibleInstanceOnly, Category = "Tile")
	int r; 		
	UPROPERTY(VisibleInstanceOnly, Category = "Tile")
	int s;
	UPROPERTY(VisibleInstanceOnly, Category = "Tile")
	int index;

	UPROPERTY(VisibleAnywhere)
	float gCost = FLT_MAX;	// distance between Current node and start node
	UPROPERTY(VisibleAnywhere)
	float hCost = FLT_MAX;	// estimated distance from current node to end node
	UPROPERTY(VisibleAnywhere)
	float fCost = FLT_MAX;	// total cost of node
	UPROPERTY(VisibleAnywhere)
	AHexTile* Parent = nullptr;


protected:
	UPROPERTY(EditDefaultsOnly, Category = "Tile")
	EHexTileType TileType;

	UPROPERTY(EditDefaultsOnly, Category = "Tile")
	class UStaticMeshComponent* TileMesh;
	
};
