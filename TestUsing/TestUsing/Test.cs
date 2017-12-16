using System;
using CulverinEditor;
using CulverinEditor.Debug;

public class Teste : Culverin
{
    public String name;
    public Teste(String name)
    {
        this.name = name;
        //System.Console.WriteLine("Entity " + name + " constructed");
    }

    ~Teste()
    {
    }

    public String Testsd()
    {
        return Getvar().ToString();
    }

    void Update()
    {
       // int temp = Getvar();

    }

}