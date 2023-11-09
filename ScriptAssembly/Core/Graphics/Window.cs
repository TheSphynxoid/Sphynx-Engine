using System;
using System.Collections.Generic;
using System.Linq;
using System.Reflection.Emit;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Core.Graphics
{
    public struct Bounds
    {
        public int Width,Height;

        public Bounds(int width, int height) { Width = width; Height = height; } 
    }

    /// <summary>
    /// Represents a Graphical window
    /// </summary>
    public static class Window
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetTitle(string title);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetTitle();

        private static string title = GetTitle();
        public static string Title { get => title; set { SetTitle(value);title = value; } }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetVsync(bool vsync);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool GetVsync();

        private static bool vsync = GetVsync();
        public static bool Vsync { get => vsync; set { if (value != vsync){ SetVsync(vsync); vsync = value; } } }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetSize(int width, int height);
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Bounds GetSize();

        private static Bounds bounds = GetSize();
        public static Bounds Bounds { get => bounds; set { SetSize(value.Width, value.Height);bounds = value; } }

        public delegate void OnResize(Bounds bounds);
        /// <summary>
        /// Gets called when window resizes.
        /// </summary>
        public static event OnResize OnResizeEvent;

        /// <summary>
        /// Internal function that handles invoking the OnResizeEvent.
        /// </summary>
        /// <param name="Width">New Window Width</param>
        /// <param name="Height">New Window Height</param>
        internal static void InvokeResize(int Width, int Height)
        {
            OnResizeEvent?.Invoke(new Bounds(Width, Height));
        }

    }
}
