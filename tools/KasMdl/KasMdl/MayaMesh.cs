using Autodesk.Maya.OpenMaya;
using System.Collections.Generic;
namespace KasMdl
{
	public class MayaMesh
	{
		public string name = "";
		public MDagPath mayaObjectPath = new MDagPath();
		public MayaXform sourceXForm = null;
		public List<Vertex> vertices = new List<Vertex>();
		public List<uint> indices = new List<uint>();
		public int id = -1;
		public bool isInstanced = false;
	}
}
