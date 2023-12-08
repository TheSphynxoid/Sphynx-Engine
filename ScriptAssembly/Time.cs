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
        internal extern static float GetDeltaTime();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static ulong GetDeltaTicks();

        public static float deltaTime { get => GetDeltaTime(); }
        public static ulong deltaTicks { get => GetDeltaTicks();}
        
    }
}
