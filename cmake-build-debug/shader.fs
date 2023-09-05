#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
//     FragColor = vec4(lightColor * texture(texture1, TexCoord).xyz * objectColor, 1.0);
    FragColor = vec4(lightColor * objectColor, 1.0);
}