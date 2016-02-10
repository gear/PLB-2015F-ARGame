using UnityEngine;
using System.Collections;

public class ChangeMaterial : MonoBehaviour {
	[SerializeField]
	private Material mate;
	private Renderer render;
	private int length;
	private Material[] materials;
	// Use this for initialization
	void Start () {
		render = GetComponentInChildren<Renderer>();
		length = render.materials.Length;
		materials = render.materials;
	}
	
	// Update is called once per frame
	void Update () {
	}
	public void InfectedColor (){
		System.Array.Resize<Material>( ref materials, length + 1);
		materials[ length ] = mate;
		render.materials = materials;
	}
}
