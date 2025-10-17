#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform float uTime;
uniform vec2 uResolution;

const float pixelSize = 16.0;
const float paletteColors = 16.0;
const float ditherStrength = 0.10;

float bayer4x4(vec2 coord)
{
  const float bayerMatrix[16] = float[16]
    (
     0.0 , 8.0 , 2.0 , 10.0,
     12.0, 4.0 , 14.0, 6.0 ,
     3.0 , 11.0, 1.0 , 9.0 ,
     15.0, 7.0 , 13.0, 5.0
     );

  int x = int(mod(coord.x, 4.0));
  int y = int(mod(coord.y, 4.0));
  return bayerMatrix[y * 4 + x] / 16.0;
}

vec3 quantizeColor(vec3 color, float levels)
{
  return floor(color * levels) / levels;
}

void main()
{
  vec2 pixelCoord = floor(TexCoord * uResolution / pixelSize) * pixelSize;
  vec2 pixelUV = pixelCoord / uResolution;

  vec3 color = texture(uTexture, pixelUV).rgb;

  float dither = bayer4x4(gl_FragCoord.xy);
  color += (dither - 0.5) * ditherStrength;

  color = quantizeColor(color, paletteColors);

  vec2 pixelPos = mod(TexCoord * uResolution, pixelSize);
  float border = step(pixelSize - 1.0, pixelPos.x) + step(pixelSize - 1.0, pixelPos.y);

  color *= 1.0 - border * 0.1;

  float scanline = sin(TexCoord.y * uResolution.y * 2.0) * 0.05 + 0.95;
  color *= scanline;

  FragColor = vec4(color, 1.0);
  
}
