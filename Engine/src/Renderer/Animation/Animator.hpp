#pragma once
#include "Animation.hpp"

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace Cresta
{
    class Animator
    {
    public:
        Animator()
        {
            Init();
        }

        void Init();
        Animator(Animation* Animation);
        void UpdateAnimation();
        void PlayAnimation(Animation* pAnimation);

        void ResetTime();

        void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform);

        std::vector<glm::mat4>& GetFinalBoneMatrices() 
        { 
            return m_FinalBoneMatrices; 
        }

        void StartAnimation();
        void EndAnimation();

    private:
        std::vector<glm::mat4> m_FinalBoneMatrices;
        Animation* m_CurrentAnimation;
        float m_CurrentTime;
        float m_DeltaTime;

        using BoneInfoMap = std::map<std::string, BoneInfo>;
        std::map<Animation*, BoneInfoMap> m_BoneInfoMap;

        std::thread* Thread;
        bool Play = false;
    };
}