// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionButton.h"


USessionButton::USessionButton() {
	OnClicked.AddDynamic(this, &USessionButton::OnClick);
}
void USessionButton::OnClick()
{
	APlayerController* pController = GetOuter()->GetWorld()->GetFirstPlayerController();
	if (pController)
	{
		FString cmd = "open " + this->sessionInfo->serverip + ":" +
			FString::FromInt(this->sessionInfo->serverport);
		pController->ConsoleCommand(cmd);
	}
}

void USessionButton::SetSessionInfo(FSessionInfo* sinfo)
{
	sessionInfo = sinfo;
}
