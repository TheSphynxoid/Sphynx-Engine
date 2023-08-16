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
        public virtual void OnDestroy() { }

        public void Destroy()
        {
            ComponentFactory.DestroyComponent(this);
        }
    }

    public sealed class Transform : Component
    {

    }
}
