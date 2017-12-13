using System;
using CulverinEditor;
using CulverinEditor.Console;

public class Test : Culverin
{
    public String name;
    public Test(String name)
    {
        this.name = name;
        //System.Console.WriteLine("Entity " + name + " constructed");
    }

    ~Test()
    {
    }

    public String Testsd()
    {
        return "Jprdo";
    }

    void Update()
    {
        // int temp = Getvar();

    }

}