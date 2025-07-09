![After Darkness Ingame Logo](https://github.com/user-attachments/assets/6c2a4edd-925f-44ab-8694-50076bbd7904)
# AFTER DARKNESS QUEST SYSTEM

`v.1.0.2 HF`

`After Darkness © 2020 - 2025 Skydream Interactive. All rights reserved.`

Developed in Unreal Engine 5. "Unreal Engine" and its logo are trademarks or registered trademarks of Epic Games, Inc.
"After Darkness" and its logo are trademarks or registered trademarks of Skydream Interactive LLC

This project and its contents are the intellectual property of Skydream Interactive & Deviunter. Unauthorized copying, reproduction, or distribution is strictly prohibited.

# WHAT'S NEW

`v. 1.0.2 HF`
A small hotfix, the quotation mark in the #include list was omitted


`v. 1.0.2`
Added interface implementation to display the receipt of new quests.


`v. 1.0.1`
Minor fixes and improvements.


`v. 1.0.0`
Improved UQuestBase: if the goal was multiple, it was impossible to complete it, because the goal could increase the CurrentAmmound value. Now, the logic of ObjectiveComplete has been refined and improved. Now it is possible to complete the goal, after increasing the CurrentAmmound value.
``` c++
if (GetObjectiveData(ObjectiveID).CurrentAmmound >= GetObjectiveData(ObjectiveID).MaxAmmound)
			{
				MakeCompletedObjective(LocalObjective, EObjectiveCompleteType::Completed);
				if (bIsQuestTracked == true)
				{
					AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
					if (CurrentGameMode)
					{
						IQuestInterface::Execute_RemoveLine((UObject*)CurrentGameMode, ObjectiveID, EObjectiveCompleteType::Completed);
					}
				}
				else
				{
					AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
					if (CurrentGameMode)
					{
						IQuestInterface::Execute_NonTrackedTaskUpdated((UObject*)CurrentGameMode, this);
					}
				}
				int32 LocalIndex;
				LocalIndex = GetObjectiveIndex(ObjectiveID);
				CurrentObjectives.RemoveAt(LocalIndex);
				CheckConflictObjectives(ObjectiveID);
				if (CurrentObjectives.Num() < 1)
				{
					//StageReward();
					/*if (FOnStageCompleted.IsBound())
					{
						FOnStageCompleted.Broadcast(CurrentStage);
					}*/
					GetWorld()->GetTimerManager().SetTimer(UpdateDelay, this, &UQuestBase::UpdateStage, 2.f, false, -1.f);
				}
			}
```

# TUTORIAL

**INITIALLY**

1. For correct operation, you need to make changes to the YourGame.Build.cs file by adding paths to the files according to the following example:
``` c#
PublicIncludePaths.AddRange(new string[]
    {
        "AfterDarkness",
        "AfterDarkness/TaskSystem",
        "AfterDarkness/TaskSystem/Classes",
        "AfterDarkness/TaskSystem/Interfaces",
        "AfterDarkness/TaskSystem/Structures",
        "AfterDarkness/TaskSystem/Enumerators"
    });
```

2. In all files you need to replace AFTERDARKNESS_API with YOURAGAMENAME_API. According to the following example:

``` c++
UCLASS(Blueprintable, BlueprintType)
class AFTERDARKNESS_API UQuestBase : public UObject
```
3. It is necessary to replace the paths to DataTable in the executable file QuestBase.cpp
``` c++
// INITIALIZATION
UDataTable* QuestData = LoadObject<UDataTable>(nullptr,
				TEXT("/Game/SD/Systems/TaskSystem/DataTables/DT_Quests.DT_Quests"));
			if (QuestData)
//REPLACEOBJECTIVE
UDataTable* ReplaceData = LoadObject<UDataTable>(nullptr,
				TEXT("/Game/SD/Systems/TaskSystem/DataTables/DT_ReplaceObjectives.DT_ReplaceObjectives"));
			if (ReplaceData)
```
**HIERARCHY OF OBJECTS AND THE PRINCIPLE OF OPERATION OF THE SYSTEM**

*Classes*

`QuestBase` - class inherited from UObject. contains the logic of completing/ignoring/failing goals, updating quest data, stage and decides how this affects further task completion. After completing the quest, it is marked as completed and becomes inactive, remaining in the game and storing information about the quest if it is suddenly needed. Located in `TaskSystem/Classes`

`TaskLogger` - class inherited from AActorComponent. Сontains FName arrays for current and completed quests and the QuestBase array, which stores all quest managers. Located in `TaskSystem/Classes`

`QuestObjective` - class inherited from AActorComponent. The class is attached to the character and is activated on demand when the manager defines the character as a task target. After that, a correct interaction with the component owner will call a method that will trigger the task execution process in the manager. An incorrect interaction will cause the task to fail or be ignored, provided that the task can be ignored or failed, otherwise the game will be over. Located in `TaskSystem/Classes`

*Structures*

`CompletedObjective` - A structure for recording completed goals and the method for achieving them. Located in `TaskSystem/Structures`

`CurrentObjectives` - A structure containing current targets. quantity of each target, its ID, etc. Located in `TaskSystem/Structures`

`ObjectiveDetails` - The main structure of the target, it stores all the data that is edited from the DataTable. Located in `TaskSystem/Structures`

`QuestBaseSave` - Structure for save QuestBase data: QuestID, CurrentStage ect. Located in `TaskSystem/Structures`

`QuestDetails` - The main data structure of a quest contains the name, an array of quest stages, and goals. Located in `TaskSystem/Structures`

`StageDetails` - Here is stored information on the stages of the quest. Located in `TaskSystem/Structures`

`TaskSystemSave` - Basic structure for saving. Located in `TaskSystem/Structures`

*Enumerators*

`QuestEnumerators` - contains several enumerators for the system to work. Located in `TaskSystem/Enumerators`

*Interfaces*

`QuestInterface` - The connecting link of the entire system. It is used to send updates to the HUD and activate mission objectives. Located in `TaskSystem/Interfaces`


![After Darkness Ingame Logo](https://github.com/user-attachments/assets/6c2a4edd-925f-44ab-8694-50076bbd7904)
`© 2025 Skydream Interactive LLC`
`© 2025 Deviunter`
