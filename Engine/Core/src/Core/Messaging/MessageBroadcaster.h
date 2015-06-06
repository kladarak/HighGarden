#pragma once

#include <vector>
#include <unordered_map>

#include <Core/RTTI/TypeID.h>

class MessageBroadcaster
{
private:
	class MessageHandlerWrapper
	{
	private:
		template<typename THandler, typename TMessage, typename void (THandler::*THandlerFunc)(const TMessage&)>
		static inline void TypeCastHandlerFunc(void* inHandler, const void* inMessage)
		{
			auto handler = static_cast<THandler*>(inHandler);
			((*handler).*THandlerFunc)( *static_cast<const TMessage*>(inMessage) );
		}
		
		typedef void (*HandlerFunction)(void*, const void*);
		
		HandlerFunction	mHandlerFunc;
		void*			mHandler;

		MessageHandlerWrapper(HandlerFunction inFunc, void* inHandler) : mHandlerFunc(inFunc), mHandler(inHandler) { }

	public:
		template<typename THandler, typename TMessage, typename void (THandler::*THandlerFunc)(const TMessage&)>
		static MessageHandlerWrapper sCreate(THandler* inHandler)
		{
			return MessageHandlerWrapper( &TypeCastHandlerFunc<THandler, TMessage, THandlerFunc>, inHandler );
		}
		
		template<typename TMessage>
		void Handle(const TMessage* inMessage) const	{ mHandlerFunc(mHandler, inMessage); }

		template<typename THandler>
		bool operator==(THandler* inHandler) const		{ return mHandler == inHandler; }

		template<typename THandler>
		bool operator!=(THandler* inHandler) const		{ return !(*this == inHandler); }
	};
	
	std::unordered_map< TypeID, std::vector<MessageHandlerWrapper> > mHandlers;

public:

	template<typename TMessage, typename THandler, typename void (THandler::*THandlerFunc)(const TMessage&)>
	void Register(THandler* inHandler)
	{
		MessageHandlerWrapper wrapper = MessageHandlerWrapper::sCreate<THandler, TMessage, THandlerFunc>(inHandler);
		mHandlers[ gGetTypeID<TMessage>() ].push_back( wrapper );
	}

	template<typename TMessage, typename THandler>
	void Unregister(THandler* inHandler)
	{
		auto& handlers = mHandlers[ gGetTypeID<TMessage>() ];
		for (auto iter = handlers.begin(); iter != handlers.end(); ++iter)
		{
			if ((*iter) == inHandler)
			{
				handlers.erase(iter);
				break;
			}
		}
	}

	template<typename TMessage>
	void Broadcast(const TMessage& inMessage) const
	{
		auto typeID			= gGetTypeID<TMessage>();
		auto handlersIter	= mHandlers.find(typeID);

		if (handlersIter != mHandlers.end())
		{
			auto& handlers = (*handlersIter).second;
			for (auto& handler : handlers)
			{
				handler.Handle(&inMessage);
			}
		}
	}
};
