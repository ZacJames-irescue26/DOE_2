// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionActor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASelectionActor::ASelectionActor()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SelectionArea = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SelectionArea"));
	SelectionArea->SetupAttachment(RootComponent);
	
}

