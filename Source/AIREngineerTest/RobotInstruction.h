// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RobotInstruction.generated.h"

/*  */
UCLASS(Blueprintable)
class AIRENGINEERTEST_API URobotInstruction : public UObject
{
	GENERATED_BODY()


	TArray<FString> instruction_list;

public:
	URobotInstruction();

	//Modifiable variables.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robot Movement")
	float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robot Movement")
	float travel_distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robot Movement")
		float rotation_speed;



	//Callable_Functions

	UFUNCTION(BlueprintCallable)
		bool Populate_Instructions();

	UFUNCTION(BlueprintCallable)
		bool ExecuteInstruction();

	UFUNCTION(BlueprintCallable)
		void Move();
	
	UFUNCTION(BlueprintCallable)
		void Rotate_Left();

	UFUNCTION(BlueprintCallable)
		void Rotate_Right();

	TQueue<FString> instructions;
};
