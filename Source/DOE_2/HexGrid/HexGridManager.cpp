// Fill out your copyright notice in the Description page of Project Settings.


#include "HexGridManager.h"
#include "HexTile.h"
// Sets default values
AHexGridManager::AHexGridManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GridWidth = 20;
	GridHeight = 20;
	TileHorizontalOffset = FMath::Sqrt(3) * 100;
	OddRowTileHorizontalOffset = (FMath::Sqrt(3) * 100) / 2;
	TileVerticleOffset = 200 * 0.75;
	TileRadius = 100.0f;
	TABLE_SIZE = GridWidth * GridHeight;
	HexHashTable.Init(nullptr, TABLE_SIZE);

}

// Called when the game starts or when spawned
void AHexGridManager::BeginPlay()
{
	Super::BeginPlay();
	CreateHexGrid();
	Print_Table();
	
}

void AHexGridManager::CreateHexGrid()
{
	HexGrid2DArray.SetNumZeroed(GridWidth);
	for (int32 i = 0; i < HexGrid2DArray.Num(); i++)
	{
		HexGrid2DArray[i].SetNumZeroed(GridHeight);
	}

	for (int32 y = 0; y < GridHeight; y++)
	{
		for (int32 x = 0; x < GridWidth; x++)
		{
			const bool OddRow = y % 2 == 1;
			const float xPos = OddRow ? (x * TileHorizontalOffset) + OddRowTileHorizontalOffset : (x * TileHorizontalOffset);
			const float yPos = y * TileVerticleOffset;
			//FVector2D Pos;
			//Pos.X = x;
			//Pos.Y = y * 0.86f;
			//if(y % 2 == 0)
			//{
			//	Pos.X += 0.5f;
			//}
			TSubclassOf<AHexTile> TileToSpawn = GrassHexTile;
			if (TileToSpawn == nullptr) return;
			AHexTile* NewTile = GetWorld()->SpawnActor<AHexTile>(TileToSpawn, FVector(FIntPoint(xPos, yPos)), FRotator::ZeroRotator);
			if (NewTile)
			{
				NewTile->TileIndex = FIntPoint(x, y);
				Oddr_To_Axial(NewTile);
				Hash_Table_Insert(NewTile);
				//Axial_To_Cube(NewTile);
				NewTile->SetActorLabel(FString::Printf(TEXT("Tile_%d_%d"), NewTile->q, NewTile->r));
				HexGrid2DArray[x][y] = NewTile;
			}
		}
	}
}

// Called every frame
void AHexGridManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector2D AHexGridManager::SpaceToHexTile(FVector2D point)
{
	/*const bool OddRow = 200 % 2 == 1;
	const float xPos = OddRow ? (x * TileHorizontalOffset) + OddRowTileHorizontalOffset : (x * TileHorizontalOffset);
	const float yPos = y * TileVerticleOffset;*/

	/*return std::floor(value / size) * size;*/
	/*float X = ((FMath::Sqrt(3)/3 * point.X - 1.0/3.0 * point.Y) / size);
	float Y = ((2.0f/3.0f * point.Y) / size);
	return FVector2D(X, Y);*/

	float q = FMath::RoundHalfToZero((FMath::Sqrt(3)/3 * point.X - 1.0f/3 * point.Y)/ TileRadius);
	float r = FMath::RoundHalfToZero((2.0f/3 * point.Y) / TileRadius);
	return FVector2D(q, r);

}

/*
* Axial: Col = q ROws = r
*
* Cube: COl = q ROws = r z = s
* 
* 
*/

// needs to be in axial already 
void AHexGridManager::Axial_To_Cube(AHexTile* Tile)
{
	Tile->s = -Tile->q-Tile->r;
}

void AHexGridManager::Oddr_To_Axial(AHexTile* Tile)
{
	Tile->q = Tile->TileIndex.X - (Tile->TileIndex.Y - (Tile->TileIndex.Y&1)) / 2;
	Tile->r = Tile->TileIndex.Y;
}

FVector2D AHexGridManager::Hex_To_Pixel(float q, float r)
{
	float x = TileRadius * (FMath::Sqrt(3) * q + FMath::Sqrt(3)/2 * r);
	float y = TileRadius * (3.0f/2 * r);
	return FVector2D(x, y);
}

FVector2D AHexGridManager::Axial_Round(AHexTile* Tile)
{
	//const float XGrid = FMath::RoundHalfFromZero(Tile->q);
	const float XGrid = Tile->q;
	const float YGrid = Tile->r;

	int X = Tile->q;
	int Y = Tile->r;
	X -= XGrid;
	Y -= YGrid;
	float dx = FMath::RoundHalfToZero(X + 0.5*Y) * (X * X >= Y * Y);
	float dy = FMath::RoundHalfToZero(Y + 0.5 * X) * (X * X < Y * Y);

	return FVector2D(XGrid + dx, YGrid + dy);
}
TPair<int, int> AHexGridManager::Axial_Direction(int direction)
{
	return Axial_Direction_Vectors[direction];
}

FVector2D AHexGridManager::Axial_Add(AHexTile* Tile, TPair<int, int> vec)
{
	int q = Tile->q;
	int r = Tile->r;
	q += vec.Key;
	r += vec.Value;
	return FVector2D(q, r);
}

FVector2D AHexGridManager::Axial_Neighbor(AHexTile* Tile, int direction)
{
	return Axial_Add(Tile, Axial_Direction(direction));
}

int AHexGridManager::Axial_Distance(AHexTile* Start, AHexTile* End)
{
	if (Start && End)
	{
		return (FMath::Abs(Start->q - End->q) + FMath::Abs(Start->q + Start->r - End->q - End->r) + FMath::Abs(Start->r - End->r))/2;
	}
	return 0;
}

int AHexGridManager::Hash(int q, int r)
{
	return FMath::Abs((q + r)*2 + (q - r)*10) % TABLE_SIZE;
}

bool AHexGridManager::Hash_Table_Insert(AHexTile* Tile)
{
	if (Tile == nullptr) return false;
	int index = Hash(Tile->q, Tile->r);
	Tile->index = index;
	for (int i = 0; i < HexHashTable.Num(); i++)
	{
		int Try = (i + index) % HexHashTable.Num();
		if (HexHashTable[Try] == nullptr)
		{
			HexHashTable[Try] = Tile;
			return true;
		}
	}
	return false;
}

AHexTile* AHexGridManager::Hash_Table_Lookup(int q, int r)
{
	int index = Hash(q, r);
	for (int i = 0; i < HexHashTable.Num(); i++)
	{
		int Try = (i + index) % HexHashTable.Num();
		if (HexHashTable[Try] != nullptr && q == HexHashTable[Try]->q && r == HexHashTable[Try]->r)
		{
			return HexHashTable[Try];
		}
	}
	return nullptr;
}

AHexTile* AHexGridManager::Hash_Table_Delete(int q, int r)
{
	int index = Hash(q, r);
	for (int i = 0; i < HexHashTable.Num(); i++)
	{
		int Try = (i + index) % HexHashTable.Num();
		if (HexHashTable[Try] != nullptr && q == HexHashTable[Try]->q && r == HexHashTable[Try]->r)
		{
			AHexTile* tmp = HexHashTable[Try];
			HexHashTable[Try] = nullptr;
			return tmp;
		}
	}
	return nullptr;
}

void AHexGridManager::Print_Table()
{
	for (int i = 0; i < HexHashTable.Num(); i++)
	{
		if (HexHashTable[i] == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("\t %d \t---"), i);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("\t%d\tindex: %d \t%d_%d"), i, HexHashTable[i]->index, HexHashTable[i]->q, HexHashTable[i]->r);
		}
	}
}

double AHexGridManager::Heuristic(AHexTile* a, AHexTile* b)
{
	if(a ==nullptr || b == nullptr) return 0;
	FVector2D First = Hex_To_Pixel(a->q, a->r);
	FVector2D Second = Hex_To_Pixel(b->q, b->r);
	return FMath::Abs(First.X - Second.X) + FMath::Abs(First.Y - Second.Y);
}

/*
* Axial: Col = q ROws = r
*
* Cube: COl = q ROws = r z = s
*
*
*/

float AHexGridManager::Cost(AHexTile* From_Tile, AHexTile* To_Tile)
{
	if (From_Tile != nullptr && To_Tile != nullptr)
	{
		int dstQ = FMath::Abs(From_Tile->q - To_Tile->q);
		int dstR = FMath::Abs(From_Tile->r - To_Tile->r);
		if (dstQ > dstR)
		{
			return 14 * dstR + 10 * (dstQ - dstR);
		}
		else
		{
			return 14 * dstQ + 10 * (dstR - dstQ);
		}
	}

	return FLT_MAX;

}

void AHexGridManager::A_star_Search(AHexTile* Start, AHexTile* Goal, TArray<AHexTile*>& OutArray)
{
	/*Frontier.Empty();
	Came_From.Empty();
	Cost_So_Far.Empty();


	Frontier.Push({Start, 0.0f});
	Came_From.Push(Start);
	Cost_So_Far.FindOrAdd(Start, 0);
	while (!(Frontier.Num() <= 0))
	{
		TPair<AHexTile*, float> current = Frontier.Pop();
		if (current.Key == Goal)
		{
			Came_From.Push(Goal);
			break;
			
		}
		GetNeighbours(current.Key, Neighbours);
		for (AHexTile* next : Neighbours)
		{
			float gcost = Cost_So_Far.FindOrAdd(current.Key) + Cost(current.Key, next);
			Cost_So_Far.FindOrAdd(next, 0);
			if (coNew_Cost < Cost_So_Far[next])
			{
				Cost_So_Far.FindOrAdd(next) = New_Cost;
				float priority = New_Cost + Heuristic(next, Goal);
				Frontier.Push({next, priority});
				Came_From.Push(current.Key);
			}
		}
	}*/
	// based on SebLague implementation
	TArray<AHexTile*> OpenSet;
	TArray<AHexTile*> ClosedSet;

	OpenSet.Add(Start);
	while (OpenSet.Num() > 0)
	{
		AHexTile* Tile = OpenSet[0];
		for (int i = 0; i < OpenSet.Num(); i++)
		{
			if(OpenSet[i] == nullptr) break;
			if (OpenSet[i]->fCost < Tile->fCost || OpenSet[i]->fCost == Tile->fCost)
			{
				if (OpenSet[i]->hCost < Tile->hCost)
				{
					Tile = OpenSet[i];
				}
			}
		}
		OpenSet.Remove(Tile);
		ClosedSet.Add(Tile);
		if (Tile == Goal)
		{
			OutArray = ClosedSet;
			return;
		}
		GetNeighbours(Tile, Neighbours);
		for (AHexTile* next : Neighbours)
		{
			if(next == nullptr) continue;
			// add walkable here if()
			int NewCostToNeighbour = Tile->gCost + Cost(Tile, next);
			if (NewCostToNeighbour < next->gCost || !OpenSet.Contains(next))
			{
				next->gCost = NewCostToNeighbour;
				next->hCost = Cost(next, Goal);
				next->Parent = Tile;

				if (!OpenSet.Contains(next))
				{
					OpenSet.Add(next);
				}
			}
		}
	}

}

TArray<AHexTile*> AHexGridManager::RetracePath(AHexTile* StartTile, AHexTile* EndTile)
{
	//TArray<AHexTile*> Path;
	//TArray<AHexTile*> CorrectPath;
	//AHexTile* CurrentTile = EndTile;
	//while (CurrentTile != StartTile || CurrentTile->Parent != nullptr)
	//{
	//	Path.Add(CurrentTile);
	//	CurrentTile = CurrentTile->Parent;
	//}
	//for (int i = Path.Num() -1; i >= 0; i--)
	//{
	//	CorrectPath.Add(Path[i]);
	//}
	//return CorrectPath;
	//Causes memory leak as repeated parent links
	TArray<AHexTile*> EmptyArray;
	return EmptyArray;
}

void AHexGridManager::GetNeighbours(AHexTile* Current, TArray<AHexTile*>& OutNeighbours)
{
	OutNeighbours.Empty();
	if (Current != nullptr)
	{
		for (int i = 0; i < Axial_Direction_Vectors.Num(); i++)
		{
			FVector2D NeighbourPositions = Axial_Neighbor(Current, i);
			AHexTile* Neighbour = Hash_Table_Lookup(NeighbourPositions.X, NeighbourPositions.Y);
			OutNeighbours.Add(Neighbour);
		}
	}
}


