// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "DOEGameMode.generated.h"

/**
 * 
 */
UCLASS()
class DOE_2_API ADOEGameMode : public AGameMode
{
	GENERATED_BODY()
	virtual void PostLogin(class APlayerController* NewPlayer) override;
	
};
