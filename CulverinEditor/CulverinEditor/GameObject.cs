using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;

namespace CulverinEditor
{
    public class GameObject
    {
        public GameObject()
        {
            CreateGameObject(this);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void CreateGameObject(GameObject go);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void Destroy(GameObject go);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void SetActive(bool go_active);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void IsActive();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void SetParent(GameObject go_parent);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void SetName(string go_name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern void GetName();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Component AddComponent(Type componentType);

        public T AddComponent<T>() where T : Component
        {
            return AddComponent(typeof(T)) as T;
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern Component GetComponent(Type componentType);

        public T GetComponent<T>() where T : Component
        {
            return GetComponent(typeof(T)) as T;
        }
    }
}
