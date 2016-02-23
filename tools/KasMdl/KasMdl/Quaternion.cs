namespace KasMdl
{
	public class Quaternion
	{
		public float x = 0.0f;
		public float y = 0.0f;
		public float z = 0.0f;
		public float w = 1.0f;

		public Quaternion()
		{
		}
		public Quaternion( float xx, float yy, float zz, float ww=1.0f )
		{
			this.x = xx;
			this.y = yy;
			this.z = zz;
			this.w = ww;
		}
	}
}
