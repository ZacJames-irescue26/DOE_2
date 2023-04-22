// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicUnit.h"
#include "DOE_2/HexGrid/HexGridManager.h"
#include <Kismet/GameplayStatics.h>

ABasicUnit::ABasicUnit()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}



void ABasicUnit::BeginPlay()
{
	Super::BeginPlay();
	HexManager = Cast<AHexGridManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AHexGridManager::StaticClass()));

}


void ABasicUnit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ABasicUnit::MoveUnit(FVector2D TargetLocation_in_qr)
{
	FVector2D MoveLocation = HexManager->Hex_To_Pixel(TargetLocation_in_qr.X, TargetLocation_in_qr.Y);
	UnitLocation = TargetLocation_in_qr;
	//SetActorLocation(FVector(MoveLocation.X, MoveLocation.Y));
}