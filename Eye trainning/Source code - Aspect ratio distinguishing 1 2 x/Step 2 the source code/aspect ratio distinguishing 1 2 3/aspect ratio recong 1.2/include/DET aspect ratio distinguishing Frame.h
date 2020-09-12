#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <sstream>
#include <wx/dcbuffer.h>

#include "DET aspect ratio distinguishing Window.h"

class DET_ard_Frame : public wxFrame
{
public:
	

	DET_ard_Frame()
		:wxFrame(nullptr, wxID_ANY, "Dirac painting tool --- Eye trainning --- Aspect ratio distinguishing 1.2.3 --- github.com/YagaoDirac/Dirac-s-Painting-Tool/tree/master/Eye trainning/ --- pixiv.net/users/10845271")
	{
		SetBackgroundStyle(wxBG_STYLE_PAINT);//For using double buffering.
		wind = new DET_ard_Window(this);

		//Sizer. 不是很清楚细节
		wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);//参数可能不适合
		wxSizerFlags SizerFlags;
		SizerFlags.Proportion(1).Expand();//It seems that these 2 functions co op to make the panel(child obj) scales automatically.
		sizer->Add(wind, SizerFlags);//这句可能有错
		this->SetSizerAndFit(sizer);
		this->SetAutoLayout(true);
		
		SetSize(1280, 720);//In order to make sure the whole title seen~


	}
	~DET_ard_Frame()
	{
	}



public:
	DET_ard_Window* wind;
};

