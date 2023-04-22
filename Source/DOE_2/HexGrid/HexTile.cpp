// Fill out your copyright notice in the Description page of Project Settings.


#include "HexTile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

AHexTile::AHexTile()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
	TileMesh->SetupAttachment(RootComponent);
}


