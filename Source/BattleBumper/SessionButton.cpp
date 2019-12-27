// Fill out your copyright notice in the Description page of Project Settings.


#include "SessionButton.h"


USessionButton::USessionButton() {
	OnClicked.AddDynamic(this, &USessionButton::OnClick);
}
void USessionButton::OnClick()
{
	//JOIN SESSION
}

void USessionButton::SetSessionInfo(FSessionInfo* sinfo)
{
	sessionInfo = sinfo;
}
