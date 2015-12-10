in vec3 vert_vertexpos;
out vec4 vertexpos;
main() {
	vertexpos = vec4(vert_vertexpos,1.0);
}
