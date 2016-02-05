using UnityEngine;
using System.Collections;

public class Diffuculty_controller : MonoBehaviour {
	private int flag = 0;
	private GameObject E;
	private GameObject N;
	private GameObject H;
	private GameObject D;
	// Use this for initialization
	void Start () {
		E = GameObject.Find ("EASY") as GameObject;
		N = GameObject.Find ("NORMAL") as GameObject;
		H = GameObject.Find ("HARD") as GameObject;
		D = GameObject.Find ("GameDataObject") as GameObject;
		D.GetComponent<GameData> ().SetDifficulty (1);
	}
	
	// Update is called once per frame
	void Update () {
		bool r = Input.GetKeyDown(KeyCode.RightArrow);
		bool l = Input.GetKeyDown(KeyCode.LeftArrow);
		bool u = Input.GetKeyDown (KeyCode.UpArrow);
		if (u) {
			Application.LoadLevel ("HighScore");
		}
		switch (flag) {
		case 0:
			if (r) {
				H.GetComponent<Slide_Pic> ().SlideIn ();
				D.GetComponent<GameData> ().SetDifficulty (2);
				flag = 1;
			}
			if (l) {
				E.GetComponent<Slide_Pic> ().SlideIn ();
				D.GetComponent<GameData> ().SetDifficulty (0);
				flag = -1;
			}
			break;
		case 1:
			if (l) {
				H.GetComponent<Slide_Pic> ().SlideOut ();
				D.GetComponent<GameData> ().SetDifficulty (1);
				flag = 0;
			}
			break;
		case -1:
			if (r) {
				E.GetComponent<Slide_Pic> ().SlideOut ();
				D.GetComponent<GameData> ().SetDifficulty (1);
				flag = 0;
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
