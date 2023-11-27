// Copyright Telephone Studios. All Rights Reserved.


#include "Notification.h"

FNotification::FNotification()
{
	Identifier = TEXT("");
	Negative = false;
	Positive = false;
	Time = 0.0f;
	Duration = 0.0f;
	Message = TEXT("");
	Icon = nullptr;
}
