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
        internal static extern NativeComponent CreateNative();

        internal static void DestroyComponent(Component component)
        {
            component.OnDestroy();
            component.Native.Dispose(); 
        }

        public static T CreateComponent<T>(GameObject go) where T : Component, new()
        {
            var TComp = new T();
            TComp.Native = CreateNative();
            TComp.gameObject = go;

            TComp.Start();

            return TComp;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static void CopyNativeComponent(GameObject Source, GameObject Destination, NativeComponent component);

        public static void CopyComponent<T>(GameObject Origin, GameObject Destination) where T : Component, new() 
        {
            var comp = Origin.GetComponent<T>();
            if (comp != null)
            {
                CopyNativeComponent(Origin,Destination, comp.Native);
            }
            else
            {
                //Implement Debbuger
                Debugger.Break();
                throw new NullReferenceException("");
            }
        }
    }
}
