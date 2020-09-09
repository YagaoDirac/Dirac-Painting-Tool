#include<utility>
#include<vector>

//.h of this
#include "../include/DCT bmp layer memory.h"





//=================== uint8_x3 ====================
//only drawing functions are here now.


void DCT_bmp_layer_memory_uchar_x3::draw_dot(int x, int y, bool safecheck)
{
	if (safecheck)
	{
		if (x < 0 || x >= static_cast<int>(width) || y < 0 || y >= static_cast<int>(height))
			return;
	}

	__set_mem(get_offset(x, y), ForeGroundcolor);
	return;
}
void DCT_bmp_layer_memory_uchar_x3::draw_line_xyx(int x_start, int y, int x_end_involved, bool safecheck)
{
	if (safecheck)
	{
		if (y < 0 || y >= static_cast<int>(height))return;
		if (x_start < 0 && x_end_involved < 0)return;
		if (x_start >= static_cast<int>(width) && x_end_involved >= static_cast<int>(width))return;

		x_start = clamp_coord(x_start, 0, static_cast<int>(width));
		x_end_involved = clamp_coord(x_end_involved, 0, static_cast<int>(width));

		if (x_start >= x_end_involved)std::swap(x_start, x_end_involved);
	}

	//drawing
	if (x_start == x_end_involved)
	{
		draw_dot(x_start, y, false);
		return;
	}

	size_t offset = get_offset(x_start, y);
	size_t last_offset = offset + (x_end_involved - x_start);
	for (; offset <= last_offset; ++offset)
		//				^ Notice this is less ***equal***.
	{
		__set_mem(offset, ForeGroundcolor);
	}
}
void DCT_bmp_layer_memory_uchar_x3::draw_line_xyy(int x, int y_start, int y_end_involved, bool safecheck)
{
	if (safecheck)
	{
		if (x < 0 || x >= static_cast<int>(width))return;
		if (y_start < 0 && y_end_involved < 0)return;
		if (y_start >= static_cast<int>(height) && y_end_involved >= static_cast<int>(height))return;

		y_start = clamp_coord(y_start, 0, static_cast<int>(height));
		y_end_involved = clamp_coord(y_end_involved, 0, static_cast<int>(height));

		if (y_start >= y_end_involved)std::swap(y_start, y_end_involved);
	}

	//drawing
	if (y_start == y_end_involved)
	{
		draw_dot(x, y_start, false);
		return;
	}

	size_t offset = get_offset(x, y_start);
	size_t last_offset = get_offset(x, y_end_involved);
	for (; offset <= last_offset; offset += width)
		//				^ Notice this is less ***equal***.
	{
		__set_mem(offset, ForeGroundcolor);
	}
}
void DCT_bmp_layer_memory_uchar_x3::draw_sq(int x_start, int x_end_involved, int y_start, int y_end_involved, bool safecheck)
{
	if (safecheck)
	{
		if (x_start < 0 && x_end_involved < 0)return;
		if (x_start >= static_cast<int>(width) && x_end_involved >= static_cast<int>(width))return;
		if (y_start < 0 && y_end_involved < 0)return;
		if (y_start >= static_cast<int>(height) && y_end_involved >= static_cast<int>(height))return;

		x_start = clamp_coord(x_start, 0, static_cast<int>(width));
		x_end_involved = clamp_coord(x_end_involved, 0, static_cast<int>(width));
		y_start = clamp_coord(y_start, 0, static_cast<int>(height));
		y_end_involved = clamp_coord(y_end_involved, 0, static_cast<int>(height));

		if (x_start >= x_end_involved)std::swap(x_start, x_end_involved);
		if (y_start >= y_end_involved)std::swap(y_start, y_end_involved);
	}

	//drawing
	if (x_start == x_end_involved)
	{
		if (y_start == y_end_involved)
		{// a dot
			__set_mem(get_offset(x_start, y_start), ForeGroundcolor);
		}
		else
		{//vertical
			draw_line_xyy(x_start, y_start, y_end_involved, false);
		}
	}
	else
	{
		if (y_start == y_end_involved)
		{//horizontal
			draw_line_xyx(x_start, y_start, x_end_involved, false);
		}
		else
		{
			size_t offset, last_x_for_row;
			for (int y = y_start; y <= y_end_involved; ++y)
				//					       ^ Notice this is less ***equal***.
			{
				offset = get_offset(x_start, y);
				last_x_for_row = offset + (x_end_involved - x_start);
				for (; offset <= last_x_for_row; ++offset)
					//         ^ Notice this is less ***equal***.
				{
					__set_mem(offset, ForeGroundcolor);
				}
			}
		}
	}
}


//=================== uint8_x3 ====================

//=================== uint8_x4 ====================



//utilities
//inline void DCT_bmp_layer_memory_uchar_x4::__set_mem(size_t offset, unsigned char r, unsigned char g, unsigned char b)
//{memory[offset].r = r;memory[offset].g = g;memory[offset].b = b;}
//inline int DCT_bmp_layer_memory_uchar_x4::clamp_coord(int in, int lo, int hi)
//{return in < lo ? lo : (in < hi ? in : (hi - 1));}



void DCT_bmp_layer_memory_uchar_x4::draw_dot(int x, int y, bool safecheck)
{
	if (safecheck)
	{
		if (x < 0 || x >= static_cast<int>(width) || y < 0 || y >= static_cast<int>(height))
			return;
	}

	__pixil_overlapped_by(get_offset(x, y), ForeGroundcolor);
	return;
}

void DCT_bmp_layer_memory_uchar_x4::draw_line_xyx(int x_start, int y, int x_end_involved, bool safecheck)
{
	if (safecheck)
	{
		if (y < 0 || y >= static_cast<int>(height))return;
		if (x_start < 0 && x_end_involved < 0)return;
		if (x_start >= static_cast<int>(width) && x_end_involved >= static_cast<int>(width))return;

		x_start = clamp_coord(x_start, 0, static_cast<int>(width));
		x_end_involved = clamp_coord(x_end_involved, 0, static_cast<int>(width));

		if (x_start >= x_end_involved)std::swap(x_start, x_end_involved);
	}

	//drawing
	if (x_start == x_end_involved)
	{
		draw_dot(x_start, y, false);
		return;
	}

	size_t offset = get_offset(x_start, y);
	size_t last_offset = offset + (x_end_involved - x_start);
	for (; offset <= last_offset; ++offset)
		//				^ Notice this is less ***equal***.
	{
		__pixil_overlapped_by(offset, ForeGroundcolor);
	}
}
void DCT_bmp_layer_memory_uchar_x4::draw_line_xyy(int x, int y_start, int y_end_involved, bool safecheck)
{
	if (safecheck)
	{
		if (x < 0 || x >= static_cast<int>(width))return;
		if (y_start < 0 && y_end_involved < 0)return;
		if (y_start >= static_cast<int>(height) && y_end_involved >= static_cast<int>(height))return;

		y_start = clamp_coord(y_start, 0, static_cast<int>(height));
		y_end_involved = clamp_coord(y_end_involved, 0, static_cast<int>(height));

		if (y_start >= y_end_involved)std::swap(y_start, y_end_involved);
	}

	//drawing
	if (y_start == y_end_involved)
	{
		draw_dot(x, y_start, false);
		return;
	}

	size_t offset = get_offset(x, y_start);
	size_t last_offset = get_offset(x, y_end_involved);
	for (; offset <= last_offset; offset += width)
		//				^ Notice this is less ***equal***.
	{
		__pixil_overlapped_by(offset, ForeGroundcolor);
	}
}
void DCT_bmp_layer_memory_uchar_x4::draw_sq(int x_start, int x_end_involved, int y_start, int y_end_involved, bool safecheck)
{
	if (safecheck)
	{
		if (x_start < 0 && x_end_involved < 0)return;
		if (x_start >= static_cast<int>(width) && x_end_involved >= static_cast<int>(width))return;
		if (y_start < 0 && y_end_involved < 0)return;
		if (y_start >= static_cast<int>(height) && y_end_involved >= static_cast<int>(height))return;

		x_start = clamp_coord(x_start, 0, static_cast<int>(width));
		x_end_involved = clamp_coord(x_end_involved, 0, static_cast<int>(width));
		y_start = clamp_coord(y_start, 0, static_cast<int>(height));
		y_end_involved = clamp_coord(y_end_involved, 0, static_cast<int>(height));

		if (x_start >= x_end_involved)std::swap(x_start, x_end_involved);
		if (y_start >= y_end_involved)std::swap(y_start, y_end_involved);
	}

	//drawing
	if (x_start == x_end_involved)
	{
		if (y_start == y_end_involved)
		{// a dot
			__pixil_overlapped_by(get_offset(x_start, y_start), ForeGroundcolor);
		}
		else
		{//vertical
			draw_line_xyy(x_start, y_start, y_end_involved, false);
		}
	}
	else
	{
		if (y_start == y_end_involved)
		{//horizontal
			draw_line_xyx(x_start, y_start, x_end_involved, false);
		}
		else
		{
			size_t offset, last_x_for_row;
			for (int y = y_start; y <= y_end_involved; ++y)
				//					       ^ Notice this is less ***equal***.
			{
				offset = get_offset(x_start, y);
				last_x_for_row = offset + (x_end_involved - x_start);
				for (; offset <= last_x_for_row; ++offset)
					//         ^ Notice this is less ***equal***.
				{
					__pixil_overlapped_by(offset, ForeGroundcolor);
				}
			}
		}
	}
}
 

//saving to png 暂时不提供
/*
DCT_bmp_layer_component_uchar_x4::saving_error_info_enum DCT_bmp_layer_component_uchar_x4::save_to_file(const char* filename)
{
	FILE * fp;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	//size_t x, y;
	png_byte ** row_pointers = NULL;
	int pixel_size(4);
	int depth(8);

	//set up
	auto result = fopen_s(&fp, filename, "wb");
	if (!fp) {
		return saving_error_info_enum::Failed_to_open_file;
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		fclose(fp);
		return saving_error_info_enum::Failed_to_create_PNG_write_struct;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return saving_error_info_enum::Failed_to_create_PNG_info_struct;
	}

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_write_struct(&png_ptr, &info_ptr);
		fclose(fp);
		return saving_error_info_enum::Failed_to_set_jmp_for_PNG_lib;
	}

	png_set_IHDR(png_ptr, info_ptr, width, height, depth,
		PNG_COLOR_TYPE_RGBA, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

	png_init_io(png_ptr, fp);

	//prepare the row_pointers
	row_pointers = new png_byte*[height];
	for (size_t i = 0; i < height; ++i)
	{
		row_pointers[i] = reinterpret_cast<png_byte*>(&(memory[i*width]));
	}
	png_set_rows(png_ptr, info_ptr, row_pointers);
	//write to file.
	png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	//clear
	delete[]row_pointers;
	png_destroy_write_struct(&png_ptr, &info_ptr);
	fclose(fp);

	//return
	return saving_error_info_enum::OK;
}
*/

void DCT_bmp_layer_memory_uchar_x4::how_to_inside()
{
	/*
	Step 1 ctor.
	DCT_image_layer_uint8 il;

	Step 2 init()
	il.init(100, 100);
	If the size is bigger than 4096, init_force() instead.

	Step 3 set alpha to 255()
	il.set_alpha_255();
	This functions simply assign all the alpha place to 255( in normalized way, it's equavalent to 1.f )

	Step 4, drawing.
	All the draw_* functions are prepared for this. Or directly set the values.
	All values are stored in DCT_image_layer_uint8::memory. The DCT_image_layer_uint8::get_offset(...) helps with calculating the offset, which works with the __set_mem(...).
	But if you prefer some fast test, do it like
	il.__set_rand_rgb();

	Step 5, saving to file.
	il.save_to_file("image file name.png");

	A typical test code could look like:
	int main(int argc, char* argv[])
	{
		DCT_image_layer_uint8 il;
		il.init(100, 100);
		il.set_alpha_255();
		il.__set_rand_rgb();//or draw_*()functions, or anyother way to modify the image memory.
		il.save_to_file("random color.png");

		return 0;
	}
	*/
}



