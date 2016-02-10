using UnityEngine;
using System.Collections;

public class ToGame : MonoBehaviour {
	private GameObject indata;
	private InputData indatacomp;
	// Use this for initialization
	void Start () {
		indata = GameObject.Find ("Indata") as GameObject;
		indatacomp = indata.GetComponent<InputData> ();
	}
	
	// Update is called once per frame
	void Update () {
		//if (Input.GetMouseButtonDown (0)){
		//	Application.LoadLevel ("main");
		//}
		//Debug.Log(Physics2D.OverlapPoint (new Vector2(indatacomp.x,indatacomp.y)));
		if (indatacomp.ColliderClickDown(gameObject)){
			Application.LoadLevel ("main");
		}
	}
	//void OnMouseDown(){
	//	Application.LoadLevel ("main");
	//}
}
