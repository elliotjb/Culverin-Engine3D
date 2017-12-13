using System.Configuration.Assemblies;
using System.Runtime.CompilerServices;

namespace CulverinEditor.Console
{
    public class Console
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public static extern string Log(string var);
    }
}