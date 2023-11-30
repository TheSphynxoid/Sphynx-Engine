using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    public class Time
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static double GetDeltaTime();

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static ulong GetDeltaTicks();

        public double deltaTime { get => GetDeltaTime(); }
        public ulong deltaTicks { get => GetDeltaTicks();}
        
    }
}
