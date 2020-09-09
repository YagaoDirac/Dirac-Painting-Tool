#include<utility>
#include<vector>

#include "../inc/DCT color.h"

//.h of this
#include "../inc/DCT image layer.h"



const size_t DCT_image_layer_uint8::safe_size(16384);




//for const member, init them as typename::typename():const member name1{init code}const member name2{init code}{function body here}
DCT_image_layer_uint8::DCT_image_layer_uint8() {};
DCT_image_layer_uint8::~DCT_image_layer_uint8() {};//not very sure whether this one has to be virtual.
//the following 4 ctors are private.
DCT_image_layer_uint8::DCT_image_layer_uint8(const DCT_image_layer_uint8&) { throw; };
DCT_image_layer_uint8::DCT_image_layer_uint8(DCT_image_layer_uint8&&) { throw; };
DCT_image_layer_uint8& DCT_image_layer_uint8::operator=(const DCT_image_layer_uint8&) { throw; return *this; };
DCT_image_layer_uint8& DCT_image_layer_uint8::operator=(DCT_image_layer_uint8&&) { throw; return *this; };


//init
DCT_image_layer_uint8::error_type DCT_image_layer_uint8::init(size_t in_width, size_t in_height)
{
	if (in_width > 4096 || in_height > 4096)
		return error_type::to_init_size_greater_than_4096__invoke_init_force;

	width = in_width;
	height = in_height;
	memory.resize(width*height);
	has_memory = true;
	return error_type::OK;
}
DCT_image_layer_uint8::error_type DCT_image_layer_uint8::__init_force(size_t in_width, size_t in_height)
{
	if (in_width > safe_size || in_height > safe_size)
		return error_type::bad_param;

	width = in_width;
	height = in_height;
	memory.resize(width*height);
	has_memory = true;
	return error_type::OK;
}
void DCT_image_layer_uint8::set_alpha_255()
{
	for (auto &pixil : memory)
		pixil.a = unsigned char(255);
}
void DCT_image_layer_uint8::__set_rand_rgb()
{
	for (auto &pixil : memory)
	{
		int random_number = rand();
		pixil.r = unsigned char(random_number & 0xff);
		pixil.g = unsigned char((random_number & 0xfff)>>4);
		pixil.r = unsigned char((random_number& 0x7fff)>>7);
	}
}


//gets sets
bool DCT_image_layer_uint8::get_has_memory() const { return has_memory; }

//utilities
inline void DCT_image_layer_uint8::__set_mem(size_t offset, unsigned char r, unsigned char g, unsigned char b)
{
	memory[offset].r = r;
	memory[offset].g = g;
	memory[offset].b = b;
}
inline int DCT_image_layer_uint8::clamp_coord(int in, int lo, int hi)
{
	return in < lo ? lo : (in < hi ? in : (hi - 1));
}
inline size_t DCT_image_layer_uint8::get_offset(int x, int y) const
{
	return (y*width + x);
}
inline size_t DCT_image_layer_uint8::get_row_offset() const
{
	return size_t(width * 4);
}



void DCT_image_layer_uint8::draw_dot_rgb_raw(int x, int y, unsigned char r, unsigned char g, unsigned char b, bool safecheck)
{
	if (safecheck)
	{
		if (x < 0 || x >= static_cast<int>(width) || y < 0 || y >= static_cast<int>(height))
			return;
	}

	__set_mem(get_offset(x, y), r, g, b);
	return;
}
void DCT_image_layer_uint8::draw_dot_rgb_raw(int x, int y, DCT_color_rgba_f color, bool safecheck)
{
	draw_dot_rgb_raw(x, y,
		static_cast<unsigned char>(color.getr() * 255),
		static_cast<unsigned char>(color.getg() * 255),
		static_cast<unsigned char>(color.getb() * 255), safecheck);
}
void DCT_image_layer_uint8::draw_line_xyx_rgb_raw(int x_start, int y, int x_end_involved, unsigned char r, unsigned char g, unsigned char b, bool safecheck)
{
	if (safecheck)
	{
		if (y < 0 || y >= static_cast<int>(height))return;
		if (x_start < 0 && x_end_involved < 0)return;
		if (x_start >= static_cast<int>(width) && x_end_involved >= static_cast<int>(width))return;

		x_start = clamp_coord(x_start, 0, width);
		x_end_involved = clamp_coord(x_end_involved, 0, width);

		if (x_start >= x_end_involved)std::swap(x_start, x_end_involved);
	}

	//drawing
	if (x_start == x_end_involved)
	{
		draw_dot_rgb_raw(x_start, y, r, g, b, false);
		return;
	}

	size_t offset = get_offset(x_start, y);
	size_t last_offset = offset + (x_end_involved - x_start);
	for (; offset <= last_offset; ++offset)
		//				^ Notice this is less ***equal***.
	{
		__set_mem(offset, r, g, b);
	}
}
void DCT_image_layer_uint8::draw_line_xyx_rgb_raw(int x_start, int y, int x_end_involved, DCT_color_rgba_f color, bool safecheck)
{
	draw_line_xyx_rgb_raw(x_start, y, x_end_involved,
		static_cast<unsigned char>(color.getr() * 255),
		static_cast<unsigned char>(color.getg() * 255),
		static_cast<unsigned char>(color.getb() * 255), safecheck);
}
void DCT_image_layer_uint8::draw_line_xyy_rgb_raw(int x, int y_start, int y_end_involved, unsigned char r, unsigned char g, unsigned char b, bool safecheck)
{
	if (safecheck)
	{
		if (x < 0 || x >= static_cast<int>(width))return;
		if (y_start < 0 && y_end_involved < 0)return;
		if (y_start >= static_cast<int>(height) && y_end_involved >= static_cast<int>(height))return;

		y_start = clamp_coord(y_start, 0, height);
		y_end_involved = clamp_coord(y_end_involved, 0, height);

		if (y_start >= y_end_involved)std::swap(y_start, y_end_involved);
	}

	//drawing
	if (y_start == y_end_involved)
	{
		draw_dot_rgb_raw(x, y_start, r, g, b, false);
		return;
	}

	size_t offset = get_offset(x, y_start);
	size_t last_offset = get_offset(x, y_end_involved);
	for (; offset <= last_offset; offset += width)
		//				^ Notice this is less ***equal***.
	{
		__set_mem(offset, r, g, b);
	}
}
void DCT_image_layer_uint8::draw_line_xyy_rgb_raw(int x, int y_start, int y_end_involved, DCT_color_rgba_f color, bool safecheck)
{
	draw_line_xyy_rgb_raw(x, y_start, y_end_involved,
		static_cast<unsigned char>(color.getr() * 255),
		static_cast<unsigned char>(color.getg() * 255),
		static_cast<unsigned char>(color.getb() * 255), safecheck);
}

void DCT_image_layer_uint8::draw_sq_rgb_raw(int x_start, int x_end_involved, int y_start, int y_end_involved, unsigned char r, unsigned char g, unsigned char b, bool safecheck)
{
	if (safecheck)
	{
		if (x_start < 0 && x_end_involved < 0)return;
		if (x_start >= static_cast<int>(width) && x_end_involved >= static_cast<int>(width))return;
		if (y_start < 0 && y_end_involved < 0)return;
		if (y_start >= static_cast<int>(height) && y_end_involved >= static_cast<int>(height))return;

		x_start = clamp_coord(x_start, 0, width);
		x_end_involved = clamp_coord(x_end_involved, 0, width);
		y_start = clamp_coord(y_start, 0, height);
		y_end_involved = clamp_coord(y_end_involved, 0, height);

		if (x_start >= x_end_involved)std::swap(x_start, x_end_involved);
		if (y_start >= y_end_involved)std::swap(y_start, y_end_involved);
	}

	//drawing
	if (x_start == x_end_involved)
	{
		if (y_start == y_end_involved)
		{// a dot
			__set_mem(get_offset(x_start, y_start), r, g, b);
		}
		else
		{//vertical
			draw_line_xyy_rgb_raw(x_start, y_start, y_end_involved, r, g, b, false);
		}
	}
	else
	{
		if (y_start == y_end_involved)
		{//horizontal
			draw_line_xyx_rgb_raw(x_start, y_start, x_end_involved, r, g, b, false);
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
					__set_mem(offset, r, g, b);
				}
			}
		}
	}
}
void DCT_image_layer_uint8::draw_sq_rgb_raw(int x_start, int x_end_involved, int y_start, int y_end_involved, DCT_color_rgba_f color, bool safecheck)
{
	draw_sq_rgb_raw(x_start, x_end_involved, y_start, y_end_involved,
		static_cast<unsigned char>(color.getr() * 255),
		static_cast<unsigned char>(color.getg() * 255),
		static_cast<unsigned char>(color.getb() * 255), safecheck);
}


//saving to png
DCT_image_layer_uint8::saving_error_info_enum DCT_image_layer_uint8::save_to_file(const char* filename)
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






void DCT_image_layer_uint8::how_to_inside()
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



