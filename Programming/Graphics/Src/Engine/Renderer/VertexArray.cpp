#include"VertexArray.h"
#include<GL/glew.h>

VertexArray::VertexArray(const float* verts, unsigned int numVerts,
    const unsigned int* indices, unsigned int numIndices)
{
    // 创建顶点数组对象，存储id
    glGenVertexArrays(1, &mVertexArray);    
    glBindVertexArray(mVertexArray);        // 激活为当前 VAO ID

    // 创建顶点缓冲区
    glGenBuffers(1, &mVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVerts * 5 * sizeof(float), verts, GL_STATIC_DRAW);

    // 创建索引缓冲区，与复制数据
    glGenBuffers(1, &mIndexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIndexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    // 指定顶点布局，启动顶点属性
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(
        0,      // 属性id
        3,      // 分量数量，xyz
        GL_FLOAT,   // 分量类型
        GL_FALSE,   // 只与整型相关，这里为false
        sizeof(float) * 5,    // 步幅，连续顶点属性之间的字节偏移量
        0   // 距离首地址的偏移量，第一个属性为0
    );

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(
        1,
        2,          // UV坐标
        GL_FLOAT,
        GL_FALSE,
        sizeof(float) * 5,
        reinterpret_cast<void*>(sizeof(float) * 3)     // 强制转换成 void* 指针
    );
}

VertexArray::~VertexArray(){
    glDeleteBuffers(1, &mVertexBuffer);
    glDeleteBuffers(1, &mIndexBuffer);
    glDeleteVertexArrays(1, &mVertexArray);
}

void VertexArray::SetActive()
{
	glBindVertexArray(mVertexArray);
}