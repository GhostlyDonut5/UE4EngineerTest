// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Robot.h"
#include "Camera/CameraComponent.h"
#include "Grabber.generated.h"

/**
 * 
 */
UCLASS()
class AIRENGINEERTEST_API AGrabber : public ARobot
{
	GENERATED_BODY()
	AGrabber();

	FQuat Look(FVector dest_point, FVector curr_loc);
	void Look_At_Target(FVector target);

	void Find_Spheres();

	void Grab_Sphere();

	void Find_Closest_Sphere();

	void Take_Screenshot();

	void Get_All_Names();

public:
	//Arrays
	TArray<AActor*> Dropper_Spheres;
	TArray<FString> Viewport_Actors;

	//Grabber Timer Handle
	FTimerHandle Grabber_Handle;

	//Misc. Variables
	AActor* Closest_Sphere;
	int spheres_documented;

	UPROPERTY(EditAnywhere, Category = "Camera")
		UCameraComponent* Camera;

protected:
	virtual void BeginPlay() override;

	virtual void Execute_Instruction(TQueue<FString>* queue) override;
};
