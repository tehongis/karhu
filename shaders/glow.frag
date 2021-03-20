#version 330

varying vec3 vertPos;
varying vec3 vertNV;
varying vec3 vertCol;

uniform sampler2D texture;

uniform float u_glow;

void main()
{
    vec3 color = vertCol;

    float shininess = 10.0;
    vec3  normalV = normalize( vertNV );
    vec3  eyeV    = normalize( -vertPos );
    vec3  halfV   = normalize( eyeV + normalV );
    float NdotH   = max( 0.0, dot( normalV, halfV ) );
    float glowFac = ( shininess + 2.0 ) * pow( NdotH, shininess ) / ( 2.0 * 3.14159265 );

    gl_FragColor = vec4( u_glow * (0.1 + color.rgb * glowFac * 0.5), 1.0 );
}