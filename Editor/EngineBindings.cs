using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace Editor
{
    static class EngineBindings
    {
        private const string EngineDllName = "Engine.dll";

        [DllImport(EngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int runWinForms(IntPtr panelHandle);


        [DllImport(EngineDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern int runStandalone();
    }
}
