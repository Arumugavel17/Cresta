#include "Animator.hpp"
#include "Core/Time.hpp"

namespace Cresta
{
    Animator::Animator(Animation* Animation) 
    {
        Init();

        m_CurrentAnimation = Animation;
        m_BoneInfoMap[m_CurrentAnimation] = m_CurrentAnimation->GetBoneIDMap();
    }

    void Animator::Init() 
    {
        m_CurrentTime = 0.0;
        m_CurrentAnimation = nullptr;

        m_FinalBoneMatrices.reserve(200);

        for (int i = 0; i < 200; i++)
        {
            m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
        }
    }

    void Animator::UpdateAnimation()
    {
        while (Play)
        {
            m_DeltaTime = Time::DeltaTime();
            m_DeltaTime *= 10.0f;
            if (m_CurrentAnimation)
            {
                m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * m_DeltaTime;
                m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
                CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // Prevent overloading
        }
    }

    void Animator::PlayAnimation(Animation* pAnimation)
    {
        m_CurrentAnimation = pAnimation;
        m_BoneInfoMap[m_CurrentAnimation] = m_CurrentAnimation->GetBoneIDMap();
        m_CurrentTime = 0.0f;
    }

    void Animator::ResetTime()
    {
        m_CurrentTime = 0.0f;
    }

    void Animator::CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
    {
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

        for (int i = 0; i < node->childrenCount; i++)
        {
            CalculateBoneTransform(&node->children[i], globalTransformation);
        }
    }

    void Animator::StartAnimation()
    {
        Play = true;
        Thread = new std::thread(&Animator::UpdateAnimation,this);
    }

    void Animator::EndAnimation()
    {
        Play = false;
        Thread->join();
        delete Thread;
    }
}