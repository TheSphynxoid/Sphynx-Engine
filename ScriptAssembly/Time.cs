using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    public static class Time
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern float GetDeltaTime();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern ulong GetDeltaTicks();

        public static float deltaTime  => GetDeltaTime();
        public static ulong deltaTicks => GetDeltaTicks();
    }
}
