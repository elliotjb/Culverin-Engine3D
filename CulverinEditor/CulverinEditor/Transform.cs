using System.Runtime.CompilerServices;

namespace CulverinEditor
{
    public class Transform : Component
    {
        //protected Transform();

        //public Quaternion rotation { get; set; }
        //public Vector3 position { get; set; }
        public Vector3 position {
            get
            {
                return GetPosition();
            }
            set
            {
                SetPosition(value);
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Vector3 GetPosition();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void SetPosition(Vector3 value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Vector3 GetRotation();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetRotation(Vector3 value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Vector3 GetScale();

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetScale(Vector3 value);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void LookAt(Vector3 value);
    }
}
