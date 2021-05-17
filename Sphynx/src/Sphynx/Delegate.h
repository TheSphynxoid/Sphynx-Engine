#pragma once
#ifndef Sphynx_Delegate
#define Sphynx_Delegate
namespace Sphynx {
    //For Storing Gets Rid of the Instance Parameter. Base For Delegates.
    //Is there a way to convert this to Delegate ?
    template<typename Return, typename ...Args>
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
        template<>
        struct Function<true> {
            typedef Return(Instance::* InstFunc)(Args...);
            InstFunc func;
            Instance* inst;
            Function() {};
            Function(Instance* i, InstFunc cb) : func(cb), inst(i) {};
            Return operator()(Args...args) {
                (inst->*func)(std::forward<Args>(args)...);
            }
        };
        template<>
        struct Function<false> {
            typedef Return(*FreeFunc)(Args...);
            FreeFunc func;
            Function(FreeFunc cb) :func(cb) {};
            Return operator()(Args...args) {
                func(std::forward<Args>(args)...);
            }
        };
        typedef Function<std::is_class_v<Instance>> _Function;
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
    public:
        struct Function {
            typedef Return(*FreeFunc)(Args...);
            FreeFunc func;
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
        }
        //If instance is void. Func is a function Pointer. else
        //func is the struct InstFunc.
        Delegate(Function cb) : callback(cb) {};
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

}
#else
#endif