#pragma once

template<class T>
void swap(T& a, T& b);

template<class _Ty>
class std::allocator;
template<class _Ty, class _Alloc = std::allocator<_Ty> >
class std::vector;

class DCT_color_rgba_f;

//#include "simple_mem_to_png.h"
#include "../libpng code/png.h"


//rgba form.
struct __rgba_struct
{
	unsigned char r, g, b, a;
};


class DCT_image_layer_uint8
{
public:
	size_t width, height;
	std::vector<__rgba_struct> memory;//has to be vector
	bool has_memory;
	//bool reverse_alpha; to do 

	static const size_t safe_size;//The class never handle any image greater this size. 16384 for now.

	enum class error_type
	{
		OK,
		to_init_size_greater_than_4096__invoke_init_force,
		bad_param,
	};

	DCT_image_layer_uint8();
	virtual ~DCT_image_layer_uint8();
private:
	//I don't want these functions to be invoked in a casual way. To copy an object of this class with all its content, invode copy() explicitly.
	DCT_image_layer_uint8(const DCT_image_layer_uint8&);
	DCT_image_layer_uint8(DCT_image_layer_uint8&&);
	DCT_image_layer_uint8& operator=(const DCT_image_layer_uint8&);
	DCT_image_layer_uint8& operator=(DCT_image_layer_uint8&&);

public:
	//init
	error_type init(size_t in_width, size_t in_height);
	error_type __init_force(size_t in_width, size_t in_height);
	void set_alpha_255();
	void __set_rand_rgb();

	//gets sets
	bool get_has_memory() const;

	//utilities
	void __set_mem(size_t offset, unsigned char r, unsigned char g, unsigned char b);
	int clamp_coord(int in, int lo, int hi);// lo <= result < hi
	size_t get_offset(int x, int y)const;
	size_t get_row_offset()const;

	//drawing
	void draw_dot_rgb_raw(int x, int y, unsigned char r, unsigned char g, unsigned char b, bool safecheck = true);
	void draw_dot_rgb_raw(int x, int y, DCT_color_rgba_f color, bool safecheck = true);
	void draw_line_xyx_rgb_raw(int x_start, int y, int x_end_involved,unsigned char r, unsigned char g, unsigned char b, bool safecheck = true);//horizontal line.
	void draw_line_xyx_rgb_raw(int x_start, int y, int x_end_involved, DCT_color_rgba_f color, bool safecheck = true);//horizontal line.
	void draw_line_xyy_rgb_raw(int x, int y_start, int y_end_involved, unsigned char r, unsigned char g, unsigned char b, bool safecheck = true);//vertical line.
	void draw_line_xyy_rgb_raw(int x, int y_start, int y_end_involved, DCT_color_rgba_f color, bool safecheck = true);//vertical line.
	void draw_sq_rgb_raw(int x_start, int x_end_involved, int y_start, int y_end_involved, unsigned char r, unsigned char g, unsigned char b, bool safecheck = true);
	void draw_sq_rgb_raw(int x_start, int x_end_involved, int y_start, int y_end_involved, DCT_color_rgba_f color, bool safecheck = true);

	//saving to png
	enum class saving_error_info_enum
	{
		OK,

		Failed_to_open_file,
		Failed_to_create_PNG_write_struct,
		Failed_to_create_PNG_info_struct,
		Failed_to_set_jmp_for_PNG_lib
	};
	saving_error_info_enum save_to_file(const char* filename);



	//how to 
	void how_to_inside();

};

