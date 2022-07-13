#version 450
layout (push_constant) uniform constants
{
	mat4 model;
	mat4 view;
	mat4 proj;
} pushConstants;	
layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec3 inColor;
layout (location = 0) out vec3 fragColor;


void main ()
{
	gl_Position = pushConstants.proj * pushConstants.view * pushConstants.model * vec4 (inPosition, 0.0, 1.0);
	fragColor = inColor;
}