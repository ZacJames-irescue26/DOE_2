// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Overseer.generated.h"

UCLASS()
class DOE_2_API AOverseer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AOverseer();

protected:
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;

private:
	void SpawnSelectionArea();
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	class ASelectionActor* SelectionArea;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASelectionActor> SelectionAreaClass;
	UPROPERTY(VisibleAnywhere)
	class ADOEPlayerController* PlayerController;
	UPROPERTY(VisibleAnywhere)
	class AHexGridManager* HexManager;
	UPROPERTY(VisibleAnywhere)
	TArray<class AHexTile*> Path;
};
