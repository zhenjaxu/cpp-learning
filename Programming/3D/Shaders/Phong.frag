#version 330

in vec2 fragTexCoord;
in vec3 fragWorldPos;
in vec3 fragNormal;

out vec4 outColor;

uniform sampler2D uTexture;

struct uDirectionLight
{
    vec3 mDirection;
    vec3 mDiffuseColor;
    vec3 mSpecColor;
};

uniform vec3 uCameraPos;
uniform vec3 uAmbientLight;
uniform float uSpecPower;
uniform uDirectionLight uDirLight;

void main()
{
    vec3 N = normalize(fragNormal);
    vec3 L = normalize(-uDirLight.mDirection);
    vec3 V = normalize(uCameraPos - fragWorldPos);
    vec3 R = normalize(reflect(-L, N));

    vec3 Phong = uAmbientLight;
    float NdotL = dot(N, L);    // 点积
    if(NdotL > 0)
    {
        vec3 Diffuse = uDirLight.mDiffuseColor * NdotL;
        vec3 Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);
        Phong += Diffuse + Specular;
    }

    outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0);
}