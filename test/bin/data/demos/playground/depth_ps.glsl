#version 440

out vec4 out_color;

void main() {
	// float NEAR = 0.1;
	// float FAR = 160.0;
	// float z = gl_FragCoord.z * 2.0 - 1.0;
	// float depth = (2.0 * NEAR * FAR) / (FAR + NEAR - z * (FAR - NEAR));
	// out_color = vec4(vec3(depth) / FAR, 1.0f);
	out_color = vec4(gl_FragCoord.z);
}
