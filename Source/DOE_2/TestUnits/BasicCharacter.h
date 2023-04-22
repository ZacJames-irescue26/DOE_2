// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasicCharacter.generated.h"

UCLASS()
class DOE_2_API ABasicCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABasicCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void SetTileLocation(FVector2D Location) {TileLocation = Location;}
	FORCEINLINE int GetTeam() { return Team; }
	FORCEINLINE int GetPlayerOwner() { return PlayerOwner; }

private:
	FVector2D TileLocation;
	UPROPERTY(EditAnywhere)
	int Team;
	UPROPERTY(EditAnywhere)
	int PlayerOwner;
};
