using UnityEngine;
using System.Collections;

public class TiledBackground : MonoBehaviour {

	public int textureSize = 32;
	public bool scaleHorizontaially = true;
	public bool scaleVertically = true;

	// Use this for initialization
	void Start () {
	
		var newWidth = !scaleHorizontaially ? 1 : Mathf.Ceil (Screen.width / (textureSize * PixelPerfectCamera.scale));
		var newHeight = !scaleVertically ? 1 : Mathf.Ceil (Screen.height / (textureSize * PixelPerfectCamera.scale));

		transform.localScale = new Vector3 (newWidth * textureSize, newHeight * textureSize, 1);

		GetComponent<Renderer> ().material.mainTextureScale = new Vector3 (newWidth, newHeight, 1);

	}
}
