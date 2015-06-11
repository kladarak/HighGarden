

template<typename TMessage>
int MessageBroadcaster::CallbackContainer<TMessage>::Register(const Functor& inFunctor)
{
	mFunctors.emplace_back( mNextID, inFunctor ); 
	return mNextID++;
}

template<typename TMessage>
void MessageBroadcaster::CallbackContainer<TMessage>::Unregister(int inID)
{
	for (auto iter = mFunctors.begin(), end = mFunctors.end(); iter != end; ++iter)
	{
		if ((*iter).mID == inID)
		{
			mFunctors.erase(iter);
			break;
		}
	}
}
		
template<typename TMessage>
void MessageBroadcaster::CallbackContainer<TMessage>::Broadcast(const TMessage& inMessage) const
{
	for (auto& functor : mFunctors)
	{
		functor(inMessage);
	}
}

			
template<typename TMessage>
MessageBroadcaster::CallbackContainer<TMessage>::FunctorIDWrapper::FunctorIDWrapper(int inID, const Functor& inFunctor)
	: mID		(inID)
	, mFunctor	(inFunctor)
{
}
template<typename TMessage>
void MessageBroadcaster::CallbackContainer<TMessage>::FunctorIDWrapper::operator()(const TMessage& inMsg) const
{
	mFunctor(inMsg);
}

template<typename TMessage>
MessageRegistrationHandle MessageBroadcaster::Register(const std::function<void (const TMessage&)>& inFunctor)
{
	TypeID messageID = gGetTypeID<TMessage>();

	if ( mHandlers.find(messageID) == mHandlers.end() )
	{
		mHandlers[messageID] = new CallbackContainer<TMessage>();
	}

	auto container = static_cast< CallbackContainer<TMessage>* >( mHandlers[messageID] );
	int callbackID = container->Register(inFunctor);

	return MessageRegistrationHandle(this, messageID, callbackID);
}

template<typename TMessage>
void MessageBroadcaster::Broadcast(const TMessage& inMessage) const
{
	auto typeID			= gGetTypeID<TMessage>();
	auto handlersIter	= mHandlers.find(typeID);

	if (handlersIter != mHandlers.end())
	{
		auto baseContainer		= (*handlersIter).second;
		auto handlerContainer	= static_cast<CallbackContainer<TMessage>*>( baseContainer );
		handlerContainer->Broadcast(inMessage);
	}
}
