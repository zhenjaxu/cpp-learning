

class VertexArray{
public:
    VertexArray(const float* verts, unsigned int numVerts,
                const unsigned int* indices, unsigned int numIndices);
    ~VertexArray();

    void SetActive();

    unsigned int GetNumVerts() const {return mNumVerts;}
    unsigned int GetNumIndices() const {return mNumIndices;}

private:
    unsigned int mNumVerts;
    unsigned int mNumIndices;
    unsigned int mVertexBuffer;     // 顶点缓冲id
    unsigned int mIndexBuffer;      // 索引缓存id
    unsigned int mVertexArray;      // 顶点数组对象id
};