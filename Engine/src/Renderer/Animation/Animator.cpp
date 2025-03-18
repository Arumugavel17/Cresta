#include "Animator.hpp"

namespace Cresta
{

    ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this] {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) return;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    task();
                }
                });
        }
    }

    ThreadPool::~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers)
            worker.join();
    }

    void ThreadPool::Enqueue(std::function<void()> task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.push(std::move(task));
        }
        condition.notify_one();
    }

    Animator::Animator(Animation* Animation)
    {
        Init();

        m_CurrentAnimation = Animation;
        m_BoneInfoMap[m_CurrentAnimation] = m_CurrentAnimation->GetBoneIDMap();
    }

    void Animator::Init()
    {
        m_Pool = new ThreadPool(std::thread::hardware_concurrency());
        m_BoneMatrixMutex = new std::mutex();
        m_CurrentTime = 0.0;
        m_CurrentAnimation = nullptr;

        m_FinalBoneMatrices.reserve(200);

        for (int i = 0; i < 200; i++)
        {
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
        }
    }

    void Animator::UpdateAnimation(float dt)
    {
        m_DeltaTime = dt;
        if (m_CurrentAnimation)
        {
            m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
            m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
            CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f), *m_Pool);
        }
    }

    void Animator::PlayAnimation(Animation* pAnimation)
    {
        m_CurrentAnimation = pAnimation;
        m_BoneInfoMap[m_CurrentAnimation] = m_CurrentAnimation->GetBoneIDMap();
        m_CurrentTime = 0.0f;
    }

    void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform, ThreadPool& pool)
    {
        //std::cout << "Current thread id: " << std::this_thread::get_id() << std::endl;

        //std::unique_lock<std::mutex> lock(*m_BoneMatrixMutex);
        std::string nodeName = node->name;
        glm::mat4 nodeTransform = node->transformation;


        Bone* Bone_ = m_CurrentAnimation->FindBone(nodeName);
           
        if (Bone_)
        {
            Bone_->Update(m_CurrentTime);
            nodeTransform = Bone_->GetLocalTransform();
        }

        glm::mat4 globalTransformation = parentTransform * nodeTransform;

        std::map<std::string,BoneInfo> boneInfoMap = m_BoneInfoMap[m_CurrentAnimation];
        if (boneInfoMap.find(nodeName) != boneInfoMap.end())
        {
            int index = boneInfoMap[nodeName].id;
            glm::mat4 offset = boneInfoMap[nodeName].offset;
            m_FinalBoneMatrices[index] = globalTransformation * offset;
        }

        //lock.release();
        //m_BoneMatrixMutex->unlock();

        for (int i = 0; i < node->childrenCount; i++)
        {
            //std::cout << node->name << " " << node->childrenCount << "\n";

            //    pool.Enqueue([this, &node, i, globalTransformation, &pool] {
            //        if (node && node->children.size() > 0)
            //        {
                        CalculateBoneTransform(&node->children[i], globalTransformation, pool);
                    /*}
                    });*/
        }
    }
}