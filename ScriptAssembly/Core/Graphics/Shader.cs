using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Core.Graphics
{
    public enum ShaderDataType
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

    public sealed class Shader : IDisposable
    {

        internal UIntPtr id;
        
        public UIntPtr ID { get => id; }

        public ShaderType Type { get; private set; }

        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void CreateShader(Shader obj, byte stype);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static bool IsShaderValid(UIntPtr id);
        [MethodImpl(MethodImplOptions.InternalCall)]
        internal extern static void DeleteShader(UIntPtr id);

        public bool Valid { get => IsShaderValid(id); }

        /// <summary>
        /// Create a shader with it's Native id. (Internal use only.)
        /// </summary>
        /// <param name="id">Native ID.</param>
        /// <param name="type">Shader Type.</param>
        internal Shader(UIntPtr id, ShaderType type)
        {
            this.id = id;
            Type=type;
        }

        public Shader(string path, ShaderType shadertype)
        {
            CreateShader(this, (byte)shadertype);
        }

        ~Shader()
        {
            Dispose();
        }
        public void Dispose() 
        {
            DeleteShader(id);
            id = UIntPtr.Zero;
        }
    }
    [StructLayout(LayoutKind.Sequential)]
    public struct ShaderPack
    {
        Shader Vert;
        Shader Frag;
        Shader TessEval;
        Shader TessControl;
        Shader Geom;

        /// <summary>
        /// Creates a shaderpack (a structure that hold shaders for rendering).
        /// </summary>
        /// <param name="vert">Vertex Shader</param>
        /// <param name="frag">Fragment Shader</param>
        /// <param name="tess1">Tessellation Evaluation Shader</param>
        /// <param name="tess2">Tessellation Control Shader</param>
        /// <param name="geom">Geometry Shader</param>
        ShaderPack(Shader vert, Shader frag, Shader tess1, Shader tess2, Shader geom)
        {
            Vert = vert;
            Frag = frag;
            Geom = geom;
            TessEval = tess1;
            TessControl = tess2;
        }

        void ReplaceShader(Shader shader)
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
    }
}
