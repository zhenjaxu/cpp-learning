#pragma once

struct AABB{
    float x, y;
    float w, h;

    // AABB碰撞检测：两个矩形是否重叠
    bool intersects(AABB const& other) const{
        return x<other.x+other.w&&      // 左边缘在对方右边缘的左边
               x+w>other.x&&            // 右边缘在对方左边缘的右边
               y<other.y+other.h&&      // 下边缘在对方上边缘的下边
               y+h>other.y;             // 上边缘在对方下边缘的上边
    }
};

