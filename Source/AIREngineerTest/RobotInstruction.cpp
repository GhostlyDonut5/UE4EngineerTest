// Copyright Epic Games, Inc. All Rights Reserved.

#include "RobotInstruction.h"

URobotInstruction::URobotInstruction()
{
	speed = 1.0f;
	rotation_speed = 5.0f;
	travel_distance = 3000.0f;
}

//--------------------Get Instructions from a text file in the content directory to execute specific instructions--------------------
bool URobotInstruction::Get_Instructions_From_Text_File(FString FileName, FString Desired_Robot)
{
	FString directory = FPaths::ProjectContentDir();
	FString result;
	IPlatformFile& file = FPlatformFileManager::Get().GetPlatformFile();

	if (file.CreateDirectory(*directory))
	{
		FString myFile = directory + FileName + ".txt";
		FFileHelper::LoadFileToString(result, *myFile);

		if (Desired_Robot == "Dropper" || Desired_Robot == "dropper")
		{
			result.ParseIntoArrayLines(dropper_list, true);
			Queue_Instructions(&dropper_instructions, dropper_list);

			return true;
		}

		if (Desired_Robot == "Grabber" || Desired_Robot == "grabber")
		{
			result.ParseIntoArrayLines(grabber_list, true);
			Queue_Instructions(&grabber_instructions, grabber_list);

			return true;
		}

	}
	return false;
}

//--------------------Get Instructions from a Premade Blueprint Array--------------------
bool URobotInstruction::Get_Instructions_From_Array(TArray<FString> stored_list)
{
	if (stored_list.Num() <= 0)
	{
		return false;
	}
	if (stored_list == grabber_list)
	{
		Queue_Instructions(&grabber_instructions, stored_list);
		return true;
	}

	if (stored_list == dropper_list)
	{
		Queue_Instructions(&dropper_instructions, stored_list);
		return true;
	}

	return false;
}

//--------------------populate a set of instructions into the given Queue from the Array--------------------
void URobotInstruction::Queue_Instructions(TQueue<FString>* instructions, TArray<FString> stored_list)
{
	for (int i = 0; i < stored_list.Num(); ++i)
	{
		if (stored_list[i] == "move")
		{
			instructions->Enqueue("move");
		}
		if (stored_list[i] == "right")
		{
			instructions->Enqueue("right");
		}
		if (stored_list[i] == "left")
		{
			instructions->Enqueue("left");
		}
		if (stored_list[i] == "drop")
		{
			instructions->Enqueue("drop");
		}
		if (stored_list[i] == "grab")
		{
			instructions->Enqueue("grab");
		}
	}
}