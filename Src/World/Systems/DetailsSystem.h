//=======================================================================================================================|
// Created 2014.01.05 by Daniel L. Watkins
//
// Copyright (C) 2013-2016 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#ifndef _glz_DETAILS_SYSTEM_H
#define _glz_DETAILS_SYSTEM_H

#include "System.h"
#include "../Components/Details.h"
#include "../EntityTemplate.h"

namespace glz
{
	namespace world
	{
		class DetailsSystem : public System
		{
		private:

		public:
			void createDetails(int entity, EntityTemplate &et);
			void update() override {}
		};
		

		class DetailsSystemUser
		{
		protected:
			DetailsSystem *mDetailsSystem = nullptr;
		};
	};
};

#endif

