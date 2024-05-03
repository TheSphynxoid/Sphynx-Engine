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
    public sealed class Camera : Component
    {
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
        /// Accessed by the engine internally.
        /// </summary>
        static Camera PrimCam;

        /// <summary>
        /// The active <see cref="Scene"/>'s primary <see cref="Camera"/>.
        /// </summary>
        public static Camera PrimaryCamera { get => PrimCam; /*set;*/ }

        //FrameBuffer**
        FrameBuffer frameBuffer;
        public FrameBuffer RenderTarget { get => frameBuffer;}

        bool isOrthographic;
        float AspectRatio;
        float Zoom;
        float FOV;
        float NearClip;
        float FarClip;
        Viewport Viewport;
        mat4 ProjectionMatrix;
        public mat4 ViewMatrix { get; }

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern mat4 GetViewMatrix(IntPtr cam);

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern IntPtr GetNativePrimaryCamera();

        static Camera GetPrimaryCamera()
        {
            
            return null;
        }

        public delegate void OnFramebufferResize();

        public event OnFramebufferResize FramebufferResizeEvent;

        internal Camera()
        {
        }
    }
}
