// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveIcon.h"
#include "Components/Button.h"
#include "DOE_2/PlayerState/DOEPlayerState.h"

void UMoveIcon::NativeConstruct()
{
	if (MoveIconButton == nullptr) return;
	MoveIconButton->OnClicked.AddDynamic(this, &UMoveIcon::Move);
	return;

}

void UMoveIcon::Move()
{
	ADOEPlayerState* playerstate = Cast<ADOEPlayerState>(GetOwningPlayerState());
	playerstate->SetMovingGrid(true);
}
