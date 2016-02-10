using UnityEngine;
using System.Collections;

public class Background : MonoBehaviour {
	public bool IsClicked = false;
	private GameObject indata;
	private InputData indatacomp;
	// Use this for initialization
	void Start () {
		indata = GameObject.Find ("Indata") as GameObject;
		indatacomp = indata.GetComponent<InputData> ();
	}
	
	// Update is called once per frame
	void Update () {
		OMD ();
	}
	void OMD (){
		if (indatacomp.ColliderClickDown(gameObject)){
			IsClicked = true;
		}
	}
}
