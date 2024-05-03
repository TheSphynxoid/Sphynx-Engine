#ifndef Sphynx_Delegate
#define Sphynx_Delegate
#include "Object.h"

//Will Probably Remove This.
//There is a fundamental problem with this.
//Need Refactoring

namespace Sphynx {
    //For Storing Gets Rid of the Instance Parameter. Base For Delegates.
    //Is there a way to convert this to Delegate ?
    template<typename Return, typename ...Args>
    class BaseDelegate {
    protected:
        virtual Return Invocation(Args... args) {};
        BaseDelegate() {};
    public:
        template<typename rt, typename...args>
        BaseDelegate(BaseDelegate<rt, args...>& bd) {}
        virtual ~BaseDelegate() = default;
        //What is this function ?
        Return Invoke(Args... args) { Invocation(SPH_FORWARD(args)...); };
        Return operator()(Args&... args) { Invoke(SPH_FORWARD(args)...); };
    };

    template<typename Return>
    class BaseDelegate<Return, void> {
    protected:
        virtual Return Invocation() {};
        BaseDelegate() {};
    public:
        template<typename rt>
        BaseDelegate(BaseDelegate<rt>& bd) {}
        virtual ~BaseDelegate() = default;
        //What is this function ?
        Return Invoke() { Invocation(); };
        Return operator()() { Invoke(); };
    };

    //a sealed Delagate Class That inherits from BaseDelegate.
    template<typename Return, class Instance, typename ...Args>
    class Delegate final : public BaseDelegate<Return, Args...> {
    public:
        //Function Wrapper Template.
        template<bool is_member>
        struct Function {};
        //Method member Wrapper Template.
        template<>
        struct Function<true> {
            typedef Return(Instance::* InstFunc)(Args...);
            InstFunc func;
            Instance inst;
            Function() {};
            Function(Instance* i, InstFunc cb) : func(cb), inst(i) {};
            Return operator()(Args...args) {
                if (inst == nullptr) { Core_Error("Instance is null, cannot call function."); return; }
                return (inst->*func)(SPH_FORWARD(args)...);
            }
        };
        //Free/Static Function Wrapper.
        template<>
        struct Function<false> {
            typedef Return(*FreeFunc)(Args...);
            FreeFunc func;
            Function(FreeFunc cb) :func(cb) {};
            Return operator()(Args...args) {
                return func(SPH_FORWARD(args)...);
            }
        };
        typedef Function<std::is_class_v<Instance>> I_Function;
    protected:
        Function<std::is_class_v<Instance>> callback;
    public:
        //Delegate.
        Delegate() { callback = NULL; };
        Delegate(const Delegate& d) noexcept {
            this->callback = d.callback;
        }
        Delegate(Delegate&& d) noexcept {
            std::swap(this->callback, d.callback);
        }
        //If instance is void. Func is a function Pointer. else
        //func is the struct InstFunc.
        //Delegate(I_Function cb) : callback(cb) {};
        Delegate(Instance* inst, typename I_Function::InstFunc f) : callback(inst, f) {}
        virtual ~Delegate()override { callback = nullptr; };
        Delegate& operator=(Delegate d) noexcept {
            std::swap(callback, d.callback);
        }
        Delegate& operator=(I_Function f) noexcept {
            std::swap(callback, f);
        }
        Return Invocation(Args... args) override { return callback(SPH_FORWARD(args)...); };
    };
    template<class Instance, typename ...Args>
    class Delegate<void, Instance, Args...> final : public BaseDelegate<void, Args...> {
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
                (inst->*func)(SPH_FORWARD(args)...);
            }
        };
        //Free/Static Function Wrapper.
        template<>
        struct Function<false> {
            typedef void(*FreeFunc)(Args...);
            FreeFunc func;
            Function(FreeFunc cb) :func(cb) {};
            void operator()(Args...args) {
                func(SPH_FORWARD(args)...);
            }
        };
        typedef Function<std::is_class_v<Instance>> I_Function;
    protected:
        Function<std::is_class_v<Instance>> callback;
    public:
        //Delegate.
        Delegate() { callback = NULL; };
        Delegate(const Delegate& d) noexcept {
            this->callback = d.callback;
        }
        Delegate(Delegate&& d) noexcept {
            std::swap(this->callback, d.callback);
        }
        //If instance is void. Func is a function Pointer. else
        //func is the struct InstFunc.
        Delegate(I_Function cb) : callback(cb) {};
        Delegate(Instance* inst, typename I_Function::InstFunc f) : callback(inst, f) {}
        Delegate& operator=(Delegate d) noexcept{
            std::swap(callback, d.callback);
        }
        Delegate& operator=(I_Function f) noexcept {
            std::swap(callback, f);
        }
        void Invocation(Args... args) override { /*return*/ callback(SPH_FORWARD(args)...); };
    };
    template<class Instance>
    class Delegate<void, Instance, void> final : public BaseDelegate<void, void> {
    public:
        //Function Wrapper Template.
        template<bool is_member>
        struct Function {};
        //Method member Wrapper Template.
        template<>
        struct Function<true> {
            typedef void(Instance::* InstFunc)();
            InstFunc func;
            Instance* inst;
            Function() {};
            Function(Instance* i, InstFunc cb) : func(cb), inst(i) {};
            void operator()() {
                if (inst == nullptr) { Core_Error("Instance is null, cannot call function."); return; }
                (inst->*func)();
            }
        };
        //Free/Static Function Wrapper.
        template<>
        struct Function<false> {
            typedef void(*FreeFunc)();
            FreeFunc func;
            Function(FreeFunc cb) :func(cb) {};
            void operator()() {
                func();
            }
        };
        typedef Function<std::is_class_v<Instance>> I_Function;
    protected:
        Function<std::is_class_v<Instance>> callback;
    public:
        //Delegate.
        Delegate() { callback = NULL; };
        Delegate(const Delegate& d) noexcept {
            this->callback = d.callback;
        }
        Delegate(Delegate&& d) noexcept {
            std::swap(this->callback, d.callback);
        }
        //If instance is void. Func is a function Pointer. else
        //func is the struct InstFunc.
        Delegate(I_Function cb) : callback(cb) {};
        Delegate(Instance* inst, typename I_Function::InstFunc f) : callback(inst, f) {}
        Delegate& operator=(Delegate d) noexcept {
            std::swap(callback, d.callback);
        }
        Delegate& operator=(I_Function f) noexcept {
            std::swap(callback, f);
        }
        void Invocation() override { /*return*/ callback(); };
    };
    template<typename Return, typename ...Args>
    class Delegate<Return, void, Args...> final : public BaseDelegate<Return, Args...> {
    public:
        struct Function {
            typedef Return(*FreeFunc)(Args...);
            FreeFunc func;
            Function(FreeFunc cb) :func(cb) {};
            Return operator()(Args...args) {
                return func(SPH_FORWARD(args)...);
            }
        };
    private:
        Function callback;
    public:
        //Delegate.
        Delegate() { callback = NULL; };
        Delegate(const Delegate& d) noexcept {
            this->callback = d.callback;
        }
        Delegate(Delegate&& d) noexcept {
            std::swap(this->callback, d.callback);
        }
        //If instance is void. Func is a function Pointer. else
        //func is the struct InstFunc.
        Delegate(Function cb) : callback(cb) {};
        Delegate& operator=(Delegate d) noexcept {
            std::swap(callback, d.callback);
        }
        Delegate& operator=(Function f) noexcept {
            std::swap(callback, f);
        }
        Return Invocation(Args... args) override { return callback(SPH_FORWARD(args)...); };
    };
    template<typename ...Args>
    class Delegate<void, void, Args...> final : public BaseDelegate<void, Args...> {
    public:
        struct Function {
            typedef void(*FreeFunc)(Args...);
            FreeFunc func;
            Function() :func(nullptr) {};
            Function(FreeFunc cb) :func(cb) {};
            void operator()(Args...args) {
                func(SPH_FORWARD(args)...);
            }
        };
    private:
        Function callback;
    public:
        //Delegate.
        Delegate() { callback = NULL; };
        Delegate(const Delegate& d) noexcept {
            this->callback = d.callback;
        }
        Delegate(Delegate&& d) noexcept {
            std::swap(this->callback, d.callback);
        }
        //If instance is void. Func is a function Pointer. else
        //func is the struct InstFunc.
        Delegate(typename Function::FreeFunc cb) : callback(cb) {};
        Delegate& operator=(Delegate d) noexcept {
            std::swap(callback, d.callback);
        }
        Delegate& operator=(Function f) noexcept {
            std::swap(callback, f);
        }
        void Invocation(Args... args) override { callback(SPH_FORWARD(args)...); };
    };
}
#else
#endif