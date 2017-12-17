using CulverinEditor;
using CulverinEditor.Debug;

public class Tank_script
{
    public GameObject tank;
    //public float speed;
    public float rotationSpeed;

    void Start()
    {
        //speed = 2;
        //rotationSpeed = 2;
    }

    void Update()
    {
        //test = 2;
        //Debug.Log("J");
        //Debug.Log(tank.name);
        //tank.GetComponent<Transform>().Loge("Elliot");v
        //float translation = Input.GetMouseXAxis() * speed;
        float rotation = Input.GetMouseXAxis() * rotationSpeed;
        //translation *= Time.DeltaTime();
        rotation *= Time.DeltaTime();

        Vector3 temp = Vector3.Zero;
        //temp.Set(0, 0, translation);
        //GameObject.gameObject.GetComponent<Transform>().Position = temp;
        temp.Set(0, rotation, 0);
        GameObject.gameObject.GetComponent<Transform>().Rotation += temp;
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