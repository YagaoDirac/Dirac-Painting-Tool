/*
This is the color tool of DCT(Dirac cpp tool - bitmap).
This file is gonna be very long. But content will keep simple.
Classes with various channel(component) count. They are actually pixil data structures. The data members are limited to the essential channel data. Nothing else. So, a std::vector<DCT_color_rgba_uchar> stores the same as array of uchar[4].
Now, 4 types are provided, g(for gray, uchar), rgb(uchar[3]), rgba(uchar[4]), rgba_f(float[4]). More are to be provided in the future.

What's interesting is that, due to the circulating including problem, if class A has a ctor function that constructs A with B, then B is not very likely to have a constructor with "const A&" as parameter. This problem can be handled with a very tricky way, but it based on the sequence the header files are inlucded. I'm not gonna form the library in that way only to make sure the library work in as many cases. So, the convertion functions, I call them tran(short for transformation), are in the end of this file. 
They are all with a prefix of DCT_color_tran. For the data structures share the same count of channels(rgba_f >>> rgba_uchar and so on), the functions are named DCT_color_tran directly. Others have longer names.

to do :
classes with more channel or higher precision should be able to constructed with simpler classes. ctor directly.
[4] should be capable to construct from [3]+[1]


Hope you like this library.

To get the latest:
Github.com/yagaodirac/Dirac-cpp-tool
*/


#pragma once

namespace DCT_color 
{
	static float clamp_to_0_1(const float ref)
	{
		float result = ref;
		if (ref < 0.f)result = 0.f;
		else if (ref > 1.0f)result = 1.0f;
		else if (!(ref >= 0.f && ref <= 1.0f)) result = 0.f;//for nan
		return result;
	};
	static int clamp_to_0_255(const int ref)
	{
		int result = ref;
		if (ref < 0)result = 0;
		else if (ref > 255)result = 255;
		return result;
	};
};

class DCT_color_uchar
{
public:
	unsigned char opaque;
	DCT_color_uchar() { opaque = 0; }
	DCT_color_uchar(unsigned char gray) { opaque = gray;}
};


class DCT_color_rgb_uchar
{
public:
	unsigned char r, g, b;
	DCT_color_rgb_uchar() { r = 0; g = 0; b = 0; }
	DCT_color_rgb_uchar(const DCT_color_rgb_uchar& in) { r = in.r; g = in.g; b = in.b;}
	DCT_color_rgb_uchar(unsigned char gray) { r = gray; g = gray; b = gray;}
	DCT_color_rgb_uchar(unsigned char in_r, unsigned char in_g, unsigned char in_b)
	{r = in_r; g = in_g; b = in_b;}

	static DCT_color_rgb_uchar make_gray_f(float src = 0.5f)
	{
		int temp(DCT_color::clamp_to_0_255(src*255));
		return DCT_color_rgb_uchar(temp, temp, temp);
	};
	static DCT_color_rgb_uchar make_gray(int src = 0x7f)
	{
		return DCT_color_rgb_uchar(src, src, src);
	};

	//operators
	DCT_color_rgb_uchar& operator=(const DCT_color_rgb_uchar& in)
	{
		r = in.r; g = in.g; b = in.b;
		return *this;
	};
	DCT_color_rgb_uchar& operator=(DCT_color_rgb_uchar&& in)
	{
		r = in.r; g = in.g; b = in.b;
		return *this;
	};
	DCT_color_rgb_uchar operator+(const DCT_color_rgb_uchar& other)
	{//add by rgb and linear.
		return DCT_color_rgb_uchar(r + other.r, g + other.g, b + other.b);
	}
	DCT_color_rgb_uchar operator-(const DCT_color_rgb_uchar& other)
	{//add by rgb and linear.
		return DCT_color_rgb_uchar(r - other.r, g - other.g, b - other.b);
	}




};



class DCT_color_rgba_uchar
{
public:
	unsigned char r, g, b, a;
	DCT_color_rgba_uchar() { r = 0; g = 0; b = 0; a = 255; }
	DCT_color_rgba_uchar(unsigned char gray) { r = gray; g = gray; b = gray; a = 255; }
	DCT_color_rgba_uchar(unsigned char in_r, unsigned char in_g, unsigned char in_b)
	{
		r = in_r; g = in_g; b = in_b; a = 255;
	}
	DCT_color_rgba_uchar(unsigned char in_r, unsigned char in_g, unsigned char in_b, unsigned char in_a)
	{
		r = in_r; g = in_g; b = in_b; a = in_a;
	}
	DCT_color_rgba_uchar(const DCT_color_rgba_uchar& in){ r = in.r; g = in.g; b = in.b; a = 255; }

	//blending
	DCT_color_rgba_uchar& overlapped_by(const DCT_color_rgba_uchar& upper)
	{
		int lower_a__mul__one_minus_upper_a = a * (255 - upper.a) / 255;
		r = (lower_a__mul__one_minus_upper_a * r + upper.a*upper.r) / 255;
		g = (lower_a__mul__one_minus_upper_a * g + upper.a*upper.g) / 255;
		b = (lower_a__mul__one_minus_upper_a * b + upper.a*upper.b) / 255;
		a = (lower_a__mul__one_minus_upper_a + upper.a) / 255;
		return *this;
	};
	void overlapped_by_no_return(const DCT_color_rgba_uchar& upper)
	{
		int lower_a__mul__one_minus_upper_a = a * (255 - upper.a) / 255;
		r = (lower_a__mul__one_minus_upper_a * r + upper.a*upper.r) / 255;
		g = (lower_a__mul__one_minus_upper_a * g + upper.a*upper.g) / 255;
		b = (lower_a__mul__one_minus_upper_a * b + upper.a*upper.b) / 255;
		a = (lower_a__mul__one_minus_upper_a + upper.a) / 255;
	};
	//to do 
	
	//overlapped_by precise. and no return


	DCT_color_rgba_uchar& operator=(const DCT_color_rgba_uchar& in)
	{
		r = in.r; g = in.g; b = in.b; a = in.a;
		return *this;
	};
	DCT_color_rgba_uchar& operator=(DCT_color_rgba_uchar&& in)
	{
		r = in.r; g = in.g; b = in.b; a = in.a;
		return *this;
	};
};


class DCT_color_rgba_f
{
	/*
	this color class is designed to do some complex calculating. Some in rgb space, some in hsv space, some in other space.
	some fomular are from https://www.easyrgb.com/en/ .emmm, but not yet.
	*/
public:
	//build in ctor
	DCT_color_rgba_f() { r = 0.f; g = 0.f; b = 0.f; a = 1.f; };
	DCT_color_rgba_f(const DCT_color_rgba_f& in) { r = in.r; g = in.g; b = in.b; a = in.a; };
	DCT_color_rgba_f(DCT_color_rgba_f&& in) { r = in.r; g = in.g; b = in.b; a = in.a; };//not very optimized?

	//customized ctor
	DCT_color_rgba_f(float in_r,  float in_g,  float in_b,  float in_a = 1.f)
	{r = in_r;g = in_g;b = in_b;a = in_a;};

	//from shorter or less precise
	DCT_color_rgba_f(const DCT_color_rgba_uchar& in)
	{r = in.r/255.f; g = in.g / 255.f; b = in.b / 255.f; a = in.a / 255.f;};
	DCT_color_rgba_f(const DCT_color_rgb_uchar& in)
	{r = in.r / 255.f; g = in.g / 255.f; b = in.b / 255.f; a = 1.f;};


	//should this be private? not very sure.
	//dtor
	virtual ~DCT_color_rgba_f() {};

	//operator overload
	DCT_color_rgba_f& operator=(const DCT_color_rgba_f& in)
	{
		r = in.r; g = in.g; b = in.b; a = in.a;
		return *this;
	};
	DCT_color_rgba_f& operator=(DCT_color_rgba_f&& in)
	{
		r = in.r; g = in.g; b = in.b; a = in.a;
		return *this;
	};
	//意义不明。
	//DCT_color_rgba_f operator+(const DCT_color_rgba_f& other)
	//{//add by rgb and linear.
	//	return DCT_color_rgba_f(this->r + other.r, this->g + other.g, this->b + other.b);
	//}
	//DCT_color_rgba_f operator-(const DCT_color_rgba_f& other)
	//{//add by rgb and linear.
	//	return DCT_color_rgba_f(this->r - other.r, this->g - other.g, this->b - other.b);
	//}





	//gray scale
	void set_to_gray_01(const float src = 0.5f, const float in_a = 1.f)
	{ r = g = b = DCT_color::clamp_to_0_1(src); a = DCT_color::clamp_to_0_1(in_a); };
	static DCT_color_rgba_f make_gray(const float src = 0.5f, const float in_a = 1.f)
	{
		float temp(DCT_color::clamp_to_0_1(src));
		return DCT_color_rgba_f(temp, temp, temp, DCT_color::clamp_to_0_1(in_a));
	};
	//特殊gets sets
	//?????????
	//计算，混合那些还没加

	//blending
	DCT_color_rgba_f& overlapped_by(const DCT_color_rgba_f& upper)
	{
		float lower_a__mul__one_minus_upper_a = a * (1.f - upper.a);
		r = lower_a__mul__one_minus_upper_a * r + upper.a*upper.r;
		g = lower_a__mul__one_minus_upper_a * g + upper.a*upper.g;
		b = lower_a__mul__one_minus_upper_a * b + upper.a*upper.b;
		a = lower_a__mul__one_minus_upper_a + upper.a;
		return *this;
	};
	void overlapped_by_no_return(const DCT_color_rgba_f& upper)
	{
		float lower_a__mul__one_minus_upper_a = a * (1.f - upper.a);
		r = lower_a__mul__one_minus_upper_a * r + upper.a*upper.r;
		g = lower_a__mul__one_minus_upper_a * g + upper.a*upper.g;
		b = lower_a__mul__one_minus_upper_a * b + upper.a*upper.b;
		a = lower_a__mul__one_minus_upper_a + upper.a;
	};
	//不慌DCT_color_rgba_f& rgb_lerp(const DCT_color_rgba_f& upper);//Linear interpolation

protected:
	//inner helper tool
	//static float clamp_to_0_1(const float ref)
	//{
	//	float result = ref;
	//	if (ref < 0.f)result = 0.f;
	//	else if (ref > 1.0f)result = 1.0f;
	//	else if (!(ref >= 0.f && ref <= 1.0f)) result = 0.f;//for nan
	//	return result;
	//};

	//data members
public:
	//according to DCT specification, these members should be public.
	float r, g, b, a;
	//gets sets
public:
	//float getr()const { return clamp_to_0_1(this->r); };
	//float getg()const { return clamp_to_0_1(this->g); };
	//float getb()const { return clamp_to_0_1(this->b); };
	//float geta()const { return clamp_to_0_1(this->a); };
	//though the data members are public, the setters are also very helpful. 
	void setr(const float in) { r = in; DCT_color::clamp_to_0_1(r); };
	void setg(const float in) { g = in; DCT_color::clamp_to_0_1(g); };
	void setb(const float in) { b = in; DCT_color::clamp_to_0_1(b); };
	void seta(const float in) { a = in; DCT_color::clamp_to_0_1(a); };
};

namespace DCT_color 
{
	


	//DCT_color_rgba_f DCT_color_tran(const DCT_color_rgba_uchar& in);
	DCT_color_rgba_uchar DCT_color_tran(const DCT_color_rgba_f& in);
	DCT_color_rgba_uchar DCT_color_tran_safe(const DCT_color_rgba_f& in);
	DCT_color_rgb_uchar DCT_color_tran_float4_to_uchar3(const DCT_color_rgba_f& in);
}






