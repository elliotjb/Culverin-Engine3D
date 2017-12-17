using CulverinEditor;
using CulverinEditor.Debug;

//Attach this script to the tank parent object if you want to see it rotate
public class myTank
{
    public float movSpeed = 0.0f;
    public Vector3 final_mov;

    void Update()
    {
        if (Input.KeyRepeat("Up"))
        {
            final_mov = (movSpeed * Time.DeltaTime()) * Vector3.Forward;
            GameObject.gameObject.GetComponent<Transform>().Position += final_mov;
        }
        if (Input.KeyRepeat("Down"))
        {
            final_mov = (movSpeed * Time.DeltaTime()) * Vector3.Backward;
            GameObject.gameObject.GetComponent<Transform>().Position += final_mov;
        }
        if (Input.KeyRepeat("Left"))
        {
            final_mov = (movSpeed * Time.DeltaTime()) * Vector3.Left;
            GameObject.gameObject.GetComponent<Transform>().Position -= final_mov;
        }
        if (Input.KeyRepeat("Right"))
        {
            final_mov = (movSpeed * Time.DeltaTime()) * Vector3.Right;
            GameObject.gameObject.GetComponent<Transform>().Position -= final_mov;
        }
    }
}