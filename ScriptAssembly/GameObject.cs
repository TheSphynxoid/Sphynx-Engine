using Sphynx.Core.Native;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    public class GameObject
    {

        public enum Primitive { Sphere, Capsule, Cube, Plane, Triangle};

        public string Name { get; set; }

        public ulong ID { get; internal set; }

        public Transform transform { get; set; }

        public GameObject()
        {
            Console.WriteLine("GameObject Constructed");

        }

        internal Dictionary<Type,Component> components = new Dictionary<Type, Component>();
        internal void InternalAddComp(Component comp,Type type)
        {
            components.Add(type, comp);
            comp.gameObject = this;
        }

        public static GameObject CreatePrimitive(Primitive primitive)
        {
            return null;
        }

        public T GetComponent<T>() where T : Component, new()
        {
            foreach(var component in components.Values)
            {
                if(component.GetType() == typeof(T))
                {
                    return (T)component;
                }
            }
            return null;
        }

        public T AddComponent<T>() where T : Component, new()
        {           
            var comp = ComponentFactory.CreateComponent<T>(this);
            components.Add(typeof(T),comp);
            return comp;
        }

        public void RemoveComponent<T>() where T : Component, new()
        {
            var comp = components[typeof(T)];
            if (comp.GetType() == typeof(T))
            {
                components.Remove(typeof(T));
                comp.Destroy();
            }

        }

        public static GameObject FindGameObjectByName(string name)
        {
            return null;
        }

        public static GameObject FindGameObjectByID(ulong ID)
        {
            return null;
        }

        /// <summary>
        /// Avoid
        /// </summary>
        /// <param name="index"></param>
        /// <returns></returns>
        /// <exception cref="IndexOutOfRangeException"></exception>
        public Component FindComponentByIndex(int index)
        {
            if(index >= components.Count)
            {
                throw new IndexOutOfRangeException();
            }
            return components.ElementAt(index).Value;
        }

        internal void Update()
        {
            foreach (var comp in components.Values)
            {
                comp.Update();
            }
        }
    }
}
