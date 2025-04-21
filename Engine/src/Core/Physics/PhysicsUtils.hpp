#pragma once
#include "Crestaph.hpp"
#include "PhysicsHeaders.hpp"
#include "Core/Dispatcher.hpp"

using namespace JPH;
using namespace JPH::literals;

namespace Cresta
{
	enum class CollisionEventType
	{
		ContactAdded,
		ContactPersisted,
		ContactRemoved
	};

	class CollisionEvent
	{
	public:
		CollisionEvent(CollisionEventType EventType,const Body& inBody1, const Body& inBody2) : m_Body1(inBody1), m_Body2(inBody2)
		{
			m_EventType = EventType;
		}

		CollisionEventType GetCollisionType()
		{
			return m_EventType;
		}

		CollisionEventType m_EventType;
		const Body& Body1;
		const Body& Body2;
	};

	//class ContactAdded : CollisionEvent
	//{
	//public:
	//	ContactAdded(const Body& inBody1, const Body& inBody2) : CollisionEvent(CollisionEventType::ContactAdded,inBody1,inBody2)
	//	{
	//	}
	//};

	//class ContactPersisted : CollisionEvent
	//{
	//	ContactPersisted(const Body& inBody1, const Body& inBody2) : CollisionEvent(CollisionEventType::ContactPersisted, inBody1, inBody2)
	//	{
	//	}
	//};

	//class ContactRemoved : CollisionEvent
	//{
	//	ContactRemoved(const Body& inBody1, const Body& inBody2) : CollisionEvent(CollisionEventType::ContactRemoved, inBody1, inBody2)
	//	{
	//	}
	//};


	enum class ColliderShape
	{
		BoxCollider,
		SphereCollider,
		CapsuleCollider,
		MeshCollider
	};

	namespace Layers
	{
		static constexpr ObjectLayer NON_MOVING = 0;
		static constexpr ObjectLayer MOVING = 1;
		static constexpr ObjectLayer NoCollision = 2;
		static constexpr ObjectLayer Colliders = 3;
		static constexpr ObjectLayer NUM_LAYERS = 4;
	};

	/// Class that determines if two object layers can collide
	class ObjectLayerPairFilterImpl : public ObjectLayerPairFilter
	{
	public:
		virtual bool ShouldCollide(ObjectLayer inObject1, ObjectLayer inObject2) const override
		{
			switch (inObject1)
			{
			case Layers::NON_MOVING:
				return false; // Non moving only collides with moving
			case Layers::MOVING:
				return false; // Moving collides with everything
			case Layers::Colliders:
				return inObject2 == Layers::Colliders; // Colliders Collide with everything
			case Layers::NoCollision:
				return false; // "Nocollision" Layer does not collide with anything
			default:
				JPH_ASSERT(false);
				return false;
			}
		}
	};

	namespace BroadPhaseLayers
	{
		static constexpr BroadPhaseLayer NON_MOVING(0);
		static constexpr BroadPhaseLayer MOVING(1);
		static constexpr BroadPhaseLayer NoCollision(2);
		static constexpr BroadPhaseLayer Colliders(3);
		static constexpr uint NUM_LAYERS(4);
	};

	class BPLayerInterfaceImpl final : public BroadPhaseLayerInterface
	{
	public:
		BPLayerInterfaceImpl()
		{
			// Create a mapping table from object to broad phase layer
			mObjectToBroadPhase[Layers::NON_MOVING] = BroadPhaseLayers::NON_MOVING;
			mObjectToBroadPhase[Layers::MOVING] = BroadPhaseLayers::MOVING;
			mObjectToBroadPhase[Layers::NoCollision] = BroadPhaseLayers::NoCollision;
			mObjectToBroadPhase[Layers::Colliders] = BroadPhaseLayers::Colliders;
		}

		virtual uint GetNumBroadPhaseLayers() const override
		{
			return BroadPhaseLayers::NUM_LAYERS;
		}

		virtual BroadPhaseLayer	GetBroadPhaseLayer(ObjectLayer inLayer) const override
		{
			JPH_ASSERT(inLayer < Layers::NUM_LAYERS);
			return mObjectToBroadPhase[inLayer];
		}

#if defined(JPH_EXTERNAL_PROFILE) || defined(JPH_PROFILE_ENABLED)
		virtual const char* GetBroadPhaseLayerName(BroadPhaseLayer inLayer) const override
		{
			switch ((BroadPhaseLayer::Type)inLayer)
			{
			case (BroadPhaseLayer::Type)BroadPhaseLayers::NON_MOVING:	return "NON_MOVING";
			case (BroadPhaseLayer::Type)BroadPhaseLayers::MOVING:		return "MOVING";
			case (BroadPhaseLayer::Type)BroadPhaseLayers::NoCollision:	return "NoCollision";
			case (BroadPhaseLayer::Type)BroadPhaseLayers::Colliders:	return "Colliders";
			default:													JPH_ASSERT(false); return "INVALID";
			}
		}
#endif // JPH_EXTERNAL_PROFILE || JPH_PROFILE_ENABLED

	private:
		BroadPhaseLayer	mObjectToBroadPhase[Layers::NUM_LAYERS];
	};

	/// Class that determines if an object layer can collide with a broadphase layer
	class ObjectVsBroadPhaseLayerFilterImpl : public ObjectVsBroadPhaseLayerFilter
	{
	public:
		virtual bool ShouldCollide(ObjectLayer inLayer1, BroadPhaseLayer inLayer2) const override
		{
			switch (inLayer1)
			{
			case Layers::NON_MOVING:
				return false;
			case Layers::MOVING:
				return false;
			case Layers::NoCollision:
				return false;
			case Layers::Colliders:
				return inLayer2 == BroadPhaseLayers::Colliders;
			default:
				JPH_ASSERT(false);
				return false;
			}
		}
	};

	// An example contact listener
	class ColliderContactListener : public ContactListener
	{
	public:
		// See: ContactListener
		virtual ValidateResult	OnContactValidate(const Body& inBody1, const Body& inBody2, RVec3Arg inBaseOffset, const CollideShapeResult& inCollisionResult) override
		{
			CRESTA_TRACE("Contact validate callback");
			DispatchEvent(CollisionEventType::ContactAdded, inBody1, inBody2);
			// Allows you to ignore a contact before it is created (using layers to not make objects collide is cheaper!)
			return ValidateResult::AcceptAllContactsForThisBodyPair;
		}

		virtual void OnContactAdded(const Body& inBody1, const Body& inBody2, const ContactManifold& inManifold, ContactSettings& ioSettings) override
		{
			DispatchEvent(CollisionEventType::ContactPersisted, inBody1, inBody2);
			CRESTA_TRACE("A contact was added");
		}

		virtual void OnContactPersisted(const Body& inBody1, const Body& inBody2, const ContactManifold& inManifold, ContactSettings& ioSettings) override
		{
			CRESTA_TRACE("A contact was persisted");
			DispatchEvent(CollisionEventType::ContactRemoved, inBody1, inBody2);
		}

		virtual void OnContactRemoved(const SubShapeIDPair& inSubShapePair) override
		{
			CRESTA_TRACE("A contact was removed");
		}

		void DispatchEvent(CollisionEventType EventType,const Body& inBody1, const Body& inBody2);

	};

	// An example activation listener
	class ColliderBodyActivationListener : public BodyActivationListener
	{
	public:
		virtual void OnBodyActivated(const BodyID& inBodyID, uint64 inBodyUserData) override
		{
			CRESTA_TRACE("{} Body Activated", inBodyID.GetIndexAndSequenceNumber());
		}

		virtual void OnBodyDeactivated(const BodyID& inBodyID, uint64 inBodyUserData) override
		{
			CRESTA_TRACE("{} Body De-Activated", inBodyID.GetIndexAndSequenceNumber());
		}
	};
}