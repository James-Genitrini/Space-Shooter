// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SpaceGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYSPACESHOOTER_API ASpaceGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASpaceGameMode();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayArea");
	float MinX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayArea");
	float MaxX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayArea");
	float MinY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayArea");
	float MaxY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayArea");
	float MinXPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayArea");
	float MaxXPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayArea");
	float MinYPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayArea");
	float MaxYPawn;
	
	UFUNCTION(BlueprintCallable)
	bool IsInsidePlayArea(const FVector& Location) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> GameOverWidgetClass;

	UFUNCTION(BlueprintCallable)
	void ShowGameOver();
};