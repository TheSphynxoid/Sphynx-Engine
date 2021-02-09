#pragma once
#ifndef Sphynx_Events
#define Sphynx_Events
#include <map>
#include <list>
#include <typeindex>

//Observer pattern Event System
//Header-Only EventSystem
namespace Sphynx::Events {

    //Event Base Class
    class Event
    {
    public:
        bool isHandled = false;
    };
    //Event Handlers.
    class EventCallBackBase {
    private:
        virtual void Call(Event& e) = 0;
    public:
        EventCallBackBase() {};
        void Invoke(Event& e) { Call(e); };

    };

    template<class T, class EventType>
    class EventMemberCallBack : public EventCallBackBase {
    public:
        typedef void (T::* MemberFunction)(EventType&);
        EventMemberCallBack(T* instance, MemberFunction memberFunction) : instance(instance), memberFunction(memberFunction) {};
    private:
        T* instance;
        MemberFunction memberFunction;
        void Call(Event& e) {
            (instance->*memberFunction)(static_cast<EventType&>(e));
        }
    };

    template<class EventType>
    class EventFreeMethodCallBack : public EventCallBackBase {
    public:
        typedef void (*Function)(EventType&);
        EventFreeMethodCallBack(Function function) : function (function) {};
    private:
        Function function;
        void Call(Event& e) {
            (*function)(static_cast<EventType&>(e));
        }
    };

    class EventSystem {
    private:
        typedef std::list<EventCallBackBase*> Handlers;
        std::map<std::type_index, Handlers*> subscribers;
    public:
        EventSystem() : subscribers() {};
        template<class T, class EventType>
        void Subscribe(T* instance, void (T::* memberFunction)(EventType&)) 
        {
            Handlers* handlers = subscribers[std::type_index(typeid(EventType))];

            //First time initialization
            if (handlers == nullptr) {
                handlers = new Handlers();
                subscribers[std::type_index(typeid(EventType))] = handlers;
            }

            handlers->push_back(dynamic_cast<EventCallBackBase*>(new EventMemberCallBack<T, EventType>(instance, memberFunction)));
        };
        template<class EventType>
        void Subscribe(void (*Function)(EventType&))
        {
            Handlers* handlers = subscribers[std::type_index(typeid(EventType))];

            //First time initialization
            if (handlers == nullptr) {
                handlers = new Handlers();
                subscribers[std::type_index(typeid(EventType))] = handlers;
            }

            handlers->push_back(dynamic_cast<EventCallBackBase*>(new EventFreeMethodCallBack<EventType>(Function)));
        };
        template<class T, class EventType>
        void UnSubscribe(T* instance, void (T::* memberFunction)(EventType&)) 
        {
            Handlers* handlers = subscribers[std::type_index(typeid(EventType))];
            EventMemberCallBack<T, EventType> mf = EventMemberCallBack(memberFunction);
            EventCallBackBase* ToDel;
            for (auto& handle : *handlers) {
                if (*handle == mf) {
                    ToDel = handle;
                    break;
                }
            }
            handlers->remove(memberFunction);
            delete ToDel;
        };
        template<class EventType>
        void UnSubscribe(void (*function)(EventType&))
        {
            Handlers* handlers = subscribers[std::type_index(typeid(EventType))];
            EventFreeMethodCallBack<EventType> mf = EventFreeMethodCallBack(function);
            EventCallBackBase* ToDel;
            for (auto& handle : *handlers) {
                if (*handle == mf) {
                    ToDel = handle;
                    break;
                }
            }
            handlers->remove(function);
            delete ToDel;
        };
        template<class EventType>
        void Dispatch(EventType& e)
        {
            Handlers* handlers = subscribers[std::type_index(typeid(EventType))];
            if (handlers == nullptr)return;
            for (auto& handle : *handlers) {
                if (handle != nullptr) {
                    handle->Invoke(e);
                }
            }
        };
        ~EventSystem() {
            for (auto& obj : subscribers) {
                Handlers* handlers = obj.second;
                for (auto& handle : *handlers) {
                    delete handle;
                }
                delete handlers;
            }
        }
    };
    //Pre-Initialize (not thread-safe because of singlton) EventSystem. 
    //Used for Events without an EventSystem instance(idk errors for example or startup or craches).
    //TODO: Thread-Safety.
    class GlobalEventSystem : public EventSystem {
    private:
        GlobalEventSystem() {};
        static GlobalEventSystem* Instance;
    public:
        static GlobalEventSystem* GetInstance()  {
            if (Instance == nullptr)Instance = new GlobalEventSystem();
            return Instance;
        };
    };
    inline GlobalEventSystem* GlobalEventSystem::Instance = NULL;
}
#elif
#endif