using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    public static class SceneManager
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static Scene GetCurrentScene();
    }
}
