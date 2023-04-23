// Fill out your copyright notice in the Description page of Project Settings.


#include "UnitOverlay.h"
#include <Components/HorizontalBox.h>

//UUnitOverlay::UUnitOverlay(const FObjectInitializer& ObjectInitializer)
//{
//
//}

void UUnitOverlay::AddIcon(UUserWidget* Icon)
{
	if(Icon == nullptr) return;
	UnitIconHorizontalBox->AddChild(Icon);

}


void UUnitOverlay::RemoveChild(UUserWidget* Child)
{
	if (Child == nullptr) return;
	UnitIconHorizontalBox->RemoveChild(Child);
}

void UUnitOverlay::RemoveAllChildren()
{
	int count = UnitIconHorizontalBox->GetChildrenCount();
	for(int i = 0; i < count; i++)
	{
		UnitIconHorizontalBox->RemoveChildAt(i);
	}
}
