struct Input {
	float4 Pos : SV_POSITION;
	float3 position : TEXCOORD1;
	float3 normal : NORMAL0;
	float2 Tex : TEXCOORD0;
	float3 CamPos : TEXCOORD2;
};

static float3 LightDirection = float3(-0.5265408, -0.5735765,-0.6275069);

// cloth variables
static float NoiseScale = 0.07;
static float Sheen = 0.296; //0.74 * 0.4;
static float Furriness = 20.0;
static float3 Color = float3(1.0, 0.08, 0.5);

void doPattern( float3 pos, float noiseScale, float noisy, inout float patt, inout float freq, inout float scale ) {
	float3 fp = frac(pos * freq);
	fp *= (1.0 - fp);
	patt += dot(fp, float3(1.0, 1.0, 1.0)) * scale;
	freq *= 2.0;
	scale *= 0.7;
}

float4 main( Input input ) : SV_Target {
	float3 L = -LightDirection;
	float3 N = normalize(input.normal);

	// pattern shit
	float3 pos = input.position * 2.5;
	float noisy = float(2.0 * noise(pos) - 1.0);
	float patt = 0.0;
	float freq = 1.47;
	float scale = 0.5;
	for( int i = 0; i < 7; ++i ) { // let's hope your implementation likes unrolling loops!
		doPattern(pos, NoiseScale, noisy, patt, freq, scale);
	}

	// fabric style lighting
	float diffuse = 0.25 * (2.0 + dot(N, L));
	float3 viewVec = normalize(input.CamPos - pos);
	float cosView = clamp(dot(viewVec, N), 0.0, 1.0);
	float shine = pow(1.0 - cosView * cosView, Furriness);

	return float4((patt * Color + Sheen * shine) * diffuse, 1.0);
}
