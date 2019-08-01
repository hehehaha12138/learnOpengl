#version 330 core
out vec4 FragColor;
layout(depth_greater) out float gl_FragDepth;


uniform sampler2D frontTexture;
uniform sampler2D backTexture;

void main()
{
	FragColor = vec4(1.0);
	gl_FragDepth = gl_FragCoord.z + 0.1;
		
}