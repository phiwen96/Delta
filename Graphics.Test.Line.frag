#version 450

layout (push_constant) uniform constants
{
	int imageIdx;
} pushConstants;	

layout (location = 0) in vec3 fragColor;


layout (location = 0) out vec4 outColor;
// layout (set = 0, binding = 0) uniform sampler samp;
// layout (set = 0, binding = 1) uniform texture2D textures ;//[2];

// layout (push_constant) uniform PER_OBJECT 
// { 
// 	int imgIdx;
// }pc;

void main () {
	//outColor = vec4 (fragColor, 1.0);

	outColor = vec4 (fragColor, 1.0);
	
	
	// outColor = texture (sampler2D(textures,  samp), fragTexCoord);
	// outColor = texture (sampler2D(textures [pc.imgIdx], samp), fragTexCoord);
}