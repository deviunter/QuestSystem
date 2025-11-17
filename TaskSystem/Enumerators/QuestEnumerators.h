// � Skydream Interactive LLC 2020-2025

#pragma once

#include "CoreMinimal.h"
#include "QuestEnumerators.generated.h" 

/*        QUEST TYPE        */	
UENUM(BlueprintType)
enum class EQuestType : uint8
{
	MainTask UMETA(DisplayName = "Main Task"), // Main Quest. If Quest failed - GameOver
	SideTask UMETA(DisplayName = "Side Task"), // SideQuest. If Quest failed - GameOver
	Investigation UMETA(DisplayName = "Investigation"), // Investigation Quest.
	KeyData UMETA(DisplayName = "Important Data"), 
	Measure UMETA(DisplayName = "Measures"), 
	Ocg UMETA(DisplayName = "OCG Confrontation"), 
	Echo UMETA(DisplayName = "Echo of the Past") 
};

/*        OBJECTIVE TYPE        */
UENUM(BlueprintType)
enum class EObjectiveType : uint8
{
	None UMETA(DisplayName = "None"), 
	LocEnter UMETA(DisplayName = "Location - Enter"), 
	LocLeave UMETA(DisplayName = "Location - Leave"), 
	Kill UMETA(DisplayName = "Kill"), 
	Stun UMETA(DisplayName = "Neutralization"), 
	Interact UMETA(DisplayName = "Interact & Collect"), 
	Dialogue UMETA(DisplayName = "Dialogue"), 
	LookAt UMETA(DisplayName = "Look At") 
};

/*        OBJECTIVE COMPLETE TYPE        */
UENUM(BlueprintType)
enum class EObjectiveCompleteType : uint8
{
	Completed UMETA(DisplayName = "Objective Completed"),
	Ignored UMETA(DisplayName = "Objective Ignored"),
	Failed UMETA(DisplayName = "Objective Failed")
};

UENUM(BlueprintType)
enum class ETaskGiverType : uint8
{
	NewTask UMETA(DisplayName = "New Task"),
	Failed UMETA(DisplayName = "Task Failed"),
	Completed UMETA(DisplayName = "Task Completed")
};