#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/dcbuffer.h>


#include <vector>
#include <string>
#include <sstream>

#include "DET aspect ratio trainer data.h"
#include "DCT wxWidgets no focus Button.h"







class DET_ard_Window : public wxWindow//   继续
{
public:

	DET_ard_Window(wxWindow* parent)
		:wxWindow(parent,wxID_ANY,wxDefaultPosition,wxDefaultSize, wxFULL_REPAINT_ON_RESIZE )
	{
		SetBackgroundStyle(wxBG_STYLE_PAINT);//Double buffering. Not very sure yet.
		last_paint_time = wxDateTime::UNow();

		Bind(wxEVT_KEY_DOWN, &DET_ard_Window::OnKeyDown, this);
		Bind(wxEVT_PAINT, &DET_ard_Window::OnpaintEvent, this);
		Bind(wxEVT_SIZE, &DET_ard_Window::Onresize, this);

		SetBackgroundStyle(wxBackgroundStyle::wxBG_STYLE_PAINT);//double buffering

		//backend
		trainer_data = new DET_aspect_ratio_trainer_data();

		//Text colour, modify it if you would like
		{
			DCT_color_rgb_uchar temp = trainer_data->background_color;
			temp = temp - DCT_color_rgb_uchar(25, 25, 25);
			DCT_color_rgba_uchar temp2(temp);

			answer_text_colour.SetRGB((*reinterpret_cast<int*>(&temp2)));//and this line seems to work too..
			//answer_text_colour.SetRGB(temp2.r<<16| temp2.g<<8| temp2.b);
		}

		//buttons to control answer manager.
		button_lower_bound = new no_focus_button(this, wxID_ANY, trainer_data->get_lower_label_for_ui(), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
		button_lower_bound->Bind(wxEVT_BUTTON, &DET_ard_Window::On_lower_clicked, this);
		//button_lower_bound->DisableFocusFromKeyboard();好像不用了
		button_lower_bound->SetBackgroundColour(wxColour(120, 120, 120));

		button_upper_bound = new no_focus_button(this, wxID_ANY, trainer_data->get_upper_label_for_ui(), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
		button_upper_bound->Bind(wxEVT_BUTTON, &DET_ard_Window::On_upper_clicked, this);
		button_upper_bound->SetBackgroundColour(wxColour(140, 140, 140));

		button_division_value = new no_focus_button(this, wxID_ANY, trainer_data->get_division_label_for_ui(), wxDefaultPosition, wxDefaultSize, wxBORDER_NONE);
		button_division_value->Bind(wxEVT_BUTTON, &DET_ard_Window::On_division_clicked, this);
		button_division_value->SetBackgroundColour(wxColour(120, 120, 120));

		//sizer
		wxBoxSizer* top_sizer = new wxBoxSizer(wxOrientation::wxHORIZONTAL);
		wxBoxSizer* mid_level_sizer_1 = new wxBoxSizer(wxOrientation::wxVERTICAL);
		top_sizer->Add(mid_level_sizer_1);
		wxSizerFlags SizerFlags;
		SizerFlags.Left().Top();
		//SizerFlags.Border(wxDirection::wxTOP, 16);
		//SizerFlags.Border(wxDirection::wxLEFT, 16);
		mid_level_sizer_1->Add(button_lower_bound, SizerFlags);
		mid_level_sizer_1->Add(button_upper_bound, SizerFlags);
		mid_level_sizer_1->Add(button_division_value, SizerFlags);
	

		//buttons to control width manager.
		button_min_width = new no_focus_button(this, wxID_ANY, trainer_data->get_min_width_label_for_ui(), wxDefaultPosition, wxSize(110, 22), wxBORDER_NONE);
		button_min_width->SetMinSize(wxSize(110, 24));
		button_min_width->Bind(wxEVT_BUTTON, &DET_ard_Window::On_min_width_clicked, this);
		button_min_width->SetBackgroundColour(wxColour(140, 140, 140));

		button_max_width = new no_focus_button(this, wxID_ANY, trainer_data->get_max_width_label_for_ui(), wxDefaultPosition, wxSize(110, 24), wxBORDER_NONE);
		button_max_width->Bind(wxEVT_BUTTON, &DET_ard_Window::On_max_width_clicked, this);
		button_max_width->SetBackgroundColour(wxColour(120, 120, 120));

		wxBoxSizer* mid_level_sizer_2 = new wxBoxSizer(wxOrientation::wxVERTICAL);
		top_sizer->Add(mid_level_sizer_2);
		mid_level_sizer_2->Add(button_min_width, SizerFlags);
		mid_level_sizer_2->Add(button_max_width, SizerFlags);
		

		//buttons for help info.
		button_help_info_mode_shifting = new no_focus_button(this, wxID_ANY, help_info_manager.get_mode_button_lebal_for_ui(), wxDefaultPosition, wxSize(100, 24), wxBORDER_NONE);
		button_help_info_mode_shifting->Bind(wxEVT_BUTTON, &DET_ard_Window::On_help_info_mode_shifting_button_clicked, this);
		button_help_info_mode_shifting->SetBackgroundColour(wxColour(120, 120, 120));

		wxBoxSizer* mid_level_sizer_3 = new wxBoxSizer(wxOrientation::wxVERTICAL);
		top_sizer->Add(mid_level_sizer_3);
		mid_level_sizer_3->Add(button_help_info_mode_shifting, SizerFlags);

		button_help_info_text = new no_focus_button(this, wxID_ANY, help_info_manager.get_help_text_button_lebal_for_ui(), wxDefaultPosition, wxSize(400, 24), wxBORDER_NONE);
		button_help_info_text->Bind(wxEVT_BUTTON, &DET_ard_Window::On_help_info_text_button_clicked, this);
		button_help_info_text->SetBackgroundColour(wxColour(120, 120, 120));
		help_info_manager.set_text_button(button_help_info_text);
		button_help_info_text->SetBackgroundStyle(wxBG_STYLE_PAINT);

		SetSizer(top_sizer);
		top_sizer->Fit(this);

		SetFocusFromKbd();

		//Note: this timer times out for once after the object is instantiated for 1 second, queues a repaint event.
		//I tried 500ms and it works but 300ms does not. For safety, I picked 1 second.
		timer_first_refresh = new __timer_DET_ard_Window__Refresh_the_first_time(this,static_cast<int>( __timer_DET_ard_Window__Refresh_the_first_time::timer_id_const::the_const));
		timer_first_refresh->Start(1000, true);
		Bind(wxEVT_TIMER, &DET_ard_Window::On_timer_first_freshing, this);
	};
	~DET_ard_Window()
	{
		delete trainer_data;
	}

public:
	//wx EVENTs for children buttons
	void On_lower_clicked(wxCommandEvent& event)
	{
		trainer_data->change_lower_bound_from_ui();
		button_lower_bound->SetLabel(trainer_data->get_lower_label_for_ui());

		do_the_refreshing();
		event.Skip();
	}
	void On_upper_clicked(wxCommandEvent& event)
	{
		trainer_data->change_upper_bound_from_ui();
		button_upper_bound->SetLabel(trainer_data->get_upper_label_for_ui());

		do_the_refreshing();
		event.Skip();
	}
	void On_division_clicked(wxCommandEvent& event)
	{
		trainer_data->change_division_value_from_ui();
		button_division_value->SetLabel(trainer_data->get_division_label_for_ui());
		
		do_the_refreshing();
		event.Skip();
	}

	void On_min_width_clicked(wxCommandEvent& event)
	{
		trainer_data->change_min_width_from_ui();
		button_min_width->SetLabel(trainer_data->get_min_width_label_for_ui());

		do_the_refreshing();
		event.Skip();
	}
	void On_max_width_clicked(wxCommandEvent& event)
	{
		trainer_data->change_max_width_from_ui();
		button_max_width->SetLabel(trainer_data->get_max_width_label_for_ui());

		do_the_refreshing();
		event.Skip();
	}

	void On_help_info_mode_shifting_button_clicked(wxCommandEvent& event)
	{
		help_info_manager.On_change_mode_from_ui();
		button_help_info_mode_shifting->SetLabel(help_info_manager.get_mode_button_lebal_for_ui());

		if (!help_info_manager.mode_manager.should_help_text_visible())
			do_the_refreshing();

		event.Skip();
	}
	void On_help_info_text_button_clicked(wxCommandEvent& event)
	{
		help_info_manager.On_change_help_text_from_ui();
		button_help_info_text->SetLabel(help_info_manager.get_help_text_button_lebal_for_ui());
		event.Skip();
	}
	
	//Events for timer(s)
	void On_timer_first_freshing(wxTimerEvent& event)
	{
		Refresh();
	}

		
	//inner util
	void do_the_refreshing() { Refresh(); };
	void __position_the_help_text()
	{
		wxSize help_info_position = GetSize();
		help_info_position.x /= 2;
		help_info_position.x -= 200;
		help_info_position.y *= 0.75;
		button_help_info_text->SetPosition(wxPoint(help_info_position.x, help_info_position.y));
	}

	//wx EVENTs for this
	void OnKeyDown(wxKeyEvent& event)
	{//redraw itself immediately
		do_the_refreshing();
	};
	void OnpaintEvent(wxPaintEvent& event)
	{
		wxDateTime now;
		now = wxDateTime::UNow();
		if ((*reinterpret_cast<long long*>(&now)) - (*reinterpret_cast<long long*>(&last_paint_time)) < 500)
			return;

		last_paint_time = now;
		if(DET_aspect_ratio_trainer_data::schedule_enum::between_stage_1_and_2 != trainer_data->schedule)
		{
			do_stage_1();
			return;
		}
		else
		{
			do_stage_2();
			return;
		}
	};
	void Onresize(wxSizeEvent& event)
	{
		//Resizing makes the memory size wrong. The backend needs another initializing.
		trainer_data->schedule = DET_aspect_ratio_trainer_data::schedule_enum::Not_initialized_yet;
		//stage = stage_enum::to_do_stage_1;

		wxSize temp_size = GetSize();
		trainer_data->image.init(temp_size.x, temp_size.y);

		__position_the_help_text();

		event.Skip();//不知道对不对。说的让我不要catch这个event，那应该就是这个意思了。

		Refresh();
	};


	void do_stage_1()
	{
		wxBufferedPaintDC bpdc(this);

		wxSize panel_size = bpdc.GetSize();

		trainer_data->set_size(panel_size.x, panel_size.y);

		//generate the question
		trainer_data->generate_to_image_data_member_stage_1();

		wxImage img(panel_size.x, panel_size.y, false);
		int len = panel_size.x* panel_size.y * 3;
		unsigned char* pdata = reinterpret_cast<unsigned char*>(malloc(len));

		for (int y = 0; y < panel_size.y; ++y)
			for (int x = 0; x < panel_size.x; ++x)
			{
				int offset_from = (y * panel_size.x + x);
				int offset_to = offset_from * 3;

				//memcpy((void*)pdata[offset_to], (void*)&(trainer_data->image.memory[offset_from]), 3);
				// 算了，先不研究了，先用笨办法。
				pdata[offset_to] = trainer_data->image.memory[offset_from].r;
				pdata[offset_to + 1] = trainer_data->image.memory[offset_from].g;
				pdata[offset_to + 2] = trainer_data->image.memory[offset_from].b;
			}

		img.SetData(pdata);

		wxBitmap bmp(img);
		bpdc.DrawBitmap(bmp, 0, 0, true);
	}

	void do_stage_2()
	{
		wxBufferedPaintDC bpdc(this);

		wxSize panel_size = bpdc.GetSize();

		//generate the question
		trainer_data->generate_to_image_data_member_stage_2();

		wxImage img(panel_size.x, panel_size.y, false);
		int len = panel_size.x* panel_size.y * 3;
		unsigned char* pdata = reinterpret_cast<unsigned char*>(malloc(len));

		for (int y = 0; y < panel_size.y; ++y)
			for (int x = 0; x < panel_size.x; ++x)
			{
				int offset_from = (y * panel_size.x + x);
				int offset_to = offset_from * 3;

				//memcpy((void*)pdata[offset_to], (void*)&(trainer_data->image.memory[offset_from]), 3);
				// 算了，先不研究了，先用笨办法。
				pdata[offset_to] = trainer_data->image.memory[offset_from].r;
				pdata[offset_to + 1] = trainer_data->image.memory[offset_from].g;
				pdata[offset_to + 2] = trainer_data->image.memory[offset_from].b;
			}

		img.SetData(pdata);

		wxBitmap bmp(img);
		bpdc.DrawBitmap(bmp, 0, 0, true);

		//now, let's print the answer text.
		bpdc.SetTextForeground(answer_text_colour);
		//font size
		wxFont font = bpdc.GetFont();
		font.SetPixelSize(font.GetPixelSize() * 3);
		bpdc.SetFont(font);
		std::string to_print;
		{//display only the first 3 decimal
			std::stringstream temp_stream;
			temp_stream.precision(3);
			temp_stream << std::fixed << trainer_data->answer;
			to_print = temp_stream.str();
		}
		to_print += "  w: ";
		to_print += std::to_string(trainer_data->question_shape.width);
		to_print += "  h: ";
		to_print += std::to_string(trainer_data->question_shape.height);

		//font position
		//wxSize panel_size = panel->GetSize(); 前面有了
		bpdc.DrawText(wxString(to_print), panel_size.x / 2, panel_size.y - 80);

		//stage = stage_enum::to_do_stage_1;

	}


	DET_aspect_ratio_trainer_data* trainer_data;

	wxColour answer_text_colour;//according to wx customization, colour rather than color.

	no_focus_button* button_lower_bound;
	no_focus_button* button_upper_bound;
	no_focus_button* button_division_value;
	no_focus_button* button_min_width;
	no_focus_button* button_max_width;
	no_focus_button* button_help_info_mode_shifting;
	no_focus_button* button_help_info_text;

	wxDateTime last_paint_time;

	//a timer for the first Refresh() call. There might be some smarter way, but, you know.
	class __timer_DET_ard_Window__Refresh_the_first_time :public wxTimer
	{
	public:
		enum class timer_id_const
		{
			the_const
		};
		__timer_DET_ard_Window__Refresh_the_first_time(wxEvtHandler*owner, int id = static_cast<int>(timer_id_const::the_const))
			:wxTimer(owner, id)
		{};
	};
	__timer_DET_ard_Window__Refresh_the_first_time* timer_first_refresh;
	





	class help_info_manager_nested_class
	{
	public:
		help_info_manager_nested_class()
		{
			text_button = nullptr;
		}
		void set_text_button(wxButton* in) { text_button = in; }

		//the data for the button on the top left corner, which controls the display behavior of the real help info text control.
		class __mode__
		{
		public:
			__mode__()
			{
				mode_button_lebals.push_back("Help text: On");
				mode_button_lebals.push_back("Help text: Off");
				//display_mode = display_mode_enum::always;
				index = 0;
			}
			std::vector<std::string> mode_button_lebals;
			int index;
			void On_change_mode_from_ui()
			{
				index += 1;
				index %= mode_button_lebals.size();
			}
			std::string get_mode_button_lebal_for_ui()
			{
				return mode_button_lebals[index];
			}
			bool should_help_text_visible()
			{
				if (0 == index)return true;
				else return false;
			}
		};
		__mode__ mode_manager;
		void On_change_mode_from_ui()
		{
			mode_manager.On_change_mode_from_ui();
			if (text_button)
			{
				if (mode_manager.should_help_text_visible())
					text_button->Show(true);
				else
					text_button->Show(false);
			}
		}
		std::string get_mode_button_lebal_for_ui(){return mode_manager.get_mode_button_lebal_for_ui();}

		//the text(but actually a button) really displays the help info.
		class __help_text__
		{
		public:
			__help_text__()
			{
				help_text.push_back("Press any key to continue");
				help_text.push_back("Lebals in top left corner are buttons. Click them.");
				help_text_postfix = " [Click for more]";
				index = 0;
			}
			std::vector<std::string> help_text;
			std::string help_text_postfix;
			int index;
			void On_change_help_text_from_ui()
			{
				index += 1;
				index %= help_text.size();
			}
			std::string get_help_text_button_lebal_for_ui()
			{
				return help_text[index] + help_text_postfix;
			}
		};
		__help_text__ help_text_manager;
		void On_change_help_text_from_ui(){help_text_manager.On_change_help_text_from_ui();}
		std::string get_help_text_button_lebal_for_ui(){return help_text_manager.get_help_text_button_lebal_for_ui();}

		wxButton* text_button;
	};
	help_info_manager_nested_class help_info_manager;

};






