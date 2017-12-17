using CulverinEditor;
using CulverinEditor.Debug;

public class myTurret
{
    public GameObject myturret;

    void Update()
    {
        if (Input.MouseButtonRepeat(0))
        {
            myturret.GetComponent<Transform>().IncrementRotation(Vector3.Left);
        }
        if (Input.MouseButtonRepeat(1))
        {
            myturret.GetComponent<Transform>().IncrementRotation(Vector3.Right);
        }
        if (Input.KeyDown("Space"))
        {
            Debug.Log("BOOOOM!!!");
        }
    }
}
