using UnityEngine;
using System.Collections;

public class InputData : MonoBehaviour {
	private static bool created = false;
	public int x, y;
	public bool Rotate, Tap, Click;
	public Vector3 prepoint;
	public Vector3 point;
	public Collider2D detectedcol;
	public GameObject detectedobj;
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
	
	}
	
	// Update is called once per frame
	void Update () {
		if (Input.GetMouseButtonDown (0)) {
			x = (int)Input.mousePosition.x;
			y = (int)Input.mousePosition.y;
			Click = true;
		}
		if (Input.GetKeyDown (KeyCode.UpArrow)) {
			Tap = true;
		}
		if (Input.GetKeyDown (KeyCode.DownArrow)) {
			Rotate = true;
		}
	}
	public bool ColliderClickDown(GameObject obj){
		if (Click == false || x<0 || y<0) {
			Click = false;
			return false;
		} else {
			prepoint = new Vector3 (x, y,0f);
			point = Camera.main.ScreenToWorldPoint (prepoint);
			detectedcol = Physics2D.OverlapPoint (point);
			if (detectedcol != null) {
				detectedobj = detectedcol.gameObject;
				Debug.Log(detectedobj);
				Debug.Log (obj);
				if (obj == detectedobj) {
					//Debug.Log(detectedobj);
					//Debug.Log (obj);
					Click = false;
					return true;
				} else {
					return false;
				}
			} else {
				Click = false;
				return false;
			}
		}
	}
	public bool ClickDown(){
		if (Click == true) {
			Click = false;
			return true;
		} else {
			return false;
		}
	}
	public bool IsRotate(){
		return Rotate;
	}
	public bool IsTap(){
		return Tap;
	}
	public void ChangeRotate(){
		if (Rotate) {
			Rotate = false;
		} else {
			Rotate = true;
		}
	}
	public void ChangeTap(){
		if (Tap) {
			Tap = false;
		} else {
			Tap = true;
		}
	}


}


