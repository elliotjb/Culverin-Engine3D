using CulverinEditor;
using CulverinEditor.Debug;

public class Turret
{
    public GameObject myturret;

    void Update()
    {
        if (Input.MouseButtonRepeat(0))
        {
            myturret.GetComponent<Transform>().RotateAroundAxis(Vector3.Left);
        }
        if (Input.MouseButtonRepeat(1))
        {
            myturret.GetComponent<Transform>().RotateAroundAxis(Vector3.Right);
        }
        if (Input.KeyDown("Space"))
        {
            Debug.Log("BOOOOM!!!");
        }
    }
}