namespace Sphynx{
    //Mixin ?
    [Header("Component.h")]
    shared abstract class Component : Sphynx::Core::IComponent {
        private GameObject@ GO;
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

        // void ToString(){};
    }
}