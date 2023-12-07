// Copyright Telephone Studios. All Rights Reserved.


#include "ControlsSettingsWidget.h"
#include "OptionBoxes/CheckOptionBox.h"
#include "OptionBoxes/KeyOptionBox.h"
#include "OptionBoxes/SliderOptionBox.h"
#include "Interfaces/CharacterSettingsInterface.h"
#include "WildOmissionGameUserSettings.h"

UControlsSettingsWidget::UControlsSettingsWidget(const FObjectInitializer& ObjectInitializer) : USettingsCategoryWidget(ObjectInitializer)
{
	InvertMouseYCheckOptionBox = nullptr;
	MouseSensitivitySliderOptionBox = nullptr;
	MoveForwardKeyOptionBox = nullptr;
	MoveBackwardKeyOptionBox = nullptr;
	MoveLeftKeyOptionBox = nullptr;
	MoveRightKeyOptionBox = nullptr;
	SprintKeyOptionBox = nullptr;
	CrouchKeyOptionBox = nullptr;
	JumpKeyOptionBox = nullptr;

	PrimaryKeyOptionBox = nullptr;
	SecondaryKeyOptionBox = nullptr;
	InteractKeyOptionBox = nullptr;
	ReloadKeyOptionBox = nullptr;

	InventoryKeyOptionBox = nullptr;
	CraftingKeyOptionBox = nullptr;
	ChatKeyOptionBox = nullptr;
}

void UControlsSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MouseSensitivitySliderOptionBox->SetMinValue(1.0f);
	MouseSensitivitySliderOptionBox->SetMaxValue(20.0f);
}

void UControlsSettingsWidget::OnApply()
{
	Super::OnApply();

	UWildOmissionGameUserSettings* UserSettings = UWildOmissionGameUserSettings::GetWildOmissionGameUserSettings();
	if (UserSettings == nullptr)
	{
		return;
	}

	const float MouseSensitivity = FMath::RoundToInt32(MouseSensitivitySliderOptionBox->GetValue()) / 10.0f;

	UserSettings->SetInvertedMouseY(InvertMouseYCheckOptionBox->IsChecked());
	UserSettings->SetMouseSensitivity(MouseSensitivity);

	UserSettings->SetMoveForwardKey(MoveForwardKeyOptionBox->GetSelectedKey());
	UserSettings->SetMoveBackwardKey(MoveBackwardKeyOptionBox->GetSelectedKey());
	UserSettings->SetMoveLeftKey(MoveLeftKeyOptionBox->GetSelectedKey());
	UserSettings->SetMoveRightKey(MoveRightKeyOptionBox->GetSelectedKey());
	UserSettings->SetSprintKey(SprintKeyOptionBox->GetSelectedKey());
	UserSettings->SetCrouchKey(CrouchKeyOptionBox->GetSelectedKey());
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

	const float MouseSensitivity = UserSettings->GetMouseSensitivity() * 10.0f;

	InvertMouseYCheckOptionBox->SetChecked(UserSettings->GetInvertedMouseY());
	MouseSensitivitySliderOptionBox->SetValue(MouseSensitivity);

	MoveForwardKeyOptionBox->SetSelectedKey(UserSettings->GetMoveForwardKey());
	MoveBackwardKeyOptionBox->SetSelectedKey(UserSettings->GetMoveBackwardKey());
	MoveLeftKeyOptionBox->SetSelectedKey(UserSettings->GetMoveLeftKey());
	MoveRightKeyOptionBox->SetSelectedKey(UserSettings->GetMoveRightKey());
	SprintKeyOptionBox->SetSelectedKey(UserSettings->GetSprintKey());
	CrouchKeyOptionBox->SetSelectedKey(UserSettings->GetCrouchKey());
	JumpKeyOptionBox->SetSelectedKey(UserSettings->GetJumpKey());

	PrimaryKeyOptionBox->SetSelectedKey(UserSettings->GetPrimaryKey());
	SecondaryKeyOptionBox->SetSelectedKey(UserSettings->GetSecondaryKey());
	InteractKeyOptionBox->SetSelectedKey(UserSettings->GetInteractKey());
	ReloadKeyOptionBox->SetSelectedKey(UserSettings->GetReloadKey());

	InventoryKeyOptionBox->SetSelectedKey(UserSettings->GetInventoryKey());
	CraftingKeyOptionBox->SetSelectedKey(UserSettings->GetCraftingKey());
	ChatKeyOptionBox->SetSelectedKey(UserSettings->GetChatKey());
}
