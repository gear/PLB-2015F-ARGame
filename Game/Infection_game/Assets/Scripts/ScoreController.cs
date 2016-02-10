using UnityEngine;
using System.Collections;
//high scoreの表示機能
public class ScoreController : MonoBehaviour {
	private GameObject GameDataObj;
	private int difficulty;
	public UnityEngine.UI.Text score;
	// Use this for initialization
	private int scorenum;
	private GameObject indata;
	private InputData indatacomp;
	void Start () {
		GameDataObj = GameObject.Find ("GameDataObject") as GameObject;
		scorenum = GameDataObj.GetComponent<GameData> ().score;
		difficulty = GameDataObj.GetComponent<GameData> ().difficulty;
		GameDataObj.GetComponent<GameData> ().AlignScore (scorenum,difficulty);
		score.text = "Score:" + scorenum.ToString();
		indata = GameObject.Find ("Indata") as GameObject;
		indatacomp = indata.GetComponent<InputData> ();
	}


	// Update is called once per frame
	void Update () {
		bool d = indatacomp.IsRotate ();
		if (d) {
			Application.LoadLevel ("Start");
			indatacomp.ChangeRotate ();
		}
		bool u = indatacomp.IsTap ();
		if (u) {
			Application.LoadLevel ("HighScore");
			indatacomp.ChangeTap ();
		}
	}
}
