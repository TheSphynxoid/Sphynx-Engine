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
        private Vector3 position;
        private bool IsChanged;
        private bool IsRelative;
        public Vector3 Position
        {
            get => position;
            set
            {
                position = value;
                IsChanged = true;
            }
        }

        //Implement Parent-Child relation.
        //public GameObject Parent { get; set; }
        //public List<GameObject> Children { get; set; }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern void SetPosition(GameObject go, Vector3 pos);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern Vector3 GetPosition(GameObject go);

        public override void Awake()
        {
            Position = GetPosition(gameObject);
        }

        public override void Update()
        {
            if (IsChanged)
            {
                SetPosition(gameObject, position);
            }
        }
    }
}
