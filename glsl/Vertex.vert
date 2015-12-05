#version 330 core
in vec3 vert_vertexpos;
uniform mat4 Perspective;
uniform mat4 Viewpoint;
uniform mat4 Model;
uniform mat3 vert_colour;
uniform vec3 vert_lightPos;
uniform vec3 vert_lightPos2;
uniform mat3 vert_lightColour;
uniform vec3 vert_normal;
out mat3 mat_colour;
out mat3 lightColour;
out vec3 lightPos;
out vec3 lightPos2;
out vec4 vertexpos;
out vec4 normal;
void main()
{
    mat4 MVP = Perspective*Viewpoint*Model;
    vec4 normaltrans = vec4(vert_normal,0.0);
    mat_colour = vert_colour;
    lightColour = vert_lightColour;
    lightPos = (Viewpoint*vec4(vert_lightPos,1.0)).xyz;
    lightPos2 = (Viewpoint*vec4(vert_lightPos2,1.0)).xyz;
    vec4 posi = vec4(vert_vertexpos,1.0);
    vertexpos = Viewpoint*Model*posi;
    normal = transpose(inverse(MVP))*normaltrans;
    gl_Position = vec4(Perspective*vertexpos);
    //gl_Position = vec4(vertexpos.yxz,1.0);
}
