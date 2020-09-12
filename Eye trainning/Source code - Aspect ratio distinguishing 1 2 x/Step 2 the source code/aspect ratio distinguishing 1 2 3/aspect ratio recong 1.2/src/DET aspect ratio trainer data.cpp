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


	//拿到里面去了。


// ================    DET_aspect_ratio_trainer_data::DET_auxiliary_line_data_nestedClass    ==================
	
// ================    DET_aspect_ratio_trainer_data    ==================


//ctor
DET_aspect_ratio_trainer_data::DET_aspect_ratio_trainer_data()
{
	filling_mode = filling_mode_enum::random;
	train_mode = train_mode_enum::question_then_auxiliary_lines;

	noise_mode = 1;

	background_color = DCT_color_rgb_uchar(156);

	schedule = schedule_enum::Not_initialized_yet;

	return;
}


//inner tool
//拿到里面去了。



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

	//randomly chooses the answer
	answer = answer_manager.get_random_answer();


	//randomly chooses width and move up ratio
	//shape_data.width;
	question_shape.width = width_manager.get_random_width();

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





	filling_mode = get_random_filling_mode();

	//calculate other data
	question_shape.height = DCT_round2i(question_shape.width * answer);
	question_shape.top = DCT_round2i((image.height - question_shape.height) / 2.f * (1.f - question_shape.move_up_ratio));
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
		for (float r = 0.2f; r < answer+0.21f; r += 0.2f)
			auxiliary_line_data.push_raw(r, 25);
		for (float r = 0.3f; r < answer + 0.21f; r += 0.2f)
			auxiliary_line_data.push_raw(r, 15);
		for (float r = 0.25f; r < answer + 0.21f; r += 0.1f)
			auxiliary_line_data.push_raw(r, 5);
		for (float r = 1.f; r < answer + 0.21f; r += 1.f)
			auxiliary_line_data.push_raw(r, 50);
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


	//Notice, this part is the THIRD pard of the noice squars. Draw them first to put them at the very bottom.
	//新的，第三部分，最外面的噪点。但是有个问题，就是可能会挡住前面画的东西，所以要放到最开头。
	{
		size_t noise_rect_count = DCT_randomi(15 + static_cast<int>(question_shape.width*0.1f), 25 + static_cast<int>(question_shape.width*0.1f));//改过
		DCT_vector2_i coord(DCT_random01() * 10000, DCT_random01() * 10000);
		DCT_vector2_i coord_random_stride(DCT_random01() * 3457891, DCT_random01() * 5676782);
		DCT_vector2 mod(static_cast<float>(question_shape.width), question_shape.width*0.7f + question_shape.height * 0.3f);
		mod = DCT_vector2(static_cast<int>(image.width/2), static_cast<int>(image.height/2));//改过


		std::vector<DCT_color_rgb_uchar> color_options_noise =
		{
			background_color + DCT_color_rgb_uchar::make_gray(12),
			background_color + DCT_color_rgb_uchar::make_gray(8),
			background_color + DCT_color_rgb_uchar::make_gray(4),

			background_color - DCT_color_rgb_uchar::make_gray(16),
			background_color - DCT_color_rgb_uchar::make_gray(12),
			background_color - DCT_color_rgb_uchar::make_gray(8),
			background_color - DCT_color_rgb_uchar::make_gray(4)
		};

		DCT_vector2_i mid_point_of_shape{
			(question_shape.left + question_shape.right) / 2,
			(question_shape.top + question_shape.bottom) / 2 };

	//这儿的那些代码先去掉了。

		for (size_t i = 0; i < noise_rect_count; ++i)
		{
			//size
			const int int509mul937 = 509 * 937;
			int temp = rand() * int509mul937 % 863;
			temp = temp * int509mul937 % 863;
		
			int delta_of_top_left = temp % 119 + 35;
			int delta_of_bottom_right = delta_of_top_left;
			//position
			DCT_vector2_i mid(coord.x , coord.y );
			mid.x *= int509mul937 % 863 * int509mul937 % 863;
			mid.y *= int509mul937 % 863 * int509mul937 % 863 * int509mul937 % 863;
			mid.x %= static_cast<int>(mod.x);
			mid.y %= static_cast<int>(mod.y);

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
				//测试。用别的颜色
				if (1)
				{
					DCT_color::gen genor;
					genor.velocity_min = 0.5f;
					genor.velocity_max = 0.62f;
					genor.saturation_min = 0.2f;
					genor.saturation_max = 0.35f;
					DCT_color_rgb_f temp = genor.gen_rgb_f();
					image.set_color(DCT_color::DCT_color_tran_safe(temp));
				}

				image.draw_sq(top_left.x, bottom_right.x, top_left.y, bottom_right.y);

				//late update for loop
				coord = { mid.x + coord_random_stride.x,mid.y + coord_random_stride.y };
				//原来的。mod = { mod.x + 3, static_cast<int>(mod.y + 3 * (9.f / 16.f)) };
				mod = DCT_vector2(mod.x- 11.f, mod.y - 11.f * (static_cast<float>(image.height)/image.width));//改过
			}


		}//for (noise_rect_count)

	}//scattering noise rects













	//drawing the question
	{
		image.set_color(question_shade_color);
		if (1)
		{
			DCT_color::gen genor;
			genor.velocity_min = 0.2f;
			genor.velocity_max = 0.8f;
			genor.saturation_min = 0.2f;
			genor.saturation_max = 0.5f;
			DCT_color_rgb_f temp = genor.gen_rgb_f();
			image.set_color(DCT_color::DCT_color_tran_safe(temp));
		}
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
	if(0)//这个是之前的，现在用新的。以后做为2个模式，加按钮控制。
	{
		size_t noise_rect_count = DCT_randomi(6+static_cast<int>(question_shape.width*0.04f), 5+static_cast<int>(question_shape.width*0.07f));
		DCT_vector2_i coord(DCT_random01() * 10000, DCT_random01() * 10000);
		DCT_vector2_i coord_random_stride(DCT_random01() * 3457891, DCT_random01() * 5676782);
		DCT_vector2 mod(static_cast<float>(question_shape.width), question_shape.width*0.7f+question_shape.height * 0.3f);
		mod = DCT_vector2(mod.x*0.8f, mod.y*0.8f);


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

		/*#adjusting but,, not implemented yet. ok, to do list.
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
			DCT_vector2_i mid(coord.x % static_cast<int>(mod.x), coord.y % static_cast<int>(mod.y));
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
				//原来的。mod = { mod.x + 3, static_cast<int>(mod.y + 3 * (9.f / 16.f)) };
				mod = DCT_vector2(mod.x + 1.2f, mod.y + 1.2f * (answer*0.7f + 0.3f));
			}


		}//for (noise_rect_count)

	}//scattering noise rects





	//新的，第一部分，还是那些噪点，但是密度更大。
	{
		size_t noise_rect_count = DCT_randomi(-5 + static_cast<int>(question_shape.width*0.2f), 5 + static_cast<int>(question_shape.width*0.3f));//改过
		DCT_vector2_i coord(DCT_random01() * 10000, DCT_random01() * 10000);
		DCT_vector2_i coord_random_stride(DCT_random01() * 3457891, DCT_random01() * 5676782);
		DCT_vector2 mod(static_cast<float>(question_shape.width), question_shape.width*0.7f + question_shape.height * 0.3f);
		mod = DCT_vector2(mod.x*0.3f, mod.y*0.3f);//改过

		//颜色也改过。
		std::vector<DCT_color_rgb_uchar> color_options_noise =
		{
			background_color + DCT_color_rgb_uchar::make_gray(25),
			question_shade_color + DCT_color_rgb_uchar::make_gray(20),
			question_shade_color + DCT_color_rgb_uchar::make_gray(15),

			background_color - DCT_color_rgb_uchar::make_gray(30),
			question_shade_color - DCT_color_rgb_uchar::make_gray(25),
			question_shade_color - DCT_color_rgb_uchar::make_gray(20),
			question_shade_color - DCT_color_rgb_uchar::make_gray(15)
		};

		DCT_vector2_i mid_point_of_shape{
			(question_shape.left + question_shape.right) / 2,
			(question_shape.top + question_shape.bottom) / 2 };

		/*#adjusting but,, not implemented yet. ok, to do list.
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
			DCT_vector2_i mid(coord.x % static_cast<int>(mod.x), coord.y % static_cast<int>(mod.y));
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
				if (1)
				{
					DCT_color::gen genor;
					genor.velocity_min = 0.5f;
					genor.velocity_max = 0.62f;
					genor.saturation_min = 0.35f;
					genor.saturation_max = 0.6f;
					DCT_color_rgb_f temp = genor.gen_rgb_f();
					image.set_color(DCT_color::DCT_color_tran_safe(temp));
				}
				image.draw_sq(top_left.x, bottom_right.x, top_left.y, bottom_right.y);

				//late update for loop
				coord = { mid.x + coord_random_stride.x,mid.y + coord_random_stride.y };
				//原来的。mod = { mod.x + 3, static_cast<int>(mod.y + 3 * (9.f / 16.f)) };
				mod = DCT_vector2(mod.x + 2.f, mod.y + 2.f * (answer*0.7f + 0.3f));
			}


		}//for (noise_rect_count)

	}//scattering noise rects

	//新的，第二部分，中心区域加入一些细碎的噪点。
	{
		size_t noise_rect_count = DCT_randomi(-5 + static_cast<int>(question_shape.width*0.1f), 5 + static_cast<int>(question_shape.width*0.1f));//改过
		DCT_vector2_i coord(DCT_random01() * 10000, DCT_random01() * 10000);
		DCT_vector2_i coord_random_stride(DCT_random01() * 3457891, DCT_random01() * 5676782);
		DCT_vector2 mod(static_cast<float>(question_shape.width), question_shape.width*0.7f + question_shape.height * 0.3f);
		mod = DCT_vector2(mod.x*0.1f, mod.y*0.1f);//改过


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

		/*#adjusting but,, not implemented yet. ok, to do list.
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
			int delta_of_top_left = i % 4/* + 4*/;
			int delta_of_bottom_right = (i / 4) % 4/* + 4*/;
			//position
			DCT_vector2_i mid(coord.x % static_cast<int>(mod.x), coord.y % static_cast<int>(mod.y));
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
				//原来的。mod = { mod.x + 3, static_cast<int>(mod.y + 3 * (9.f / 16.f)) };
				mod = DCT_vector2(mod.x + 1.f, mod.y + 1.f * (answer*0.7f + 0.3f));//改过
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


	//draw the reference lines
	{
		DCT_color_rgb_uchar auxiliary_line_color = question_shade_color;
		
		image.set_color(auxiliary_line_color);
		for (auto element : auxiliary_line_data.data)
		{
			if(element.half_len>23)
			{
				image.draw_line_xyx(question_shape.left - element.half_len, element.y, question_shape.left-3);
				image.draw_line_xyx(question_shape.right + element.half_len, element.y, question_shape.right + 3);//符号和以前的不一样。从右往左画。
			}
			else
			{
				//else里面还是用的以前的。
				image.draw_line_xyx(question_shape.left - element.half_len, element.y, question_shape.left + element.half_len);
				image.draw_line_xyx(question_shape.right - element.half_len, element.y, question_shape.right + element.half_len);
			}

			/*以前的代码，可以留着以后作为一个选项用
			image.draw_line_xyx(question_shape.left - element.half_len, element.y, question_shape.left + element.half_len);
			image.draw_line_xyx(question_shape.right - element.half_len, element.y, question_shape.right + element.half_len);
			*/
		}
	}
	
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
