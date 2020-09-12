#pragma once

//等一下，这个事情有点神奇呢。。不用include wxWidgets的header吗？？？

/*
Tips.
The event objects are transfered like, for the clicking event:
os >>> window >>> button.
So, the wxWindow::clicking handling function(which is written by me) get the event first. If it calls event.Skip(), the event object is passed to the wxButton::clicking handling function.
I tried giving the fucos back to window inside the button's clicking handling function.'
I didn't test the setting-focus event. It should work in a similar way.
*/

/*
Tips - 2
I didn't find a method to modify the colour of border. In the docs, wxWidgets provides a way to avoid showing border. At the position of "long style = 0, " in the parameter list of constructor function, a wxBORDER_NONE would do.
*/

//This class simply gives focus back to its parent. Make sure the parent of this button accepts focus. Or you have to assign the destination manually.
class no_focus_button :public wxButton
{
public:
	no_focus_button
	(			wxWindow *		parent,
				wxWindowID		id = wxID_ANY,
		const wxString &		label = wxEmptyString,
		const wxPoint &		pos = wxDefaultPosition,
		const wxSize &			size = wxDefaultSize,
				long				style = 0,
		const wxValidator &  validator = wxDefaultValidator,
		const wxString &		name = wxButtonNameStr
	)
		:wxButton(parent, id, label, pos, size, style, validator, name)
	{
		Bind(wxEVT_BUTTON, &no_focus_button::On_Button, this);
		focus_back_to_this = parent;
	}

	void On_Button(wxCommandEvent& event)
	{//Give the focus back to its parent control.
		focus_back_to_this->SetFocus();
		event.Skip();
	}

	wxWindow* focus_back_to_this;
};
