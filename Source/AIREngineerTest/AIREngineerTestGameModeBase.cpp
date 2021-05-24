// Copyright Epic Games, Inc. All Rights Reserved.

#include "RobotInstruction.h"
#include "AIREngineerTestGameModeBase.h"

AAIREngineerTestGameModeBase::AAIREngineerTestGameModeBase()
{
	RobotInstruction = NewObject<URobotInstruction>(this, URobotInstruction::StaticClass(), FName("RobotInstruction"));
}
