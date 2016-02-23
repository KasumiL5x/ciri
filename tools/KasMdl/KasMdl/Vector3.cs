namespace KasMdl
{
	public class Vector3
	{
		public float x = 0.0f;
		public float y = 0.0f;
		public float z = 0.0f;

		public Vector3()
		{
		}
		public Vector3( float xx, float yy, float zz )
		{
			this.x = xx;
			this.y = yy;
			this.z = zz;
		}
		public Vector3 minus( Vector3 rhs )
		{
			return new Vector3(this.x - rhs.x, this.y - rhs.y, this.z - rhs.z);
		}
	}
}
