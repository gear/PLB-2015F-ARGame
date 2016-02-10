using UnityEngine;
using System.Collections;

public class GameData : MonoBehaviour {
	private static bool created = false;
	public int difficulty;
	public float dotscale;
	public int score;
	public bool pauser;
	[SerializeField]
	private int recordnumber = 5;
	public int[,] scorerecord = new int[3,5];
	public int[] highscore = new int[3];
	public int dotnumber = 10;
	public int randmin = 2;
	public int randmax = 4;
	public int numinfected;
	public bool BALLTYPE = true; // true is normal ball
	void Awake(){
		if(!created){
			// シーンを切り替えても指定のオブジェクトを破棄せずに残す
			DontDestroyOnLoad(this.gameObject);
			// 生成した
			created = true;
		} else {
			Destroy(this.gameObject);
		}
	}

	// Use this for initialization
	void Start () {
		dotscale = 0.3f;
		pauser = true;
		for (int j=1; j<=3; j++){
			for (int i = 1; i <=recordnumber; i++) {
				scorerecord [j-1,i - 1] = 0;
			}
			highscore[j-1] = 0;
		}
	}

	// Update is called once per frame
	void Update () {
	
	}
	public void SetScale(float s){
		dotscale = s;
	}
	public void SetDifficulty (int d){
		difficulty = d;
		if (d == 0){
			dotnumber = 7;
			SetNumInfected (1);
			//SetScale (1.5f);
		}
		else if (d == 2){
			SetNumInfected (3);
			dotnumber = 15;
			//SetScale(0.5f);
		}
		else{
			dotnumber = 10;
			SetNumInfected (2);
			//SetScale(1f);
		}
	}
	public void SetScore(){
		score += 1;
	}
	public void InitScore(){
		score = 0;
	}
	public void Pauser(bool p){
		pauser = p;
	}
	public void AlignScore(int n,int d){
		for (int i = 1; i <= recordnumber - 1; i++) {
			scorerecord [d,i - 1] = scorerecord [d,i];
		}
		scorerecord [d,recordnumber - 1] = n;
		if (n > highscore[d]) {
			highscore[d] = n;
		}
	}
	public void SetNumInfected (int n){
		numinfected = n;
	}
	public void Ballchange (){
		if (BALLTYPE) {
			BALLTYPE = false;
		} else {
			BALLTYPE = true;
		}
	}
}
