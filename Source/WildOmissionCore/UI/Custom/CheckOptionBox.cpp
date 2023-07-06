// Copyright Telephone Studios. All Rights Reserved.


#include "CheckOptionBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UObject/ConstructorHelpers.h"

static UMaterialInterface* CheckedMaterial = nullptr;
static UMaterialInterface* UncheckedMaterial = nullptr;

UCheckOptionBox::UCheckOptionBox(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CheckedMaterialBlueprint(TEXT("/Game/WildOmissionCore/UI/Icons/Options/M_Checked_Inst"));
	if (CheckedMaterialBlueprint.Succeeded())
	{
		CheckedMaterial = CheckedMaterialBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> UncheckedMaterialBlueprint(TEXT("/Game/WildOmissionCore/UI/Icons/Options/M_Unchecked_Inst"));
	if (UncheckedMaterialBlueprint.Succeeded())
	{
		UncheckedMaterial = UncheckedMaterialBlueprint.Object;
	}

	Checked = false;
}

void UCheckOptionBox::NativeConstruct()
{
	Button->OnClicked.AddDynamic(this, &UCheckOptionBox::OnClicked);
	RefreshState();
}

void UCheckOptionBox::SetChecked(bool IsChecked)
{
	Checked = IsChecked;
	RefreshState();
}

bool UCheckOptionBox::IsChecked() const
{
	return Checked;
}

void UCheckOptionBox::RefreshState()
{
	if (Checked)
	{
		Image->SetBrushFromMaterial(CheckedMaterial);
	}
	else
	{
		Image->SetBrushFromMaterial(UncheckedMaterial);
	}
}

void UCheckOptionBox::OnClicked()
{
	Checked = !Checked;
	RefreshState();
	OnCheckStateChanged.Broadcast(Checked);
}
