using Autodesk.Maya.OpenMaya;
namespace KasMdl
{
	public class MayaLight
	{
		public enum Type
		{
			kInvalid = 0,
			kAmbient = 1,
			kDirectional = 2,
			kPoint = 3,
			kSpot = 4
		}

		public string name = "";
		public MDagPath mayaObjectPath = new MDagPath();
		public MayaXform sourceXform = null;
		public int id = -1;
		public Type type = Type.kInvalid;

		public Color color = new Color(1.0f, 1.0f, 1.0f, 1.0f);
		public float intensity = 1.0f;
		public float range = 10.0f; // radius for pointlight, height for spotlight
		public float coneInnerAngle = 10.0f;
		public float coneOuterAngle = 12.0f;
	}
}
