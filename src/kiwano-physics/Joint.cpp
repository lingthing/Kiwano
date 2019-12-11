// Copyright (c) 2018-2019 Kiwano - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <kiwano-physics/Joint.h>
#include <kiwano-physics/World.h>

namespace kiwano
{
	namespace physics
	{
		//
		// Joint
		//

		Joint::Joint()
			: joint_(nullptr)
			, world_(nullptr)
			, type_(Type::Unknown)
		{
		}

		Joint::Joint(b2Joint* joint)
			: Joint()
		{
			SetB2Joint(joint);
		}

		Joint::Joint(World* world, b2JointDef* joint_def)
			: Joint()
		{
			Init(world, joint_def);
		}

		Joint::~Joint()
		{
			if (world_)
			{
				world_->RemoveJoint(this);
			}
		}

		void Joint::Init(World* world, b2JointDef* joint_def)
		{
			world_ = world;
			if (world_)
			{
				world_->AddJoint(this);

				b2Joint* joint = world_->GetB2World()->CreateJoint(joint_def);
				SetB2Joint(joint);
			}
		}

		BodyPtr Joint::GetBodyA() const
		{
			KGE_ASSERT(joint_);

			b2Body* body = joint_->GetBodyA();
			return BodyPtr(static_cast<Body*>(body->GetUserData()));
		}

		BodyPtr Joint::GetBodyB() const
		{
			KGE_ASSERT(joint_);

			b2Body* body = joint_->GetBodyB();
			return BodyPtr(static_cast<Body*>(body->GetUserData()));
		}

		void Joint::SetB2Joint(b2Joint* joint)
		{
			joint_ = joint;
			if (joint_)
			{
				type_ = Joint::Type(joint_->GetType());
			}
		}

		//
		// DistanceJoint
		//

		DistanceJoint::DistanceJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		DistanceJoint::DistanceJoint(World* world, b2DistanceJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		DistanceJoint::DistanceJoint(World* world, DistanceJoint::Param const& param)
			: Joint()
			, raw_joint_(nullptr)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2DistanceJointDef def;
			def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor_a), world->Stage2World(param.anchor_b));
			def.frequencyHz = param.frequency_hz;
			def.dampingRatio = param.damping_ratio;

			Init(world, &def);
			raw_joint_ = static_cast<b2DistanceJoint*>(GetB2Joint());
		}

		void DistanceJoint::SetLength(float length)
		{
			KGE_ASSERT(raw_joint_ && world_);
			raw_joint_->SetLength(world_->Stage2World(length));
		}

		float DistanceJoint::GetLength() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetLength());
		}

		//
		// FrictionJoint
		//

		FrictionJoint::FrictionJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		FrictionJoint::FrictionJoint(World* world, b2FrictionJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		FrictionJoint::FrictionJoint(World* world, FrictionJoint::Param const& param)
			: Joint()
			, raw_joint_(nullptr)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2FrictionJointDef def;
			def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor));
			def.maxForce = param.max_force;
			def.maxTorque = world->Stage2World(param.max_torque);

			Init(world, &def);
			raw_joint_ = static_cast<b2FrictionJoint*>(GetB2Joint());
		}

		void FrictionJoint::SetMaxForce(float length)
		{
			KGE_ASSERT(raw_joint_);
			raw_joint_->SetMaxForce(length);
		}

		float FrictionJoint::GetMaxForce() const
		{
			KGE_ASSERT(raw_joint_);
			return raw_joint_->GetMaxForce();
		}

		void FrictionJoint::SetMaxTorque(float length)
		{
			KGE_ASSERT(raw_joint_ && world_);
			raw_joint_->SetMaxTorque(world_->Stage2World(length));
		}

		float FrictionJoint::GetMaxTorque() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetMaxTorque());
		}

		//
		// GearJoint
		//

		GearJoint::GearJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		GearJoint::GearJoint(World* world, b2GearJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		GearJoint::GearJoint(World* world, GearJoint::Param param)
			: Joint()
			, raw_joint_(nullptr)
		{
			KGE_ASSERT(param.joint_a && param.joint_b);

			b2GearJointDef def;
			def.joint1 = param.joint_a->GetB2Joint();
			def.joint2 = param.joint_b->GetB2Joint();
			def.ratio = param.ratio;

			Init(world, &def);
			raw_joint_ = static_cast<b2GearJoint*>(GetB2Joint());
		}

		void GearJoint::SetRatio(float ratio)
		{
			KGE_ASSERT(raw_joint_);
			raw_joint_->SetRatio(ratio);
		}

		float GearJoint::GetRatio() const
		{
			KGE_ASSERT(raw_joint_);
			return raw_joint_->GetRatio();
		}

		//
		// MotorJoint
		//

		MotorJoint::MotorJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		MotorJoint::MotorJoint(World* world, b2MotorJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		MotorJoint::MotorJoint(World* world, MotorJoint::Param const& param)
			: Joint()
			, raw_joint_(nullptr)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2MotorJointDef def;
			def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body());
			def.maxForce = param.max_force;
			def.maxTorque = world->Stage2World(param.max_torque);

			Init(world, &def);
			raw_joint_ = static_cast<b2MotorJoint*>(GetB2Joint());
		}

		void MotorJoint::SetMaxForce(float length)
		{
			KGE_ASSERT(raw_joint_);
			raw_joint_->SetMaxForce(length);
		}

		float MotorJoint::GetMaxForce() const
		{
			KGE_ASSERT(raw_joint_);
			return raw_joint_->GetMaxForce();
		}

		void MotorJoint::SetMaxTorque(float length)
		{
			KGE_ASSERT(raw_joint_ && world_);
			raw_joint_->SetMaxTorque(world_->Stage2World(length));
		}

		float MotorJoint::GetMaxTorque() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetMaxTorque());
		}

		//
		// PrismaticJoint
		//

		PrismaticJoint::PrismaticJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		PrismaticJoint::PrismaticJoint(World* world, b2PrismaticJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		PrismaticJoint::PrismaticJoint(World* world, PrismaticJoint::Param const& param)
			: Joint()
			, raw_joint_(nullptr)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2PrismaticJointDef def;
			def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor), Stage2World(param.axis));
			def.enableLimit = param.enable_limit;
			def.lowerTranslation = world->Stage2World(param.lower_translation);
			def.upperTranslation = world->Stage2World(param.upper_translation);
			def.enableMotor = param.enable_motor;
			def.maxMotorForce = param.max_motor_force;
			def.motorSpeed = world->Stage2World(param.motor_speed);

			Init(world, &def);
			raw_joint_ = static_cast<b2PrismaticJoint*>(GetB2Joint());
		}

		float PrismaticJoint::GetJointTranslation() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetJointTranslation());
		}

		float PrismaticJoint::GetJointSpeed() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetJointSpeed());
		}

		float PrismaticJoint::GetLowerLimit() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetLowerLimit());
		}

		float PrismaticJoint::GetUpperLimit() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetUpperLimit());
		}

		void PrismaticJoint::SetLimits(float lower, float upper)
		{
			KGE_ASSERT(raw_joint_ && world_);
			raw_joint_->SetLimits(world_->Stage2World(lower), world_->Stage2World(upper));
		}
		
		//
		// PulleyJoint
		//

		PulleyJoint::PulleyJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		PulleyJoint::PulleyJoint(World* world, b2PulleyJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		PulleyJoint::PulleyJoint(World* world, PulleyJoint::Param const& param)
			: Joint()
			, raw_joint_(nullptr)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2PulleyJointDef def;
			def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.ground_anchor_a), world->Stage2World(param.ground_anchor_b),
				world->Stage2World(param.anchor_a), world->Stage2World(param.anchor_b), param.ratio);

			Init(world, &def);
			raw_joint_ = static_cast<b2PulleyJoint*>(GetB2Joint());
		}

		Point PulleyJoint::GetGroundAnchorA() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetGroundAnchorA());
		}

		Point PulleyJoint::GetGroundAnchorB() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetGroundAnchorB());
		}

		float PulleyJoint::GetRatio() const
		{
			KGE_ASSERT(raw_joint_);
			return raw_joint_->GetRatio();
		}

		float PulleyJoint::GetLengthA() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetLengthA());
		}

		float PulleyJoint::GetLengthB() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetLengthB());
		}

		float PulleyJoint::GetCurrentLengthA() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetCurrentLengthA());
		}

		float PulleyJoint::GetCurrentLengthB() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetCurrentLengthB());
		}

		//
		// RevoluteJoint
		//

		RevoluteJoint::RevoluteJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		RevoluteJoint::RevoluteJoint(World* world, b2RevoluteJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		RevoluteJoint::RevoluteJoint(World* world, RevoluteJoint::Param const& param)
			: Joint()
			, raw_joint_(nullptr)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2RevoluteJointDef def;
			def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor));
			def.enableLimit = param.enable_limit;
			def.lowerAngle = math::Degree2Radian(param.lower_angle);
			def.upperAngle = math::Degree2Radian(param.upper_angle);
			def.enableMotor = param.enable_motor;
			def.maxMotorTorque = world->Stage2World(param.max_motor_torque);
			def.motorSpeed = math::Degree2Radian(param.motor_speed);

			Init(world, &def);
			raw_joint_ = static_cast<b2RevoluteJoint*>(GetB2Joint());
		}

		float RevoluteJoint::GetJointAngle() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return math::Radian2Degree(raw_joint_->GetJointAngle());
		}

		float RevoluteJoint::GetJointSpeed() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return math::Radian2Degree(raw_joint_->GetJointSpeed());
		}

		float RevoluteJoint::GetLowerLimit() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return math::Radian2Degree(raw_joint_->GetLowerLimit());
		}

		float RevoluteJoint::GetUpperLimit() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return math::Radian2Degree(raw_joint_->GetUpperLimit());
		}

		void RevoluteJoint::SetLimits(float lower, float upper)
		{
			KGE_ASSERT(raw_joint_ && world_);
			raw_joint_->SetLimits(math::Degree2Radian(lower), math::Degree2Radian(upper));
		}

		void RevoluteJoint::SetMaxMotorTorque(float torque)
		{
			KGE_ASSERT(raw_joint_ && world_);
			raw_joint_->SetMaxMotorTorque(world_->Stage2World(torque));
		}

		float RevoluteJoint::GetMaxMotorTorque() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetMaxMotorTorque());
		}

		//
		// RopeJoint
		//

		RopeJoint::RopeJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		RopeJoint::RopeJoint(World* world, b2RopeJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		RopeJoint::RopeJoint(World* world, RopeJoint::Param const& param)
			: Joint()
			, raw_joint_(nullptr)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2RopeJointDef def;
			def.bodyA = param.body_a->GetB2Body();
			def.bodyB = param.body_b->GetB2Body();
			def.localAnchorA = world->Stage2World(param.local_anchor_a);
			def.localAnchorB = world->Stage2World(param.local_anchor_b);
			def.maxLength = world->Stage2World(param.max_length);

			Init(world, &def);
			raw_joint_ = static_cast<b2RopeJoint*>(GetB2Joint());
		}

		void RopeJoint::SetMaxLength(float length)
		{
			KGE_ASSERT(raw_joint_ && world_);
			raw_joint_->SetMaxLength(world_->Stage2World(length));
		}

		float RopeJoint::GetMaxLength() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetMaxLength());
		}

		//
		// WeldJoint
		//

		WeldJoint::WeldJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		WeldJoint::WeldJoint(World* world, b2WeldJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		WeldJoint::WeldJoint(World* world, WeldJoint::Param const& param)
			: Joint()
			, raw_joint_(nullptr)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2WeldJointDef def;
			def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor));
			def.frequencyHz = param.frequency_hz;
			def.dampingRatio = param.damping_ratio;

			Init(world, &def);
			raw_joint_ = static_cast<b2WeldJoint*>(GetB2Joint());
		}

		//
		// WheelJoint
		//

		WheelJoint::WheelJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		WheelJoint::WheelJoint(World* world, b2WheelJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		WheelJoint::WheelJoint(World* world, WheelJoint::Param const& param)
			: Joint()
			, raw_joint_(nullptr)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2WheelJointDef def;
			def.Initialize(param.body_a->GetB2Body(), param.body_b->GetB2Body(), world->Stage2World(param.anchor), Stage2World(param.axis));
			def.enableMotor = param.enable_motor;
			def.maxMotorTorque = world->Stage2World(param.max_motor_torque);
			def.motorSpeed = world->Stage2World(param.motor_speed);
			def.frequencyHz = param.frequency_hz;
			def.dampingRatio = param.damping_ratio;

			Init(world, &def);
			raw_joint_ = static_cast<b2WheelJoint*>(GetB2Joint());
		}

		float WheelJoint::GetJointTranslation() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetJointTranslation());
		}

		float WheelJoint::GetJointLinearSpeed() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetJointLinearSpeed());
		}

		void WheelJoint::SetMaxMotorTorque(float torque)
		{
			KGE_ASSERT(raw_joint_ && world_);
			raw_joint_->SetMaxMotorTorque(world_->Stage2World(torque));
		}

		float WheelJoint::GetMaxMotorTorque() const
		{
			KGE_ASSERT(raw_joint_ && world_);
			return world_->World2Stage(raw_joint_->GetMaxMotorTorque());
		}

		//
		// MouseJoint
		//

		MouseJoint::MouseJoint()
			: Joint()
			, raw_joint_(nullptr)
		{
		}

		MouseJoint::MouseJoint(World* world, b2MouseJointDef* def)
			: Joint(world, def)
			, raw_joint_(nullptr)
		{
		}

		MouseJoint::MouseJoint(World* world, MouseJoint::Param const& param)
			: Joint()
			, raw_joint_(nullptr)
		{
			KGE_ASSERT(param.body_a && param.body_b);

			b2MouseJointDef def;
			def.bodyA = param.body_a->GetB2Body();
			def.bodyB = param.body_b->GetB2Body();
			def.target = world->Stage2World(param.target);
			def.maxForce = param.max_force;
			def.frequencyHz = param.frequency_hz;
			def.dampingRatio = param.damping_ratio;

			Init(world, &def);
			raw_joint_ = static_cast<b2MouseJoint*>(GetB2Joint());
		}

		void MouseJoint::SetMaxForce(float length)
		{
			KGE_ASSERT(raw_joint_);
			raw_joint_->SetMaxForce(length);
		}

		float MouseJoint::GetMaxForce() const
		{
			KGE_ASSERT(raw_joint_);
			return raw_joint_->GetMaxForce();
		}

}
}
