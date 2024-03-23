using System;
using Sphynx.Core;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Security;

namespace Sphynx.Graphics
{
    [Header("Material.h", NativeTypeName = "Uniform")]
    [NativeWrapper("Uniform", true)]
    public struct Uniform : IDisposable
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        [SuppressUnmanagedCodeSecurity]
        internal static extern UIntPtr CreateUniform(byte[] data, byte shadertype); 

        /// <summary>
        /// Pointer to the C++ Uniform interface (It's an "Abstract" class).
        /// </summary>
        internal UIntPtr NativeUniform;
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
            NativeUniform = CreateUniform(bytes,(byte)dataType);
            DataType = dataType;
        }
        /// <summary>
        /// Creates an empty uniform object.
        /// </summary>
        /// <param name="dataType"></param>
        public Uniform(ShaderDataType dataType)
        {
            NativeUniform = CreateUniform(null, (byte)dataType);
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

        public void Dispose()
        {
            throw new NotImplementedException();
        }
    }
}