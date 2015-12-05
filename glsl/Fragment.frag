#version 330 core
out vec4 colour;
in mat3 mat_colour;
in mat3 lightColour;
in vec3 lightPos;
in vec3 lightPos2;
in vec4 vertexpos;
in vec4 normal;
float calcdiffuseco(vec3 local_lightPos,vec3 norm)
{
    float diffco;
    vec3 vert_to_light  = normalize(local_lightPos - vertexpos.xyz);

    diffco = -dot(norm,vert_to_light);
    /*if(diffco  < 0.1)
    {
        return 0.0;
    }*/
    return diffco;
}
float calcspecualrco(vec3 local_lightPos,vec3 norm)
{
    float specularco;
    vec3 vert_to_light  = normalize(local_lightPos - vertexpos.xyz);
    vec3 vert_to_eye = normalize(vertexpos.xyz);
    vec3 reflectcion = normalize(reflect(vert_to_light,norm));
    specularco = pow(dot(vert_to_eye,reflectcion),128);
    if(specularco < 0.01)
    {
        specularco = 0;
    }
    return specularco;
}
void main()
{
    //ambient light
    vec3 testambient = mat_colour[0]*lightColour[0];
    vec3 ambient = vec3(1.0,0.0,0.0);
    vec3 norm = normalize(normal.xyz);
    vec3 diffuse = (calcdiffuseco(lightPos,norm) + calcdiffuseco(lightPos2,norm))*mat_colour[1]*lightColour[1];
    vec3 specular = (calcspecualrco(lightPos,norm)+calcspecualrco(lightPos2,norm))*mat_colour[2]*lightColour[2];
    vec3 testcolour = normalize(testambient);
    vec3 fragcolour = ambient+diffuse + specular;//diffuse + specular;

    //fragcolour.r = 1.0;


    //fragcolour.g = 1.0;

    //fragcolour.b = 1.0;

    //fragcolour = normalize(fragcolour);
    //colour = vec4(fragcolour,1.0);
    colour = vec4(testcolour,1.0);
}

