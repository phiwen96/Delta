#version 450

layout (location = 0) in vec3 fragColor;
layout (location = 1) in vec2 fragTexCoord;

layout (location = 0) out vec4 outColor;
layout (binding = 0) uniform sampler2D texSampler;
layout(set = 0, binding = 1) uniform texture2D textures[2];
layout(push_constant) uniform PER_OBJECT
{
	int imgIdx;
}pc;


void main ()
{
	//outColor = vec4 (fragColor, 1.0);
	// outColor = texture (texSampler, fragTexCoord);
	outColor = texture (sampler2D(textures[pc.imgIdx], texSampler), fragTexCoord);
}