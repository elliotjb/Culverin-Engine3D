using System.Configuration.Assemblies;

namespace CulverinEditor.Console
{
    public class Console
    {
        public string Log(string var)
        {
            return var;
        }
        public string Log(int var)
        {
            return var.ToString();
        }
        public string Log(float var)
        {
            return var.ToString();
        }
    }
}