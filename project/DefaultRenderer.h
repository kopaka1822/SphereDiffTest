#pragma once
#include "GBufferRenderer.h"

class DefaultRenderer
{
public:
	static void render(const Pixels<GBuffer>& src, Pixels<uint32_t>& dst)
	{
		auto cs = src.data();
		auto cd = dst.data();

		while (cs != src.end())
		{
			// convert ray to color
			auto dir = cs->ray.direction;
			

			++cs; ++cd;
		}
	}
};