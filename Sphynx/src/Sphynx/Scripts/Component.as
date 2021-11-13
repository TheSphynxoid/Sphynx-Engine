[Native("Component.h")]
shared class Component{
    [NativeProp(uintptr_t)]
    private uint64 InstanceID;
    [NativeFunction(bool)]
    virtual bool IsActive(){}
}