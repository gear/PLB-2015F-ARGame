using UnityEngine;
using System.Collections;

public class GameController_infection : MonoBehaviour {
	private GameObject GameDataObj;
	public UnityEngine.UI.Text score;
	private float maxX, minX, maxY, minY;
	private GameObject [] GameDot;
	private dot_infection[] GameDotComp;
	private GameObject [] GameLineObj;
	private LineRenderer[] GameLine;
	private int numdot;
	private int numLine;
	private int [,] LineMat;
	private int rmin,rmax;
	private Vector3[] randvector;
	private int maxedge;
	private int numdestroy = 0;
	private bool myinit = true;
	private bool spinflag = false;
	[SerializeField]
	GameObject Lines;
	[SerializeField]
	GameObject dots;
	[SerializeField]
	Camera maincamera;
	// Use this for initialization
	void Start () {
		Initial ();
		tmpMakemat ();
		dotline ();
		myinit = true;
	}
	
	// Update is called once per frame
	void Update () {
		Initialization ();
		DetectSpin ();
		IsDestroyed ();
	}



	/// <summary>
	/// function
	/// </summary>

	private void Endphase () {
		bool fl = true;
		for (int i = 0; i < numdot; i++) {
			if (GameDotComp [i].IsInfected ()) {
				for (int j = 0; j < numdot; j++) {
					if (LineMat [i, j] > 0 && !GameDotComp [j].IsInfected ()) {
						fl = false;
					}
				}
			}
		}
		if (fl) {
			for (int i = 0; i < numdot; i++) {
				if (!GameDotComp [i].IsInfected ()) {
					GameDataObj.GetComponent<GameData> ().SetScore();
				}
			}
			Application.LoadLevel ("Results");
		}
	}
	private void Initial (){
		GameDataObj = GameObject.Find ("GameDataObject") as GameObject;
		GameDataObj.GetComponent<GameData> ().InitScore ();
		maxX = maincamera.ViewportToWorldPoint (new Vector2 (1, 1)).x;
		minX = maincamera.ViewportToWorldPoint (new Vector2 (0, 1)).x;
		maxY = maincamera.ViewportToWorldPoint (new Vector2 (0, 0)).y;
		minY = maincamera.ViewportToWorldPoint (new Vector2 (0, 1)).y;
		numdot = GameDataObj.GetComponent<GameData> ().dotnumber;
		rmin = GameDataObj.GetComponent<GameData> ().randmin;
		rmax = GameDataObj.GetComponent<GameData> ().randmax;
	}
	private void tmpdotline (){
		numLine = numdot+1;
		GameDot = new GameObject[numdot+1];
		GameLineObj = new GameObject[numLine];
		GameLine = new LineRenderer[numLine];
		for (int i = 0; i < numLine; i++) {
			GameLineObj[i] = Instantiate (Lines) as GameObject;
			GameLine [i] = GameLineObj [i].GetComponent<LineRenderer> ();
			GameLine [i].SetWidth (0.1f, 0.1f);
			GameLine [i].SetVertexCount (2);
		}
		for(int i=0;i<numdot;i++) {
			Vector3 randvector = new Vector3 (Random.Range (minX, maxX), Random.Range (minY, maxY), 0);
			GameDot[i] = Instantiate (dots, randvector, new Quaternion (0, 0, 0, 0)) as GameObject;
			GameLine[i].SetPosition(0,randvector);
			GameLine [(i+numdot-1)%numdot].SetPosition (1, randvector);
		}
	}
	private void dotline(){
		randvector = new Vector3[numdot];
		GameDot = new GameObject[numdot];
		GameDotComp = new dot_infection[numdot];
		for(int i=0;i<numdot;i++) {
			randvector[i] = new Vector3 (Random.Range (minX, maxX), Random.Range (minY, maxY), 0);
			GameDot[i] = Instantiate (dots, randvector[i], new Quaternion (0, 0, 0, 0)) as GameObject;
			GameDotComp [i] = GameDot [i].GetComponent<dot_infection> ();
		}

		numLine = maxedge;
		GameLineObj = new GameObject[numLine];
		GameLine = new LineRenderer[numLine];
		for (int i = 0; i < numdot - 1; i++) {
			for (int j = i + 1; j < numdot; j++) {
				if (LineMat [i, j] > 0) {
					int tmp = LineMat [i, j] - 1;
					GameLineObj[tmp] = Instantiate (Lines) as GameObject;
					GameLine [tmp] = GameLineObj [tmp].GetComponent<LineRenderer> ();
					GameLine [tmp].SetWidth (0.1f, 0.1f);
					GameLine [tmp].SetVertexCount (2);
					GameLine[tmp].SetPosition(0,randvector[i]);
					GameLine [tmp].SetPosition (1, randvector[j]);
					Debug.Log (tmp);
				}
			}
		}
	}
	private void tmpMakemat(){
		int counter = 1;
		LineMat = new int[numdot,numdot];
		for (int i = 0; i < numdot; i++) {
			LineMat [i, (i + 1) % numdot] = counter;
			LineMat [(i + 1) % numdot, i] = counter;
			counter = counter + 1;
		}
		//LineMat [0, numdot - 1] = counter;
		//LineMat [numdot - 1, 0] = counter;
		//counter = counter + 1;
		maxedge = counter - 1;
	}
	private void Makemat (){
		int counter = 1;
		LineMat = new int[numdot, numdot]; //Adjacency matrix
		int [] baserow = new int[numdot];// for rand
		int [] numofrow = new int [numdot];// sum of row in Adjacency matrix
		int [] randrow = new int [numdot];// Max num of edge
		for (int i = 0; i < numdot; i++) {
			for (int j = 0; j < numdot; j++) {
				LineMat [i, j] = 0;
			}
			baserow [i] = 0; 
			numofrow[i] = 0; 
			randrow [i] = Random.Range (rmin, rmax + 1); 
		}
		for (int i = 0;i<numdot;i++){
			int localcounter = 0;
			for (int j = i; j < numdot; j++) {
				numofrow [j] = 0;
				for (int k = 0; k < numdot; k++) {
					if (LineMat [j, k] > 0) {
						numofrow [j] = numofrow [j] + 1;
					}
				}
				if (numofrow [j] < randrow [j]) {
					baserow [localcounter] = j;
					localcounter = localcounter + 1;
				}
			}
			if (numofrow [i] < randrow[i]) {
				int scounter = 0;
				for (int j = 0; j < randrow[i] - numofrow[i]; j++) {
					if (localcounter - scounter > 0) {
						int rr = Random.Range (0, localcounter - scounter);
						int rrr = baserow [rr];
						LineMat [i, rrr] = counter;
						baserow [rrr] = baserow [localcounter - scounter - 1];
						scounter = scounter + 1;
						counter = counter + 1;
					}
				}
			}
		}
	}
	private void IsDestroyed(){
		for (int i = 0; i < numdot; i++) {
			if (GameDot [i] == null) {
				GameDot[i] = new GameObject("dummy");
				for (int j = 0; j < numdot; j++) {
					if (LineMat [i, j] > 0) {
						Destroy(GameLine[LineMat [i, j] - 1]);
					}
					LineMat [i, j] = 0;
					LineMat [j, i] = 0;
				}
				UpdateInfection ();
				Endphase ();
			}
		}
	}
	private void UpdateInfection(){
		if (!spinflag) {
			for (int i = 0; i < numdot; i++) {
				if (GameDotComp [i].IsInfected ()) {
					for (int j = 0; j < numdot; j++) {
						if (LineMat [i, j] > 0) {
							GameDotComp [j].SetInfected ();	
							for (int k = 0; k < numdot; k++) {
								if (LineMat [j, k] > 0) {
									GameLine [LineMat [j, k] - 1].GetComponent<ChangeMaterial> ().InfectedColor ();
								}
							}
						}
					}
				}
			}
		} 
		else {
			spinflag = false;
		}
	}
	private void DetectSpin(){
		bool d = Input.GetKeyDown(KeyCode.DownArrow);
		if (d){
			spinflag = true;
		}
	}
	private void Initialization(){
		if (myinit) {
			for (int i = 0; i < 2; i++) {
				int tmprnd = Random.Range (0, numdot);
				GameDotComp [tmprnd].SetInfected ();
				for (int j = 0; j < numdot; j++) {
					if (LineMat [tmprnd, j] > 0) {
						GameLine [LineMat [tmprnd, j] - 1].GetComponent<ChangeMaterial> ().InfectedColor ();
					}
				}
			}
			myinit = false;
		}
	}
}
