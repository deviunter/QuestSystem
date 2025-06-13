// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestBase.h"
#include "Enumerators/QuestEnumerators.h"
#include "QuestObjective.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AFTERDARKNESS_API UQuestObjective : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UQuestObjective();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Default")
	void OnQuestInvolved(EObjectiveType InvolveType);

	UFUNCTION(BlueprintCallable, Category = "Default")
	void ActivateObjective(FObjectiveDetails Objective, UQuestBase* QuestBaseReference);

	UFUNCTION(BlueprintCallable, Category = "Default")
	void DeactivateObjective();

	UFUNCTION(BlueprintCallable, Category = "Default")
	void RefreshObjective(FObjectiveDetails Objective, UQuestBase* QuestBaseReference);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FString ObjectiveID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	FObjectiveDetails ObjectiveDetails;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	UQuestBase* QuestBase;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	//
};
