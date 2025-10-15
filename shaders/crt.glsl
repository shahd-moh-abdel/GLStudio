#version 330 core

in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform vec2 uResolution;
uniform float uTime;

// I'll add imgui to control these parameters later
const float scanlineIntensity = 0.3;
const float scanlineCount = 800.0;
const float vignetteIntensity = 0.6;
const float vignetteExtent = 0.4;
const float distortion = 0.2;
const float brightness = 1.1;

const float flickerAmount = 0.03;

vec2 curveRemapUV(vec2 uv)
{
  uv = uv * 2.0 - 1.0;

  float r2 = uv.x * uv.x + uv.y * uv.y;
  uv *= 1.0 + distortion * r2;
  
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

float flicker(float time)
{
  return 1.0 - flickerAmount + flickerAmount * sin(time * 10.0);
}

void  main()
{
  vec2 uv = curveRemapUV(TexCoord);

  //if uv goes out of bounds return black
  if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
    {
      FragColor = vec4(0.0, 0.0, 0.0, 1.0);
      return;
    }
  
  vec3 color = texture(uTexture, uv).rgb;

  //apply effects
  color *= scanline(uv);
  color *= vignette(uv);
  color *= brightness;
  color *= flicker(uTime);
  
  //slight color shift
  color.r *= 0.8;
  color.b *= 1.2;

  FragColor = vec4(color, 1.0);
}
