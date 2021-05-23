// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Robot.h"
#include "RobotInstruction.generated.h"

/*  */
UCLASS(Blueprintable)
class AIRENGINEERTEST_API URobotInstruction : public UObject
{
	GENERATED_BODY()


	FQuat Look(FVector dest_point, FVector curr_loc);

	FRotator Look_At_Target(FRotator rotation, FVector position, FVector target, float rot_speed);


public:
	URobotInstruction();	

	ARobot* Robot;

	UFUNCTION(BlueprintCallable)
		bool ExecuteInstruction();

	UFUNCTION(BlueprintCallable)
		void Move(float distance, float speed);
	
	UFUNCTION(BlueprintCallable)
		FRotator Rotate_Left(FRotator rotation, float rot_speed);

	UFUNCTION(BlueprintCallable)
		FRotator Rotate_Right(FRotator rotation, float rot_speed);

	float set_distance;
};
