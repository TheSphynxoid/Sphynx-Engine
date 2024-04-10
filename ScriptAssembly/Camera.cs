using GlmSharp;
using Sphynx.Core;
using Sphynx.Core.Native;
using Sphynx.Graphics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Security;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx
{
    /// <summary>
    /// This is the managed version of the native camera component.
    /// </summary>
    [Header("Camera.h", NativeTypeName = "Camera")]
    public class Camera : Component
    {
        NativeComponent NativeCamera;

        public struct NativeOnFBResize
        {
            public vec2 OldDimensions;
            public vec2 NewDimensions;
            //A pointer to the camera component object.
            public IntPtr Cam;
            //A pointer to the framebuffer object.
            public IntPtr Frame;
        }
        /// <summary>
        /// The active scene's primary camera.
        /// </summary>
        public static Camera PrimaryCamera { get; set; }

        FrameBuffer frameBuffer;
        public FrameBuffer RenderTarget { get => frameBuffer;}

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern IntPtr GetNativePrimaryCamera();

        public delegate void OnFramebufferResize();

        public event OnFramebufferResize FramebufferResizeEvent;

        internal Camera()
        {
        }
    }
}
