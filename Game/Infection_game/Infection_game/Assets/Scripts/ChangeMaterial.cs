using UnityEngine;
using System.Collections;

public class ChangeMaterial : MonoBehaviour {
	[SerializeField]
	private Material mate;
	// Use this for initialization
	void Start () {
	}
	
	// Update is called once per frame
	void Update () {
	}
	public void InfectedColor (){
		var render = GetComponentInChildren<Renderer>();
		int length = render.materials.Length;
		var materials = render.materials;
		System.Array.Resize<Material>( ref materials, length + 1);
		materials[ length ] = mate;
		render.materials = materials;
	}
}
