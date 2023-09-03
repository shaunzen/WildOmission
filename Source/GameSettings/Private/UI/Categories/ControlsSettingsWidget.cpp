// Copyright Telephone Studios. All Rights Reserved.


#include "ControlsSettingsWidget.h"
#include "OptionBoxes/KeyOptionBox.h"
#include "Interfaces/CharacterSettingsInterface.h"
#include "WildOmissionGameUserSettings.h"

void UControlsSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UControlsSettingsWidget::OnApply()
{
	Super::OnApply();

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	UserSettings->SetMoveForwardKey(MoveForwardKeyOptionBox->GetSelectedKey());
	UserSettings->SetMoveBackwardKey(MoveBackwardKeyOptionBox->GetSelectedKey());
	UserSettings->SetMoveLeftKey(MoveLeftKeyOptionBox->GetSelectedKey());
	UserSettings->SetMoveRightKey(MoveRightKeyOptionBox->GetSelectedKey());
	UserSettings->SetSprintKey(SprintKeyOptionBox->GetSelectedKey());
	UserSettings->SetJumpKey(JumpKeyOptionBox->GetSelectedKey());

	UserSettings->SetPrimaryKey(PrimaryKeyOptionBox->GetSelectedKey());
	UserSettings->SetSecondaryKey(SecondaryKeyOptionBox->GetSelectedKey());
	UserSettings->SetInteractKey(InteractKeyOptionBox->GetSelectedKey());
	UserSettings->SetReloadKey(ReloadKeyOptionBox->GetSelectedKey());

	UserSettings->SetInventoryKey(InventoryKeyOptionBox->GetSelectedKey());
	UserSettings->SetCraftingKey(CraftingKeyOptionBox->GetSelectedKey());
	UserSettings->SetChatKey(ChatKeyOptionBox->GetSelectedKey());

	ICharacterSettingsInterface* CharacterSettingsInterface = Cast<ICharacterSettingsInterface>(GetOwningPlayerPawn());
	if (CharacterSettingsInterface == nullptr)
	{
		return;
	}

	CharacterSettingsInterface->ApplyInputSettings();
}

void UControlsSettingsWidget::OnRefresh()
{
	Super::OnRefresh();
	
	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	MoveForwardKeyOptionBox->SetSelectedKey(UserSettings->GetMoveForwardKey());
	MoveBackwardKeyOptionBox->SetSelectedKey(UserSettings->GetMoveBackwardKey());
	MoveLeftKeyOptionBox->SetSelectedKey(UserSettings->GetMoveLeftKey());
	MoveRightKeyOptionBox->SetSelectedKey(UserSettings->GetMoveRightKey());
	SprintKeyOptionBox->SetSelectedKey(UserSettings->GetSprintKey());
	JumpKeyOptionBox->SetSelectedKey(UserSettings->GetJumpKey());

	PrimaryKeyOptionBox->SetSelectedKey(UserSettings->GetPrimaryKey());
	SecondaryKeyOptionBox->SetSelectedKey(UserSettings->GetSecondaryKey());
	InteractKeyOptionBox->SetSelectedKey(UserSettings->GetInteractKey());
	ReloadKeyOptionBox->SetSelectedKey(UserSettings->GetReloadKey());

	InventoryKeyOptionBox->SetSelectedKey(UserSettings->GetInventoryKey());
	CraftingKeyOptionBox->SetSelectedKey(UserSettings->GetCraftingKey());
	ChatKeyOptionBox->SetSelectedKey(UserSettings->GetChatKey());
}
