//
// Created by 江李铠 on 2022/5/20.
//

#include "ffBone.h"

namespace FF{

    ffBone::ffBone(std::string _name, uint _id, aiNodeAnim *_boneAnimation):
    m_name(_name),m_id(_id),m_localTransform(1.0)//初始化
    {
        this->initAnimation(_boneAnimation);
    }

    ffBone::~ffBone() {

    }

    //每一个bone对应一个动画channel，整个一组动画，成为animation.
    //initAnimation作用：读取每个channel对应的三种关键帧
    void ffBone::initAnimation(const aiNodeAnim *channel) {
        //先抽取每个channel的Position关键帧,   mNumPositionKeys表示该bone动画中的位置关键帧的数量
        for(uint positionIndex = 0;positionIndex < channel->mNumPositionKeys;positionIndex++){
            //提取出位置关键帧的值(mValue)与时间戳(mTime)
            aiVector3D _aiPos = channel->mPositionKeys[positionIndex].mValue;
            double _timeStamp = channel->mPositionKeys[positionIndex].mTime;
            //使用ffAssimpHelper定义好的转换，assimp转换为glm数据
            //实例化结构并赋值位置与时间戳，位置需要从Assimp的数据格式转换为glm的数据格式
            ffKeyPosition _data;
            _data.m_position = ffAssimpHelper::getGLMVec3(_aiPos);
            _data.timeStamp = _timeStamp;
            //将位置关键帧数据存放值 位置关键帧数据数组中
            m_positionArr.push_back(_data);
        }
        //抽取每个channel的旋转关键帧
        for(uint rotationIndex = 0;rotationIndex < channel->mNumRotationKeys;rotationIndex++){
            //提取出位置关键帧的值(mValue)与时间戳(mTime)
            aiQuaternion _aiQuat = channel->mRotationKeys[rotationIndex].mValue;
            double _timeStamp = channel->mRotationKeys[rotationIndex].mTime;
            //使用ffAssimpHelper定义好的转换，assimp转换为glm数据
            //实例化结构并赋值位置与时间戳，位置需要从Assimp的数据格式转换为glm的数据格式
            ffKeyRotation _data;
            _data.m_rotation = ffAssimpHelper::getGLMVec3(_aiQuat);
            _data.timeStamp = _timeStamp;
            //将位置关键帧数据存放值 位置关键帧数据数组中
            m_rotationArr.push_back(_data);
        }
        //抽取每个channel的缩放关键帧
        for(uint scaleIndex = 0;scaleIndex < channel->mNumScalingKeys;scaleIndex++){
            //提取出位置关键帧的值(mValue)与时间戳(mTime)
            aiVector3D _aiScale = channel->mScalingKeys[scaleIndex].mValue;
            double _timeStamp = channel->mScalingKeys[scaleIndex].mTime;
            //使用ffAssimpHelper定义好的转换，assimp转换为glm数据
            //实例化结构并赋值位置与时间戳，位置需要从Assimp的数据格式转换为glm的数据格式
            ffKeyScale _data;
            _data.m_scale = ffAssimpHelper::getGLMVec3(_aiScale);
            _data.timeStamp = _timeStamp;
            //将位置关键帧数据存放值 位置关键帧数据数组中
            m_scaleArr.push_back(_data);
        }
    }

    void ffBone::update(float _time) {

    }

    //通过nextTime与lastTime来找到curTime的对应的值的占比
    float ffBone::getLerpFactor(float _lastTime, float _nextTime, float _curTime) {
        assert(_nextTime - _lastTime);//用于判断 next帧是不是在last帧之后，如果值为负数则为假，直接调用absorb终止程序

        return (_curTime - _lastTime)/(_nextTime - _lastTime);
    }

    //根据时间来得到插值
    glm::vec4 ffBone::interpolatePosition(float _time) {
        if(){

        }
        return glm::vec4();
    }

    glm::vec4 ffBone::interpolateRotation(float _time) {
        return glm::vec4();
    }

    glm::vec4 ffBone::interpolateScale(float _time) {
        return glm::vec4();
    }

    //找到当前时间前面的关键帧lastTime
    uint ffBone::getPositionIndexByTime(float _time) {
//        遍历数组通过比较时间戳与当前时间来得到前一帧
        for(uint i = 0;i < m_positionArr.size();i++){
            if(_time < m_positionArr[i+1].timeStamp){
                return i;
            }
        }
        return 0;
    }

    uint ffBone::getRotationIndexByTime(float _time) {
        for(uint i = 0;i < m_rotationArr.size();i++){
            if(_time < m_rotationArr[i+1].timeStamp){
                return i;
            }
        }
        return 0;
    }

    uint ffBone::getScaleIndexByTime(float _time) {
        for(uint i = 0;i < m_scaleArr.size();i++){
            if(_time < m_scaleArr[i+1].timeStamp){
                return i;
            }
        }
        return 0;
    }
}