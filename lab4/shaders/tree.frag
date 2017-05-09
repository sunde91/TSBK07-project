#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 var_Normal;

uniform sampler2D treeUnit;

void main(void)
{
	
	vec4 texel = texture(treeUnit, texCoord);
	if(texel.a < 0.5)
    	discard;
	outColor = texel;


}
