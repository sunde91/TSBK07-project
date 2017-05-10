#version 150

out vec4 outColor;
in vec2 texCoord;
in vec3 varNormal;
in vec3 varLight;
in vec3 outPosition;
in vec3 outNormal;
uniform sampler2D grassUnit;
uniform sampler2D sandUnit;
uniform sampler2D forestUnit;
uniform sampler2D mountainUnit;
uniform sampler2D snowUnit;
uniform sampler2D savannhUnit;
void main(void)
{

	//outColor = texture(tex, texCoord);

    //vec3 reflection = reflect(lightSource, normal); //2 * dot(normal, lightSource) * normal - lightSource;
    float shadeAmbient = 0.5;
    float shadeDiffuse = clamp(dot(varNormal, varLight), 0, 1);
    float shade = clamp(shadeAmbient + 0.7*shadeDiffuse,0,1);
    /*
    float shadeSpec = pow(clamp(dot(v_Pos, reflection), 0, 1), 25.0);

    float shade = shadeAmbient + k_d * shadeDiffuse + k_s * shadeSpec;
    shade = lightStrength * shade;
    vec4 tex_Color0 = shade * texture(texUnit0, var_TexCoord);
    vec4 tex_Color1 = shade * texture(grassUnit, var_TexCoord);
    vec4 tex_Color = lightColor[i].b * tex_Color0 + lightColor[i].g * tex_Color1
        + lightColor[i].r * ( tex_Color0 + tex_Color1  );
    tex_Color /= 4;
    */


		float x = outPosition.x;
		float y = outPosition.y;
		float z = outPosition.z;


		float f = sin((x+z)/3.14)*10;
		vec4 texColor;


		if(outPosition.y > 5){
			if(outPosition.x >= 467 && outPosition.x <= 557 && outPosition.z >= 467 && outPosition.z <= 557){
				vec4 v;
				v.x = sqrt(pow(467-x, 2) + pow(467-z, 2));
				v.y = sqrt(pow(557-x, 2) + pow(467-z, 2));
				v.z = sqrt(pow(467-x, 2) + pow(557-z, 2));
				v.w = sqrt(pow(557-x, 2) + pow(557-z, 2));

				v.x = 1/(v.x*v.x);
				v.y = 1/(v.y*v.y);
				v.z = 1/(v.z*v.z);
				v.w = 1/(v.w*v.w);

				float sum = v.x + v.y + v.z + v.w;
				v.x /= sum;
				v.y /= sum;
				v.z /= sum;
				v.w /= sum;

				texColor = v.x * texture(grassUnit, texCoord) + v.y * texture(forestUnit, texCoord) + v.z * texture(sandUnit, texCoord) + v.w * texture(savannhUnit, texCoord);
			}
			else if(x >= 467 && x <= 557 && z < 512){
				texColor = (x-467)/90 * texture(forestUnit, texCoord) + (557-x)/90 * texture(grassUnit, texCoord);
			}
			else if(x >= 467 && x <= 557 && z > 512){
				texColor = (x-467)/90 * texture(savannhUnit, texCoord) + (557-x)/90 * texture(sandUnit, texCoord);
			}
			else if(z >= 467 && z <= 557 && x < 512){
				texColor = (z-467)/90 * texture(sandUnit, texCoord) + (557-z)/90 * texture(grassUnit, texCoord);
			}
			else if(z >= 467 && z <= 557 && x > 512){
				texColor = (z-467)/90 * texture(savannhUnit, texCoord) + (557-z)/90 * texture(forestUnit, texCoord);
			}
			else if(outPosition.x < 512 && outPosition.z > 512){
				texColor = texture(sandUnit, texCoord);
			}
			else if(outPosition.x > 512 && outPosition.z < 512){
				texColor = texture(forestUnit, texCoord);
			}
			else if(y >= 90 && y <= 110){
				texColor = (y - 90)/20*texture(mountainUnit, texCoord) + (110-y)/20*texture(grassUnit, texCoord);
			}

			else if(y >= 150 && y <= 170+f){
				texColor = (y - 150)/(170+f-150)*texture(snowUnit, texCoord) + (170+f-y)/(170+f-150)*texture(mountainUnit, texCoord);
			}

			else if(outPosition.y > 160 ){
				texColor = texture(snowUnit, texCoord);
			}
			else if(outPosition.y > 100){
				texColor = texture(mountainUnit, texCoord);
			}
			else if(x < 512 && z < 512){
				texColor = texture(grassUnit, texCoord);
			}
			else{
				texColor = texture(savannhUnit, texCoord);
			}
		}
		else if(outPosition.y >= 4 && outPosition.y <= 5){
			vec4 texColor1;
			if(x <= 512 && z <= 512){
			 	texColor1 = (outPosition.y-4)*texture(grassUnit, texCoord);
			}
			else if(x > 512 && z <= 512){
				texColor1 = (outPosition.y-4)*texture(forestUnit, texCoord);
			}
			else if(x <= 512 && z > 512){
				texColor1 = (outPosition.y-4)*texture(sandUnit, texCoord);
			}
			else{
				texColor1 = (outPosition.y-4)*texture(savannhUnit, texCoord);
			}
			vec4 texColor2 = (5-outPosition.y)*texture(sandUnit, texCoord);
			texColor = texColor1 + texColor2;
		}
		else{
			texColor = texture(sandUnit, texCoord);

		}


    outColor = shade * texColor;
}
