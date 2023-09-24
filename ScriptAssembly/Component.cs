using Sphynx.Core.Native;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    public abstract class Component
    {
        internal NativeComponent Native;

        public GameObject gameObject { get; }

        public Transform transform { get; }

        public ulong ID { get { return Native.NativeID; } }

        public virtual void Start() { }
        public virtual void Update() { }
<<<<<<< HEAD
        public virtual void FixedUpdate() { }
=======
>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d
        public virtual void OnDestroy() { }

        public void Destroy()
        {
            ComponentFactory.DestroyComponent(this);
        }
    }
<<<<<<< HEAD
    public sealed class Transform : Component
    {
        public Vector3 Position { get; set; }
=======

    public sealed class Transform : Component
    {

>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d
    }
}
