#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <sstream>
#include <wx/dcbuffer.h>
#include "DET aspect ratio trainer data.h"




class DET_ard_Frame : public wxFrame
{
public:
	enum class stage_enum
	{
		to_do_stage_1,
		to_do_stage_2
	};
	stage_enum stage;

	wxColour answer_text_colour;//according to wx customization, colour rather than color.

	DET_ard_Frame()
		:wxFrame(nullptr, wxID_ANY, "[space] to continue --- Dirac painting tool --- Eye trainning --- Aspect ratio distinguishing 1.2 --- github.com/YagaoDirac/Dirac-s-Painting-Tool/tree/master/Eye trainning/ --- pixiv.net/users/10845271")
	{
		SetSize(1280, 720);//In order to make sure the whole title seen~

		stage = stage_enum::to_do_stage_1;

		panel = new wxPanel(this);

		panel->Bind(wxEVT_KEY_DOWN, &DET_ard_Frame::OnKeyDown, this);
		panel->Bind(wxEVT_PAINT, &DET_ard_Frame::OnpaintEvent, this);
		panel->Bind(wxEVT_SIZE, &DET_ard_Frame::Onresize, this);
		panel->SetBackgroundStyle(wxBackgroundStyle::wxBG_STYLE_PAINT);//double buffering

		//For using double buffering.
		SetBackgroundStyle(wxBG_STYLE_PAINT);

		//Sizer. 不是很清楚细节
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);//参数可能不适合
		sizer->Add(panel, 1, wxEXPAND);//这句可能有错
		this->SetSizer(sizer);
		this->SetAutoLayout(true);

		trainer_data = new DET_aspect_ratio_trainer_data();

		//Text colour, modify it if you would like
		{
			DCT_color_rgb_uchar temp = trainer_data->background_color;
			temp = temp - DCT_color_rgb_uchar(25, 25, 25);
			DCT_color_rgba_uchar temp2 = DCT_color::DCT_color_tran(temp);
			
			
			answer_text_colour.SetRGB((*reinterpret_cast<int*>(&temp2)));//and this line seems to work too..
			//answer_text_colour.SetRGB(temp2.r<<16| temp2.g<<8| temp2.b);
		}
	}
	~DET_ard_Frame()
	{
		delete trainer_data;
	}

public:
	//wx EVENTs

	void OnKeyDown(wxKeyEvent& event)
	{//redraw itself immediately
		Refresh();
	};
	void OnpaintEvent(wxPaintEvent& event)
	{
		if (stage_enum::to_do_stage_1 == stage)
		{
			do_stage_1();
			return;
		}
		if (stage_enum::to_do_stage_2 == stage)
		{
			do_stage_2();
			return;
		}
	};
	
	void Onresize(wxSizeEvent& event)
	{
		stage = stage_enum::to_do_stage_1;

		wxSize temp_size = panel->GetSize();
		trainer_data->image.init(temp_size.x, temp_size.y);

		event.Skip();//不知道对不对。说的让我不要catch这个event，那应该就是这个意思了。

		Refresh();
	};


	void do_stage_1()
	{
		wxBufferedPaintDC bpdc(panel);

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

		stage = stage_enum::to_do_stage_2;
	}

	void do_stage_2()
	{
		wxBufferedPaintDC bpdc(panel);

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

		stage = stage_enum::to_do_stage_1;

	}


	void update_the_whole_window测试代码，不要用()
	{
		wxBufferedPaintDC bpdc(panel);//好像panel才可以有dc
		bpdc.SetBackground(wxBrush(wxColour(111, 111, 111)));
		bpdc.Clear();



		int w = 100, h = 100;
		wxImage img(w, h, false);
		img.Clear(0);
		int len = w * h * 3;
		unsigned char* pdata = reinterpret_cast<unsigned char*>(malloc(len));

		for (int y = 0; y < h; ++y)
			for (int x = 0; x < w; ++x)
			{
				int basec_offset = y * 300 + x * 3;
				pdata[basec_offset] = x * 2;
				pdata[basec_offset + 1] = y * 2;
				pdata[basec_offset + 2] = 100;
			}

		img.SetData(pdata);

		wxBitmap bmp(img);
		bpdc.DrawBitmap(bmp, rand()/100, 0, true);

		bpdc.SetBackground(wxBrush(wxColour(127, 127, 127)));
		bpdc.SetBrush(wxBrush(wxColour(0,0,0)));
		bpdc.SetPen(wxPen(wxColour(255, 255, 255)));
		bpdc.SetTextForeground(wxColour(211, 122, 255));
		//font size
		wxFont font = bpdc.GetFont();
		font.SetPixelSize(font.GetPixelSize() * 3);
		bpdc.SetFont(font);
		//font position
		wxSize Frame_size = this->GetSize();
		bpdc.DrawText(wxString("0.666"), Frame_size.x / 2, Frame_size.y - 100);

	};
	
public:
	wxPanel* panel;

	DET_aspect_ratio_trainer_data* trainer_data;


};

