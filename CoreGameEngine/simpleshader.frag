#version 330

out vec4 FragColor;

uniform float gScale;

void main()
{
    FragColor = vec4(abs(gScale*gScale), abs(gScale), 0.0, 1.0);
}
