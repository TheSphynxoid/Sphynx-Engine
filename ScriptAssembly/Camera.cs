using Sphynx.Core;
using Sphynx.Core.Native;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
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
        /// <summary>
        /// The active scene's primary camera.
        /// </summary>
        public static Camera PrimaryCamera { get; set; }

        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern NativeComponent GetNativePrimaryCamera();

        internal Camera()
        {

        }
    }
}
