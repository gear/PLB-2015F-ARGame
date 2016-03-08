using UnityEngine;
using System.Collections;
using System.Collections.Generic; // Give access to List / Dictionary

public class ObjectPool : MonoBehaviour {

	// Attach to a single type of game object
	// This is a reference to a prefab.
	public RecycleGameObject prefab;

	private List<RecycleGameObject> poolInstances = new List<RecycleGameObject>();

	private RecycleGameObject CreateInstance(Vector3 pos) {

		var clone = GameObject.Instantiate (prefab);
		clone.transform.position = pos;
		clone.transform.parent = transform; // GameObject transform that ObjectPool attach to

		poolInstances.Add (clone);

		return clone;
	}

	public RecycleGameObject NextObject(Vector3 pos) {

		RecycleGameObject instance = null;

		foreach (var go in poolInstances) {
			if (go.gameObject.activeSelf != true) {
				instance = go;
				instance.transform.position = pos;
			}
		}
		if (instance == null)
			instance = CreateInstance (pos);

		instance.Restart ();

		return instance;
	}


}
