#pragma once


//暂时不着急改成模板。只有2个。以后看情况再说。




class DCT_vector2_i
{
public:
	int x, y;

	//ctor, =, dtor
	DCT_vector2_i() { x = y = 0; };
	DCT_vector2_i(int in_x, int in_y) { x = in_x; y = in_y; };
	DCT_vector2_i(float in_x, float in_y) 
	{x = static_cast<int>(in_x); y = static_cast<int>(in_y);
	}
	DCT_vector2_i(const DCT_vector2_i& src) { x = src.x; y = src.y; };
	DCT_vector2_i(DCT_vector2_i&& src) { x = src.x; y = src.y; };
	DCT_vector2_i& operator=(const DCT_vector2_i& rhs) 
	{x = rhs.x; y = rhs.y; return *this;
	};;
	DCT_vector2_i& operator=(DCT_vector2_i&& rhs)
	{x = rhs.x; y = rhs.y; return *this;
	};;
	virtual ~DCT_vector2_i() {};


	DCT_vector2_i operator+(int rhs) 
	{return DCT_vector2_i(x + rhs, y + rhs);
	};
	DCT_vector2_i operator+(const DCT_vector2_i& rhs) 
	{return DCT_vector2_i(x + rhs.x, y + rhs.y);
	};
	DCT_vector2_i operator- (int rhs) 
	{return DCT_vector2_i(x - rhs, y - rhs);
	};
	DCT_vector2_i operator-	(const DCT_vector2_i& rhs) 
	{return DCT_vector2_i(x - rhs.x, y - rhs.y);
	};
		
};




class DCT_vector2
{
public:
	float x, y;

	//ctor, =, dtor
	DCT_vector2() { x = y = 0; };
	DCT_vector2(float in_x, float in_y) { x = in_x; y = in_y; };
	DCT_vector2(int in_x, int in_y) {
		x = static_cast<float>(in_x); y = static_cast<float>(in_y);
	}
	DCT_vector2(const DCT_vector2& src) { x = src.x; y = src.y; };
	DCT_vector2(DCT_vector2&& src) { x = src.x; y = src.y; };
	DCT_vector2& operator=(const DCT_vector2& rhs) {
		x = rhs.x; y = rhs.y; return *this;
	};;
	DCT_vector2& operator=(DCT_vector2&& rhs) {
		x = rhs.x; y = rhs.y; return *this;
	};;
	virtual ~DCT_vector2() {};


	DCT_vector2 operator+(int rhs) {
		return DCT_vector2(x + rhs, y + rhs);
	};
	DCT_vector2 operator+(const DCT_vector2& rhs) {
		return DCT_vector2(x + rhs.x, y + rhs.y);
	};
	DCT_vector2 operator- (int rhs) {
		return DCT_vector2(x - rhs, y - rhs);
	};
	DCT_vector2 operator-	(const DCT_vector2& rhs) {
		return DCT_vector2(x - rhs.x, y - rhs.y);
	};
};



namespace DCT_vector
{
	DCT_vector2_i convert(const DCT_vector2& rhs);
	DCT_vector2_i convert(DCT_vector2&& rhs);

	DCT_vector2 convert(const DCT_vector2_i& rhs);
	DCT_vector2 convert(DCT_vector2_i&& rhs);
}



