using System;
using CulverinEditor;
using CulverinEditor.Debug;

public class Teste : Culverin
{
    public int a = 1;
    public int b = 2;
    public int c = 4;

    void Start()
    {
        a = 4;
        b = 3;
    }

    void Update()
    {
        c = a + b;

        Debug.Log(c.ToString());
    }

}