//standard
#include <algorithm>
#include <iostream>
//stl
#include<vector>
//os
#include <windows.h>

//other custom header
//#include "../inc/DCT color.h"好像可以不用。有新的了
#include "../include/DCT util.h"

//.h of this
#include "../include/DET aspect ratio trainer data.h"


//DCT_color_rgba_f DET_random_dark_gray()
//{
//	float temp = DCT_random01();
//	temp *= temp;
//	temp *= 1.15;
//	temp -= 0.15;
//	temp = std::max(0.f, temp);
//	int temp_i = 0x33 * temp;
//	return DCT_color_rgba_f(temp_i, temp_i, temp_i);
//}



//This .h file.
//DCT_color_rgba_f DET_random_dark_gray();


//class DET_aspect_ratio_trainer_data


//class DET_auxiliary_line_data_nestedClass  (DET_aspect_ratio_trainer_data::)
//step 1
void DET_aspect_ratio_trainer_data::DET_auxiliary_line_data_nestedClass::push_raw(float in_ratio, unsigned int in_half_len)
{
	data.push_back(auxiliary_line_info_struct{ in_ratio,in_half_len,-1 });
}
//step 2
void DET_aspect_ratio_trainer_data::DET_auxiliary_line_data_nestedClass::update(int in_question_top, int in_question_width, filling_mode_enum in_mode)
{
	question_top = in_question_top;
	question_width = in_question_width;
	mode = in_mode;

	for (auto& element : data)
	{
		element.y = question_top + static_cast<int>(question_width * (element.ratio));
		if (filling_mode_enum::normal_filling == mode)
			element.y -= 1;
	}
}
//step extra if you would like to reuse this.
void DET_aspect_ratio_trainer_data::DET_auxiliary_line_data_nestedClass::clear() { data.clear(); };

//how to 
void DET_aspect_ratio_trainer_data::DET_auxiliary_line_data_nestedClass::how_to_inside()
{
	return;
	/*
	***STEP 1*** Invoke push_raw() first.
	When all the raw data pushed,
	***STEP 2*** Invoke update().
	***STEP 3*** Invoke clear() before reusing. Or do this before step 1.
	*/
}
	


// ================    DET_aspect_ratio_trainer_data::DET_auxiliary_line_data_nestedClass    ==================
	
// ================    DET_aspect_ratio_trainer_data    ==================


//ctor
DET_aspect_ratio_trainer_data::DET_aspect_ratio_trainer_data()
{
	filling_mode = filling_mode_enum::random;
	train_mode = train_mode_enum::question_then_auxiliary_lines;

	//delay_duration = 1.f;// in seconds.

	noise_mode = 1;

	background_color = DCT_color_rgb_uchar(127, 127, 127);

	width_options = { 200, 225, 250, 275, 300, 330, 360, 400, 450, 500 };

	schedule = schedule_enum::Not_initialized_yet;

	

	////auxiliary line style.放到第一阶段里面去了。
	//{
	//	for (float r = 0.2f; r < 1.21f; r += 0.2f)
	//		auxiliary_line_data.push_raw(r, 25);
	//	for (float r = 0.3f; r < 1.21f; r += 0.2f)
	//		auxiliary_line_data.push_raw(r, 15);
	//	for (float r = 0.25f; r < 1.21f; r += 0.1f)
	//		auxiliary_line_data.push_raw(r, 5);
	//}

	return;
}


//inner tool
void DET_aspect_ratio_trainer_data::update_actual_filling_mode()
{
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
}
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



DET_aspect_ratio_trainer_data::error_type DET_aspect_ratio_trainer_data::generate_to_image_data_member_stage_1()
{
	if (schedule_enum::Not_initialized_yet == schedule) return error_type::Call_set_size_first;
	if (image.memory.empty()) return error_type::Image_memory_error;

	//  Difficulty here  !!!!!!!!!!!!!!!!!!!!!!!!!!!

	//for (float r = lower boundary here; r<= a little bit bigger than upper boundary here; r +=division value)
	//such as: for (float r = 0.6f; r <= .901f; r += 0.025f)

	{
		float lower_boundary = 0.6f;
		float upper_boundary = 0.9f;
		float division_value = 0.025f;
		answer_options.clear();
		for (float r = lower_boundary; r <= upper_boundary + 0.001f; r += division_value)
			answer_options.push_back(r);
	}
	
	////////////////////////////////////////////////////////////////////
	//          不清楚功能的话，只改上面的就可以了。下面的不用管          //
	////////////////////////////////////////////////////////////////////

	answer = answer_options[DCT_randomi(0, answer_options.size() - 1)];




	//randomly choose width and move up ratio
	//shape_data.width;
	question_shape.width = width_options[DCT_randomi(0, width_options.size() - 1)];

	//shape_data.move_up_ratio;
	{
		float temp = DCT_random01();
		temp = temp * 0.4f - 0.15f;
		temp = max(temp, 0);//{}curly brace to force automatic type inference work.
		question_shape.move_up_ratio = DCT_random01()*temp;
	}

	//control the core data here for debug purpose.
	// HERE

	// END

	update_actual_filling_mode();

	//calculate other data
	question_shape.height = static_cast<int>(question_shape.width * answer);
	question_shape.top = static_cast<int>((image.height - question_shape.height) / 2.f * (1.f - question_shape.move_up_ratio));
	question_shape.bottom = question_shape.top + question_shape.height;
	question_shape.left = (image.width - question_shape.width) / 2;
	question_shape.right = question_shape.left + question_shape.width;
	if (filling_mode_enum::normal_filling == filling_mode)
	{
		question_shape.bottom -= 1;
		question_shape.right -= 1;
	}
	//prepare data for tails.
	{
		int tail_space_length = image.height - question_shape.bottom;
		float avg_tail_length = min(tail_space_length*0.5f, float(question_shape.height));
		float tail_length_difference = (200.f + question_shape.height * 0.1f) / 2.f;
		float temp = DCT_random01();
		temp = temp * 2 - 1;
		int left_tail_length = static_cast<int>(avg_tail_length + 100 * temp);
		int right_tail_length = static_cast<int>(avg_tail_length - 100 * temp);
		//safety check
		question_shape.left_tail_bottom = max(0, min(tail_space_length, left_tail_length)) + question_shape.bottom;
		question_shape.right_tail_bottom = max(0, min(tail_space_length, left_tail_length)) + question_shape.bottom;
	}

	//prepare data for the auxiliary lines.



	//auxiliary line style.
	auxiliary_line_data.clear();//In case this is not the first time to run.
	{
		for (float r = 0.2f; r < 1.21f; r += 0.2f)
			auxiliary_line_data.push_raw(r, 25);
		for (float r = 0.3f; r < 1.21f; r += 0.2f)
			auxiliary_line_data.push_raw(r, 15);
		for (float r = 0.25f; r < 1.21f; r += 0.1f)
			auxiliary_line_data.push_raw(r, 5);
	}
	auxiliary_line_data.update(question_shape.top, question_shape.width, filling_mode);





	std::vector<DCT_color_rgb_uchar> color_options = {
			background_color - DCT_color_rgb_uchar::make_gray_f(0.4f),//dark dark dark
			background_color + DCT_color_rgb_uchar::make_gray_f(0.4f),//bright bright bright 
			background_color - DCT_color_rgb_uchar::make_gray_f(0.2f),//dark dark
			background_color + DCT_color_rgb_uchar::make_gray_f(0.2f),//bright bright
			background_color - DCT_color_rgb_uchar::make_gray_f(0.12f),//dark
			background_color - DCT_color_rgb_uchar::make_gray_f(0.12f) };//dark 

	//DCT_color_rgba_f temp;
	if (filling_mode_enum::normal_filling == filling_mode)
	{
		question_shade_color = color_options[DCT_randomi(0, color_options.size() - 1)];
	}
	else if (filling_mode_enum::frame == filling_mode)
	{
		std::vector< DCT_color_rgb_uchar> smaller_vec(color_options.begin(), color_options.begin() + 4);
		question_shade_color = smaller_vec[DCT_randomi(0, smaller_vec.size() - 1)];
	}


	//drawing
	
	//clearing the image.
	image.set_color(background_color);
	image.draw_sq(0, image.width - 1, 0, image.height - 1);

	//drawing the question
	{
		image.set_color(question_shade_color);
		switch (filling_mode)
		{
		case DET_aspect_ratio_trainer_data::filling_mode_enum::frame:
			//sides.
			image.draw_line_xyy(question_shape.left, question_shape.top, question_shape.left_tail_bottom);
			image.draw_line_xyy(question_shape.right, question_shape.top, question_shape.right_tail_bottom);
			//horizontal
			image.draw_line_xyx(question_shape.left + 1, question_shape.top, question_shape.right - 1);
			image.draw_line_xyx(question_shape.left + 1, question_shape.bottom, question_shape.right - 1);
			break;

		case DET_aspect_ratio_trainer_data::filling_mode_enum::normal_filling:
			//body
			image.draw_sq(question_shape.left, question_shape.right, question_shape.top, question_shape.bottom);
			//tails
			image.draw_line_xyy(question_shape.left, question_shape.bottom + 1, question_shape.left_tail_bottom);
			image.draw_line_xyy(question_shape.right, question_shape.bottom + 1, question_shape.right_tail_bottom);
			break;
		}
	}//drawing the question



	//scattering noise rects
	//if noise_mode in[1, 2, 3, 4]: to do 
	{
		size_t noise_rect_count = DCT_randomi(static_cast<int>(question_shape.width*0.03f), static_cast<int>(question_shape.width*0.07f));
		DCT_vector2_i coord(DCT_random01() * 10000, DCT_random01() * 10000);
		DCT_vector2_i coord_random_stride(DCT_random01() * 3457891, DCT_random01() * 5676782);
		DCT_vector2_i mod(question_shape.width * 3 / 2, question_shape.height * 3 / 2);

		std::vector<DCT_color_rgb_uchar> color_options_noise =
		{
			background_color + DCT_color_rgb_uchar::make_gray(25),
			background_color + DCT_color_rgb_uchar::make_gray(20),
			background_color + DCT_color_rgb_uchar::make_gray(15),

			background_color - DCT_color_rgb_uchar::make_gray(30),
			background_color - DCT_color_rgb_uchar::make_gray(25),
			background_color - DCT_color_rgb_uchar::make_gray(20),
			background_color - DCT_color_rgb_uchar::make_gray(15)
		};

		DCT_vector2_i mid_point_of_shape{
			(question_shape.left + question_shape.right) / 2,
			(question_shape.top + question_shape.bottom) / 2 };

		/*#adjusting
		if 2 == noise_mode:
			count = random.randint(100, 150)
		if 3 == noise_mode :
			count = random.randint(200, 275)
			color_options.append(background_color + grey_index()*(-30))
		if 4 == noise_mode :
			count = random.randint(300, 400)
			color_options.append(background_color + grey_index() * 30,
				background_color + grey_index()*(-30),
				background_color + grey_index()*(-35),
				background_color + grey_index()*(-40))*/

		for (size_t i = 0; i < noise_rect_count; ++i)
		{
			//size
			int delta_of_top_left = i % 4 + 4;
			int delta_of_bottom_right = (i / 4) % 4 + 4;
			//position
			DCT_vector2_i mid(coord.x % mod.x, coord.y % mod.y);
			if (i % 2)
				mid.x = mid_point_of_shape.x + mid.x;
			else
				mid.x = mid_point_of_shape.x - mid.x;

			if ((i / 2) % 2)
				mid.y = mid_point_of_shape.y + mid.y;
			else
				mid.y = mid_point_of_shape.y - mid.y;

			//final coordinates
			{
				DCT_vector2_i top_left(mid.x - delta_of_top_left, mid.y - delta_of_top_left);
				DCT_vector2_i bottom_right(mid.x + delta_of_bottom_right, mid.y + delta_of_bottom_right);

				DCT_color_rgb_uchar noice_color = color_options_noise[i % (color_options_noise.size())];
				//draw call
				image.set_color(noice_color);
				image.draw_sq(top_left.x, bottom_right.x, top_left.y, bottom_right.y);

				//late update for loop
				coord = { mid.x + coord_random_stride.x,mid.y + coord_random_stride.y };
				mod = { mod.x + 3, static_cast<int>(mod.y + 3 * (9.f / 16.f)) };
			}


		}//for (noise_rect_count)

	}//scattering noise rects

	//save image (now the image contains only the question)
	//image.save_to_file("puzzle.png");原来是保存成png，现在是放到ui上。


	//update the schedule
	schedule = schedule_enum::between_stage_1_and_2;
	return error_type::OK;
}



DET_aspect_ratio_trainer_data::error_type DET_aspect_ratio_trainer_data::generate_to_image_data_member_stage_2()
{
	if (schedule_enum::between_stage_1_and_2 != schedule)return error_type::Call_stage1_first;

	//defer for few seconds and then print the answer in console.So that let user to train with only console window and image browser window.
	//Sleep(static_cast<DWORD>(delay_duration * 1000));//现在是直接输出

	int display_answer;
	{
		float temp;
		if (filling_mode_enum::normal_filling == filling_mode)
			temp = (static_cast<float>(question_shape.height + 1)) / (question_shape.width + 1);
		else
			temp = (static_cast<float>(question_shape.height)) / (question_shape.width);
		display_answer = static_cast<int>(floor(temp * 1000) / 1000);//round to 3 decimals.
	}
	std::cout << "answer: " << display_answer << "   width: " << question_shape.width << "   height: " << static_cast<int>(question_shape.height) << std::endl;
	/*	if (0) ://当时py里面的调试用的
			print('--------------------')
			print('debug info:')
			print('--------------------')
			print("width" + str(width_i))
			print("height" + str(height_i))
			print("move_up_ratio" + str(move_up_ratio))

			print("top" + str(top))
			print("bottom" + str(bottom))
			print("left" + str(left))
			print("right" + str(right))
			print("left_tail_length" + str(left_tail_length))
			print("right_tail_length" + str(right_tail_length))
			print('--------------------')*/

			//if only questions needed, the function ends here.
	//if (train_mode_enum::question_only == train_mode)return;这是以前的了。可以考虑删了
	//if the function doesn't return at this point, the following is that, waiting user to press any key, them save another png file which contains reference lines. This feature is named train mode in this tool.

	//system("pause");以前的代码了。

	//draw the reference lines
	{
		DCT_color_rgb_uchar auxiliary_line_color = question_shade_color;
		
		image.set_color(auxiliary_line_color);
		for (auto element : auxiliary_line_data.data)
		{
			image.draw_line_xyx(question_shape.left - element.half_len, element.y, question_shape.left + element.half_len);
			image.draw_line_xyx(question_shape.right - element.half_len, element.y, question_shape.right + element.half_len);
		}
	}
	//image.save_to_file("puzzle.png");

	schedule = schedule_enum::end;

	return error_type::OK;
}



void DET_aspect_ratio_trainer_data::how_to_inside()
{
	
	/*
	First, construct a instance of this class. The ctor func gets invoked antomatically.
	1.2不同的是，2个阶段拆成2个不同的函数，所有结果只是放在内存里面，是配合这个版本的ui而做的变化。

	int main(int argc, char* argv[])
	{
		DET_aspect_ratio_trainer_data det;

		det.init(...)

		det.generate_to_image_data_member_stage_1();
		题目结果在image 里面

		det.generate_to_image_data_member_stage_2();
		直接在题目上标记了答案，直接用

		return 0;
	}



	For customization, all you need to do are inside the ctor of DET_aspect_ratio_trainer_data and the first 10-ish lines in generate_to_file_mode_1().

	*/
	
}
