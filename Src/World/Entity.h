//=======================================================================================================================|
// Created 2013.11.20 by Daniel L. Watkins
//
// Copyright (C) 2013 Daniel L. Watkins
//=======================================================================================================================|

#ifndef _glz_ENTITY_H
#define _glz_ENTITY_H

#include "../Main.h"
#include "Component.h"

namespace glz
{
	namespace world
	{
		typedef std::vector<Component> ComponentVector;

		class Entity
		{
		private:
			Vec2d mPos;			//position in world space
			ComponentVector mComponents;

		public:
			Entity() {}

			//accessor and mutator
			void setPos(Vec2d pos) { mPos = pos; }
			Vec2d getPos() { return mPos; }

			void addComponent(Component component);
		};
	}
};


#endif

