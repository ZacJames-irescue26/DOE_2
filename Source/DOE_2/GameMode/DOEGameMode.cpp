// Fill out your copyright notice in the Description page of Project Settings.


#include "DOEGameMode.h"
#include "DOE_2/PlayerState/DOEPlayerState.h"
#include <GameFramework/PlayerController.h>
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerState.h"

void ADOEGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ADOEPlayerState* DOEPlayerState = NewPlayer->GetPlayerState<ADOEPlayerState>();
	//DOEPlayerState->PlayerOwner = NumPlayers;
	DOEPlayerState->Server_SetPlayerOwner(NumPlayers - 1);
	UE_LOG(LogTemp, Warning, TEXT("Num of players %i"), NumPlayers);

	
}
