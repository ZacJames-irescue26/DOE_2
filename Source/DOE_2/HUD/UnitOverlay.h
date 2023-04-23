// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "UnitOverlay.generated.h"

/**
 * 
 */
UCLASS()
class DOE_2_API UUnitOverlay : public UUserWidget
{
	GENERATED_BODY()
public:
	/*UUnitOverlay(const FObjectInitializer& ObjectInitializer);*/

	void AddIcon(UUserWidget* Icon);
	void RemoveChild(UUserWidget* Child);
	void RemoveAllChildren();
private:

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* UnitIconHorizontalBox;
};
