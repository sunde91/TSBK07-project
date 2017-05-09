#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 var_Normal;

uniform sampler2D treeUnit;

void main(void)
{
	outColor = texture(treeUnit, texCoord);

}
