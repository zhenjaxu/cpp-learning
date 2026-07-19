// GLSL 3.3
#version 330

uniform mat4 uWorldTransform;   // 世界变换矩阵
uniform mat4 uViewProj;         // 裁剪变换矩阵

// 顶点属性
in vec3 inPosition;     // 顶点位置xyz

void main(){
    vec4 pos=vec4(inPosition, 1.0);     // xyzw，w设置为1.0
    gl_Position=pos*uWorldTransform*uViewProj;      // 变化矩阵，模型空间->世界空间->视图空间->裁剪空间
}