using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Graphics
{
    public struct RenderObject
    {
		public Mesh mesh;
        public Material mat;
    }

    [NativeCppClass]
    public struct Viewport
    {
        public int X = 0, Y = 0, Width = 0, Height = 0;

        public Viewport(int posX, int posY, int width, int height)
        {
            X = posX;
            Y = posY;
            Width = width;
            Height = height;
        }
    };

    public static class Renderer
    {
        private static Viewport viewport;
        private static Viewport Viewport { get => viewport; set { viewport = value;SetViewport(viewport); } }

        [NativeCppClass]
        private struct NativeRenderObject
        {
            public IntPtr mesh;
            public IntPtr mat;
        }

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetViewport(Viewport viewport);

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Submit_Internal(NativeRenderObject renderObject);

        public static void Submit(RenderObject RO)
        {
            Submit_Internal(new(){ mesh = RO.mesh.NativePtr.Handle, mat = RO.mat.NativePtr.Handle });
        }

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Render();

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern void Clear();
    }
}
