using Sphynx.Core.Native;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.Serialization;
using System.Security.Claims;
using System.Security.Principal;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    public abstract class Component
    {
        public GameObject gameObject { get; internal set; }

        public Transform transform { get; }

        internal readonly ulong ID;

        public virtual void Awake() 
        {
            //I Don't think it's possible to optimize empty function calls away
            //Unless i think of something new i'll leave them as is
            //Or maybe they get optimized already by the compiler either way i'm not sure.
            //Core.Engine.IgnoreCallback(GetType().FullName);
        }

        public virtual void Start() 
        {
        }

        public virtual void Update() 
        { 
        }

        public virtual void FixedUpdate()
        {
        }

        public virtual void OnDestroy() 
        {
        }

        public void Destroy()
        {
            ComponentFactory.DestroyComponent(this);
        }

    }
}
