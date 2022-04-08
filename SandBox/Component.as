namespace Sphynx{
    //Mixin ?
    [Header("Component.h")]
    shared abstract class Component : Sphynx::Core::IComponent {
        [NativeProperty(0,"GameObject")]
        private GameObject@ GO;
        [NativeProperty(1,"size_t")]
        private uint64 _ID;
        GameObject@ gameObject{
            get{
                return GO;
            }
        }
        const uint64 InstanceID{
            get{
                return _ID;
            }
        }
        Transform@ transform{
            get{
                return GO.transform;
            }
        }
    }
}