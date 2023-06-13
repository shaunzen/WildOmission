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
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CheckedMaterialBlueprint(TEXT("/Game/WildOmission/UI/Icons/Options/M_Checked_Inst"));
	if (CheckedMaterialBlueprint.Succeeded())
	{
		CheckedMaterial = CheckedMaterialBlueprint.Object;
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> UncheckedMaterialBlueprint(TEXT("/Game/WildOmission/UI/Icons/Options/M_Unchecked_Inst"));
	if (UncheckedMaterialBlueprint.Succeeded())
	{
		UncheckedMaterial = UncheckedMaterialBlueprint.Object;
	}
}

void UCheckOptionBox::Setup(const FString& OptionName, bool CheckedByDefault)
{
	TextBlock->SetText(FText::FromString(OptionName));
	Checked = CheckedByDefault;
	RefreshState();
	Button->OnClicked.AddDynamic(this, &UCheckOptionBox::OnClicked);
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
	OnCheckedStateChanged.Broadcast(Checked);
}
