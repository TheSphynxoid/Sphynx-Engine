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
        internal static extern int InternalKeyState(int key);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern int InternalButtonState(int button);
        /// <summary>
        /// MAJOR ISSUE : Causes reading invalid memory in core.
        /// </summary>
        /// <returns></returns>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern Vector2 GetMousePosition();
        public static bool IsMouseButtonDown(MouseButton button)
        {
            return (Action)InternalButtonState((int)button) == Action.Pressed;
        }
        public static bool IsMouseButtonUp(MouseButton button)
        {
            return (Action)InternalButtonState((int)button) == Action.Released;
        }
        public static bool IsMouseButtonRepeat(MouseButton button)
        {
            return (Action)InternalButtonState((int)button) == Action.Repeat;
        }

        public delegate void KeyEvent(Keys key);

        //Currently Unused
        public static event KeyEvent OnKeyDown;
        public static event KeyEvent OnKeyUp;
        public static event KeyEvent OnRepeat;
    }
}
