using Sphynx.Core.Graphics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Core
{
    public static class Engine
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool IsRunning();
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static string[] GetEngineArgs();
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void IgnoreCallback(string ClassName);
    }
}
