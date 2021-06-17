#pragma once

namespace AWayBack
{
	struct Color
	{
		Color()
			: R(0), G(0), B(0), A(0)
		{			
		}

		Color(int r, int g, int b, int a)
			: R(r / 255.f), G(g / 255.0f), B(b / 255.0f), A(a / 255.f)
		{
		}
		
		Color(int r, int g, int b)
			: R(r / 255.f), G(g / 255.0f), B(b / 255.0f), A(1)
		{
		}

		float R, G, B, A;
	};
}