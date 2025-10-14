#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
const vec2 uResolution = vec2(600.0, 800.0);

// I'll add imgui to control these parameters later
const float scanlineIntensity = 0.3;
const float scanlineCount = 800.0;
const float vignetteIntensity = 0.6;
const float vignetteExtent = 0.4;
const float distortion = 1.0;
const float brightness = 1.1;

vec2 curveRemapUV(vec2 uv)
{
  uv = uv * 2.0 - 1.0;
  vec2 offset = abs(uv.yx) / vec2(distortion, distortion);
  uv = uv + uv * offset * offset;
  uv = uv * 0.5 + 0.5;
  return uv;
}

float vignette(vec2 uv)
{
  uv *= 1.0 - uv.yx;
  float vig = uv.x * uv.y * 15.0;
  vig = pow(vig, vignetteExtent);
  return vig;
}

float scanline(vec2 uv)
{
  return sin(uv.y * scanlineCount) * scanlineIntensity + (1.0 - scanlineIntensity);
}

void  main()
{
  vec2 uv = curveRemapUV(TexCoord);

  //if uv goes out of bounds return black

  vec3 color = texture(uTexture, uv).rgb;

  //apply effects
  color *= scanline(uv);
  color *= vignette(uv);
  color *= brightness;

  //slight color shift
  color.r *= 1.02;
  color.b *= 0.98;

  FragColor = vec4(color, 1.0);
}
