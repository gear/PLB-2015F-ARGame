using UnityEngine;
using System.Collections;

public class GameController : MonoBehaviour {
	private float gametime;
	[SerializeField]
	private float timelimit;
	private float inittime;
	private float intime;
	private GameObject GameDataObj;
	public UnityEngine.UI.Text score;

	// Use this for initialization
	void Start () {
		inittime = Time.time;
		gametime = 0f;
		timelimit = 10f; //10 sec
		GameDataObj = GameObject.Find ("GameDataObject") as GameObject;
		GameDataObj.GetComponent<GameData> ().InitScore ();
		GameDataObj.GetComponent<GameData> ().Pauser (true);
	}
	
	// Update is called once per frame
	void Update () {
		score.text = "Score:" + GameDataObj.GetComponent<GameData> ().score.ToString() + "\n Time:"+ gametime.ToString();
		gametime = Time.time - inittime;
		if (gametime >= timelimit) {
			Endphase ();
		}
		bool d = Input.GetKeyDown(KeyCode.DownArrow);
		if (d) {
			Time.timeScale = 0;
			GameDataObj.GetComponent<GameData> ().Pauser (false);
		}
		bool u = Input.GetKeyDown(KeyCode.UpArrow);
		if (u) {
			Time.timeScale = 1;
			GameDataObj.GetComponent<GameData> ().Pauser (true);
		}
	}
	private void Endphase () {
		Application.LoadLevel ("Results");
	}
}
