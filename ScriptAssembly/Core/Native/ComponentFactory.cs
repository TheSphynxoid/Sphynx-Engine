using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;
using System.Diagnostics;

namespace Sphynx.Core.Native
{
    internal static class ComponentFactory
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern NativeComponent CreateNative(string Name = "");

        internal static void DestroyComponent(Component component)
        {
            component.Native.Dispose();
            component.OnDestroy();

        }

        public static T CreateComponent<T>(string Name) where T : Component, new()
        {
            var TComp = new T();
            TComp.Native = CreateNative(Name);

            return TComp;
        }
    }
}
