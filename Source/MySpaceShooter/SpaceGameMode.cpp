// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceGameMode.h"

ASpaceGameMode::ASpaceGameMode()
{
	// MinX = -2000.f; Spawn
	// MaxX = 600.f;
	// MinY = -2200.f;
	// MaxY = 2200.f;
	MinX = -2400.f;
	MaxX = 1200.f;
	MinY = -2600.f;
	MaxY = 3000.f;
}

bool ASpaceGameMode::IsInsidePlayArea(const FVector& Location) const
{
	return Location.X >= MinX && Location.X <= MaxX &&
		   Location.Y >= MinY && Location.Y <= MaxY;
}