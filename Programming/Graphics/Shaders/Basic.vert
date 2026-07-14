// GLSL 3.3
#version 330

// 顶点属性
in vec3 inPosition;     // 顶点位置xyz

void main(){
    gl_Position=vec4(inPosition, 1.0);      // 内置变量，xyzw，w设置为1.0
}