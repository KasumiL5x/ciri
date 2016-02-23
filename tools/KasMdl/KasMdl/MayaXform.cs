using System.Collections.Generic;
using Autodesk.Maya.OpenMaya;

namespace KasMdl
{
	public class MayaXform
	{
		public string name = "";
		public MDagPath mayaObjectPath = new MDagPath();
		public MayaXform parent = null;
		public List<MayaXform> children = new List<MayaXform>();
		public float[,] matrix = new float[4,4]{ {1.0f,0.0f,0.0f,0.0f},{0.0f,1.0f,0.0f,0.0f},{0.0f,0.0f,1.0f,0.0f}, {0.0f,0.0f,0.0f,1.0f} };
		public bool isRoot = false;
		public MayaMesh mesh = null;
		public bool isSourceXform = false;
		public int id = -1;
	}
}
