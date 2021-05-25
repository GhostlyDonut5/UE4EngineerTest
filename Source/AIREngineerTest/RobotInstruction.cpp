// Copyright Epic Games, Inc. All Rights Reserved.

#include "RobotInstruction.h"

URobotInstruction::URobotInstruction()
{
	speed = 1.0f;
	rotation_speed = 5.0f;
	travel_distance = 3000.0f;
}

void URobotInstruction::Populate_Instructions(FString filename)
{
	//Get from a text file to execute specific instructions, populate a set of instructions here.
	FString FileName = "Robot Instructions.txt";
	FString directory = FPaths::ProjectContentDir();
	FString result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory))
	{
		FString myFile = directory + FileName;
		FFileHelper::LoadFileToString(result, *myFile);
		result.ParseIntoArrayLines(instruction_list, true);
	}
}

bool URobotInstruction::ExecuteInstruction()
{
	if (instruction_list.Num() <= 0)
	{
		return false;
	}

	for (int i = 0; i < instruction_list.Num(); ++i)
	{
		if (instruction_list[i] == "move")
		{
			Move();
		}
		if (instruction_list[i] == "right")
		{
			Rotate_Right();
		}
		if (instruction_list[i] == "left")
		{
			Rotate_Left();
		}
		if (instruction_list[i] == "drop")
		{
			Drop_A_Sphere();
		}
		if (instruction_list[i] == "grab")
		{
			Document_A_Sphere();
		}
	}

	return true;
}


void URobotInstruction::Move()
{
	instructions.Enqueue("move");
}

void URobotInstruction::Rotate_Left()
{
	instructions.Enqueue("left");
}

void URobotInstruction::Rotate_Right()
{
	instructions.Enqueue("right");
}

void URobotInstruction::Drop_A_Sphere()
{
	instructions.Enqueue("drop");
}

void URobotInstruction::Document_A_Sphere()
{
	instructions.Enqueue("grab");
}