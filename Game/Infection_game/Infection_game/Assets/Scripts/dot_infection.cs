using UnityEngine;
using System.Collections;

public class dot_infection : MonoBehaviour {
	private GameObject GameDataObj;
	private float dotscale;
	private bool tmpInfected;
	private bool Infected;
	[SerializeField]
	private Sprite HoldSprite;
	private SpriteRenderer MainSpriteRenderer;
	//[SerializeField]
	//GameObject Background;
	// Use this for initialization
	void Start () {
		GameDataObj = GameObject.Find ("GameDataObject") as GameObject;
		dotscale = GameDataObj.GetComponent<GameData> ().dotscale;
		transform.localScale = new Vector3(dotscale, dotscale, dotscale);
		MainSpriteRenderer = gameObject.GetComponent<SpriteRenderer>();
		tmpInfected = false;
		Infected = false;
	}
	
	// Update is called once per frame
	void Update () {
		if (tmpInfected == true) {
			MainSpriteRenderer.sprite = HoldSprite;
			tmpInfected = false;
			Infected = true;
		}
	}

	// OnMouseDown is called once per mouse clicked down
	void OnMouseDown (){
		if (GameDataObj.GetComponent<GameData> ().pauser) {
			//GameObject clone = Instantiate (dots, new Vector3 (Random.Range (minX, maxX), Random.Range (minY, maxY), 0), new Quaternion (0, 0, 0, 0)) as GameObject;
			//GameDataObj.GetComponent<GameData> ().SetScore ();
			DestroyObject (gameObject);
			Killed ();
		}
	}
	public void SetInfected (){
		tmpInfected = true;
	}
	public bool IsInfected(){
		return Infected;
	}
	public void Killed(){
		Infected = true;
	}
}
