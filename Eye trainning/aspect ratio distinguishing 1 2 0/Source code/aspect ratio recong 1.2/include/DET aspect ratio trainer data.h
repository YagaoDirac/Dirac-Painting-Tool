//# Notice:
//# For draw functions in pillow, the start point and end point are envolved in to a draw function.
//# When draw a line for (1, 1) + (1, 3), 3 pixils are modified.
//# When draw a rectangle with param of(1, 1), (2, 2), 4 pixils are modified.
//# Which are not the same as discribed in its docs.
//# Another thing, for a frame style, distance should be measured like
//#  | . . . . |
//#  ^1 2 3 4 5^  aligns to the mid of lines.
//# But for a filled shape, it should look like
//#  | | | | | |
//# ^1 2 3 4 5 6^  aligns to the border.
//# https://github.com/YagaoDirac/Dirac-s-Painting-Tool for more info and discord.

#pragma once

//standard
//#include <algorithm>
//#include <iostream>
//os
//#include <windows.h>

#include <vector>
//template<class _Ty>
//class std::allocator;
//template<class _Ty, class _Alloc = std::allocator<_Ty> >
//class std::vector;

// Dirac cpp tool
#include "DCT util.h"
#include "DCT color.h"
//#include "DCT image layer.h"用新的
#include "DCT bmp layer memory.h"
#include "DCT vector2.h"


//This .h file.
//DCT_color_rgba_f DET_random_dark_gray();


class DET_aspect_ratio_trainer_data
{
public:
	std::vector<float> answer_options;
	float answer;

	DCT_color_rgb_uchar background_color;
	//float delay_duration;// in seconds.  暂时不用
	unsigned int noise_mode;//暂时没用 1~4 for light ~heavy noise, other value for nothing.
	//DCT_vector2_i screen_resolution;暂时不用

	enum class filling_mode_enum
	{
		random = 0,
		frame,
		normal_filling,
	};
	filling_mode_enum filling_mode;
	/*enum class auxiliary_line_mode_enum 感觉这个功能没什么用
	{
		random = 0,
		none,
		x_mark,
		circle
	};
	auxiliary_line_mode_enum auxiliary_line_mode;*/
	enum class train_mode_enum
	{
		question_only,
		question_then_auxiliary_lines
	};
	train_mode_enum train_mode;
	std::vector<unsigned int>width_options;

	//a nested class 
	class DET_auxiliary_line_data_nestedClass
	{
	public:
		int question_top;
		int question_width;
		filling_mode_enum mode;

		struct auxiliary_line_info_struct
		{
			float ratio;
			unsigned int half_len;
			int y;
		};
		std::vector<auxiliary_line_info_struct> data;

		void push_raw(float in_ratio, unsigned int in_half_len);
		void update(int in_question_top, int in_question_width, filling_mode_enum in_mode);
		void clear();

		//how to
		void how_to_inside();
	};
	DET_auxiliary_line_data_nestedClass auxiliary_line_data;

public:
	//inner use. Do not modify these. To customize the tool, all you need to concern are upon this line.
	struct question_shape_struct
	{
		int width;
		int height;
		int top;
		int bottom;
		int left;
		int right;
		float move_up_ratio;
		int left_tail_bottom;
		int right_tail_bottom;
	};
	question_shape_struct question_shape;
	DCT_color_rgb_uchar question_shade_color;
	
	//DCT_image_layer_uint8 image;//用新的
	//DCT_bmp_layer_memory_uchar_x4 image;//4通道的版本不适合现在的ui的需要。用3通道的

	DCT_bmp_layer_memory_uchar_x3 image;

public:
	//ctor
	DET_aspect_ratio_trainer_data();

	DCT_bmp_layer::error_info_enum set_size(size_t in_width, size_t in_height)
	{
		//screen_resolution = { in_width, in_height };
		
		DCT_bmp_layer::error_info_enum result = image.init(in_width, in_height);
		if (DCT_bmp_layer::error_info_enum::OK != result)
			return result;

		//image.set_alpha_255();
		schedule = schedule_enum::prepared;
		return DCT_bmp_layer::error_info_enum::OK;
	};

	

protected:
	//inner tool
	void update_actual_filling_mode();
	/*{
		if (filling_mode_enum::random == filling_mode)
			if (DCT_random01() < 0.25)
			{
				filling_mode = filling_mode_enum::frame;
				return;
			}
			else
			{
				filling_mode = filling_mode_enum::normal_filling;
				return;
			}
	}*/
	//void update_actual_auxiliary_line_mode()
	//{
	//	if (auxiliary_line_mode_enum::random == auxiliary_line_mode)
	//	{
	//		float rand_result = DCT_random01();
	//		if (rand_result < 0.5)
	//		{
	//			auxiliary_line_mode = auxiliary_line_mode_enum::none;
	//			return;
	//		}
	//		else if (rand_result < 0.75)
	//		{
	//			auxiliary_line_mode = auxiliary_line_mode_enum::circle;
	//			return;
	//		}
	//		else
	//		{
	//			auxiliary_line_mode = auxiliary_line_mode_enum::x_mark;
	//			return;
	//		}
	//	}
	//}

public:
	//暂时没用这两个。都是设置分辨率的小工具。好像确实没什么用
	//bool update_image_zise(unsigned int hint_w, unsigned int hint_h)
	//{
	//	//check the input param first.
	//	if (hint_w < 500 or hint_w>4000 or hint_h < 500 or hint_h>4000)
	//	{
	//		return false;
	//	}
	//	//shrink the size a little bit to fit the image brower in win7.
	//	image.width = hint_w - 40;//to do 
	//	image.height = hint_h - 200;
	//	return true;
	//};
	//void update_image_zise(std::string hint)
	//{
	//	if (hint == "2k" or hint == "2K")
	//	{
	//		update_image_zise(2560, 1440);
	//		return;
	//	}
	//	if (hint == "4k" or hint == "4K")
	//	{
	//		update_image_zise(3840, 2160);
	//		return;
	//	}
	//	//fallback to 1k
	//	update_image_zise(1920, 1080);
	//}

public:
	enum class error_type
	{
		OK,
		Call_set_size_first,
		Image_memory_error,
		Call_stage1_first
	};
	enum class schedule_enum
	{
		Not_initialized_yet,
		prepared,
		between_stage_1_and_2,
		end
	};
	schedule_enum schedule;

	error_type generate_to_image_data_member_stage_1();
	error_type generate_to_image_data_member_stage_2();


public:
	void how_to_inside();

};
