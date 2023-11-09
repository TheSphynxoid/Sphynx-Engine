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
        public Vector3 Position
        {
            get => position;
            set
            {
                position = value;
                IsChanged = true;
            }
        }

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
            base.Update();
            if (IsChanged)
            {
                SetPosition(gameObject, position);
            }
        }
    }
}
