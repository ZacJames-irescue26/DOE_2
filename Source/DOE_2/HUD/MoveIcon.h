// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MoveIcon.generated.h"

/**
 * 
 */
UCLASS()
class DOE_2_API UMoveIcon : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UFUNCTION()
	void Move();
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* MoveIconButton;
};
