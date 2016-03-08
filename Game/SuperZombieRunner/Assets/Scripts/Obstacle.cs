using UnityEngine;
using System.Collections;

public class Obstacle : MonoBehaviour, IRecycle {

	public Sprite[] sprites;

	public Vector2 colliderOffset = Vector2.zero;

	public void Restart() {
		// Get the current sprite
		var renderer = GetComponent<SpriteRenderer> ();
		renderer.sprite = sprites [Random.Range (0, sprites.Length)];

		var collider = GetComponent<BoxCollider2D> ();
		var size = renderer.bounds.size;

		// Re-center the box of the collider
		size.y += colliderOffset.y;

		collider.size = size;
		collider.offset = new Vector2 (-colliderOffset.x, collider.size.y / 2 - colliderOffset.y);
	}

	public void Shutdown() {
	
	}

}
