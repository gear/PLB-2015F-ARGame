using UnityEngine;
using System.Collections;

public class ScoreManager : MonoBehaviour {
	private GameObject GameDataObj;
	public UnityEngine.UI.Text easyhigh;
	public UnityEngine.UI.Text normalhigh;
	public UnityEngine.UI.Text hardhigh;
	public UnityEngine.UI.Text easyrecord;
	public UnityEngine.UI.Text normalrecord;
	public UnityEngine.UI.Text hardrecord;
	private int[,] scorerecord = new int[3,5];
	private int[] highscore = new int[3];
	// Use this for initialization
	void Start () {
		GameDataObj = GameObject.Find ("GameDataObject") as GameObject;
		scorerecord = GameDataObj.GetComponent<GameData> ().scorerecord;
		highscore = GameDataObj.GetComponent<GameData> ().highscore;
		easyhigh.text = highscore[0].ToString();
		normalhigh.text = highscore[1].ToString();
		hardhigh.text = highscore[2].ToString();
		easyrecord.text = scorerecord [0, 4] + "\n" + scorerecord [0, 3] + "\n" + scorerecord [0, 2] + "\n" + scorerecord [0, 1] + "\n" + scorerecord [0, 0];
		normalrecord.text = scorerecord [1, 4] + "\n" + scorerecord [1, 3] + "\n" + scorerecord [1, 2] + "\n" + scorerecord [1, 1] + "\n" + scorerecord [1, 0];
		hardrecord.text = scorerecord [2, 4] + "\n" + scorerecord [2, 3] + "\n" + scorerecord [2, 2] + "\n" + scorerecord [2, 1] + "\n" + scorerecord [2, 0];
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
