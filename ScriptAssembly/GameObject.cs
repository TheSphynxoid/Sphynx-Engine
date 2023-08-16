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

        public ulong Id { get; set; }

        public Transform transform { get; set; }

        public GameObject() { }

        public T GetComponent<T>() where T : Component, new()
        {
            return null;
        }

        public T AddComponent<T>() where T : Component, new()
        {
            return null;
        }

        public static GameObject FindGameObjectByName(string name)
        {
            return null;
        }

        public static GameObject FindGameObjectByID(ulong ID)
        {
            return null;
        }
    }
}
