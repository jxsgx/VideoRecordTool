#pragma once
#include "duilib.h"
#include "AVPlayer.h"

class CDuiVideoPlayer :
	public CXMLWnd
{
public:
	explicit CDuiVideoPlayer(const wchar_t* xml);
	virtual ~CDuiVideoPlayer();

	virtual void InitWindow() override;
	virtual LPCTSTR GetWindowClassName() const override { return L"CDuiVideoPlayer"; }
	virtual void Notify(DuiLib::TNotifyUI& msg) override;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	virtual void OnClick(TNotifyUI& msg) override;
	virtual CControlUI* CreateControl(LPCTSTR pstrClassName) override;

	

protected:
	//bool play_ = false;
	//cv::VideoCapture cap_ = {};
	CAVPlayer player_ = {};
	std::string video_path_ = {};
	bool show_tip_ = true;
	time_t tm_remained_ = 0;
	bool disable_click_ = false;

	void OnEndReached(void*);

public:
	bool play(const std::string& path = {});
	bool pause();
	bool resume();
	bool stop();
	bool set_pos(int pos);


	DUI_DECLARE_MESSAGE_MAP();
};

