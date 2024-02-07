// Copyright Telephone Studios. All Rights Reserved.


#include "Structs/ChatMessage.h"

FChatMessage::FChatMessage()
{
	SenderName = TEXT("NAN");
	Message = TEXT("");
	SenderIsAdminisrator = false;
	TimeRecieved = 0.0f;
	ConnectionUpdate = false;
}

FChatMessage::FChatMessage(const FString& InSenderName, const FString& InMessage, const bool& InSenderIsAdministrator, const double& InTimeRecieved, const bool& InConnectionUpdate)
{
	SenderName = InSenderName;
	Message = InMessage;
	SenderIsAdminisrator = InSenderIsAdministrator;
	TimeRecieved = InTimeRecieved;
	ConnectionUpdate = InConnectionUpdate;
}
