//=======================================================================================================================|
// Created 2013.12.31 by Daniel L. Watkins
//
// Copyright (C) 2013-2016 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#include "Spatial.h"

namespace glz
{
	namespace world
	{
		Spatial::Spatial(Vec2d pos, const EntityTemplate &et)
		: mPos(pos)
		{
			mMass					= et.mass;
			mMaxSpeed				= et.maxSpeed;
			mMaxForce				= et.maxForce;
			mMaxTurnRate			= et.maxTurnRate;

			setHeading(Vec2d(0.0,-1.0));
		}


		Spatial::Spatial(Vec2d pos, Vec2d heading, double mass, double maxSpeed, double maxForce, double maxTurnRate)
			: mPos(pos), mHeading(heading), mMass(mass), mMaxSpeed(maxSpeed), mMaxForce(maxForce), mMaxTurnRate(maxTurnRate)
		{
			setHeading(Vec2d(0.0,-1.0));
		}


		void Spatial::update()
		{
			double timeDelta = mTimeDelta.getElapsedTime().asSeconds();
			mSteeringForce.truncate(mMaxForce);

			Vec2d acceleration = mSteeringForce / mMass;
			mVelocity += acceleration * timeDelta;
			mVelocity.truncate(mMaxSpeed);
			mPos += mVelocity * timeDelta;

			if (mVelocity.lengthSq() > 0.00000001)
			{
				Vec2d velocityNorm = mVelocity;
				velocityNorm.normalize();
				setHeading(velocityNorm);
			}

			mTimeDelta.restart();
		}


		void Spatial::setHeading(Vec2d heading)
		{
			//TODO  maybe have this auto-normalize?
			mHeading = heading;
			mSide = mHeading.perp();
		}


		bool Spatial::rotateToHeading(Vec2d pos)
		{
			Vec2d toTarget = Vec2d(pos-mPos);
			toTarget.normalize();

			double angle = acos(mHeading.dot(pos));

			if (angle < 0.000001)
				return true;

			if (angle > mMaxTurnRate)
				angle = mMaxTurnRate;

			Mat3d rotationMat;
			rotationMat.rotate(angle*mHeading.sign(toTarget));
			rotationMat.transformVec2d(mHeading);
			rotationMat.transformVec2d(mVelocity);

			mSide = mHeading.perp();

			return false;
		}
	};
};