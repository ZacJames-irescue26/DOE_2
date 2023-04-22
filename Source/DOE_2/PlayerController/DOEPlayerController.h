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
	//ADOEPlayerController();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	FHitResult Hit;

private:
	virtual void OnPossess(APawn* InPawn) override;
	void SpawnSelectionArea();
	void LeftMouseButtonDown();
	void LeftMouseButtonReleased();
	void RightClick();
	void SelectUnit();
	UFUNCTION(Server, Reliable)
	void MoveUnit(FVector Location);

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
	class AOverseer* overseerer;
	class ADOEPlayerState* playerState;
public:
	FORCEINLINE FVector GetMousePos() const { return MousePos; }
	AOverseer* GetOverseerer();
};
