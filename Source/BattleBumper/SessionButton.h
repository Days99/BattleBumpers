// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "MyLevelScriptActor.h"
#include "SessionButton.generated.h"

/**
 * 
 */
UCLASS()
class BATTLEBUMPER_API USessionButton : public UButton
{
	GENERATED_BODY()

	struct FSessionInfo* sessionInfo;

	USessionButton();

	UFUNCTION()
	void OnClick();

public:

	void SetSessionInfo(struct FSessionInfo* sinfo);
};
