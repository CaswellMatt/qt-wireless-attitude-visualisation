#version 330
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 vertexColor;

uniform mat4 u_MVP;
out vec4 fragmentColor;

void main()
{
  gl_Position = u_MVP * position;
  fragmentColor = vec4(vertexColor, 1.0f);
}
