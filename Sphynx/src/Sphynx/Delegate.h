#pragma once
#ifndef Sphynx_Delegate
#define Sphynx_Delegate
<<<<<<< HEAD
=======
#include "Object.h"

//Will Probably Remove This.

>>>>>>> Dev_ComponentSystem
namespace Sphynx {
    //For Storing Gets Rid of the Instance Parameter. Base For Delegates.
    //Is there a way to convert this to Delegate ?
    template<typename Return, typename ...Args>
<<<<<<< HEAD
    class __Base_Delegate {
    protected:
        virtual Return Invokation(Args... args) = 0;
        __Base_Delegate() {};
    public:
        template<typename rt,typename...args>
        __Base_Delegate(__Base_Delegate<rt, args...>& bd) {}
        Return Invoke(Args... args) { Invokation(std::forward<Args>(args)...); };
        Return operator()(Args&... args) { Invoke(std::forward<Args&>(args)...); };
        //This Causes Problems if Args equal a Reference Type. (The EventSystem Breaks This).
        //Return operator()(Args&&... args) { Invoke(std::forward<Args&&>(args)...);};

    };

    //a sealed Delagate Class That inherits from __Base_Delegate.
    template<typename Return, class Instance, typename ...Args>
    class Delegate : public __Base_Delegate<Return, Args...> {
    public:
        template<bool cond>
        struct Function {};
=======
    class BaseDelegate {
    protected:
        virtual Return Invokation(Args... args) {};
        BaseDelegate() {};
    public:
        template<typename rt, typename...args>
        BaseDelegate(BaseDelegate<rt, args...>& bd) {}
        //What is this function ?
        Return Invoke(Args... args) { Invokation(std::forward<Args>(args)...); };
        Return operator()(Args&... args) { Invoke(std::forward<Args&>(args)...); };
    };

    //a sealed Delagate Class That inherits from BaseDelegate.
    template<typename Return, class Instance, typename ...Args>
    class Delegate : public BaseDelegate<Return, Args...> {
    public:
        //Function Wrapper Template.
        template<bool is_member>
        struct Function {};
        //Method member Wrapper Template.
>>>>>>> Dev_ComponentSystem
        template<>
        struct Function<true> {
            typedef Return(Instance::* InstFunc)(Args...);
            InstFunc func;
<<<<<<< HEAD
            Instance* inst;
            Function() {};
            Function(Instance* i, InstFunc cb) : func(cb), inst(i) {};
            Return operator()(Args...args) {
                (inst->*func)(std::forward<Args>(args)...);
            }
        };
=======
            Instance inst;
            Function() {};
            Function(Instance* i, InstFunc cb) : func(cb), inst(i) {};
            Return operator()(Args...args) {
                if (inst == nullptr) { Core_Error("Instance is null, cannot call function."); return; }
                return (inst->*func)(std::forward<Args>(args)...);
            }
        };
        //Free/Static Function Wrapper.
>>>>>>> Dev_ComponentSystem
        template<>
        struct Function<false> {
            typedef Return(*FreeFunc)(Args...);
            FreeFunc func;
            Function(FreeFunc cb) :func(cb) {};
            Return operator()(Args...args) {
<<<<<<< HEAD
                func(std::forward<Args>(args)...);
            }
        };
        typedef Function<std::is_class_v<Instance>> _Function;
=======
                return func(std::forward<Args>(args)...);
            }
        };
        typedef Function<std::is_class_v<Instance>> I_Function;
>>>>>>> Dev_ComponentSystem
    protected:
        Function<std::is_class_v<Instance>> callback;
    public:
        //Delegate.
        Delegate() { callback = NULL; };
        Delegate(const Delegate& d) {
            this->callback = d.callback;
        }
        Delegate(Delegate&& d) {
            std::swap(this->callback, d.callback);
        }
        //If instance is void. Func is a function Pointer. else
        //func is the struct InstFunc.
<<<<<<< HEAD
        Delegate(_Function cb) : callback(cb) {};
        Delegate(Instance* inst, typename _Function::InstFunc f) : callback(inst, f) {}
        Delegate& operator=(Delegate d) {
            std::swap(callback, d.callback);
        }
        Delegate& operator=(_Function f) {
            std::swap(callback,f);
        }
        Return Invokation(Args... args) override { return callback(std::forward<Args>(args)...); };
    };
    template<typename Return, typename ...Args>
    class Delegate<Return, void, Args...> : public __Base_Delegate<Return, Args...> {
=======
        Delegate(I_Function cb) : callback(cb) {};
        Delegate(Instance* inst, typename I_Function::InstFunc f) : callback(inst, f) {}
        Delegate& operator=(Delegate d) {
            std::swap(callback, d.callback);
        }
        Delegate& operator=(I_Function f) {
            std::swap(callback, f);
        }
        Return Invokation(Args... args) override { return callback(std::forward<Args>(args)...); };
    };
    template<class Instance, typename ...Args>
    class Delegate<void, Instance, Args...> : public BaseDelegate<void, Args...> {
    public:
        //Function Wrapper Template.
        template<bool is_member>
        struct Function {};
        //Method member Wrapper Template.
        template<>
        struct Function<true> {
            typedef void(Instance::* InstFunc)(Args...);
            InstFunc func;
            Instance* inst;
            Function() {};
            Function(Instance* i, InstFunc cb) : func(cb), inst(i) {};
            void operator()(Args...args) {
                if (inst == nullptr) { Core_Error("Instance is null, cannot call function."); return; }
                (inst->*func)(std::forward<Args>(args)...);
            }
        };
        //Free/Static Function Wrapper.
        template<>
        struct Function<false> {
            typedef void(*FreeFunc)(Args...);
            FreeFunc func;
            Function(FreeFunc cb) :func(cb) {};
            void operator()(Args...args) {
                func(std::forward<Args>(args)...);
            }
        };
        typedef Function<std::is_class_v<Instance>> I_Function;
    protected:
        Function<std::is_class_v<Instance>> callback;
    public:
        //Delegate.
        Delegate() { callback = NULL; };
        Delegate(const Delegate& d) {
            this->callback = d.callback;
        }
        Delegate(Delegate&& d) {
            std::swap(this->callback, d.callback);
        }
        //If instance is void. Func is a function Pointer. else
        //func is the struct InstFunc.
        Delegate(I_Function cb) : callback(cb) {};
        Delegate(Instance* inst, typename I_Function::InstFunc f) : callback(inst, f) {}
        Delegate& operator=(Delegate d) {
            std::swap(callback, d.callback);
        }
        Delegate& operator=(I_Function f) {
            std::swap(callback, f);
        }
        void Invokation(Args... args) override { /*return*/ callback(std::forward<Args>(args)...); };
    };
    template<typename Return, typename ...Args>
    class Delegate<Return,void,Args...> : public BaseDelegate<Return, Args...> {
>>>>>>> Dev_ComponentSystem
    public:
        struct Function {
            typedef Return(*FreeFunc)(Args...);
            FreeFunc func;
<<<<<<< HEAD
            Function() { func = NULL; };
            Function(FreeFunc cb) :func(cb) {};
            Return operator()(Args...args) {
                func(std::forward<Args>(args)...);
            }
        };
    protected:
        Function callback;
    public:
        //Delegate.
        Delegate() {};
        template<typename rt,typename inst,typename ...args>
        Delegate(Delegate<rt, inst, args...>& del) {
            this->callback = del.callback;
=======
            Function(FreeFunc cb) :func(cb) {};
            Return operator()(Args...args) {
                return func(std::forward<Args>(args)...);
            }
        };
    private:
        Function callback;
    public:
        //Delegate.
        Delegate() { callback = NULL; };
        Delegate(const Delegate& d) {
            this->callback = d.callback;
        }
        Delegate(Delegate&& d) {
            std::swap(this->callback, d.callback);
>>>>>>> Dev_ComponentSystem
        }
        //If instance is void. Func is a function Pointer. else
        //func is the struct InstFunc.
        Delegate(Function cb) : callback(cb) {};
<<<<<<< HEAD
        Delegate(typename Function::FreeFunc f) : callback(f) {};
        Delegate& operator=(Delegate& d) {
            std::swap(callback, d.callback);
            return *this;
        }
        Delegate& operator=(Function f) {
            callback = f;
            return *this;
        }

        Return Invokation(Args... args) override { return callback(std::forward<Args>(args)...); };
    };

=======
        Delegate& operator=(Delegate d) {
            std::swap(callback, d.callback);
        }
        Delegate& operator=(Function f) {
            std::swap(callback, f);
        }
        Return Invokation(Args... args) override { return callback(std::forward<Args>(args)...); };
    };
    template<typename ...Args>
    class Delegate<void, void, Args...> : public BaseDelegate<void, Args...> {
    public:
        struct Function {
            typedef void(*FreeFunc)(Args...);
            FreeFunc func;
            Function() :func(nullptr) {};
            Function(FreeFunc cb) :func(cb) {};
            void operator()(Args...args) {
                func(std::forward<Args>(args)...);
            }
        };
    private:
        Function callback;
    public:
        //Delegate.
        Delegate() { callback = NULL; };
        Delegate(const Delegate& d) {
            this->callback = d.callback;
        }
        Delegate(Delegate&& d) {
            std::swap(this->callback, d.callback);
        }
        //If instance is void. Func is a function Pointer. else
        //func is the struct InstFunc.
        Delegate(typename Function::FreeFunc cb) : callback(cb) {};
        Delegate& operator=(Delegate d) {
            std::swap(callback, d.callback);
        }
        Delegate& operator=(Function f) {
            std::swap(callback, f);
        }
        void Invokation(Args... args) override { callback(std::forward<Args>(args)...); };
    };
>>>>>>> Dev_ComponentSystem
}
#else
#endif