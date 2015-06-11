#include "MessageBroadcaster.h"


MessageRegistrationHandle::MessageRegistrationHandle(MessageBroadcaster* inBroadcaster, TypeID inTypeID, int inCallbackID)
	: mBroadcaster	(inBroadcaster)
	, mTypeID		(inTypeID)
	, mCallbackID	(inCallbackID)
{
}

void MessageRegistrationHandle::Unregister()
{
	if (nullptr != mBroadcaster)
	{
		mBroadcaster->Unregister(mTypeID, mCallbackID);
		mBroadcaster = nullptr;
	}
}

MessageBroadcaster::~MessageBroadcaster()
{
	for (auto container : mHandlers)
	{
		delete container.second;
	}

	mHandlers.clear();
}

void MessageBroadcaster::Unregister(TypeID inMessageID, int inCallbackID)
{
	mHandlers[inMessageID]->Unregister(inCallbackID);
}