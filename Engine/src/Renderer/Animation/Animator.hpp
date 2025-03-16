#pragma once
#include "Animation.hpp"

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

namespace Cresta
{

    class ThreadPool {
    public:
        ThreadPool(size_t numThreads);
        ~ThreadPool();
        void Enqueue(std::function<void()> task);

    private:
        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::condition_variable condition;
        bool stop;
    };

    class Animator
    {
    public:
        Animator()
        {
            Init();
        }

        void Init();
        Animator(Animation* Animation);
        void UpdateAnimation(float dt);
        void PlayAnimation(Animation* pAnimation);

        void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform, ThreadPool& pool);
        std::vector<glm::mat4> GetFinalBoneMatrices() 
        { 
            return m_FinalBoneMatrices; 
        }

    private:
        std::vector<glm::mat4> m_FinalBoneMatrices;
        Animation* m_CurrentAnimation;
        float m_CurrentTime;
        float m_DeltaTime;

        ThreadPool* m_Pool;
        std::mutex* m_BoneMatrixMutex;
        using BoneInfoMap = std::map<std::string, BoneInfo>;

        std::map<Animation*, BoneInfoMap> m_BoneInfoMap;
    };
}