#version 450

layout (push_constant) uniform constants
{
	mat4 model;
	mat4 view;
	mat4 proj;
} pushConstants;	

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec2 asTexture;

layout (location = 0) out vec3 fragColor;
layout (location = 1) out vec2 fragTexCoord;
layout (location = 2) out vec2 fragAsTexture;
/*
vec2 positions [3] = vec2 [] (
	vec2 (0.0, -0.5),
	vec2 (0.5, 0.5),
	vec2 (-0.5, 0.5)
);

vec3 colors [3] = vec3 [] (
	vec3 (1.0, 0.0, 0.0),
	vec3 (0.0, 1.0, 0.0),
	vec3 (0.0, 0.0, 1.0)
);
*/
out gl_PerVertex 
{
	vec4 gl_Position;   
};
void main () {
	//gl_Position = vec4 (positions [gl_VertexIndex], 0.0, 1.0);
	//fragColor = colors [gl_VertexIndex];
	//gl_Position = vec4(inPosition, 1.0);
	gl_Position = pushConstants.proj * pushConstants.view * pushConstants.model * vec4 (inPosition, 1.0);
	fragColor = inColor;
	fragTexCoord = inTexCoord;// + vec2 (3, 10);
	fragAsTexture = asTexture;
}