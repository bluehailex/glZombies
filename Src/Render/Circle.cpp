//=======================================================================================================================|
// Created 2013.12.14 by Daniel L. Watkins
//
// Copyright (C) 2013-2016 Daniel L. Watkins
// This file is licensed under the MIT License.
//=======================================================================================================================|

#include "Circle.h"

namespace glz
{
	void Circle::init(float radius, int segments)
	{
		clearParts();

		float theta = 2.0f * float(glz::PI) / float(segments);
		float cs = std::cosf(theta);
		float sn = std::sinf(theta);
		float t = 0.0f;

		float x = radius;
		float y = 0.0f;

		Part part;

		for (int i=0; i<segments; i++)
		{
			part.vertexData.push_back(x);
			part.vertexData.push_back(y);

			t=x;
			x = cs*x - sn*y;
			y = sn*t + cs*y;
		}

		part.drawMode = GL_LINE_LOOP;
		part.configure();
		addPart(part);
	}
};