//
// Created by 江李铠 on 2022/5/20.
//

#ifndef OPENGL_FFBONE_H
#define OPENGL_FFBONE_H
#include "Base.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "ffAssimpHelper.h"

namespace FF{
    struct ffKeyPosition{
         glm::vec3 m_position;//骨骼的位置
         float timeStamp;
    };
    struct ffKeyRotation{
         glm::quat m_rotation;//四元数
         float timeStamp;
    };
    struct ffKeyScale{
         glm::vec3 m_scale;//缩放向量
         float timeStamp;
    };

    class ffBone {
    private:
        std::vector<ffKeyPosition> m_positionArr;//vector数组，元素类型是ffKeyPosition
        std::vector<ffKeyRotation> m_rotationArr;
        std::vector<ffKeyScale> m_scaleArr;
        //local矩阵，update时可以计算当前关键帧transform状态
        glm::mat4 m_localTransform;
        std::string m_name;
        uint m_id;
    public:
        //每一个bone都会有一个自己对应的animation
        ffBone(std::string _name,uint _id,aiNodeAnim* _boneAnimation);
        ~ffBone();

        //每个bone自己的animatiob
        void initAnimation(const aiNodeAnim* channel);
        void update(float _time);

        glm::mat4 getLocalTransform(){ return m_localTransform; }
        uint getId(){ return m_id; }
        std::string getName(){ return m_name; }
    private:
        //知道前后两个关键帧，就可以知道当前帧对应的值，用插值计算公式
        float getLerpFactor(float _lastTime,float _nextTime,float _curTime);
        //根据传入的时间得到一个平移后的插值矩阵
        glm::vec4 interpolatePosition(float _time);
        glm::vec4 interpolateRotation(float _time);
        glm::vec4 interpolateScale(float _time);

        //获取到前一个位置,旋转，缩放 关键帧
        uint getPositionIndexByTime(float _time);
        uint getRotationIndexByTime(float _time);
        uint getScaleIndexByTime(float _time);
    };
}



#endif //OPENGL_FFBONE_H
