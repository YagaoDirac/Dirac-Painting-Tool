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

#include"DCT util.h"

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
		int temp(DCT_color::clamp_to_0_255(static_cast<int>(src*255)));
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



class DCT_color_rgb_f
{
	/*
	this color class is designed to do some complex calculating. Some in rgb space, some in hsv space, some in other space.
	some fomular are from https://www.easyrgb.com/en/ .emmm, but not yet.
	*/
public:
	//build in ctor
	DCT_color_rgb_f() { r = 0.f; g = 0.f; b = 0.f; };
	DCT_color_rgb_f(const DCT_color_rgb_f& in) { r = in.r; g = in.g; b = in.b;  };
	DCT_color_rgb_f(DCT_color_rgb_f&& in) { r = in.r; g = in.g; b = in.b; };//not very optimized?

	//customized ctor
	DCT_color_rgb_f(float in_r, float in_g, float in_b)
	{
		r = in_r; g = in_g; b = in_b;
	};

	//from shorter or less precise
	DCT_color_rgb_f(const DCT_color_rgb_uchar& in)
	{
		r = in.r / 255.f; g = in.g / 255.f; b = in.b / 255.f;
	};



	//should this be private? not very sure.
	//dtor
	virtual ~DCT_color_rgb_f() {};

	//operator overload
	DCT_color_rgb_f& operator=(const DCT_color_rgb_f& in)
	{
		r = in.r; g = in.g; b = in.b; 
		return *this;
	};
	DCT_color_rgb_f& operator=(DCT_color_rgb_f&& in)
	{
		r = in.r; g = in.g; b = in.b;
		return *this;
	};
	
	//gray scale
public:
	void set_to_gray_01(const float src = 0.5f)
	{
		r = g = b = DCT_color::clamp_to_0_1(src); 
	};
	static DCT_color_rgb_f make_gray(const float src = 0.5f)
	{
		float temp(DCT_color::clamp_to_0_1(src));
		return DCT_color_rgb_f(temp, temp, temp);
	};
	
	//特殊gets sets
public:
	//All 3 numbers are clamped to 0 to 1.
	//For hue, 0 - red, 1/6 - yellow, etc.
	void setHSV(float Hue, float saturation, float velocity)
	{
		Hue = DCT_color::clamp_to_0_1(Hue);
		saturation = DCT_color::clamp_to_0_1(saturation);
		velocity = DCT_color::clamp_to_0_1(velocity);

		//float min_channel = velocity * (1.f - saturation);
		
		Hue *= 6.f;
		float fractal_of_Hue = Hue - static_cast<int>(Hue);
		
		if (Hue <= 1.f)//red to yellow, r>g>b, g increaces.
		{
			r = velocity;
			b = velocity * (1.f - saturation);
			g = DCT_lerp(b, r, fractal_of_Hue);
			return;
		}
		if (Hue <= 2.f)//yellow to green, g>r>b, r decreaces.
		{
			g = velocity;
			b = velocity * (1.f - saturation);
			r = DCT_lerp(g, b, fractal_of_Hue);
			return;
		}
		if (Hue <= 3.f)//green to cyan, g>b>r, b increaces.
		{
			g = velocity;
			r = velocity * (1.f - saturation);
			b = DCT_lerp(r, g, fractal_of_Hue);
			return;
		}
		if (Hue <= 4.f)//cyan to blue, b>g>r, g decreaces.
		{
			b = velocity;
			r = velocity * (1.f - saturation);
			g = DCT_lerp(b, r, fractal_of_Hue);
			return;
		}
		if (Hue <= 5.f)//blue to purple, b>r>g, r increaces.
		{
			b = velocity;
			g = velocity * (1.f - saturation);
			r = DCT_lerp(g, b, fractal_of_Hue);
			return;
		}
		{//purple to red, r>b>g, b decreaces.
			r = velocity;
			g = velocity * (1.f - saturation);
			b = DCT_lerp(r, g, fractal_of_Hue);
			return;
		}
	}

	float get_velocity()const{return DCT_max(r, DCT_max(g, b));}
	float get_brightness()const 	{		return get_velocity(); 	};
	
	float get_saturation()const
	{
		float velocity = get_velocity();
		if (0.f == velocity)return 0.f;//safety
		return DCT_min(r, DCT_min(g, b)) / velocity;
	}
	float get_saturation_with_v(float& out_velocity)const
	{
		out_velocity = get_velocity();
		if (0.f == out_velocity)return 0.f;//safety
		return DCT_min(r, DCT_min(g, b)) / out_velocity;
	}

	// 0 - red, 1/6 - yellow, etc.
	float get_hue()const
	{
		//For fragments 0 to 5, 0 means hue is between red and yellow. 1 is between yellow and green.
		//     0, 1, 2, 3, 4, 5
		//r>g	 t  _  _  _  t  t	
		//g>b  t  t  t  _  _  _
		//b>r  _  _  t  t  t  _
		//space means false. For the fragments with 2 trues, the middle channel increases.
		//But shame, this is not needed at all... F***.

		//ok,,the code is gonna be very ugly...
		//When the channels vary from 2 open 1 close to 1 open 2 close, the index could be calculated by(max-mid)/(max-min). This works for yellow to green, cyan to blue, and purple to red.
		float max_minus_min;
		if (r >= g)
		{
			if (r >= b)
			{
				if (g >= b)
				{//rgb, red to yellow
					max_minus_min = r - b;
					if (0.f == max_minus_min)return 0.f;//safety
					return (g - b) / max_minus_min / 6.f;
				}
				else//if (g >= b)
				{//rbg, purple to red
					max_minus_min = r - g;
					if (0.f == max_minus_min)return 0.f;//safety
					return ((r - b) / max_minus_min + 5.f) / 6.f;
				}
			}
			else//if (r >= b)
			{//brg, blue to purple
				max_minus_min = b - g;
				if (0.f == max_minus_min)return 0.f;//safety
				return ((r - g) / max_minus_min + 4.f) / 6.f;
			}
		}
		else//if (r >= g)
		{
			if (r >= b)
			{//grb, yellow to green
				max_minus_min = g - b;
				if (0.f == max_minus_min)return 0.f;//safety
				return ((g - r) / max_minus_min + 1.f) / 6.f;
			}
			else//if (r >= b)
			{
				if (g >= b)
				{//gbr, green to cyan
					max_minus_min = g - r;
					if (0.f == max_minus_min)return 0.f;//safety
					return ((b - r) / max_minus_min + 2.f) / 6.f;
				}
				else//if (g >= b)
				{//bgr, cyan to blue
					max_minus_min = b - r;
					if (0.f == max_minus_min)return 0.f;//safety
					return ((b - g) / max_minus_min + 3.f) / 6.f;
				}
			}
		}
	}

	// 0 - red, 1/6 - yellow, etc.
	//If velocity is 0, then all are 0s. If saturation is 0, then hue is 0 but doesn't mean red.
	void get_HSV(float& hue, float& saturation, float& velocity)
	{
		//copied from get_hue()
		float max_minus_min;
		if (r >= g)
		{
			if (r >= b)
			{
				velocity = r;//r is already the max channel.
				if (g >= b)
				{//rgb, red to yellow
					max_minus_min = r - b;
					if (0.f == max_minus_min)
					{
						hue = saturation = 0.f;
						return;
					}
					else
					{
						hue = (g - b) / max_minus_min / 6.f;
						saturation = b / r;
						return;
					}
				}
				else//if (g >= b)
				{//rbg, purple to red
					max_minus_min = r - g;
					if (0.f == max_minus_min) 
					{
						hue = saturation = 0.f;
						return;
					}
					else
					{
						hue = ((r - b) / max_minus_min + 5.f) / 6.f;
						saturation = g / r;
						return;
					}
				}
			}
			else//if (r >= b)
			{//brg, blue to purple
				velocity = b;
				max_minus_min = b - g;
				if (0.f == max_minus_min) 
				{
					hue = saturation = 0.f;
					return;
				}
				else
				{
					hue = ((r - g) / max_minus_min + 4.f) / 6.f;
					saturation = g / b;
					return;
				}
			}
		}
		else//if (r >= g)   else means r<g
		{
			if (r >= b)
			{//grb, yellow to green
				velocity = g;
				max_minus_min = g - b;
				if (0.f == max_minus_min)
				{
					hue = saturation = 0.f;
					return;
				}
				else
				{
					hue = ((g - r) / max_minus_min + 1.f) / 6.f;
					saturation = b / g;
					return;
				}
			}
			else//if (r >= b)
			{
				if (g >= b)
				{//gbr, green to cyan
					velocity = g;
					max_minus_min = g - r;
					if (0.f == max_minus_min)
					{
						hue = saturation = 0.f;
						return;
					}
					else
					{
						hue = ((b - r) / max_minus_min + 2.f) / 6.f;
						saturation = r / g;
						return;
					}
				}
				else//if (g >= b)
				{//bgr, cyan to blue
					velocity = b;
					max_minus_min = b - r;
					if (0.f == max_minus_min)
					{
						hue = saturation = 0.f;
						return;
					}
					else
					{
						hue = ((b - g) / max_minus_min + 3.f) / 6.f;
						saturation = r / b;
						return;
					}
				}
			}
		}
	}



	//?????????
	//计算，混合那些还没加

	//blending
	static DCT_color_rgb_f lerp(const DCT_color_rgb_f& lhs, const DCT_color_rgb_f& rhs, float t)
	{
		return DCT_color_rgb_f(DCT_lerp(lhs.r, rhs.r, t), DCT_lerp(lhs.g, rhs.g, t), DCT_lerp(lhs.b, rhs.b, t));
	}
	DCT_color_rgb_f& overlapped_by(const DCT_color_rgb_f& upper, float upper_alpha)
	{
		float one_minus_upper_a = 1.f - upper_alpha;
		r = one_minus_upper_a * r + upper_alpha*upper.r;
		g = one_minus_upper_a * g + upper_alpha*upper.g;
		b = one_minus_upper_a * b + upper_alpha*upper.b;
		return *this;
	};
	void overlapped_by_no_return(const DCT_color_rgb_f& upper, float upper_alpha)
	{
		float one_minus_upper_a = 1.f - upper_alpha;
		r = one_minus_upper_a * r + upper_alpha * upper.r;
		g = one_minus_upper_a * g + upper_alpha * upper.g;
		b = one_minus_upper_a * b + upper_alpha * upper.b;
	};
	//不慌DCT_color_rgba_f& rgb_lerp(const DCT_color_rgba_f& upper);//Linear interpolation


	//data members
public:
	//according to DCT specification, these members should be public.
	float r, g, b;
	//gets sets
public:
	
	//though the data members are public, the setters are also very helpful. 
	void setr(const float in) { r = in; DCT_color::clamp_to_0_1(r); };
	void setg(const float in) { g = in; DCT_color::clamp_to_0_1(g); };
	void setb(const float in) { b = in; DCT_color::clamp_to_0_1(b); };
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
	DCT_color_rgba_uchar(const DCT_color_rgba_uchar& in) { r = in.r; g = in.g; b = in.b; a = in.a; }
	DCT_color_rgba_uchar(const DCT_color_rgb_uchar& in){ r = in.r; g = in.g; b = in.b; a = 255; }

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
	DCT_color_rgba_f(const DCT_color_rgb_uchar& in)
	{r = in.r / 255.f; g = in.g / 255.f; b = in.b / 255.f; a = 1.f;};
	DCT_color_rgba_f(const DCT_color_rgba_uchar& in)
	{r = in.r/255.f; g = in.g / 255.f; b = in.b / 255.f; a = in.a / 255.f;};
	DCT_color_rgba_f(const DCT_color_rgb_f& in) { r = in.r; g = in.g; b = in.b; a = 1.f; };

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
	
	//DCT_color_rgba_f DCT_color_tran(const DCT_color_rgba_uchar& in);not very sure. This might be among ctors of rgba f.
	DCT_color_rgba_uchar DCT_color_tran(const DCT_color_rgba_f& in);
	DCT_color_rgba_uchar DCT_color_tran_safe(const DCT_color_rgba_f& in);
	DCT_color_rgb_uchar DCT_color_tran_float4_to_uchar3(const DCT_color_rgba_f& in);

	
	DCT_color_rgb_uchar DCT_color_tran(const DCT_color_rgb_f& in);
	DCT_color_rgb_uchar DCT_color_tran_safe(const DCT_color_rgb_f& in);



	class gen
	{
		//to do 
		//The distortion function is ready but is set not to effect the color.




		//This class is designed to contain the random colour generator codes and make them reusable.
		//This version is based on rgb mode. So another version based on hsv is to be done.
	public:
		DCT_color_rgb_f adjustment_2bits[5][5][5];
		///////////////////////r//g//b
		//This array is the database for color distortion based on distorting the rgb cube.

		float hue_from, hue_to;
		float saturation_max, saturation_min;
		float velocity_max, velocity_min;
	
		//ctor
	public: 
		gen() 
		{ 
			hue_from = 0.f;
			hue_to = 1.f;
			saturation_min = velocity_min = 0.f;
			saturation_max = velocity_max = 1.f;

			for (int r = 0; r < 5; ++r)
				for (int g = 0; g < 5; ++g)
					for (int b = 0; b < 5; ++b)
					{
						adjustment_2bits[r][g][b].r = r / 4.f;
						adjustment_2bits[r][g][b].g = g / 4.f;
						adjustment_2bits[r][g][b].b = b / 4.f;
					}
		};

	//
	public:
		DCT_color_rgb_f gen_rgb_f(bool safety_check = true)
		{
			if (safety_check)
			{
				hue_from = DCT_clamp(hue_from, 0.f, 1.f);
				hue_to = DCT_clamp(hue_to, 0.f, 1.f);
				saturation_min = DCT_clamp(saturation_min, 0.f, 1.f);
				velocity_min = DCT_clamp(velocity_min, 0.f, 1.f);
				saturation_max = DCT_clamp(saturation_max, 0.f, 1.f);
				velocity_max = DCT_clamp(velocity_max, 0.f, 1.f);
			}

			//干活。怎么感觉这个类不伦不类的，又不是纯rgb，又不是纯hsv。好像纯rgb也不用我写了吧？
			if (hue_to < hue_from)hue_to += 1.f;
			float hue = DCT_random(hue_from, hue_to);
			if (hue > 1.f)hue -= 1.f;
			float saturation = DCT_random(saturation_min, saturation_max);
			float velocity = DCT_random(velocity_min, velocity_max);

			DCT_color_rgb_f temp_color; 
			temp_color.setHSV(hue, saturation, velocity);






			//Applies the distortion.
			int indec[3];
			float fractal_part[3];
			for (int i = 0; i < 3; ++i)
			{
				//This part takes care of the offset for the first float member in the color class.
				auto t = &temp_color;
				auto t2 = reinterpret_cast<char*>(t);
				t2 += offsetof(DCT_color_rgb_f, r);
				auto t3 = reinterpret_cast<float*>(t2);
				auto temp = t3[i] * 4.f;
				
				indec[i] = static_cast<int>(temp);
				fractal_part[i] = temp - static_cast<float>(indec[i]);
				if (indec[i] >= 4)//safety
				{
					indec[i] = 3;
					fractal_part[i] = 1.f;
				}
				if (indec[i] < 0)//safety
				{
					indec[i] = i;
					fractal_part[i] = 0.f;
				}
			}
			
		
			DCT_color_rgb_f lerp_result_1[4];
			lerp_result_1[0] = DCT_color_rgb_f::lerp(
				adjustment_2bits[indec[0]    ][indec[1]][indec[2]],
				adjustment_2bits[indec[0] + 1][indec[1]][indec[2]],
				fractal_part[0]);
			lerp_result_1[1] = DCT_color_rgb_f::lerp(
				adjustment_2bits[indec[0]    ][indec[1] + 1][indec[2]],
				adjustment_2bits[indec[0] + 1][indec[1] + 1][indec[2]],
				fractal_part[0]);
			lerp_result_1[2] = DCT_color_rgb_f::lerp(
				adjustment_2bits[indec[0]    ][indec[1]][indec[2] + 1],
				adjustment_2bits[indec[0] + 1][indec[1]][indec[2] + 1],
				fractal_part[0]);
			lerp_result_1[3] = DCT_color_rgb_f::lerp(
				adjustment_2bits[indec[0]    ][indec[1] + 1][indec[2] + 1],
				adjustment_2bits[indec[0] + 1][indec[1] + 1][indec[2] + 1],
				fractal_part[0]);

			DCT_color_rgb_f lerp_result_2[2];
			lerp_result_2[0] = DCT_color_rgb_f::lerp(lerp_result_1[0], lerp_result_1[1], fractal_part[1]);
			lerp_result_2[1] = DCT_color_rgb_f::lerp(lerp_result_1[2], lerp_result_1[3], fractal_part[1]);

			DCT_color_rgb_f lerp_result_3;
			lerp_result_3= DCT_color_rgb_f::lerp(lerp_result_2[0], lerp_result_2[1], fractal_part[2]);

			return lerp_result_3;
		}
	};
}






