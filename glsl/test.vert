#version 400
in vec4 in_vertexpos;
uniform mat4 MVP;
//out vec4 vertexpos;
void main() {
	gl_Position = MVP*in_vertexpos;
	//gl_Position = in_vertexpos;
}
