using UnityEngine;
using System.Collections;

public class ChangeDifficulty : MonoBehaviour {
	[SerializeField]
	private bool IsClicked;
	private GameObject indata;
	private InputData indatacomp;
	// Use this for initialization
	void Start () {
		IsClicked = false;
		indata = GameObject.Find ("Indata") as GameObject;
		indatacomp = indata.GetComponent<InputData> ();
	}
	
	// Update is called once per frame
	void Update () {
		//OMD ();
		if (indatacomp.ColliderClickDown (gameObject)) {
			IsClicked = true;
		}
	}

	//void OnMouseDown(){
		//if (indatacomp.ColliderClickDown (gameObject)) {
	//		IsClicked = true;
		//}
	//}
	public void ToFalse(){
		IsClicked = false;
	}
	public bool Clicked(){
		return IsClicked;
	}
}
