using UnityEngine;
using System.Collections;

public class HighScoreController : MonoBehaviour {
	private GameObject indata;
	private InputData indatacomp;
	// Use this for initialization
	void Start () {
		indata = GameObject.Find ("Indata") as GameObject;
		indatacomp = indata.GetComponent<InputData> ();
	}
	
	// Update is called once per frame
	void Update () {
		if (indatacomp.ClickDown()){
			Application.LoadLevel ("Select_difficulty");
		}
	}
}
