using UnityEngine;
using System.Collections;

public class Spawner : MonoBehaviour {

	public GameObject[] prefabs;
	public float delay = 2.0f;
	public bool active = true;
	public Vector2 delayRange = new Vector2(1,2);

	// Use this for initialization
	void Start () {
		ResetDelay ();
		// Run a script independently compare to a loop
		StartCoroutine (EnemyGenerator ());
	}

	IEnumerator EnemyGenerator() {
		yield return new WaitForSeconds (delay);

		if (active) {
			var newTransform = transform;
			GameObjectUtil.Instantiate (prefabs[Random.Range (0, prefabs.Length)], newTransform.position);
			ResetDelay (); 
		}

		StartCoroutine (EnemyGenerator ());
	}

	void ResetDelay() {
		delay = Random.Range (delayRange.x, delayRange.y);
	}

}
