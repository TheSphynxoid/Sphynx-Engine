using System;
using Sphynx.Core;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Security;
using System.Runtime.InteropServices;

namespace Sphynx.Graphics
{
    [Header("Material.h", NativeTypeName = "Uniform")]
    [NativeWrapper("Uniform", true)]
    public struct Uniform : IDisposable
    {
        bool disposedValue;

        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        internal static extern IntPtr CreateUniform(byte[] data, byte shadertype);

        /// <summary>
        /// Pointer to the C++ Uniform interface (It's an "Abstract" class).
        /// </summary>
        internal HandleRef NativeUniform;

        internal int Location;
        /// <summary>
        /// Gets the Data type the uniform provides.
        /// </summary>
        public ShaderDataType DataType { get; internal set; }
        /// <summary>
        /// Create a uniform object from a buffer
        /// </summary>
        /// <param name="bytes">if null it's the same as calling <code>new Uniform(datatype)</code></param>
        /// <param name="dataType">The type of the data this uniform contains</param>
        public Uniform(Byte[] bytes, ShaderDataType dataType)
        {
            //Copy Bytes into Core's (C++) Memory and send to GPU.
            NativeUniform = new HandleRef(this, CreateUniform(bytes, (byte)dataType));
            DataType = dataType;
        }
        /// <summary>
        /// Creates an empty uniform object.
        /// </summary>
        /// <param name="dataType"></param>
        public Uniform(ShaderDataType dataType)
        {
            NativeUniform = new HandleRef(this, CreateUniform(null, (byte)dataType));
            DataType = dataType;
        }
        /// <summary>
        /// Sets the data to be sent to the gpu.
        /// </summary>
        /// <param name="DataPtr"></param>
        public void SetData(GPUBuffer DataPtr) { }
        /// <summary>
        /// Engine Utility. OpenGL Specifics.
        /// </summary>
        /// <param name="loc"></param>
        internal void SetLocation(int loc)
        {
            Location = loc;
        }

        void Dispose()
        {
            if (!disposedValue)
            {
                // TODO: free unmanaged resources (unmanaged objects) and override finalizer
                // TODO: set large fields to null
                disposedValue=true;
            }
        }

        void IDisposable.Dispose()
        {
            Dispose();
        }
    }
}