//=======================================================================================================================|
// Created 2013.11.24 by Daniel L. Watkins
//
// Copyright (C) 2013-2016 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#include "World.h"
#include <dirent.h>

namespace glz
{
	namespace world
	{
		World::World(OpenGLWindow *window)
		{
			mIdTrack = 0;
			mWidth = 2;
			mHeight = 2;
			mWindow = window;

			mSpatialSystem = new SpatialSystem;
			mDetailsSystem = new DetailsSystem;
			mAISystem = new AISystem(mDetailsSystem, mSpatialSystem);
			mDrawableSystem = new DrawableSystem(mWindow);
		}


		World::~World()
		{
			delete mSpatialSystem;
			delete mDetailsSystem;
			delete mAISystem;
			delete mDrawableSystem;
		}


		void World::init()
		{
			loadTemplateEntities();
		}

		
		void World::loadWorldFile(String filepath)
		{
			std::ifstream file(filepath, std::ifstream::in);

			if (!file.is_open())
			{
				std::cout << "Unable to read world data from " << filepath.c_str() << std::endl;
				return;
			}

			char token[64];

			while (!file.eof())
			{
				file >> token;

				//create a new entity instance?
				if (String(token) == "#entity")
				{	
					file >> token;
					EntityTemplate entity = getTemplateEntity(token);

					Vec2d pos;
					file >> token; pos.x = toFloat(token);
					file >> token; pos.y = toFloat(token);

					mIdTrack++;
					mDetailsSystem->createDetails(mIdTrack, entity);
					mSpatialSystem->createSpatial(mIdTrack, pos, entity);
					mDrawableSystem->createDrawable(mIdTrack, String(gDefaultPathDataMesh)+entity.meshFilepath);
					mAISystem->createAI(mIdTrack);
				}
			}
		}


		void World::update() const
		{
			double timeDelta = 0.01;

			mDetailsSystem->update();
			mDrawableSystem->update();
			mAISystem->update();
			mSpatialSystem->update();
		}

		
		EntityTemplate World::getTemplateEntity(String name)
		{
			for (unsigned int n=0; n<mTemplateEntities.size(); n++)
			{
				if (mTemplateEntities[n].name == name)
					return mTemplateEntities[n];
			}

			return EntityTemplate();
		}

		
		void World::loadTemplateEntities()
		{
			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir (gDefaultPathDataEntity.c_str())) != nullptr)
			{
				while ((ent = readdir (dir)) != nullptr)
				{
					if (ent->d_type == DT_REG)
					{
						loadTemplateEntity(String(gDefaultPathDataEntity) + String(ent->d_name));
					}
				}
				closedir (dir);
			}
		}


#define parse(token, term, assign)				\
		if (String(token) == term)					\
		{											\
		file >> token;							\
		assign = token;							\
		}

#define parseDouble(token, term, assign)				\
		if (String(token) == term)					\
		{											\
		file >> token;							\
		assign = toDouble(token);							\
		}

		
		void World::loadTemplateEntity(String filepath)
		{
			std::ifstream file(filepath, std::ifstream::in);

			if (!file.is_open())
			{
				std::cout << "Unable to read entity data from " << filepath.c_str() << std::endl;
				return;
			}

			char token[64];
			EntityTemplate te;

			while (!file.eof())
			{
				file >> token;



				parse(token, "#mesh", te.meshFilepath)
				parse(token, "#name", te.name)
				parse(token, "#type", te.type)

				parseDouble(token, "#sp_mass", te.mass)
				parseDouble(token, "#sp_maxSpeed", te.maxSpeed)
				parseDouble(token, "#sp_maxForce", te.maxForce)
				parseDouble(token, "#sp_maxTurnRate", te.maxTurnRate)
			}

			mTemplateEntities.push_back(te);
		}
	};
};