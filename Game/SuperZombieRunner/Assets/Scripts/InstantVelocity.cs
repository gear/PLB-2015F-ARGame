using UnityEngine;
using System.Collections;

public class InstantVelocity : MonoBehaviour {

	public Vector2 velocity = Vector2.zero;

	private Rigidbody2D body2d;

	void Awake() {
		body2d = GetComponent<Rigidbody2D> ();
	}

	// Use this for initialization
	// Reserved for physic calculation, more efficient
	void FixedUpdate () {
		body2d.velocity = velocity;
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
