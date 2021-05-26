// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AIREngineerTestGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class AIRENGINEERTEST_API AAIREngineerTestGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAIREngineerTestGameModeBase();

	//Class to store instance of RobotInstruction class in Constructor.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Instructions")
		class URobotInstruction *RobotInstruction;
};
