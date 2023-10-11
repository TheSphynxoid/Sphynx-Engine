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
        public string Name { get; set; }

        public ulong Id { get; internal set; }

        public Transform transform { get; set; }

        internal Dictionary<Type,Component> components = new Dictionary<Type, Component>();

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

        //public void SwapComponents<T1,T2>() where T1 : Component, new() where T2 : Component, new()
        //{
        //    var c = components[typeof(T1)];
            
        //}

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
