using System;

namespace Example
{
    public class Entity
    {
        private String name;

        public Entity(String name)
        {
            this.name = name;
            System.Console.WriteLine("Entity " + name + " constructed");
        }

        ~Entity()
        {
            System.Console.WriteLine("Entity " + name + " destructed");
        }

        public void Process()
        {
            throw new NotImplementedException("Not implemented yet");
        }

        public String GetName()
        {
            return name;
        }

        public void Hello()
        {
            System.Console.WriteLine("Hello World");
        }
    }
}
