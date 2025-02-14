// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "RobotInstruction.generated.h"

/*  */
UCLASS(Blueprintable)
class AIRENGINEERTEST_API URobotInstruction : public UObject
{
	GENERATED_BODY()

	void Queue_Instructions(TQueue<FString>* instructions, TArray<FString> stored_list);

public:
	URobotInstruction();

	TQueue<FString> grabber_instructions;
	TQueue<FString> dropper_instructions;

	//Modifiable blueprint variables.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robot Movement")
		float speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robot Movement")
		float travel_distance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Robot Movement")
		float rotation_speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instructions")
		TArray<FString> dropper_list;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Instructions")
		TArray<FString> grabber_list;


	//Callable Blueprint Functions
	UFUNCTION(BlueprintCallable)
		bool Get_Instructions_From_Text_File(FString FileName, FString Desired_Robot);

	UFUNCTION(BlueprintCallable)
		bool Get_Instructions_From_Array(TArray<FString> stored_list);
};