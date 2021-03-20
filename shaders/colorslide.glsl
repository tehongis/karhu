#ifdef GL_ES
precision mediump float;
#endif  

uniform vec3      iResolution;           // viewport resolution (in pixels)
uniform float     iTime;                 // shader playback time (in seconds)
uniform sampler2D texture;

float map(vec2 p) {
    return length(p) - 0.2;
}

void main( out vec4 fragColor, in vec2 fragCoord )
{
  vec2 uv = (fragCoord.xy * 2.0 - iResolution.xy) / min(iResolution.x, iResolution.y);
  vec3 col;
  vec3 color = vec3(1.,1.,1.);

  for(float j = 0.0; j < 4.0; j++){
      for(float i = 1.; i < 8.0; i++){
          uv.x += (1.0 * (0.2 / (i + j) * sin(i * atan(iTime) * 2.0 * uv.y + (iTime * 0.1) + i * j)));
          uv.y+= (1.0 * (1.0 / (i + j) * cos(i * 0.6 * uv.x + (iTime * 0.25) + i * j)));
      }
      col[int(j)] = -1.0 * (uv.x * uv.y);
  }
  
  vec3 bg = vec3(1.,1.,1.);
  color = mix(
    col,
    bg,
    1.0-smoothstep(0.0,abs(sin(iTime*0.05)*3.0),map(uv))
  );   

  fragColor = vec4(vec3(color), 1.);
}