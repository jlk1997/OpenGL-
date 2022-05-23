//
// Created by 江李铠 on 2022/5/23.
//

#ifndef OPENGL_FFANIMATION_H
#define OPENGL_FFANIMATION_H
#include "Base.h"
#include"assimp/Importer.hpp"
#include"assimp/scene.h"
#include"assimp/postprocess.h"
#include"ffAssimpHelper.h"
#include "ffBone.h"

namespace FF{
    //存储动画层次结构
    struct ffAssimpNodeData{
        glm::mat4 m_transform;//没有骨骼时使用transform进行动画效果
        std::string m_name;
        std::vector<ffAssimpNodeData> m_children;

        ffAssimpNodeData():m_transform(1.0f),m_name(""){
            m_children.clear();
        };
    };
    typedef std::map<std::string,ffBoneInfo> ffBoneInfoMap;
    class ffAnimation {
    public:
        ffAnimation() = default;
        ffAnimation(const aiAnimation* _aiAnim,aiNode* _root,ffBoneInfoMap& _boneInfoMap,uint& _boneCount);

        inline float getTicksPerSecond(){ return m_ticksPerSecond;}
        inline float getDuration(){ return m_duration;}
        inline const ffAssimpNodeData& getRootNode(){return m_rootNode;}

        ffBone* findBone(std::string _name);
    private:
        float m_duration;//动画持续时间
        float m_ticksPerSecond;//每s有多少个ticks
        std::vector<ffBone> m_boneArr;
        ffBoneInfoMap m_boneInfoMap;
        ffAssimpNodeData m_rootNode;
    private:

        void readAnimationData(const aiAnimation* _aiAnim,
                               ffBoneInfoMap& _boneInfoMap,
                               uint& _boneCount);
        //将_src传入，构建我们自己的层级架构_dst
        void readHeirarchyData(ffAssimpNodeData& _dst,const aiNode* _src);
    };

}


#endif //OPENGL_FFANIMATION_H
