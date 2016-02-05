using UnityEngine;
using System.Collections;
//high scoreの表示機能
public class ScoreController : MonoBehaviour {
	private GameObject GameDataObj;
	private int difficulty;
	public UnityEngine.UI.Text score;
	// Use this for initialization
	private int scorenum;
	void Start () {
		GameDataObj = GameObject.Find ("GameDataObject") as GameObject;
		scorenum = GameDataObj.GetComponent<GameData> ().score;
		difficulty = GameDataObj.GetComponent<GameData> ().difficulty;
		GameDataObj.GetComponent<GameData> ().AlignScore (scorenum,difficulty);
		score.text = "Score:" + scorenum.ToString();
	}


	// Update is called once per frame
	void Update () {
		bool d = Input.GetKeyDown(KeyCode.DownArrow);
		if (d) {
			Application.LoadLevel ("Start");
		}
		bool u = Input.GetKeyDown (KeyCode.UpArrow);
		if (u) {
			Application.LoadLevel ("HighScore");
		}
	}
}
