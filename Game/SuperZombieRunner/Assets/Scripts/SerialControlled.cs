using UnityEngine;
using System.Collections;
using System.IO.Ports;

public class SerialReceiver : MonoBehaviour {




	SerialPort sp = new SerialPort("COM5", 57600);

	void Start () {
		// Start serial communication
		sp.Open ();
		sp.ReadTimeout = 100;

	}

	void Update () {
		
	}


}
