using CulverinEditor;
using CulverinEditor.Debug;

public class Tank_script
{

    public GameObject tank;

    void Start()
    {

    }

    void Update()
    {
        Debug.Log("Jordi");
        Debug.Log(tank.name);
        //Debug.Log(tank.GetComponent<Transform>().position.ToString());
        if (Input.KeyRepeat("Up"))
        {
            tank.GetComponent<Transform>().position += Vector3.Forward;
        }
        else if (Input.KeyRepeat("Down"))
        {
            tank.GetComponent<Transform>().position += Vector3.Backward;
        }
        else if (Input.KeyRepeat("Left"))
        {
            tank.GetComponent<Transform>().position += Vector3.Left;
        }
        else if (Input.KeyRepeat("Right"))
        {
            tank.GetComponent<Transform>().position += Vector3.Right;
        }
    }
}