#version 100
precision mediump float;

varying vec2 TexCoord;

uniform sampler2D tex;
uniform vec3 color;
uniform vec3 colorChange;

void main()
{
    float alpha = texture2D(tex, TexCoord).a;
    gl_FragColor = vec4(color + colorChange, alpha);
}
