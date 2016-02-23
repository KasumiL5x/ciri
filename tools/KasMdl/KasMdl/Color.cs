namespace KasMdl
{
	public class Color
	{
		public float r = 1.0f;
		public float g = 1.0f;
		public float b = 1.0f;
		public float a = 1.0f;

		public Color()
		{
		}
		public Color( float rr, float gg, float bb, float aa=1.0f )
		{
			this.r = rr;
			this.g = gg;
			this.b = bb;
			this.a = aa;
		}
	}
}
