using Autodesk.Maya.OpenMaya;

[assembly: MPxCommandClass(typeof(KasMdl.MyCommand), "KasMdl")]

namespace KasMdl
{
	public class MyCommand : MPxCommand, IMPxCommand
	{
		public override void doIt(MArgList argl)
		{
			MainFrm frm = new MainFrm();
			frm.Show();
		}
	}

}
