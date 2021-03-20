#version 110
#extension GL_ARB_texture_rectangle : enable

uniform sampler2D texture;

void main()
{
    // lookup the pixel in the texture
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    // multiply it by the color
    gl_FragColor = gl_Color * pixel * vec4( 1.0, 0, 0, 1 );
}