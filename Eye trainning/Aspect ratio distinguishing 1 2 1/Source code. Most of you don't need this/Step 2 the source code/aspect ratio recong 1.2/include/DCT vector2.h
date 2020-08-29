#pragma once

class DCT_vector2_i
{
public:
	int x, y;

	DCT_vector2_i();
	DCT_vector2_i(int in_x, int in_y);
	DCT_vector2_i(float in_x, float in_y);
	DCT_vector2_i(const DCT_vector2_i& src);
	DCT_vector2_i(DCT_vector2_i&& src);
	DCT_vector2_i& operator=(const DCT_vector2_i& rhs);
	DCT_vector2_i& operator=(DCT_vector2_i&& rhs);
	virtual ~DCT_vector2_i();


	DCT_vector2_i operator+(int rhs);
	DCT_vector2_i operator+(const DCT_vector2_i& rhs);
	DCT_vector2_i operator- (int rhs);
	DCT_vector2_i operator-	(const DCT_vector2_i& rhs);
		


};



