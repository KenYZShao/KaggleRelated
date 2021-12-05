#version 330 core
in vec2 TexCoords;

out vec4 color;

//Texture samplers
uniform samplerCube skybox;

void main()
{   
    //Linearly interpolate between both textures(second texture is only slightly combined) 
    color = texture(skybox, TexCoords);
}