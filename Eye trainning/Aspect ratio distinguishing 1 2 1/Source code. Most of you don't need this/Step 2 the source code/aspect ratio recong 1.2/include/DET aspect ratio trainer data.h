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
#include <sstream>


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
	
	unsigned int noise_mode;//暂时没用 1~4 for light ~heavy noise, other value for nothing.
	

	enum class filling_mode_enum
	{
		random = 0,
		frame,
		normal_filling,
	};
	filling_mode_enum filling_mode;
	
	enum class train_mode_enum
	{
		question_only,
		question_then_auxiliary_lines
	};
	train_mode_enum train_mode;
	

	//a nested class 
	class DET_auxiliary_line_data_nested_class
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
	DET_auxiliary_line_data_nested_class auxiliary_line_data;

	//answer gen
	class __difficulty_and_answer_generator_nested_class
	{
	public:
		std::vector<float> possible_bound_values;//const
		std::vector<float> possible_division_values;//const
		int division_index, lower_bound_index, upper_bound_index;
		std::vector<float> answer_options;//non const

		//ctor
		__difficulty_and_answer_generator_nested_class()
		{
			possible_division_values = { 0.1f, 0.05f, 0.03f, 0.025f, 0.02f, 0.015f, 0.01f };
			division_index = 0;
			possible_bound_values = { 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f, 1.1f, 1.2f, 1.3f, 1.4f, 1.5f, 1.6f, 1.7f, 1.8f, 1.9f, 2.f};
			lower_bound_index = 3;
			upper_bound_index = 10;
		}
		//inner util
		void update()
		{
			for (float temp = get_lower(); temp < get_upper() + get_division() * 0.5f; temp += get_division())
			{
				answer_options.push_back(temp);
			}
		}

		//getters
		inline float get_upper() { return possible_bound_values[upper_bound_index]; };
		inline float get_lower() { return possible_bound_values[lower_bound_index]; };
		inline float get_division() { return possible_division_values[division_index]; };
		float get_random_answer()
		{
			return answer_options[DCT_randomi(0, answer_options.size() - 1)];
		}

		//On click
		void On_division_change()
		{
			++division_index;
			division_index %= possible_division_values.size() ;
			update();
		}

		void On_upper_change()
		{
			if (possible_bound_values.size() - 2 == lower_bound_index)return;

			int temp = (upper_bound_index - lower_bound_index);
			//temp += 1;
			temp %= possible_bound_values.size() - lower_bound_index - 1;
			upper_bound_index = temp + lower_bound_index + 1;

			update();
		}

		void On_lower_change()
		{
			if (1 == upper_bound_index)return;

			lower_bound_index += 1;
			lower_bound_index %= upper_bound_index;

			update();
		}
	};
	__difficulty_and_answer_generator_nested_class answer_manager;

	class __width_generator
	{
	public:
		std::vector<int> possible_width_values;//const
		int min_index, max_index;
		
		//ctor
		__width_generator()
		{
			possible_width_values = { 200, 225, 250, 275, 300, 330, 360, 400, 450, 500 };
			min_index = 0;
			max_index = possible_width_values.size() - 1;
		}

		//getters
		inline int get_min_width() { return possible_width_values[min_index]; };
		inline int get_max_width() { return possible_width_values[max_index]; };
		int get_random_width()
		{
			return possible_width_values[DCT_randomi(min_index, max_index)];
		}

		//On click
		void On_max_change()
		{
			if (possible_width_values.size() - 2 == min_index)return;

			int temp = (max_index - min_index);
			//temp += 1;
			temp %= possible_width_values.size() - min_index - 1;
			max_index = temp + min_index + 1;
		}

		void On_min_change()
		{
			if (1 == max_index)return;

			min_index += 1;
			min_index %= max_index;
		}
	};
	__width_generator width_manager;

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
	
	DCT_bmp_layer_memory_uchar_x3 image;

public:
	//ctor
	DET_aspect_ratio_trainer_data();

	DCT_bmp_layer::error_info_enum set_size(size_t in_width, size_t in_height)
	{
		DCT_bmp_layer::error_info_enum result = image.init(in_width, in_height);
		if (DCT_bmp_layer::error_info_enum::OK != result)
			return result;

		schedule = schedule_enum::prepared;
		return DCT_bmp_layer::error_info_enum::OK;
	};

	

protected:
	//inner tool
	//void update_actual_filling_mode()//好像暂时没用了。 
	//{
	//	if (filling_mode_enum::random == filling_mode)
	//		filling_mode = get_random_filling_mode;
	//		return;
	//};

	filling_mode_enum get_random_filling_mode()
	{
		if (DCT_random01() < 0.25f)
			return filling_mode_enum::frame;
		else
			return filling_mode_enum::normal_filling;
	}
public:
	

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

	// interface for any ui
	void change_lower_bound_from_ui()
	{
		answer_manager.On_lower_change();
		//if (schedule_enum::between_stage_1_and_2 == schedule)
		//	schedule = schedule_enum::prepared;
	}
	void change_upper_bound_from_ui()
	{answer_manager.On_upper_change();};
	void change_division_value_from_ui()
	{		answer_manager.On_division_change();		}
	std::string get_lower_label_for_ui()
	{
		std::stringstream ss;
		ss << ">= ";
		ss.precision(2);
		ss<<answer_manager.get_lower();
		return std::string(ss.str());
	}
	std::string get_upper_label_for_ui()
	{
		std::stringstream ss;
		ss << ">= ";
		ss.precision(2);
		ss << answer_manager.get_upper();
		return std::string(ss.str());
	}
	std::string get_division_label_for_ui()
	{
		std::stringstream ss;
		ss << "div= ";
		ss.precision(3);
		ss << answer_manager.get_division();
		return std::string(ss.str());
	}

	void change_min_width_from_ui()
	{		width_manager.On_min_change();}
	void change_max_width_from_ui()
	{		width_manager.On_max_change();}
	std::string get_min_width_label_for_ui()
	{	return std::string("min width = ") + std::to_string(width_manager.get_min_width());	};
	std::string get_max_width_label_for_ui()
	{return std::string("max width = ") + std::to_string(width_manager.get_max_width());	};




public:
	void how_to_inside();

};
