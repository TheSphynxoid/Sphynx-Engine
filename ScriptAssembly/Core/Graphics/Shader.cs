using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Reflection;
using System.Diagnostics.Contracts;
using Mono.CSharp;
using Sphynx.Core;
using System.Security;

namespace Sphynx.Graphics
{
    public enum ShaderDataType : byte
    {
        None = 0, Float, Float2, Float3, Float4, Double, Double2, Double3, Double4, Int, Int2, Int3, Int4, UInt, UInt2, UInt3, UInt4, Bool,
        Mat2x2, Mat2x3, Mat2x4, Mat3x2, Mat3x3, Mat3x4, Mat4x2, Mat4x3, Mat4x4
    }
    public enum ShaderType : byte
    {
        VertexShader,
		FragmentShader,//The Same as pixel shader
		PixelShader = FragmentShader,//The Same as fragment shader
		GeometryShader,
		//OpenGL names. add Ones for DX11/DX12/Vulkan (For Future Compatibility)
		TessellationControlShader, //Optional, For more detail : https://www.khronos.org/opengl/wiki/Tessellation_Control_Shader
		TessellationEvaluationShader //Mandatory for tessellation to be active,//For more detail : https://www.khronos.org/opengl/wiki/Tessellation_Evaluation_Shader
    };

    [StructLayout(LayoutKind.Sequential)]
    public sealed class Shader : IDisposable
    {

        private bool disposedValue;

        //[MarshalAs(UnmanagedType.LPStruct)]
        internal HandleRef id = new HandleRef(null, IntPtr.Zero);

        public HandleRef ID { [Pure] get => id; }

        public ShaderType Type { [Pure] get; [Pure] private set; }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static IntPtr CreateShader(Shader obj, byte stype);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool IsShaderValid(IntPtr id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DeleteShader(IntPtr id);

        public bool Valid { [Pure] get => IsShaderValid(id.Handle); }

        /// <summary>
        /// Create a shader with it's Native id. (Internal use only.)
        /// </summary>
        /// <param name="id">Native ID.</param>
        /// <param name="type">Shader Type.</param>
        internal Shader(IntPtr id, ShaderType type)
        {
            this.id = new HandleRef(this, id);
            Type=type;
        }

        public Shader(string code, ShaderType shadertype)
        {
        }

        private void Dispose(bool disposing)
        {
            if (!disposedValue)
            {
                if (disposing)
                {
                    // TODO: dispose managed state (managed objects)
                }

                // TODO: free unmanaged resources (unmanaged objects) and override finalizer
                DeleteShader(id.Handle);
                id = new HandleRef(null, IntPtr.Zero);
                // TODO: set large fields to null
                disposedValue=true;
            }
        }

        // TODO: override finalizer only if 'Dispose(bool disposing)' has code to free unmanaged resources
        ~Shader()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: false);
        }

        void IDisposable.Dispose()
        {
            // Do not change this code. Put cleanup code in 'Dispose(bool disposing)' method
            Dispose(disposing: true);
            GC.SuppressFinalize(this);
        }
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct ShaderPack
    {
        public Shader Vert = null;
        public Shader Frag = null;
        public Shader TessEval = null;
        public Shader TessControl = null;
        public Shader Geom = null;

        /// <summary>
        /// Creates a shaderpack (a structure that hold shaders for rendering).
        /// </summary>
        /// <param name="vert">Vertex Shader</param>
        /// <param name="frag">Fragment Shader</param>
        /// <param name="tess1">Tessellation Evaluation Shader</param>
        /// <param name="tess2">Tessellation Control Shader</param>
        /// <param name="geom">Geometry Shader</param>
        public ShaderPack(Shader vert, Shader frag, Shader tess1 = null, Shader tess2 = null, Shader geom = null)
        {
            Vert = vert;
            Frag = frag;
            Geom = geom;
            TessEval = tess1;
            TessControl = tess2;
        }

        [Pure]
        public Shader GetByType(ShaderType type)
        {
            switch (type)
            {
                case ShaderType.VertexShader:
                    return Vert;
                case ShaderType.FragmentShader:
                    return Frag;
                case ShaderType.GeometryShader:
                    return Geom;
                case ShaderType.TessellationControlShader:
                    return TessControl;
                case ShaderType.TessellationEvaluationShader:
                    return TessEval;
            }
            return null;
        }
        public void ReplaceShader(ref Shader shader)
        {
            switch (shader.Type)
            {
                case ShaderType.VertexShader:
                    Vert = shader;
                    break;
                case ShaderType.FragmentShader:
                    Frag = shader;
                    break;
                case ShaderType.GeometryShader:
                    Geom = shader;
                    break;
                case ShaderType.TessellationControlShader:
                    TessControl = shader;
                    break;
                case ShaderType.TessellationEvaluationShader:
                    TessEval = shader;
                    break;
            }
        }

        [Pure]
        internal readonly Material.NativeShaderPack ToNative()
        {
            Material.NativeShaderPack native = new Material.NativeShaderPack();
            native.VShader = (Vert == null) ? IntPtr.Zero : Vert.id.Handle;
            native.FShader = (Frag == null) ? IntPtr.Zero : Frag.id.Handle;
            native.TessEval = (TessEval == null) ? IntPtr.Zero : TessEval.id.Handle;
            native.TessControl = (TessControl == null) ? IntPtr.Zero : TessControl.id.Handle;
            native.GeomShader = (Geom == null) ? IntPtr.Zero : Geom.id.Handle;
            return native;
        }
    }

    public static class Utils
    {
        //C-Style naming to avoid confusion.
        [Pure]
        [SuppressUnmanagedCodeSecurity]
        [MethodImpl(MethodImplOptions.InternalCall)]
        static extern ulong GetDataTypeSize_Internal(byte datatype);

        [Pure]
        [SuppressUnmanagedCodeSecurity]
        public static ulong GetSize(this ShaderDataType dataType)
        {
            return GetDataTypeSize_Internal((byte)dataType);
        }
    }
}
