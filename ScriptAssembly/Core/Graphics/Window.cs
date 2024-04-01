using System;
using System.Collections.Generic;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;

namespace Sphynx.Graphics
{
    [StructLayout(LayoutKind.Sequential)]
    [NativeCppClass]
    public struct Bounds
    {
        public static readonly Bounds Zero = new Bounds(0, 0);
        public int Width;
        public int Height;

        public Bounds(int width, int height) { Width = width; Height = height; }

        public static Bounds operator* (int left, Bounds right)
        {
            right.Width *= left;
            right.Height *= left;
            return right;
        }
        public static Bounds operator +(Bounds left, Bounds right)
        {
            Bounds b;
            b.Width = right.Width + left.Width;
            b.Height = right.Height + left.Height;
            return b;
        }

        public static implicit operator Vector2(Bounds v) => new Vector2(v.Width, v.Height);
        public static explicit operator Vector3(Bounds v) => new Vector3(v.Width, v.Height);
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
        [SuppressUnmanagedCodeSecurity]
        private static extern void SetVsync(bool vsync);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        private static extern bool GetVsync();

        private static bool vsync = GetVsync();
        public static bool Vsync { get => vsync; set { if (value != vsync){ SetVsync(vsync); vsync = value; } } }


        /// <summary>
        /// Avoiding Having to send a struct we seperate the Width and Height
        /// </summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        private static extern void SetSize(int width, int height);
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
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
        /// <param name="Width">New window width.</param>
        /// <param name="Height">New window height.</param>
        [MethodImpl(MethodImplOptions.NoInlining)]
        private static void InvokeResize(int Width, int Height)
        {
            OnResizeEvent?.Invoke(new Bounds(Width, Height));
        }

    }
}
