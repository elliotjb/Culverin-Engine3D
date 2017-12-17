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
        Debug.Log("J");
        Debug.Log(tank.name);
        //tank.GetComponent<Transform>().Loge("Elliot");
        if (Input.KeyRepeat("Up"))
        {
            tank.GetComponent<Transform>().Position += Vector3.Forward;
        }
        else if (Input.KeyRepeat("Down"))
        {
            tank.GetComponent<Transform>().Position += Vector3.Backward;
        }
        else if (Input.KeyRepeat("Left"))
        {
            tank.GetComponent<Transform>().Position += Vector3.Left;
        }
        else if (Input.KeyRepeat("Right"))
        {
            tank.GetComponent<Transform>().Position += Vector3.Right;
        }
    }
}