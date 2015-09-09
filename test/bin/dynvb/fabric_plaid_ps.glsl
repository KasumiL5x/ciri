#version 420

in vec3 vo_position;
in vec3 vo_normal;
in vec2 vo_texcoord;
in vec3 vo_camPos;

out vec4 out_color;

vec3 LightDirection = vec3(-0.5265408f, -0.5735765f,-0.6275069f);

// cloth variables
float NoiseScale = 0.07;
float Sheen = 0.296; //0.74 * 0.4;
float Furriness = 20.0;
vec3 Color = vec3(1.0, 0.08, 0.5);

void doPattern( vec3 pos, float noiseScale, float noisy, inout float patt, inout float freq, inout float scale ) {
	vec3 fp = fract(pos * freq);
	fp *= (1.0 - fp);
	patt += dot(fp, vec3(1.0, 1.0, 1.0)) * scale;
	freq *= 2.0;
	scale *= 0.7;
}

void main() {
	vec3 L = -LightDirection;
	vec3 N = normalize(vo_normal);

	// pattern shit
	vec3 pos = vo_position * 2.5;
	float noisy = float(2.0 * noise1(pos) - 1.0);
	float patt = 0.0;
	float freq = 1.47;
	float scale = 0.5;
	for( int i = 0; i < 7; ++i ) { // let's hope your implementation likes unrolling loops!
		doPattern(pos, NoiseScale, noisy, patt, freq, scale);
	}

	// fabric style lighting
	float diffuse = 0.25 * (2.0 + dot(N, L));
	vec3 viewVec = normalize(vo_camPos - pos);
	float cosView = clamp(dot(viewVec, N), 0.0, 1.0);
	float shine = pow(1.0 - cosView * cosView, Furriness);

	// final
	out_color = vec4((patt * Color + Sheen * shine) * diffuse, 1.0);
}
