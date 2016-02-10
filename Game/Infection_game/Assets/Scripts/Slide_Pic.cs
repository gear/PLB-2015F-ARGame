using UnityEngine;
using UnityEngine.UI;
using System.Collections;

public class Slide_Pic : MonoBehaviour {
	public AnimationCurve animCurve = AnimationCurve.Linear(0, 0, 1, 1);
	[SerializeField]
	private Vector3 inPosition;        // スライドイン後の位置
	[SerializeField]
	private Vector3 outPosition;      // スライドアウト後の位置
	[SerializeField]
	private float duration = 1.0f;    // スライド時間（秒）

	// スライドイン
	public void SlideIn(){
		StartCoroutine( StartSlidePanel(true) );
	}

	// スライドアウト
	public void SlideOut(){
		StartCoroutine( StartSlidePanel(false) );
	}

	private IEnumerator StartSlidePanel( bool isSlideIn ){
		float startTime = Time.time;    // 開始時間
		Vector3 startPos = transform.localPosition;  // 開始位置
		Vector3 moveDistance;            // 移動距離および方向

		if (isSlideIn)
			moveDistance = (inPosition - startPos);
		else {
			moveDistance = (outPosition - startPos);
		}
			while((Time.time - startTime) < duration){
				transform.localPosition = startPos + moveDistance * animCurve.Evaluate((Time.time - startTime) / duration);
				yield return 0;        // 1フレーム後、再開
			}
			transform.localPosition = startPos + moveDistance;
		}
	}