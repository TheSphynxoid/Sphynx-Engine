[Native("Component.h")]
abstract class Component{
    private uint64 InstanceID;
    [NativeFunction]
    public bool IsActive();
}