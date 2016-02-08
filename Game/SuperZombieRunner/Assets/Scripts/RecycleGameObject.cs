using UnityEngine;
using System.Collections;

public class RecycleGameObject : MonoBehaviour {

	public void Restart() {
		gameObject.SetActive (true);
	}

	public void Shutdown() {
		gameObject.SetActive (false);
	}
}
