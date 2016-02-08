using UnityEngine;
using System.Collections;
using System.IO.Ports;

public class SerialReceiver : MonoBehaviour {

	public float speed;

	private float amountToMove = 0;
	private const float g_threshold = 1.6;
	private const float time_collision_threshold = 

		SerialPort sp = new SerialPort("COM5", 57600);

	// Buffer for acceleration and rotation rate (delta w of quaternion)
	private float[] g_buffer = new float[10];
	private float[] w_buffer = new float[10];

	private float avg_g;	// current average g
	private float gra_w;	// current spin rate

	private int i;

	void Start () {
		sp.Open ();
		sp.ReadTimeout = 100;

		i = 0;
	}

	void Update () {
		amountToMove = speed * Time.deltaTime;

		if (sp.IsOpen) {
			try {
				string data = sp.ReadLine();
				char[] seperator = new char[] {','};
				string[] tuple = data.Split(seperator, 2);
				float g_rms = float.Parse(tuple[0]);
				float w = float.Parse(tuple[1]);
				if (i >= 10) {
					i = 0;
				}
				g_buffer[i] = g_rms;
				++i;
				// Some force asserts on ball
				if (g_rms > 1.6) {
					Time.t
				}


			} catch { }
		}
	}

	void MoveObject() {
		transform.Translate (Vector2.right * amountToMove, Space.World);
	}
}
