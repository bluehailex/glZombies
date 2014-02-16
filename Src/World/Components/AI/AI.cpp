//=======================================================================================================================|
// Created 2014.01.04 by Daniel L. Watkins
//
// Copyright (C) 2013-2014 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#include "AI.h"
#include "../../Systems/AISystem.h"

namespace glz
{
	namespace world
	{
		AI::AI(Details *details, Spatial *spatial) : mSteeringBehaviors(spatial), mScript(this)
		{
			mDetails = details;
			mSpatial = spatial;
		}


		void AI::init()
		{
			mScript.load(String("./Data/AI/") + mDetails->getType() + ".ai");
			mScript.bindToLua();
		}


		void AI::update(Double timeDelta)
		{
			mScript.script_update(&mSteeringBehaviors);
			mSpatial->addSteeringForce(mSteeringBehaviors.computeSteeringForce());
			mSteeringBehaviors.clear();
		}


		AI *AI::findNearestAI()
		{
			return ((AISystem*)getSystem())->findNearestAi(this->mSpatial->getPos());
		}
	};
};