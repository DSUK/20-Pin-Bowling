#version 400
//out vec4 colour;
in vec3 frag_normal;
in vec4 frag_position;
void main() {
	//float mag = clamp(abs(dot(frag_normal,frag_position.xyz)),0.0,1.0);
	float mag = -dot(frag_normal,normalize(frag_position.xyz));
	gl_FragColor = vec4(mag,0.0,0.0,1.0);
}
