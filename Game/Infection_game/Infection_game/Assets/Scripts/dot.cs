using UnityEngine;
using System.Collections;

public class dot : MonoBehaviour {
	[SerializeField]
	GameObject dots;
	[SerializeField]
	Camera maincamera;
	private GameObject GameDataObj;
	private float dotscale;
	//[SerializeField]
	//GameObject Background;
	// Use this for initialization
	private float maxX, minX, maxY, minY;
	void Start () {
		GameDataObj = GameObject.Find ("GameDataObject") as GameObject;
		dotscale = GameDataObj.GetComponent<GameData> ().dotscale;
		transform.localScale = new Vector3(dotscale, dotscale, dotscale);
		maxX = maincamera.ViewportToWorldPoint (new Vector2 (1, 1)).x;
		minX = maincamera.ViewportToWorldPoint (new Vector2 (0, 1)).x;
		maxY = maincamera.ViewportToWorldPoint (new Vector2 (0, 0)).y;
		minY = maincamera.ViewportToWorldPoint (new Vector2 (0, 1)).y;
		//for(int i=1;i<=20;i++) {
		Instantiate (dots, new Vector3 (Random.Range (minX, maxX), Random.Range (minY, maxY), 0), new Quaternion (0, 0, 0, 0));
		//}	
	}
	
	// Update is called once per frame
	void Update () {

	}

	// OnMouseDown is called once per mouse clicked down
	void OnMouseDown (){
		if (GameDataObj.GetComponent<GameData> ().pauser) {
			GameObject clone = Instantiate (dots, new Vector3 (Random.Range (minX, maxX), Random.Range (minY, maxY), 0), new Quaternion (0, 0, 0, 0)) as GameObject;
			GameDataObj.GetComponent<GameData> ().SetScore ();
			DestroyObject (gameObject);
		}
	}
}
