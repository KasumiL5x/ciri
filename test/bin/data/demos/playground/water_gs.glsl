#version 440

layout(triangles) in;
layout (triangle_strip, max_vertices=3) out;

in vec3 vo_nrm[];
in vec2 vo_texcoord[];
in mat4 vo_world[];
in mat3 vo_invworld[];
in vec3 vo_campos[];
in mat4 vo_xform[];

out vec3 go_normal;
out vec2 go_texcoord;
out vec3 go_diffuse;
out vec3 go_specular;

void main() {

	vec3 v0 = gl_in[0].gl_Position.xyz;
	vec3 v1 = gl_in[1].gl_Position.xyz;
	vec3 v2 = gl_in[2].gl_Position.xyz;

	vec3 centerPos = (v0 + v1 + v2) / 3.0;

	vec3 vn = normalize(cross(v1 - v0, v2 - v0));

	vec3 normalDirection = normalize(vo_invworld[0] * vec4(vn, 0.0).xyz);
	vec3 viewDirection = normalize(vo_campos[0] - (vo_world[0] * vec4(centerPos, 0.0)).xyz);
	vec3 lightDirection = normalize(vec3(100.0, 300.0, 42.0));
	float attenuation = 1.0;

	vec3 Color = vec3(0.56, 0.85, 0.83);//float3(0.13, 0.7, 0.66);//float3(0.0, 0.412, 0.58);
	vec3 LightColor = vec3(1.0, 1.0, 1.0);
	vec3 SpecColor = vec3(1.0, 1.0, 1.0);
	float Shininess = 64.0;
	vec3 ambientLighting = vec3(0.1, 0.1, 0.1) * Color;

	vec3 diffuseReflection = attenuation * LightColor * Color * max(0.0, dot(normalDirection, lightDirection));

	vec3 specularReflection;
	if( dot(normalDirection, lightDirection) < 0.0 ) {
		specularReflection = vec3(0.0, 0.0, 0.0);
	} else {
		specularReflection = attenuation * LightColor * SpecColor * pow(max(0.0, dot(reflect(-lightDirection, normalDirection), viewDirection)), Shininess);
	}

	gl_Position = vo_xform[0] * gl_in[0].gl_Position;
	go_normal = vn;
	go_texcoord = vo_texcoord[0];
	go_diffuse = ambientLighting + diffuseReflection;
	go_specular = specularReflection;
	EmitVertex();

	gl_Position = vo_xform[0] * gl_in[1].gl_Position;
	go_normal = vn;
	go_texcoord = vo_texcoord[1];
	go_diffuse = ambientLighting + diffuseReflection;
	go_specular = specularReflection;
	EmitVertex();

	gl_Position = vo_xform[0] * gl_in[2].gl_Position;
	go_normal = vn;
	go_texcoord = vo_texcoord[2];
	go_diffuse = ambientLighting + diffuseReflection;
	go_specular = specularReflection;
	EmitVertex();



	// for( int i = 0; i < gl_in.length(); ++i ) {
	// 	gl_Position = gl_in[i].gl_Position;
	// 	EmitVertex();
	// }
}
