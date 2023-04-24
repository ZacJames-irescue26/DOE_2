// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "DOEPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class DOE_2_API ADOEPlayerState : public APlayerState
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Replicated)
	int Team;
	UPROPERTY(EditAnywhere, Replicated)
	int PlayerOwner;
	UPROPERTY(VisibleAnywhere)
	bool bMovingGrid;
	UPROPERTY(VisibleAnywhere)
	bool bAttackGrid;
public:
	UFUNCTION(Server, Reliable)
	void Server_SetPlayerOwner(int Player);
protected:
public:

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FORCEINLINE void SetPlayerOwner(int _Owner) {PlayerOwner = _Owner;}
	FORCEINLINE void SetMovingGrid(bool IsMoving) { bMovingGrid = IsMoving; }
	FORCEINLINE void SetAttackGrid(bool IsAttacking) { bAttackGrid = IsAttacking; }

	FORCEINLINE int GetTeam() { return Team; }
	FORCEINLINE int GetPlayerOwner() { return PlayerOwner; }
	FORCEINLINE bool GetMovingGrid() { return bMovingGrid; }
	FORCEINLINE bool GetAttackGrid() { return bAttackGrid; }

};
