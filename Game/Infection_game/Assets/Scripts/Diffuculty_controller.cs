using UnityEngine;
using System.Collections;

public class Diffuculty_controller : MonoBehaviour {
	private int flag = 0;
	private GameObject E;
	private GameObject N;
	private GameObject H;
	private GameObject D;
	private GameObject R,L;
	private GameObject indata;
	private InputData indatacomp;
	// Use this for initialization
	void Start () {
		E = GameObject.Find ("EASY") as GameObject;
		N = GameObject.Find ("NORMAL") as GameObject;
		H = GameObject.Find ("HARD") as GameObject;
		D = GameObject.Find ("GameDataObject") as GameObject;
		R = GameObject.Find ("RSELECT") as GameObject;
		L = GameObject.Find ("LSELECT") as GameObject;
		D.GetComponent<GameData> ().SetDifficulty (1);
		indata = GameObject.Find ("Indata") as GameObject;
		indatacomp = indata.GetComponent<InputData> ();
	}
	
	// Update is called once per frame
	void Update () {
		bool r = R.GetComponent<ChangeDifficulty> ().Clicked ();
		bool l = L.GetComponent<ChangeDifficulty> ().Clicked ();
		//bool u = Input.GetKeyDown (KeyCode.UpArrow);
		bool u = indatacomp.IsTap();
		if (u) {
			Application.LoadLevel ("HighScore");
			indatacomp.ChangeTap ();
		}
		switch (flag) {
		case 0:
			if (r) {
				H.GetComponent<Slide_Pic> ().SlideIn ();
				D.GetComponent<GameData> ().SetDifficulty (2);
				flag = 1;
				R.GetComponent<ChangeDifficulty> ().ToFalse ();
			}
			if (l) {
				E.GetComponent<Slide_Pic> ().SlideIn ();
				D.GetComponent<GameData> ().SetDifficulty (0);
				flag = -1;
				L.GetComponent<ChangeDifficulty> ().ToFalse ();
			}
			break;
		case 1:
			if (l) {
				H.GetComponent<Slide_Pic> ().SlideOut ();
				D.GetComponent<GameData> ().SetDifficulty (1);
				flag = 0;
				L.GetComponent<ChangeDifficulty> ().ToFalse ();
			}
			break;
		case -1:
			if (r) {
				E.GetComponent<Slide_Pic> ().SlideOut ();
				D.GetComponent<GameData> ().SetDifficulty (1);
				flag = 0;
				R.GetComponent<ChangeDifficulty> ().ToFalse ();
			}
			break;
		default:
			break;
		}
	}

	private IEnumerator Waitsecond(float sec){
		yield return new WaitForSeconds (sec);
	}
}
