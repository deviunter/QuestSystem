#pragma once

#include "QuestBase.h"
#include "Modules/ModuleManager.h" 
#include "TaskLogger.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"
#include "Player/PlayerInterface.h"
#include "TaskSystem/Interfaces/QuestInterface.h"

/*Initial function. It sets default values ??for variables, clears arrays and loads the quest table to install the data manager*/
void UQuestBase::Initialization()
{
	if (TaskLogger)
	{
		bIsQuestTracked = false;
		bIsQuestCompleted = false;
		CurrentStage = 0;
		CurrentObjectives.Empty();
		AllObjectives.Empty();
		CompletedObjectives.Empty();
		if (!GEngine) return;
		if (!QuestID.IsNone())
		{
			// /Script/Engine.DataTable'/Game/SD/Systems/TaskSystem/DataTables/DT_Quests.DT_Quests'
			// /Script/Engine.DataTable'/Game/SD/Systems/TaskSystem/DataTables/DT_ReplaceObjectives.DT_ReplaceObjectives'
			UDataTable* QuestData = LoadObject<UDataTable>(nullptr,
				TEXT("/Game/SD/Systems/TaskSystem/DataTables/DT_Quests.DT_Quests"));
			if (QuestData)
			{
				if (QuestData->FindRow<FQuestDetails>(QuestID, TEXT(""), true))
				{
					FQuestDetails* Data = QuestData->FindRow<FQuestDetails>(QuestID, TEXT(""), true);
					QuestDetails = *Data;
					for (FStageDetails Elem : QuestDetails.StageDetails)
					{
						for (FObjectiveDetails LowerElem : Elem.Objectives)
						{
							FCurrentObjectives NewElem;
							NewElem.CurrentAmmound = 0;
							NewElem.MaxAmmound = LowerElem.MaxAmmound;
							NewElem.ObjectiveID = LowerElem.ObjectiveID;
							NewElem.ObjectiveDetails = LowerElem;
							AllObjectives.Add(NewElem);
						}
					}
					RefreshQuest();
				}
			}
		}
	}
}

/*Search for objective in the list of current objectives (fcurrentobjectives) if found - returns "True" if not "False"*/
bool UQuestBase::FindObjectiveByID(FString ObjectiveID)
{
	for (FCurrentObjectives CurrentElem : CurrentObjectives)
	{
		if (CurrentElem.ObjectiveID == ObjectiveID)
		{
			return true;
		}
	}
	return false;
}

/*Searches for the selected target in the list of current targets and if found returns the index of the selected target, otherwise returns "-1"*/
int32 UQuestBase::GetObjectiveIndex(FString ObjectiveID)
{
	for (int32 i = 0; i < CurrentObjectives.Num(); i++)
	{
		if (CurrentObjectives[i].ObjectiveID == ObjectiveID)
		{
			return i;
		}
	}
	return INDEX_NONE;
}

/*Searches for a target in the full list of targets, returns "true" if found*/
bool UQuestBase::FindInAllObjectives(FString ObjectiveID)
{
	for (FCurrentObjectives Elem : AllObjectives)
	{
		if (Elem.ObjectiveID == ObjectiveID)
		{
			return true;
		}
	}
	return false;
}

/*Returns the value of the structure if the specified ObjectiveID and the ObjectiveID of the array index match*/
FCurrentObjectives UQuestBase::GetObjectiveData(FString ObjectiveID)
{
	for (FCurrentObjectives CurrentElem : CurrentObjectives)
	{
		if (CurrentElem.ObjectiveID == ObjectiveID)
		{
			return CurrentElem;
		}
	}
	return FCurrentObjectives();
}

/*Activates the quest and spawns a quest marker on the current targets, unless these targets are hidden until completed (bIsObjectiveHidden == true)*/
void UQuestBase::ActivateQuest()
{
	bIsQuestTracked = true;
	if (bIsQuestTracked == true)
	{
		AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
		if (CurrentGameMode)
		{
			IQuestInterface::Execute_RefreshQuest((UObject*)CurrentGameMode, this);
		}
	}
	TArray<AActor*> LocalActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UQuestInterface::StaticClass(), LocalActors);
	for (AActor* Elem : LocalActors)
	{
		if (FindObjectiveByID(IQuestInterface::Execute_GetMyObjectiveID(Elem)))
		{
			FString LocalStr;
			LocalStr = IQuestInterface::Execute_GetMyObjectiveID(Elem);
			FObjectiveDetails LocalObjective;
			LocalObjective = GetObjectiveData(LocalStr).ObjectiveDetails;
			if (!LocalObjective.bIsObjectiveHidden)
			{
				IQuestInterface::Execute_SpawnTargetMark(Elem, QuestDetails.QuestType);
			}
		}
	}
}

/*Deactivate quest and destroys a quest marker on the current targets*/
void UQuestBase::DeactivateQuest()
{
	bIsQuestTracked = false;
	TArray<AActor*> LocalActors;
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UQuestInterface::StaticClass(), LocalActors);
	for (AActor* Elem : LocalActors)
	{
		if (FindObjectiveByID(IQuestInterface::Execute_GetMyObjectiveID(Elem)))
		{
			FString LocalStr;
			LocalStr = IQuestInterface::Execute_GetMyObjectiveID(Elem);
			FObjectiveDetails LocalObjective;
			LocalObjective = GetObjectiveData(LocalStr).ObjectiveDetails;
			IQuestInterface::Execute_DestroyTargetMark(Elem);
		}
	}
}

/*Sets bIsQuestComplete flag & call CompleteQuest Function in TaskLogger*/
void UQuestBase::CompleteQuest()
{
	bIsQuestCompleted = true;
	bIsQuestTracked = false;
	TaskLogger->CompleteQuest(QuestID, this);
}

/*Updates the current mission objectives depending on the stage*/
void UQuestBase::RefreshQuest()
{
	if (QuestDetails.StageDetails.IsValidIndex(CurrentStage))
	{
		CurrentObjectives.Empty();
		StageDetails = QuestDetails.StageDetails[CurrentStage]; //Get Stage Objectives
		for (FObjectiveDetails Elem : StageDetails.Objectives)
		{
			if (FindInAllObjectives(Elem.ObjectiveID)) //проверяем каждую цель на присутствие в глобальном квестовом массиве. защита от софтлока
			{
				FCurrentObjectives LocalCurrentObjective;
				LocalCurrentObjective.ObjectiveID = Elem.ObjectiveID;
				LocalCurrentObjective.CurrentAmmound = 0;
				LocalCurrentObjective.MaxAmmound = Elem.MaxAmmound;
				LocalCurrentObjective.ObjectiveDetails = Elem;
				CurrentObjectives.Add(LocalCurrentObjective);
			}
		}
		if (bIsQuestTracked == true)
		{
			AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
			if (CurrentGameMode)
			{
				IQuestInterface::Execute_RefreshQuest((UObject*)CurrentGameMode, this);
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
		TArray<AActor*> LocalActors;
		UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UQuestInterface::StaticClass(), LocalActors);
		for (AActor* Elem : LocalActors)
		{
			if (FindObjectiveByID(IQuestInterface::Execute_GetMyObjectiveID(Elem)))
			{
				FString LocalStr;
				LocalStr = IQuestInterface::Execute_GetMyObjectiveID(Elem);
				FObjectiveDetails LocalObjective;
				LocalObjective = GetObjectiveData(LocalStr).ObjectiveDetails;
				IQuestInterface::Execute_ActivateObjective(Elem, LocalObjective, this);
				if (!LocalObjective.bIsObjectiveHidden && bIsQuestTracked == true)
				{
					IQuestInterface::Execute_SpawnTargetMark(Elem, QuestDetails.QuestType);
				}
			}
		}
	}
}

/*Update Stage*/
void UQuestBase::UpdateStage()
{
	//логика отключения предыдущих целей, если они остались, отгрузка саблевела если он есть и тд
	GetWorld()->GetTimerManager().ClearTimer(UpdateDelay);
	CurrentStage++;
	if (CurrentStage >= QuestDetails.StageDetails.Num())
	{
		CompleteQuest();
	}
	else
	{
		RefreshQuest();
	}
}

/*ahahha*/
void UQuestBase::StageReward()
{
	ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (Player)
	{
		IPlayerInterface::Execute_AddGameplayTags((UObject*)Player, StageDetails.GameplayTagsReward);
	}
}

/*The process of completing a goal. If it is in the current list, it is either executed or its Ammound is incremented.
If it is not in the current list, it is removed from AllObjectives, then if there are few goals left,
they are checked for possible ignorability or optionality. If possible, they are ignored and the stage is updated*/
void UQuestBase::ObjectiveComplete(FString ObjectiveID, int32 AddedValue)
{
	if (FindObjectiveByID(ObjectiveID))
	{
		FCurrentObjectives LocalObjective;
		LocalObjective = GetObjectiveData(ObjectiveID);
		if (AddedValue >= LocalObjective.MaxAmmound)
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
				StageReward();
				// on stage completed
				GetWorld()->GetTimerManager().SetTimer(UpdateDelay, this, &UQuestBase::UpdateStage, 2.f, false, -1.f);
			}
			else
			{
				if (CurrentObjectives.Num() == 1)
				{
					IsOptionalRemainder();
					if (CurrentObjectives.IsEmpty())
					{
						GetWorld()->GetTimerManager().SetTimer(UpdateDelay, this, &UQuestBase::UpdateStage, 2.f, false, -1.f);
					}
				}
			}
		}
		else
		{
			int32 RefreshIndex = GetObjectiveIndex(ObjectiveID);
			if (RefreshIndex != INDEX_NONE && CurrentObjectives.IsValidIndex(RefreshIndex))
			{
				FCurrentObjectives& ObjectiveReference = CurrentObjectives[RefreshIndex];
				ObjectiveReference.CurrentAmmound += AddedValue;
			}
			if (bIsQuestTracked == true)
			{
				AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
				if (CurrentGameMode)
				{
					IQuestInterface::Execute_AddLineValue((UObject*)CurrentGameMode, ObjectiveID, AddedValue);
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
				int32 LocalInt;
				LocalInt = GetObjectiveIndex(ObjectiveID);
				CurrentObjectives.RemoveAt(LocalInt);
				CheckConflictObjectives(ObjectiveID);
				if (CurrentObjectives.Num() < 1)
				{
					StageReward();
					// on stage completed
					UpdateStage();
					//GetWorld()->GetTimerManager().SetTimer(UpdateDelay, this, &UQuestBase::UpdateStage, 2.f, false, -1.f);
				}
			}
		}
	}
	else
	{
		if (FindInAllObjectives(ObjectiveID))
		{
			FCurrentObjectives LocalObjective;
			LocalObjective = GetObjectiveData(ObjectiveID);
			MakeCompletedObjective(LocalObjective, EObjectiveCompleteType::Completed);
			for (int32 i = 0; i < AllObjectives.Num(); i++)
			{
				if (AllObjectives[i].ObjectiveID == ObjectiveID)
				{
					AllObjectives.RemoveAt(i);
					break;
				}
			}
		}
	}
}

/*The process of failing a goal. If it is in the current list, it is either executed.
If it is not in the current list, it is removed from AllObjectives, then if there are few goals left,
they are checked for possible ignorability or optionality. If possible, they are ignored and the stage is updated*/
void UQuestBase::ObjectiveFailed(FString ObjectiveID, bool ForceUpdateStage)
{
	if (FindObjectiveByID(ObjectiveID))
	{
		FCurrentObjectives LocalObjective;
		LocalObjective = GetObjectiveData(ObjectiveID);
		if (LocalObjective.ObjectiveDetails.bAllowFail == true || LocalObjective.ObjectiveDetails.bIsOptional == true)
		{
			MakeCompletedObjective(LocalObjective, EObjectiveCompleteType::Failed);
			if (bIsQuestTracked == true)
			{
				AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
				if (CurrentGameMode)
				{
					IQuestInterface::Execute_RemoveLine((UObject*)CurrentGameMode, ObjectiveID, EObjectiveCompleteType::Failed);
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
			if (!LocalObjective.ObjectiveDetails.ObjectiveReplaceID.IsEmpty())
			{
				// НАДО ПОСТАВИТЬ ЭТО НА ТАЙМЕР, ПОТОМУ ЧТО ИГРОК НЕ ВИДИТ ЧТО СТАЛО С ЕГО ЦЕЛЬЮ И НЕ УСПЕВАЕТ
				// ПОНЯТЬ ЧТО ПРОИЗОШЛО
				ReplaceObjective(LocalObjective.ObjectiveDetails.ObjectiveReplaceID);
			}
			if (ForceUpdateStage == true)
			{
				if (CurrentObjectives.Num() < 1)
				{
					StageReward();
					// on stage completed
					GetWorld()->GetTimerManager().SetTimer(UpdateDelay, this, &UQuestBase::UpdateStage, 2.f, false, -1.f);
				}
				else
				{
					if (CurrentObjectives.Num() == 1)
					{
						IsOptionalRemainder();
						if (CurrentObjectives.IsEmpty())
						{
							GetWorld()->GetTimerManager().SetTimer(UpdateDelay, this, &UQuestBase::UpdateStage, 2.f, false, -1.f);
						}
					}
				}
			}
		}
		else
		{
			ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
			if (Player)
			{
				IPlayerInterface::Execute_TaskFailed((UObject*)Player, QuestID);
			}
		}
	}
	else
	{
		if (FindInAllObjectives(ObjectiveID))
		{
			FCurrentObjectives LocalObjective;
			LocalObjective = GetObjectiveData(ObjectiveID);
			MakeCompletedObjective(LocalObjective, EObjectiveCompleteType::Failed);
			for (int32 i = 0; i < AllObjectives.Num(); i++)
			{
				if (AllObjectives[i].ObjectiveID == ObjectiveID)
				{
					AllObjectives.RemoveAt(i);
					break;
				}
			}
		}

	}

}

/*The process of ignoring a goal. If it is in the current list, it is either executed.
If it is not in the current list, it is removed from AllObjectives, then if there are few goals left,
they are checked for possible ignorability or optionality. If possible, they are ignored and the stage is updated*/
void UQuestBase::ObjectiveIgnored(FString ObjectiveID, bool ForceUpdateStage)
{
	if (FindObjectiveByID(ObjectiveID))
	{
		FCurrentObjectives LocalObjective;
		LocalObjective = GetObjectiveData(ObjectiveID);
		if (LocalObjective.ObjectiveDetails.bAllowIgnore == true || LocalObjective.ObjectiveDetails.bIsOptional == true)
		{
			MakeCompletedObjective(LocalObjective, EObjectiveCompleteType::Ignored);
			if (bIsQuestTracked == true)
			{
				AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
				if (CurrentGameMode)
				{
					IQuestInterface::Execute_RemoveLine((UObject*)CurrentGameMode, ObjectiveID, EObjectiveCompleteType::Ignored);
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
			if (!LocalObjective.ObjectiveDetails.ObjectiveReplaceID.IsEmpty())
			{
				// НАДО ПОСТАВИТЬ ЭТО НА ТАЙМЕР, ПОТОМУ ЧТО ИГРОК НЕ ВИДИТ ЧТО СТАЛО С ЕГО ЦЕЛЬЮ И НЕ УСПЕВАЕТ
				// ПОНЯТЬ ЧТО ПРОИЗОШЛО
				ReplaceObjective(LocalObjective.ObjectiveDetails.ObjectiveReplaceID);
			}
			if (ForceUpdateStage == true)
			{
				if (CurrentObjectives.Num() < 1)
				{
					StageReward();
					// on stage completed
					GetWorld()->GetTimerManager().SetTimer(UpdateDelay, this, &UQuestBase::UpdateStage, 2.f, false, -1.f);
				}
				else
				{
					if (CurrentObjectives.Num() == 1)
					{
						IsOptionalRemainder();
						if (CurrentObjectives.IsEmpty())
						{
							GetWorld()->GetTimerManager().SetTimer(UpdateDelay, this, &UQuestBase::UpdateStage, 2.f, false, -1.f);
						}						
					}
				}
			}

		}
	}
	else
	{
		if (FindInAllObjectives(ObjectiveID))
		{
			FCurrentObjectives LocalObjective;
			LocalObjective = GetObjectiveData(ObjectiveID);
			MakeCompletedObjective(LocalObjective, EObjectiveCompleteType::Ignored);
			for (int32 i = 0; i < AllObjectives.Num(); i++)
			{
				if (AllObjectives[i].ObjectiveID == ObjectiveID)
				{
					AllObjectives.RemoveAt(i);
					break;
				}
			}
		}

	}
}

/*If there are less than 2 targets, then this method is launched, the targets are checked for the possibility of ignoring and optionality.
if the target is bIsOptional or bAllowFail then this target is ignored*/
void UQuestBase::IsOptionalRemainder()
{
	for (FCurrentObjectives CurrentElem : CurrentObjectives)
	{
		if (CurrentElem.ObjectiveDetails.bAllowIgnore == true || CurrentElem.ObjectiveDetails.bIsOptional == true)
		{
			ObjectiveIgnored(CurrentElem.ObjectiveID, false);
		}
	}
}

/*Overwrites the maximum number of the target if it is in the CurrentObjectives list*/
void UQuestBase::WriteMaxAmmound(FString ObjectiveID, int32 NewMaxAmmound)
{
	int32 RefreshIndex = GetObjectiveIndex(ObjectiveID);
	if (RefreshIndex != INDEX_NONE && CurrentObjectives.IsValidIndex(RefreshIndex))
	{
		FCurrentObjectives& ObjectiveReference = CurrentObjectives[RefreshIndex];
		ObjectiveReference.MaxAmmound = NewMaxAmmound;
	}
}

/*If the target was failed or ignored, it is replaced with another target(s)
if it has them in the ReplaceObjectives array in the FObjectiveDetails structure*/
void UQuestBase::ReplaceObjective(TArray<FName> ReplaceID)
{
	// надо добавить возможность добавлять замену на следующую стадию
	// тоесть сначала мы проверяем AddToNextStage == true если да, то делаем апрейт стейдж
	// ПРОБЛЕМА: СИСТЕМА МОЖЕТ ВЫПОЛНИТЬ КВЕСТ ПОСЛЕ ПРОВАЛЕННОЙ ЦЕЛИ, ЕСЛИ ОНА В ПОСЛЕДНЕЙ СТАДИИ
	// НУЖНО СДЕЛАТЬ ЗАЩИТУ ОТ ЭТОГО БУДЕТ
	// и после апдейт стейдж мы уже добавляем новые цели на новую стадию
	// а если AddToNextStage == false тогда похуй
	// НО: НАДО СДЕЛАТЬ ПРОВЕРКУ НА ЦЕЛИ, НЕЛЬЗЯ ЖЕ ОБНОВЛЯТЬ СТАДИЮ ЕСЛИ НЕ ВСЕ ЦЕЛИ НА ЭТОЙ ЗАВЕРШЕНЫ
	// ТОЛЬКО ЕСЛИ ЦЕЛЬ, КОТОРУЮ ЗАМЕНИЛИ ОСТАЛАСЬ ОДНА НА СТАДИИ ИЛИ БЫЛА ОДНА ИЗНАЧАЛЬНО
	for (FName Elem : ReplaceID)
	{
		if (!Elem.IsNone())
		{
			UDataTable* ReplaceData = LoadObject<UDataTable>(nullptr,
				TEXT("/Game/SD/Systems/TaskSystem/DataTables/DT_ReplaceObjectives.DT_ReplaceObjectives"));
			if (ReplaceData)
			{
				if (ReplaceData->FindRow<FObjectiveDetails>(Elem, TEXT(""), true))
				{
					FObjectiveDetails* Data = ReplaceData->FindRow<FObjectiveDetails>(Elem, TEXT(""), true);
					FObjectiveDetails LocalObjective = *Data;
					FCurrentObjectives LocalCurrentObjective;
					LocalCurrentObjective.CurrentAmmound = 0;
					LocalCurrentObjective.MaxAmmound = LocalObjective.MaxAmmound;
					LocalCurrentObjective.ObjectiveID = LocalObjective.ObjectiveID;
					LocalCurrentObjective.ObjectiveDetails = LocalObjective;
					AllObjectives.Add(LocalCurrentObjective);
					CurrentObjectives.Add(LocalCurrentObjective);
					if (bIsQuestTracked == true)
					{
						AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
						if (CurrentGameMode)
						{
							IQuestInterface::Execute_RefreshQuest((UObject*)CurrentGameMode, this);
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
					TArray<AActor*> LocalActors;
					UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UQuestInterface::StaticClass(), LocalActors);
					for (AActor* ActorElem : LocalActors)
					{
						if (FindObjectiveByID(IQuestInterface::Execute_GetMyObjectiveID(ActorElem)))
						{
							FString LocalStr;
							LocalStr = IQuestInterface::Execute_GetMyObjectiveID(ActorElem);
							FObjectiveDetails LowerObjective;
							LowerObjective = GetObjectiveData(LocalStr).ObjectiveDetails;
							IQuestInterface::Execute_ActivateObjective(ActorElem, LowerObjective, this);
							if (!LowerObjective.bIsObjectiveHidden && bIsQuestTracked == true)
							{
								IQuestInterface::Execute_SpawnTargetMark(ActorElem, QuestDetails.QuestType);
							}
						}
					}
				}
			}				
		}
	}
}

/*The check whether the goal contains a list of conflicting goals is activated only in the ObjectiveComplete() method,
if the goal is completed, then the conflicting goals will be automatically ignored*/
void UQuestBase::CheckConflictObjectives(FString ObjectiveID)
{
	if (FindObjectiveByID(ObjectiveID))
	{
		int32 Index;
		Index = GetObjectiveIndex(ObjectiveID);
		FCurrentObjectives LocalObjective;
		LocalObjective = CurrentObjectives[Index];
		for (FString Elem : LocalObjective.ObjectiveDetails.ConflictWith)
		{
			if (FindObjectiveByID(Elem))
			{
				ObjectiveIgnored(Elem, false);
			}
		}
	}
	else
	{
		if (FindInAllObjectives(ObjectiveID))
		{
			FCurrentObjectives LocalObjective;
			for (int32 i = 0; i < AllObjectives.Num(); i++)
			{
				if (AllObjectives[i].ObjectiveID == ObjectiveID)
				{
					LocalObjective = AllObjectives[i];
					break;
				}				
			}
			for (FString Elem : LocalObjective.ObjectiveDetails.ConflictWith)
			{
				if (FindObjectiveByID(Elem))
				{
					ObjectiveIgnored(Elem, false);
				}
			}
		}
	}
}

/*Create CompleteObjective Array Elem*/
void UQuestBase::MakeCompletedObjective(FCurrentObjectives Objective, EObjectiveCompleteType CompleteType)
{
	FCompletedObjective LocalCompletedObjective;
	LocalCompletedObjective.Objective = Objective.ObjectiveDetails;
	LocalCompletedObjective.CompleteType = CompleteType;
	CompletedObjectives.Add(LocalCompletedObjective);
}

/*Return Save Data for SaveGameToSlot*/
FQuestBaseSave UQuestBase::GetSaveData()
{
	FQuestBaseSave LocalData;
	LocalData.QuestID = QuestID;
	LocalData.QuestDetails = QuestDetails;
	LocalData.CurrentStage = CurrentStage;
	LocalData.CurrentObjectives.Empty();
	LocalData.CurrentObjectives = CurrentObjectives;
	LocalData.AllObjectives.Empty();
	LocalData.AllObjectives = AllObjectives;
	LocalData.StageDetails = StageDetails;
	LocalData.CompletedObjectives.Empty();
	LocalData.CompletedObjectives = CompletedObjectives;
	LocalData.bIsQuestTracked = bIsQuestTracked;
	LocalData.bIsQuestCompleted = bIsQuestCompleted;
	return LocalData;
}

/*Get LoadedData from LoadGameFromSlot*/
void UQuestBase::SetLoadData(FQuestBaseSave LoadedData)
{
	QuestID = LoadedData.QuestID;
	CurrentStage = LoadedData.CurrentStage;
	CurrentObjectives = LoadedData.CurrentObjectives;
	AllObjectives = LoadedData.AllObjectives;
	QuestDetails = LoadedData.QuestDetails;
	bIsQuestCompleted = LoadedData.bIsQuestCompleted;
	bIsQuestTracked = LoadedData.bIsQuestTracked;
	StageDetails = LoadedData.StageDetails;
	CompletedObjectives = LoadedData.CompletedObjectives;
	if (!bIsQuestCompleted)
	{
		if (bIsQuestTracked == true)
		{
			TArray<AActor*> LocalActors;
			UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UQuestInterface::StaticClass(), LocalActors);
			for (AActor* Elem : LocalActors)
			{
				if (FindObjectiveByID(IQuestInterface::Execute_GetMyObjectiveID(Elem)))
				{
					FString LocalStr;
					LocalStr = IQuestInterface::Execute_GetMyObjectiveID(Elem);
					FObjectiveDetails LocalObjective;
					LocalObjective = GetObjectiveData(LocalStr).ObjectiveDetails;
					IQuestInterface::Execute_ActivateObjective(Elem, LocalObjective, this);
					if (!LocalObjective.bIsObjectiveHidden && bIsQuestTracked == true)
					{
						IQuestInterface::Execute_SpawnTargetMark(Elem, QuestDetails.QuestType);
					}
				}
			}
			AGameModeBase* CurrentGameMode = UGameplayStatics::GetGameMode(GetWorld());
			if (CurrentGameMode)
			{
				IQuestInterface::Execute_RefreshQuest((UObject*)CurrentGameMode, this);
			}
		}
	}
}