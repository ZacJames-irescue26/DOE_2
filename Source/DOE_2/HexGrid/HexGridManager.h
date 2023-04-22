// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HexGridManager.generated.h"


UCLASS()
class DOE_2_API AHexGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHexGridManager();

protected:
	TArray<TArray<class AHexTile*>> HexGrid2DArray;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CreateHexGrid();

	UPROPERTY(EditAnywhere, Category = HexGrid)
	int32 GridWidth;
	UPROPERTY(EditAnywhere, Category = HexGrid)
	int32 GridHeight;
	UPROPERTY(EditAnywhere, Category = HexGrid)
	float TileHorizontalOffset;
	UPROPERTY(EditAnywhere, Category = HexGrid)
	float OddRowTileHorizontalOffset;
	UPROPERTY(EditAnywhere, Category = HexGrid)
	float TileVerticleOffset;
	UPROPERTY(EditAnywhere, Category = HexGrid)
	TSubclassOf<AHexTile> GrassHexTile;
	UPROPERTY(EditAnywhere, Category = HexGrid)
	float TileRadius;
	TArray<TPair<int, int>> Axial_Direction_Vectors = { {1,0}, {1,-1}, {0,-1}, {-1,0}, {-1,1}, {0, 1} };
	TArray<TPair<AHexTile*, float>> Frontier;
	TArray<AHexTile*> Neighbours;


	//Hash table
	TArray<AHexTile*> HexHashTable;
	int32 TABLE_SIZE = GridWidth * GridHeight;




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	FVector2D SpaceToHexTile(FVector2D point);
	void Axial_To_Cube(AHexTile* Tile);
	void Oddr_To_Axial(AHexTile* Tile);
	FVector2D Axial_Round(AHexTile* Tile);
	FVector2D Hex_To_Pixel(float q, float r);
	TPair<int, int> Axial_Direction(int direction);
	FVector2D Axial_Add(AHexTile* Tile, TPair<int, int> vec);
	FVector2D Axial_Neighbor(AHexTile* Tile, int direction);
	void GetNeighbours(AHexTile* Current, TArray<AHexTile*>& OutNeighbours);
	void A_star_Search(AHexTile* Start, AHexTile* Goal, TArray<AHexTile*>& OutArray);
	double Heuristic(AHexTile* a, AHexTile* b);
	float Cost(AHexTile* From_Tile, AHexTile* To_Tile);
	TArray<AHexTile*> RetracePath(AHexTile* StartTile, AHexTile* EndTile);


	int Hash(int q, int r);
	bool Hash_Table_Insert(AHexTile* Tile);
	AHexTile* Hash_Table_Lookup(int q, int r);
	AHexTile* Hash_Table_Delete(int q, int r);
	void Print_Table();


	FORCEINLINE float GetTileHorizontalOffset() const { return TileHorizontalOffset; }
	FORCEINLINE float GetOddRowTileHorizontalOffset() const { return OddRowTileHorizontalOffset; }
	FORCEINLINE float GetTileVerticleOffset() const { return TileVerticleOffset; }

};