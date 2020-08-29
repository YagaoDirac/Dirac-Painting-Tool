/*
This is the bitmap layer tool of DCT(Dirac cpp tool - bitmap).
search for 
void how_to_inside();
for help.

This file is gonna be very long. But content will keep simple.

Now I have DCT memory managing tool. This bitmap component is based on that.
All the classes are derived from DCT_component. They all have to be contained by some container. And the container classes are in "DCT bmp layer node.h"
This component idea is mainly about the memory managing. A component is a chunk of memory which stores bitmap info. The basic pixil structure are included from "DCT color.h". The standard container is std::vector. Notice that I develop this library on MSVS2017, so it should probably work on MSVS 2015 and 2019. But due to the implementation of stl or even the standard library of c++, the code might go wrong on other compilers or os. 



Notice:
The invoke sequence of all the drawing functions are
The versions with parameters of "int r, int g, int b" call "DCT_color_rgb_uchar".


to do :
save to png.
reverse alpha for speeding up png saving.
现在暂时加了2个，只有3个通道的和只有1个的。没有画图，保存那些。后面看。


Hope you like this library.

To get the latest:
Github.com/yagaodirac/Dirac-cpp-tool
*/

#pragma once

template<class T>
void swap(T& a, T& b);

//yeah, this is how I avoid including too many headers. github.com/yagaodirac/cpp-code-snipet-set for the tool.
template<class _Ty>
class std::allocator;
template<class _Ty, class _Alloc = std::allocator<_Ty> >
class std::vector;

template<class _Elem>
struct std::char_traits;
template<class _Ty>
class std::allocator;
template<class _Elem,
	class _Traits = std::char_traits<_Elem>,
	class _Alloc = std::allocator<_Elem>>
	class std::basic_string;
using string = std::basic_string<char, std::char_traits<char>, std::allocator<char>>;





//#include "simple_mem_to_png.h"
//#include "../libpng code/png.h"


#include "DCT color.h"
#include "../include/DCT object.h"




namespace DCT_bmp_layer
{
	enum class error_info_enum
	{
		OK,
		to_init_size_greater_than_4096__invoke_init_force,
		bad_param,
		already_initialized
	};
}




class DCT_bmp_layer_memory_base
{
public:
	size_t width, height;
	//std::string lebal;//保存的时候就是这个名字。
	//bool reverse_alpha; to do 

	size_t safe_size() { return 16384; };//The class never handle any image greater this size. 16384 for now.

	//for const member, init them as typename::typename():const member name1{init code}const member name2{init code}{function body here}
	//ctor and dtor
	DCT_bmp_layer_memory_base() {};
	virtual ~DCT_bmp_layer_memory_base() {};
private:
	//I don't want these functions to be invoked in a casual way. To copy an object of this class with all its content, invode copy() explicitly.
	DCT_bmp_layer_memory_base(const DCT_bmp_layer_memory_base&) { throw; };
	DCT_bmp_layer_memory_base(DCT_bmp_layer_memory_base&&) { throw; };
	DCT_bmp_layer_memory_base& operator=(const DCT_bmp_layer_memory_base&) { throw; return *this; };
	DCT_bmp_layer_memory_base& operator=(DCT_bmp_layer_memory_base&&) { throw; return *this; };

public:
	//init
	virtual DCT_bmp_layer::error_info_enum init(size_t in_width, size_t in_height) = 0;

	//In fact, I'm not sure whether to keep this function in future version.
	//DCT_bmp_layer::error_info_enum __init_force(size_t in_width, size_t in_height)

	virtual void __set_rand_rgb__DEBUG() = 0;

	//gets sets
	virtual bool get_has_memory() const = 0; 

	//utilities
	int clamp_coord(int in, int lo, int hi) { return in < lo ? lo : (in < hi ? in : (hi - 1)); };// lo <= result < hi
	size_t get_offset(int x, int y)const { return (y*width + x); };
	virtual size_t get_row_offset() const  = 0;

	//暂时不提供保存功能。
	////saving to png
	//enum class saving_error_info_enum
	//{
	//	OK,

	//	Failed_to_open_file,
	//	Failed_to_create_PNG_write_struct,
	//	Failed_to_create_PNG_info_struct,
	//	Failed_to_set_jmp_for_PNG_lib
	//};
};


class DCT_bmp_layer_memory_uchar_x3:public DCT_bmp_layer_memory_base
{
public:
	std::vector<DCT_color_rgb_uchar> memory;//has to be vector

public:
	DCT_bmp_layer_memory_uchar_x3() {};
	virtual ~DCT_bmp_layer_memory_uchar_x3() {};
private:
	//I don't want these functions to be invoked in a casual way. To copy an object of this class with all its content, invode copy() explicitly.
	DCT_bmp_layer_memory_uchar_x3(const DCT_bmp_layer_memory_uchar_x3&) { throw; };
	DCT_bmp_layer_memory_uchar_x3(DCT_bmp_layer_memory_uchar_x3&&) { throw; };
	DCT_bmp_layer_memory_uchar_x3& operator=(const DCT_bmp_layer_memory_uchar_x3&) { throw; return *this; };
	DCT_bmp_layer_memory_uchar_x3& operator=(DCT_bmp_layer_memory_uchar_x3&&) { throw; return *this; };

public:
	virtual DCT_bmp_layer::error_info_enum init(size_t in_width, size_t in_height) override
	{
		if (in_width > 4096 || in_height > 4096)
			return DCT_bmp_layer::error_info_enum::to_init_size_greater_than_4096__invoke_init_force;

		width = in_width;
		height = in_height;
		memory.clear();
		memory.resize(width*height);
		return DCT_bmp_layer::error_info_enum::OK;
	};
	DCT_bmp_layer::error_info_enum __init_force(size_t in_width, size_t in_height) {
		if (in_width > safe_size() || in_height > safe_size())
			return DCT_bmp_layer::error_info_enum::bad_param;

		width = in_width;
		height = in_height;
		memory.resize(width*height);
		return DCT_bmp_layer::error_info_enum::OK;
	};

	void __set_rand_rgb__DEBUG() override{
		for (auto &pixil : memory)
		{
			int random_number = rand();
			pixil.r = unsigned char(random_number);
			pixil.g = unsigned char(random_number >> 3);
			pixil.r = unsigned char(random_number >> 6);
		}
	};

	virtual bool get_has_memory() const { return !memory.empty(); };

	
	//utilities
	void __set_mem(size_t offset, const DCT_color_rgb_uchar& color) 
	{ memory[offset] = color; };
	size_t get_row_offset()const override  { return size_t(width * 3); };;

	//drawing

	DCT_color_rgb_uchar ForeGroundcolor;
	void set_color(unsigned char in_r, unsigned char in_g, unsigned char in_b)
	{ForeGroundcolor.r = in_r;ForeGroundcolor.g = in_g;ForeGroundcolor.b = in_b;};
	void set_color(const DCT_color_rgb_uchar& in) { ForeGroundcolor = in; };

	void draw_dot(int x, int y, bool safecheck = true);
	void draw_line_xyx(int x_start, int y, int x_end_involved, bool safecheck = true);//horizontal line.
	void draw_line_xyy(int x, int y_start, int y_end_involved, bool safecheck = true);//vertical line.
	void draw_sq(int x_start, int x_end_involved, int y_start, int y_end_involved, bool safecheck = true);

	//no saving to png, no how to(). temporarily.

};





class DCT_bmp_layer_memory_uchar_x4 : public DCT_bmp_layer_memory_base
{
public:
	std::vector<DCT_color_rgba_uchar> memory;//has to be vector

public:
	//size_t width, height;
	//std::string lebal;//保存的时候就是这个名字。
	//std::vector<DCT_color_rgba_uchar> memory;//has to be vector
	//bool reverse_alpha; to do 

	//size_t safe_size() {return 16384;};//The class never handle any image greater this size. 16384 for now.

	//for const member, init them as typename::typename():const member name1{init code}const member name2{init code}{function body here}
	//ctor and dtor
	DCT_bmp_layer_memory_uchar_x4() {};
	virtual ~DCT_bmp_layer_memory_uchar_x4() {};
private:
	//I don't want these functions to be invoked in a casual way. To copy an object of this class with all its content, invode copy() explicitly.
	DCT_bmp_layer_memory_uchar_x4(const DCT_bmp_layer_memory_uchar_x4&) { throw; };
	DCT_bmp_layer_memory_uchar_x4(DCT_bmp_layer_memory_uchar_x4&&) { throw; };
	DCT_bmp_layer_memory_uchar_x4& operator=(const DCT_bmp_layer_memory_uchar_x4&) { throw; return *this; };
	DCT_bmp_layer_memory_uchar_x4& operator=(DCT_bmp_layer_memory_uchar_x4&&) { throw; return *this; };

public:
	//init
	DCT_bmp_layer::error_info_enum init(size_t in_width, size_t in_height)override
	{
		if (in_width > 4096 || in_height > 4096)
			return DCT_bmp_layer::error_info_enum::to_init_size_greater_than_4096__invoke_init_force;

		width = in_width;
		height = in_height;
		memory.clear();
		memory.resize(width*height);
		return DCT_bmp_layer::error_info_enum::OK;
	};
	DCT_bmp_layer::error_info_enum __init_force(size_t in_width, size_t in_height) {
		if (in_width > safe_size() || in_height > safe_size())
			return DCT_bmp_layer::error_info_enum::bad_param;

		width = in_width;
		height = in_height;
		memory.resize(width*height);
		return DCT_bmp_layer::error_info_enum::OK;
	};
	void set_alpha_255()
	{
		for (auto &pixil : memory)
			pixil.a = unsigned char(255);
	};
	void __set_rand_rgb__DEBUG() override{
		for (auto &pixil : memory)
		{
			int random_number = rand();
			pixil.r = unsigned char(random_number);
			pixil.g = unsigned char(random_number >> 3);
			pixil.b = unsigned char(random_number >> 6);
			pixil.a = unsigned char((random_number >> 8) | 0x80);

		}
	};

	//gets sets
	virtual bool get_has_memory() const override { return !memory.empty(); };

	//utilities
	void __set_mem(size_t offset, const DCT_color_rgba_uchar& color)
	{memory[offset] = color;};
	void __pixil_overlapped_by(size_t offset, const DCT_color_rgba_uchar& color)
	{
		memory[offset].overlapped_by_no_return(color);
	};

	//int clamp_coord(int in, int lo, int hi);// lo <= result < hi 基类的应该没有问题
	size_t get_offset(int x, int y)const { return (y*width + x); };
	size_t get_row_offset()const { return size_t(width * 4); };



	DCT_color_rgba_uchar ForeGroundcolor;
	void set_color(unsigned char in_r, unsigned char in_g, unsigned char in_b, unsigned char in_a)
	{ForeGroundcolor.r = in_r; ForeGroundcolor.g = in_g; ForeGroundcolor.b = in_b; ForeGroundcolor.a = in_a;};
	void set_color(const DCT_color_rgba_uchar& in) { ForeGroundcolor = in; };

	//drawing
	void draw_dot(int x, int y, bool safecheck = true);
	void draw_line_xyx(int x_start, int y, int x_end_involved,  bool safecheck = true);//horizontal line.
	void draw_line_xyy(int x, int y_start, int y_end_involved,  bool safecheck = true);//vertical line.
	void draw_sq(int x_start, int x_end_involved, int y_start, int y_end_involved, bool safecheck = true);


	
	//saving_error_info_enum save_to_file(const char* filename);暂时不提供



	//how to 
	void how_to_inside();

};

