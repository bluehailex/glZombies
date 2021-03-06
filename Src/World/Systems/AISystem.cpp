//=======================================================================================================================|
// Created 2014.01.04 by Daniel L. Watkins
//
// Copyright (C) 2013-2016 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#include "AISystem.h"
#include "../Components/AI/AI.h"


namespace glz
{
	namespace world
	{
		AISystem::AISystem(DetailsSystem *detailsSystem,
						   SpatialSystem *spatialSystem)
		{
			mDetailsSystem = detailsSystem;
			mSpatialSystem = spatialSystem;
		}


		void AISystem::createAI(int entity)
		{
			Details *details = static_cast<Details*>(mDetailsSystem->getComponentForEntity(entity));
			Spatial *spatial = static_cast<Spatial*>(mSpatialSystem->getComponentForEntity(entity));
			addComponent(entity, new AI(details, spatial));
		}


		void AISystem::generateBuckets()
		{
			const int updatesPerSecond = 6;
			const int targetFPS = 60;
			int aiCount = mComponents.size();
			double bucketSize = static_cast<double>(aiCount) / targetFPS * updatesPerSecond;
		
			ComponentList::iterator iter = mComponents.begin();
			Bucket currentBucket; 
			double currentSize = 0.0;

			while (iter != mComponents.end())
			{
				AI *ai = static_cast<AI*>(*iter);
				currentBucket.push_back(ai);

				currentSize += 1.0;

				if (currentSize >= bucketSize)
				{
					currentSize -= bucketSize;
					mUpdateBuckets.push_back(currentBucket);
					currentBucket.clear();
				}

				++iter;
			}

			if (!currentBucket.empty())
				mUpdateBuckets.push_back(currentBucket);
		}


		void AISystem::update()
		{
			if (mUpdateBuckets.empty())
				generateBuckets();

			if (!mUpdateBuckets.empty())
			{
				Bucket currentBucket = mUpdateBuckets.front();
				Bucket::iterator iter = currentBucket.begin();

				while (iter != currentBucket.end())
				{
					(*iter)->update();

					++iter;
				}

				mUpdateBuckets.pop_front();
			}
		}


		AI *AISystem::findNearestAi(Vec2d pos)
		{
			#undef max
			double closest = std::numeric_limits<double>::max();
			AI *closestAi = nullptr;
			bool foundClosest = false;

			ComponentList::iterator aiSearchCmp = mComponents.begin();
			while (aiSearchCmp != mComponents.end())
			{
				AI *aiSearch = static_cast<AI*>(*aiSearchCmp);
				Vec2d testPos = aiSearch->mSpatial->getPos();
				double testDistance = pos.distance(testPos);

				if (testDistance < closest  &&  pos != testPos)
				{
					closest = testDistance;
					closestAi = aiSearch;
					foundClosest = true;
				}

				++aiSearchCmp;
			}

			return closestAi;
		}


		AI *AISystem::findNearestAi(Vec2d pos, String type)
		{
			#undef max
			double closest = std::numeric_limits<double>::max();
			AI *closestAi = nullptr;
			bool foundClosest = false;

			ComponentList::iterator aiSearchCmp = mComponents.begin();
			while (aiSearchCmp != mComponents.end())
			{
				AI *aiSearch = static_cast<AI*>(*aiSearchCmp);
				if (aiSearch->mDetails->getType() == type)
				{
					double testDistance = pos.distance(aiSearch->mSpatial->getPos());
					if (testDistance < closest)
					{
						closest = testDistance;
						closestAi = aiSearch;
						foundClosest = true;
					}
				}

				++aiSearchCmp;
			}

			return closestAi;
		}


		double AISystem::lds_calculateSectorDesnity(Sector &sector, AI *target)
		{
			Sector::iterator iter = sector.begin();
			double density = 0.0;
			Vec2d targetPos = target->mSpatial->getPos();

			while (iter != sector.end())
			{
				density += (1 / (targetPos.distanceSq((*iter)->mSpatial->getPos())));
				++iter;
			}
			 
			return density;
		}

		struct SectorNode
		{
			int sector;
			double density;
		};

		struct MergeNode
		{
			std::list<SectorNode> sectors;

			double calculateDensity()
			{
				if (sectors.size() == 0)
					return 0.0;

				double densitySum = 0.0;
				std::list<SectorNode>::iterator iter = sectors.begin();
				while (iter != sectors.end())
				{
					densitySum += iter->density;
					++iter;
				}

				return densitySum / sectors.size();
			}

			int calculateAverageSectorIndex()
			{
				if (sectors.size() == 0)
					return 0;

				int sum = 0;;
				std::list<SectorNode>::iterator iter = sectors.begin();

				while (iter != sectors.end())
				{
					sum += iter->sector;
					++iter;
				}

				if (sectors.size() > 1)
					int a = 0;

				return sum / sectors.size();
			}
		};

		Vec2d AISystem::findLeastDenseSector(AI *target, std::vector<String> typeMasks, int sectors)
		{
			SectorList sectorList;
			double sectorSize = TWO_PI/sectors;
			Vec2d targetPos = target->mSpatial->getPos();

			//populate the sectorList
			for (int n=0; n<sectors; n++)
				sectorList.push_back(Sector());

			//go through each AI and see which sector it belongs in (if it matches a type in @typeMasks)
			ComponentList::iterator iter = mComponents.begin();
			while (iter != mComponents.end())
			{
				AI *ai = static_cast<AI*>(*iter);
				String type = ai->mDetails->getType();

				//only consider this AI if it has a type to consider
				if (std::find(typeMasks.begin(), typeMasks.end(), type) != typeMasks.end())
				{
					Vec2d aiPos = ai->mSpatial->getPos();
					Vec2d localPos = aiPos - targetPos;
					localPos.normalize();
					double angle = acos(localPos.dot(Vec2d(1.0,0.0)));

					if (localPos.y < 0)
						angle = TWO_PI-angle;

					sectorList[int(angle / sectorSize)].push_back(ai);
				}

				++iter;
			}

			//find the least dense sector
			std::list<SectorNode> baseNodes;
			MergeNode *mergeNode = new MergeNode();
			std::list<MergeNode> mergeNodes;
			SectorNode previousSectorNode;

			for (int n=0; n<sectors; n++)
			{
				SectorNode node;
				node.density = lds_calculateSectorDesnity(sectorList.at(n), target);
				node.sector = n;

				if (n != 0  &&  previousSectorNode.density != node.density)
				{
					mergeNodes.push_back(*mergeNode);
					mergeNode->sectors.clear();
				}

				baseNodes.push_back(node);
				mergeNode->sectors.push_back(node);
				previousSectorNode = node;
			}

			mergeNodes.push_back(*mergeNode);

			//use the least dense mergeNode
			double minimumDensity = std::numeric_limits<double>::max();
			MergeNode leastDenseMergeNode = mergeNodes.front();
			std::list<MergeNode> mergeNodeTies;

			std::list<MergeNode>::iterator mergeIter = mergeNodes.begin();
			while (mergeIter != mergeNodes.end())
			{
				double density = mergeIter->calculateDensity();

				if (density == minimumDensity)
				{
					mergeNodeTies.push_back(*mergeIter);
				}
				else if (density < minimumDensity)
				{
					minimumDensity = density;
					leastDenseMergeNode = *mergeIter;

					//all previous ties are invalid since this is now the minimum
					mergeNodeTies.clear();
				}

				++mergeIter;
			}

			//resolve ties by setting score equal to size
			unsigned int score = 0;
			std::list<MergeNode>::iterator tieIter = mergeNodeTies.begin();
			while (tieIter != mergeNodeTies.end())
			{
				if (tieIter->sectors.size() > score)
				{
					score = tieIter->sectors.size();
					leastDenseMergeNode = *tieIter;
				}

				++tieIter;
			}

			//determine a seek point on the least dense sector
			double centerAngleMeasure = (sectorSize*leastDenseMergeNode.calculateAverageSectorIndex()) + (sectorSize/2.0);
			Vec2d seekPoint(cos(centerAngleMeasure), sin(centerAngleMeasure));
			
			//lengthen the seek point so it last's a while
			seekPoint *= 4.0;

			//convert to world-space
			seekPoint += targetPos;

			return seekPoint;
		}
	};
};