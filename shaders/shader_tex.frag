#version 130

uniform vec3 objectColor;
uniform vec3 lightDir;
uniform sampler2D sampler;

varying vec3 interpNormal;
varying vec2 texCoord;

void main()
{
	vec4 textureColor = texture2D(sampler, texCoord);
	vec3 normal = normalize(interpNormal);
	gl_FragColor = vec4(textureColor.xyz, 1.0);
}
