using Sphynx.Core.Graphics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Core
{
    /// <summary>
    /// Engine Utility Class
    /// </summary>
    public static class Engine
    {
        /// <summary>
        /// Checks if the Engine is running.
        /// </summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static bool IsRunning();
        /// <summary>
        /// Gets the args that the Application received to start
        /// </summary>
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static string[] GetEngineArgs();
        /// <summary>
        /// If the C# Scripting Engine is Running on Mono this has a true value. (This is here in case i decide to add the MicrosoftCLR)
        /// </summary>
        public static readonly bool IsMono = (Type.GetType("Mono.Runtime") != null);
    }
}
