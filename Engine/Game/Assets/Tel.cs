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
        //Debug.Log("J");
        //Debug.Log(tank.name);
        //tank.GetComponent<Transform>().Loge("Elliot");v
        if (Input.KeyRepeat("Up"))
        {
            tank.GetComponent<Transform>().Position += Vector3.Forward;
            GameObject.gameObject.GetComponent<Transform>().Position += Vector3.Forward;
        }
        if (Input.KeyRepeat("Down"))
        {
            tank.GetComponent<Transform>().Position += Vector3.Backward;
            GameObject.gameObject.GetComponent<Transform>().Position += Vector3.Backward;
        }
        if (Input.KeyRepeat("Left"))
        {
            tank.GetComponent<Transform>().Position -= Vector3.Left;
            GameObject.gameObject.GetComponent<Transform>().Position -= Vector3.Left;
        }
        if (Input.KeyRepeat("Right"))
        {
            tank.GetComponent<Transform>().Position -= Vector3.Right;
            GameObject.gameObject.GetComponent<Transform>().Position -= Vector3.Right;
        }
    }
}