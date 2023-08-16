using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    public static class Input
    {
        /// <summary>
        /// Return true if key is pressed
        /// </summary>
        /// <param name="key"></param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool IsKeyDown(Keys key);
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool IsKeyUp(Keys key);
        [MethodImpl(MethodImplOptions.InternalCall, MethodCodeType = MethodCodeType.Runtime)]
        public extern static bool IsKeyRepeat(Keys key);

        /// <summary>
        /// Avoid For now.
        /// </summary>
        /// <param name="key">The Key to get the state of</param>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static Action GetKeyState(Keys key);

        public delegate void KeyEvent(Keys key);

        public static event KeyEvent OnKeyDown;
        public static event KeyEvent OnKeyUp;
        public static event KeyEvent OnRepeat;
    }
}
