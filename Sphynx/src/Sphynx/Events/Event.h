#pragma once
#ifndef Sphynx_Events
#define Sphynx_Events
#include <map>
#include <list>
#include <typeindex>
#include <stack>
#include <forward_list>
#include <variant>

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
    class EventSystem{
    private:
        typedef std::list<EventCallBackBase*> Handlers;
        std::map<std::type_index, Handlers*> subscribers;
        std::forward_list<std::pair<std::type_index, Event*>> Queue;
    public:
        EventSystem() : subscribers() {};
        const std::list<Handlers*> GetSubscriberFunctions() {
            std::list<Handlers*> rt;
            for (auto& handles : subscribers) {
                rt.push_back(handles.second);
            }
            return rt;
        }
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
        void QueueEvent(EventType& e)
        {
            Handlers* handlers = subscribers[std::type_index(typeid(EventType))];
            if (handlers == nullptr)return;
            //Extending Lifetime. Being In list doesn't count as referance thus causes the object to be collected.
            EventType* ptr = new EventType(e);
            auto g = std::pair(std::type_index(typeid(EventType)), ptr);
            Queue.push_front(g);
        };
        //This Should not be overused. A blocking function that dispatch the event on call.
        template<class EventType>
        void DispatchImmediate(EventType& e) 
        {
            Handlers* handlers = subscribers[std::type_index(typeid(e))];
            if (handlers == nullptr)return;
            for (auto& handle : *handlers) {
                if (handle != nullptr) {
                    handle->Invoke(e);
                    //if Event is Handled it won't propagate.
                    if (e.isHandled == true)return;
                }
            }
        }
        //Event Bus.
        void Dispatch() {
            Queue.reverse();
            //C++17
            for (auto& pair : Queue) {
                Handlers* handlers = subscribers[pair.first];
                for (auto& handle : *handlers) {
                    if (handle != nullptr) {
                        handle->Invoke(*pair.second);
                        //if Event is Handled it won't propagate.
                        if ((*pair.second).isHandled == true)break;
                    }
                }
                //Delete the temporary object.
                delete pair.second;
            }
            //Clear Queue.
            Queue.clear();
        }
        template<class EventType>
        void ProcessFunction() {}

        ~EventSystem() {
        }
    };
    //Pre-Initialize (thread-safe because of static local singleton) EventSystem. 
    //Used for Events without an EventSystem instance(idk errors for example or startup or craches).
    //Static local makes it that Instance (The local variable in this case) gets initialized before any posible call to GetInstance.
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

#elif
#endif