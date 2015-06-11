#pragma once

#include <vector>
#include <unordered_map>
#include <functional>

#include <Core/RTTI/TypeID.h>

class MessageBroadcaster;

class MessageRegistrationHandle
{
public:
	MessageRegistrationHandle();
	MessageRegistrationHandle(MessageBroadcaster* inBroadcaster, TypeID	inTypeID, int inCallbackID);
	void				Unregister();

private:
	MessageBroadcaster* mBroadcaster;
	TypeID				mTypeID;
	int					mCallbackID;
};

class MessageBroadcaster
{
private:
	class CalbackContainerBase
	{
	public:
		virtual ~CalbackContainerBase() { }
		virtual void Unregister(int inCallbackID) = 0;
	};

	template<typename TMessage>
	class CallbackContainer : public CalbackContainerBase
	{
	public:
		typedef std::function<void (const TMessage&)> Functor;

		CallbackContainer() : mNextID(0) { }

		int					Register(const Functor& inFunctor);
		inline virtual void	Unregister(int inID); // "inline" fixes "unreferenced local function has been removed" warning.
		void				Broadcast(const TMessage& inMessage) const;

	private:
		struct FunctorIDWrapper
		{
			int		mID;
			Functor mFunctor;

			FunctorIDWrapper(int inID, const Functor& inFunctor);
			void operator()(const TMessage& inMsg) const;
		};

		std::vector<FunctorIDWrapper>	mFunctors;
		int								mNextID;
	};
	
	std::unordered_map< TypeID, CalbackContainerBase* > mHandlers;

public:
	~MessageBroadcaster();

	template<typename TMessage>
	MessageRegistrationHandle	Register(const std::function<void (const TMessage&)>& inFunctor);

	void						Unregister(TypeID inMessageID, int inCallbackID);

	template<typename TMessage>
	void						Broadcast(const TMessage& inMessage) const;
};

#include "MessageBroadcaster.inl"
