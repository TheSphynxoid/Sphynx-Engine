using Sphynx.Core;
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
    /// <summary>
    /// Basic Construction block for the game.
    /// Inherit from this to create scripts that bind to <see cref="GameObject"/> instance .
    /// </summary>
    public abstract class Component
    {
        public GameObject gameObject { get; internal set; }

        public Transform transform { get => gameObject.transform; }

        internal readonly EntityID ID;

        public virtual void Awake() 
        {
            //This may become possible.
            //Core.Engine.IgnoreCallback(GetType().FullName);
        }

        public virtual void Start() 
        {
        }
        /// <summary>
        /// Called every Engine Update Loop (Every Frame).
        /// </summary>
        public virtual void Update() 
        { 
        }
        /// <summary>
        /// Called every physics update (on a fixed period).
        /// </summary>
        public virtual void FixedUpdate()
        {
        }
        /// <summary>
        /// Called When Component Removed or GameObject Destroyed.
        /// </summary>
        public virtual void OnDestroy() 
        {
        }

        public void Destroy()
        {
            ComponentFactory.DestroyComponent(this);
        }

        public EntityID GetID()
        {
            return ID;
        }
    }
}
