[Native("Component.h")]
shared abstract class Component{
    private uint64 InstanceID;
    [NativeFunction]
    public bool IsActive(){}
}