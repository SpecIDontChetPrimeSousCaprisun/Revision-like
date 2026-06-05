#version 100
precision mediump float;

varying vec2 TexCoord;

uniform float alpha;
uniform sampler2D tex;
uniform vec3 colorChange;
uniform vec3 color;
uniform bool useColor;

void main() {
  vec4 finalColor;

  if (useColor) {
    finalColor = vec4(color, 1);
  } else {
    finalColor = texture2D(tex, TexCoord);
  }

  gl_FragColor = vec4(
      finalColor.rgb + colorChange,
      finalColor.a * alpha
  );
}
