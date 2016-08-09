#version 130

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat3 normal_matrix;

in  vec3 in_Position;
in  vec3 in_Normal;
in  vec3 in_Color;

out vec3 ex_Normal;
out vec3 ex_View;
out vec3 ex_Light;
flat out vec3 ex_Color;
out vec4 res_Color;

vec3 light_pos = vec3(0.0f, 0.0f, 0.0f);

void main() {
	vec4 pos = view_matrix * model_matrix * vec4(in_Position, 1.0);
	gl_Position = projection_matrix * pos;
	vec3 n = (normal_matrix*in_Normal);
	vec3 v = (-pos.rgb);
	vec3 l = (light_pos - pos.rgb);
	vec3 h = (v+l);
	ex_Normal = n;
	ex_View = v;
	ex_Light = l;
	float diff = max(0.0f, dot(l, n));
	float spec = pow(max(0.0f, dot(h, n)), 128.0f);
	//ex_Color = diff*in_Color + vec3(spec);
	ex_Color = in_Color;
	}
