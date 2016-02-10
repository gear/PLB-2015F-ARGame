using UnityEngine;
using System.Collections;
using System.IO.Ports;

public class SerialControlled : MonoBehaviour {

	public float speed;

	private float amountToMove = 0;

	// Change serial port name to read from port
	// Windows: "COMX" , Mac/Linux: "/dev/usb.xxxxxx"
	SerialPort sp = new SerialPort("COM8", 57600);

	void Start () {
		// Start serial communication
		sp.Open ();
		sp.ReadTimeout = 100;
	}

	void Update () {
		amountToMove = speed * Time.deltaTime;

		if (sp.IsOpen) {
			try {
				string data = sp.ReadLine();
				switch(data) {
				case "DTAP":
					MoveObject();
					break;
				case "COLLIDED":
					MoveObject();
					MoveObject();
					MoveObject();
					break;
				case "TAP":
					MoveObject();
					MoveObject();
					break;
				default:
					break;
				}

			} catch { }
		}
	}

	void MoveObject() {
		transform.Translate (Vector2.right * amountToMove, Space.World);
	}
}
