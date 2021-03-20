#version 110
#extension GL_ARB_texture_rectangle : enable

uniform sampler2D texture;
uniform float transparency;

void main()
{
    vec4 color = texture2D(texture, gl_TexCoord[0].st);
    color.w *= transparency;
    gl_FragColor = color;
}
