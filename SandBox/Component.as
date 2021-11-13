[Native("Component.h")]
abstract shared class Component {
    private uint64 InstanceID;
    virtual void Start() = 0;
    bool IsActive(){
        return IsNativeActive(this);
    }
    string GetName(){
        return NativeGetName(this);
    }
}