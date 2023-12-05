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
        public static bool IsKeyDown(Keys key)
        {
            var state = (Action)InternalKeyState((int)key);
            return state == Action.Pressed || state == Action.Repeat;
        }
        public static bool IsKeyUp(Keys key)
        {
            return (Action)InternalKeyState((int)key) == Action.Released;
        }
        public static bool IsKeyRepeat(Keys key)
        {
            return (Action)InternalKeyState(((int)key)) == Action.Repeat;
        }
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static int InternalKeyState(int key);

        public delegate void KeyEvent(Keys key);

        public static event KeyEvent OnKeyDown;
        public static event KeyEvent OnKeyUp;
        public static event KeyEvent OnRepeat;
    }
}
