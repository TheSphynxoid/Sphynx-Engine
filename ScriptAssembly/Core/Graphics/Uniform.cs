using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Core.Graphics
{
    [Header("Material.h", NativeTypeName = "Uniform")]
    [NativeWrapper("Uniform", true)]
    public struct Uniform : IDisposable
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal static extern UIntPtr CreateUniform(); 

        /// <summary>
        /// Pointer to the C++ Uniform interface (It's an "Abstract" class).
        /// </summary>
        internal UIntPtr NativeUniform;

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
            NativeUniform = CreateUniform();

            DataType = dataType;
        }
        /// <summary>
        /// Creates a uniform object from a pointer to the data.
        /// </summary>
        /// <param name="DataPtr">if null it's the same as Calling <code>new Uniform(datatype)</param>
        /// <param name="dataType">The type of the data this uniform contains</param>
        public Uniform(UIntPtr DataPtr, ShaderDataType dataType)
        {
            NativeUniform = CreateUniform();
            DataType = dataType;
        }
        /// <summary>
        /// Creates an empty uniform object.
        /// </summary>
        /// <param name="dataType"></param>
        public Uniform(ShaderDataType dataType)
        {
            NativeUniform = CreateUniform();
            DataType = dataType;
        }
        /// <summary>
        /// Sets the data to be sent to the gpu.
        /// </summary>
        /// <param name="DataPtr"></param>
        public void SetData(UIntPtr DataPtr) { }
        /// <summary>
        /// Engine Utility. OpenGL Specifics.
        /// </summary>
        /// <param name="loc"></param>
        internal void SetLocation(uint loc)
        {

        }

        public void Dispose()
        {
            throw new NotImplementedException();
        }
    }
}