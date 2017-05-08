#version 150

in vec3 in_Position;
in vec2 in_TexCoord;
in vec3 in_Normal;

out vec2 texCoord;

uniform mat4 modelMatrix;
uniform mat4 cameraMatrix;
uniform mat4 projMatrix;

void main(void)
{
	mat4 affineMatrix = cameraMatrix * modelMatrix;

	gl_Position = projMatrix * affineMatrix * vec4(in_Position, 1.0);
	texCoord = in_TexCoord;
}
