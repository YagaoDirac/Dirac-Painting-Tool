#pragma once

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include"DET aspect ratio distinguishing Frame.h"

//arr == aspect ratio distinguishing.
class DET_ard_App :public wxApp
{
public:
	virtual bool OnInit() 
	{
		if (!wxApp::OnInit())return false;

		frame_ = new DET_ard_Frame();
		frame_->Show(true);

		return true;
	};

public:
	DET_ard_Frame* frame_;

};

