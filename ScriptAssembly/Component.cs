using Sphynx.Core.Native;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    [AttributeUsage(AttributeTargets.Method,Inherited = true)]
    public class ComponentBaseMethodAttribute : Attribute
    {
        string Name = "";
        public ComponentBaseMethodAttribute() { }
        public ComponentBaseMethodAttribute(string name) { Name = name; }
        public string GetName() => Name;
    }
    public abstract class Component
    {
        internal NativeComponent Native;

        public GameObject gameObject { get; internal set; }

        public Transform transform { get; }

        public ulong ID { get { return Native.NativeID; } }

        [ComponentBaseMethod("Awake")]
        public virtual void Awake() 
        {
            //I Don't think it's possible to optimize them away
            //Unless i think of something new i'll leave them as is
            //Or maybe they get optimized already by the compiler either way i'm not sure.

            //Core.Engine.IgnoreCallback(GetType().FullName);
        }
        [ComponentBaseMethod("Start")]
        public virtual void Start() 
        {
            //Core.Engine.IgnoreCallback(GetType().FullName);
        }
        [ComponentBaseMethod("Update")]
        public virtual void Update() 
        { 
            //Core.Engine.IgnoreCallback(GetType().FullName);
        }
        [ComponentBaseMethod("FixedUpdate")]
        public virtual void FixedUpdate()
        {
            //Core.Engine.IgnoreCallback(GetType().FullName);
        }
        [ComponentBaseMethod("OnDestroy")]
        public virtual void OnDestroy() 
        {
            //Core.Engine.IgnoreCallback(GetType().FullName);
        }

        public void Destroy()
        {
            ComponentFactory.DestroyComponent(this);
        }
    }
}
