#version 400
layout(location=0) in vec4  in_vertexpos;
layout(location=1) in vec3  in_normal;
uniform mat4 MVP;
uniform mat3 NormalTransform;
out vec3 frag_normal;
out vec4 frag_position;
//out vec4 vertexpos; 
void main() {
	vec4 temp_vertexpos = MVP*in_vertexpos;
	frag_position = temp_vertexpos;
	frag_normal = normalize(NormalTransform*in_normal);
	gl_Position = temp_vertexpos;
}
