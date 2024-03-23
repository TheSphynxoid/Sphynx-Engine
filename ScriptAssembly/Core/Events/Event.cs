using Sphynx.Graphics;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Sphynx.Core.Events
{
    /// <summary>
    /// Native event base class.
    /// </summary>
    [StructLayout(LayoutKind.Sequential)]
    public abstract class Event
    {
        public bool IsHandled;
    }
}
