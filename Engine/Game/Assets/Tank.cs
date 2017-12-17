using CulverinEditor;
using CulverinEditor.Debug;

public class myTank
{
    public GameObject tank;

    void Update()
    {
        if (Input.MouseButtonRepeat(1))
        {
            Debug.Log("LEFT");
            tank.GetComponent<Transform>().IncrementRotation(Vector3.Up);
        }
        if (Input.MouseButtonRepeat(3))
        {
            Debug.Log("RIGHT");
            tank.GetComponent<Transform>().IncrementRotation(Vector3.Down);
        }
        if (Input.KeyRepeat("Up"))
        {
            tank.GetComponent<Transform>().Position += Vector3.Forward;
        }
        if (Input.KeyRepeat("Down"))
        {
            tank.GetComponent<Transform>().Position += Vector3.Backward;
        }
        if (Input.KeyRepeat("Left"))
        {
            tank.GetComponent<Transform>().Position -= Vector3.Left;
        }
        if (Input.KeyRepeat("Right"))
        {
            tank.GetComponent<Transform>().Position -= Vector3.Right;
        }
    }
}
