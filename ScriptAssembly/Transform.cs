using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    public sealed class Transform : Component
    {
        public Vector3 Position
        {
            get => GetPosition(gameObject);
            set
            {
                SetPosition(gameObject, value);
            }
        }

        //TODO: Implement Parent-Child relation.
        
        //public Transform Parent { get; set; }
        //public List<Transform> Children { get; set; }
        //bool IsRelative;
        

        internal Transform() { }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetPosition(GameObject go, Vector3 pos);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern Vector3 GetPosition(GameObject go);

        public override void Awake()
        {
        }

        public override void Update()
        {
        }
    }
}
