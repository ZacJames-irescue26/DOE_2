// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DOEPlayerController.generated.h"


UCLASS()
class DOE_2_API ADOEPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ADOEPlayerController();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FHitResult Hit;

private:
	virtual void OnPossess(APawn* InPawn) override;
	void SpawnSelectionArea();
	void LeftMouseButtonDown();
	void LeftMouseButtonReleased();
	void RightClick();
	void SelectUnit();

	void UnitUI();

	UFUNCTION(Server, Reliable)
	void MoveUnit(FVector Location);
	void CreateUnitUI(int UnitId);


	UFUNCTION(Server, Reliable)
	void ServerSelectUnit(AActor* SelectingCharacter);
protected:
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void BeginPlay() override;
private:
	FVector MousePos;
	class ASelectionActor* SelectionArea;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASelectionActor> SelectionAreaClass;
	bool leftMouseButton = false;
	
	UPROPERTY(Replicated, VisibleAnywhere)
	AActor* SelectedUnit;
	UPROPERTY()
	class AOverseer* overseerer;
	class ADOEPlayerState* playerState;

	bool bIsSpawnedMoving = false;
	bool bIsSpawnedAttacking = false;


	/******UI****************/
	UPROPERTY()
	TSubclassOf<UUserWidget> UnitOverlayTemplate;
	UPROPERTY()
	class UUnitOverlay* UnitOverlay;
	
	UPROPERTY()
	TSubclassOf<UUserWidget> MoveIconTemplate;
	UPROPERTY()
	class UMoveIcon* MoveIcon;
	UPROPERTY()
	TSubclassOf<UUserWidget> AttackIconTemplate;
	UPROPERTY()
	class UAttackIcon* AttackIcon;

public:
	FORCEINLINE FVector GetMousePos() const { return MousePos; }
	AOverseer* GetOverseerer();
};
