using UnityEngine;
using System.Collections;

public class NormalBall : MonoBehaviour {
	private GameObject GameDataObj;
	[SerializeField]
	private Sprite KBall;
	[SerializeField]
	private Sprite NBall;
	private SpriteRenderer BallSpriteRenderer;
	private bool IsNormal;
	private bool changeflag;
	private Component GAMEDATA;
	//[SerializeField]
	//GameObject Background;
	// Use this for initialization
	void Start () {
		GameDataObj = GameObject.Find ("GameDataObject") as GameObject;
		//GAMEDATA = GameDataObj.GetComponent<GameData> ();
		BallSpriteRenderer = gameObject.GetComponent<SpriteRenderer>();
		IsNormal = true;
	}
	
	// Update is called once per frame
	void Update () {
		if (changeflag == true) {
			if (IsNormal) {
				BallSpriteRenderer.sprite = NBall;
				GameDataObj.GetComponent<GameData> ().Ballchange ();
			} else {
				BallSpriteRenderer.sprite = KBall;
				GameDataObj.GetComponent<GameData> ().Ballchange ();
			}
			changeflag = false;
		}
	}
	public void ChangeBall (){
		if (IsNormal) {
			IsNormal = false;
		} else {
			IsNormal = true;
		}
		changeflag = true;
	}
}
