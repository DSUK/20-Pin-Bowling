#version 400
in vec3 in_vertexpos;
//out vec4 vertexpos;
void main() {
	gl_Position = vec4(in_vertexpos,1.0);
}
