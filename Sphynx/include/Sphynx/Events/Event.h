#pragma once
#ifndef Sphynx_Events
#define Sphynx_Events
#include <map>
#include <list>
#include <typeindex>
#include <stack>
#include <deque>
#include <variant>
//Observer pattern Event System
//Header-Only EventSystem

//For Now.
#if defined(Sphynx_Delegate)
#undef Sphynx_Delegate
#define SPDEL
#endif

//This is Fucking ugly

namespace Sphynx::Events {
    class EventSystem;
}

namespace Sphynx::Util {
    extern void InternalSkipCurrentEvent(Events::EventSystem& es);
};

namespace Sphynx::Events {
    template<class T, class EventType>
    class EventMemberCallBack;
    //Event Base Class
    class Event
    {
    public:
        bool isHandled = false;
    };
    //Event Handlers.
    class EventCallBackBase {
    protected:
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
    protected:
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
        EventFreeMethodCallBack(Function function) : function(function) {};
    protected:
        Function function;
        void Call(Event& e) {
            EventType& ev = static_cast<EventType&>(e);
            (*function)(ev);
        }
    };

    //A Non-Blocking Observer pattern EventSystem (Event Bus).
    //TODO: Replace EventCallBackXXXX with Delegates.
    class EventSystem {
    private:
#ifndef Sphynx_Delegate
        typedef std::list<EventCallBackBase*> Handlers;
#else
        typedef BaseDelegate<void, Event&> BaseDelegate;
        typedef std::list <BaseDelegate*> Handlers;
#endif // !Sphynx_Delegate
        std::map<std::type_index, Handlers*> subscribers;
        std::deque<std::pair<std::type_index, Event*>> Queue;
        bool PropagateEvent = true;
        void SkipCurrentEvent() {
            PropagateEvent = false;
        };
    public:
        EventSystem() : subscribers() {};
        ~EventSystem() {}
        EventSystem(const EventSystem& es) {
            subscribers = es.subscribers;
            Queue = es.Queue;
        }
        const std::list<Handlers*> GetSubscriberFunctions() {
            std::list<Handlers*> rt;
            for (auto& handles : subscribers) {
                rt.push_back(handles.second);
            }
            return rt;
        }
#ifndef Sphynx_Delegate
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
        void Subscribe(T* instance, void (T::* memberFunction)(EventType&), int position)
        {
            Handlers* handlers = subscribers[std::type_index(typeid(EventType))];

            //First time initialization
            if (handlers == nullptr) {
                handlers = new Handlers();
                subscribers[std::type_index(typeid(EventType))] = handlers;
            }
            auto it = handlers->begin();
            for (int i = 0; i < position; i++)it++;
            handlers->insert(it, dynamic_cast<EventCallBackBase*>(new EventMemberCallBack<T, EventType>(instance, memberFunction)));
        };
        template<class EventType>
        void Subscribe(void (*Function)(EventType&), int position)
        {
            Handlers* handlers = subscribers[std::type_index(typeid(EventType))];

            //First time initialization
            if (handlers == nullptr) {
                handlers = new Handlers();
                subscribers[std::type_index(typeid(EventType))] = handlers;
            }
            auto it = handlers->begin();
            for (int i = 0; i < position; i++)it++;
            handlers->insert(it++, dynamic_cast<EventCallBackBase*>(new EventFreeMethodCallBack<EventType>(Function)));
        };
        template<class T, class EventType>
        void UnSubscribe(T* instance, void (T::* memberFunction)(EventType&))
        {
            Handlers* handlers = subscribers[std::type_index(typeid(EventType))];
            //EventMemberCallBack<T, EventType> mf = EventMemberCallBack<T, EventType>(instance, memberFunction);
            EventCallBackBase* ToDel;
            for (auto& handle : *handlers) {
                if (typeid(*handle) == typeid(EventMemberCallBack<T, EventType>)) {
                    ToDel = handle;
                    break;
                }
            }
            handlers->remove(ToDel);
            delete ToDel;
        };
        template<class EventType>
        void UnSubscribe(void (*function)(EventType&))
        {
            Handlers* handlers = subscribers[std::type_index(typeid(EventType))];
            EventFreeMethodCallBack<EventType> mf = EventFreeMethodCallBack(function);
            EventCallBackBase* ToDel;
            for (auto& handle : *handlers) {
                if (typeid(*handle) == typeid(mf)) {
                    ToDel = handle;
                    break;
                }
            }
            handlers->remove(function);
            delete ToDel;
        };
#else
        template<class Instance, typename EventType>
        void Subscribe(Delegate<void, Instance, EventType&> func)
        {
            Handlers* handlers = subscribers[std::type_index(typeid(EventType))];

            //First time initialization
            if (handlers == nullptr) {
                handlers = new Handlers();
                subscribers[std::type_index(typeid(EventType))] = handlers;
            }
            auto it = handlers->begin();
            for (int i = 0; i < position; i++)it++;
            handlers->push_back(it++, dynamic_cast<BaseDelegate*>(new Delegate<void, Instance, EventType&>(func)));
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

            handlers->push_back(dynamic_cast<BaseDelegate*>(new Delegate<void, void, EventType&>(Function)));
        };
        template<class T, class EventType>
        void Subscribe(T* instance, void (T::* memberFunction)(EventType&))
        {
            Handlers* handlers = subscribers[std::type_index(typeid(EventType))];

            //First time initialization
            if (handlers == nullptr) {
                handlers = new Handlers();
                subscribers[std::type_index(typeid(EventType))] = handlers;
            }
            //auto f = Delegate<void, T, Event&>::Function<true>(instance, memberFunction);
            Delegate<void, T, EventType&>* dt = new Delegate<void, T, EventType&>(instance, memberFunction);
            handlers->push_back(dynamic_cast<BaseDelegate*>(dt));
        };
#endif
        template<class EventType>
        void QueueEvent(EventType e)
        {
            Handlers* handlers = subscribers[std::type_index(typeid(EventType))];
            if (handlers == nullptr)return;
            //Extending Lifetime. Being In list doesn't count as referance thus causes the object to be collected.
            EventType* ptr = new EventType(e);
            auto g = std::pair<std::type_index, EventType*>(std::type_index(typeid(EventType)), ptr);
            Queue.push_back(g);
        };
        //This Should not be overused. A blocking function that dispatch the event on call.
        template<class EventType>
        void DispatchImmediate(EventType e)
        {
            Handlers* handlers = subscribers[std::type_index(typeid(e))];
            if (handlers == nullptr)return;
            for (auto& handle : *handlers) {
                if (handle != nullptr) {
                    handle->Invoke(e);
                    if (!PropagateEvent)/*PropagateEvent = true; */return;
                }
            }
        }
        //Event Bus.
        virtual void Dispatch() {
            //C++17
            for (auto& pair : Queue) {
                Handlers* handlers = subscribers[pair.first];
                for (auto& handle : *handlers) {
                    if (handle != nullptr) {
                        handle->Invoke(*pair.second);
                        //if Event is Handled it won't propagate.
                        //if ((*pair.second).isHandled == true)break;
                        if (!PropagateEvent)/*PropagateEvent = true; */return;
                    }
                }
                //Delete the temporary object.
                delete pair.second;
            }
            //Clear Queue.
            Queue.clear();
        }
        template<class EventType>
        void ClearEventSubscibers() {
            for (auto& pair : subscribers) {
                if (pair.first == typeid(EventType)) {
                    delete pair.second;
                    subscribers.erase(pair.first);
                    return;
                }
            }
        };
        friend void Sphynx::Util::InternalSkipCurrentEvent(EventSystem& es);
    };
    //Pre-Initialize (thread-safe because of static local singleton) EventSystem. 
    //Used for Events without an EventSystem instance(idk errors for example or startup or craches).
    //Static local makes it that the Instance gets initialized before any posible call to GetInstance.
    class GlobalEventSystem final : public EventSystem {
    private:
        GlobalEventSystem() {};
    public:
        static GlobalEventSystem* GetInstance() {
            static GlobalEventSystem Instance;
            return &Instance;
        };
    };
}

#else
#endif
#ifdef SPDEL
#define Sphynx_Delegate
#endif