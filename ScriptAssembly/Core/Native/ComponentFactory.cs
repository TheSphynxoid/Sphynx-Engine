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

<<<<<<< HEAD
        internal delegate void CompInternalMethod();

        internal static event CompInternalMethod UpdateAll;

        internal static void DestroyComponent(Component component)
        {
            component.OnDestroy();
            component.Native.Dispose(); 
=======
        internal static void DestroyComponent(Component component)
        {
            component.Native.Dispose();
            component.OnDestroy();

>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d
        }

        public static T CreateComponent<T>(string Name) where T : Component, new()
        {
            var TComp = new T();
            TComp.Native = CreateNative(Name);

<<<<<<< HEAD
            UpdateAll+= TComp.Update;

            TComp.Start();

=======
>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d
            return TComp;
        }
    }
}
