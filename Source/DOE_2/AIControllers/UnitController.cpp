// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitController.h"
#include "DOE_2/HexGrid/HexGridManager.h"
#include "DOE_2/TestUnits/BasicCharacter.h"

#include <Kismet/GameplayStatics.h>

void AUnitController::BeginPlay()
{
	Super::BeginPlay();
	HexManager = Cast<AHexGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AHexGridManager::StaticClass()));
	//DOE_PlayerState = GetDOEPlayerState();
}
//ADOEPlayerState* AUnitController::GetDOEPlayerState()
//{
//	if (DOE_PlayerState == nullptr)
//	{
//		DOE_PlayerState = GetPlayerState<ADOEPlayerState>();
//	}
//	return DOE_PlayerState;
//}

//ABasicCharacter* AUnitController::GetRTSCharacter()
//{
//	if (character == nullptr)
//	{
//		character = Cast<ADecay_of_environmentCharacter>(GetCharacter());
//	}
//	return character;
//
//}

void AUnitController::MoveUnit(FVector Location /*ABasicCharacter* character*/)
{
	
	if (HexManager)
	{
		character = character == nullptr ? Cast<ABasicCharacter>(GetCharacter()) : character;
		if (character)
		{

			{
				FVector2D HexSpace = HexManager->SpaceToHexTile(FVector2D(Location.X, Location.Y));
				FVector2D RoundToIntHexSpace;
				RoundToIntHexSpace.X = (int)(HexSpace.X);
				RoundToIntHexSpace.Y = (int)(HexSpace.Y);

				if (HexManager->Hash_Table_Lookup(RoundToIntHexSpace.X, RoundToIntHexSpace.Y) != nullptr)
				{
					FVector2D Hex_To_Pixel = HexManager->Hex_To_Pixel(RoundToIntHexSpace.X, RoundToIntHexSpace.Y);
					character->SetActorLocation(FVector(Hex_To_Pixel.X, Hex_To_Pixel.Y, Location.Z));
					character->SetTileLocation(RoundToIntHexSpace);
					//MoveTo(FVector(Hex_To_Pixel.X, Hex_To_Pixel.Y, Location.Z));
				}

			}

		}
	}


}