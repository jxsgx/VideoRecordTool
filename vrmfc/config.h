#pragma once

#include <set>


#define VR_ROOT_FOLDER		"数据"
#define VR_VIDEO_FOLDER		"视频" 
#define VR_VIDEO_EXT		".avi"
#define VR_CAPTURE_FOLDER	"照片"
#define VR_CAPTRUE_EXT		".bmp"
#define VR_THUMBNAIL_FOLDER "thumb"
#define VR_THUMBNAIL_EXT	".png"
#define VR_SEL_PIC_FOLDER   "sel"

using misz = std::pair<int, int>;
struct media_info {
	std::string type = {};
	misz default_sz = {};
	std::set<misz> sizes = {};
	int fps = {};

	void dump() const;
};

#define MT_YUY2 "YUY2"
#define MT_MJPG "MJPG"

using mi = std::map<std::string, media_info>;

struct vq {
	long valid_ = {};
	long min_ = {};
	long max_ = {};
	long step_ = {};
	long default_ = {};
	long flags_ = {};
	long val_ = {};

	void dump() const;
};

struct procamp {
	vq backlight = {};
	vq brightness = {};
	vq contrast = {};
	vq gain = {};
	vq gamma = {};
	vq hue = {};
	vq saturation = {};
	vq sharpness = {};
	vq white_balance = {};

	void dump() const;
};

struct camera_set {
	vq exposure = {};
	vq focus = {};
	vq iris = {};
	vq pan = {};
	vq roll = {};
	vq tilt = {};
	vq zoom = {};

	void dump() const;
};

class config : public dp::singleton<config>, public dp::observable<int>
{
protected:
	config();
	bool load();
	bool save();
	void init();
	void init_root();
	
	mi _mi = {};
	procamp _procamp = {};
	camera_set _camera = {};

private:
	std::string cfg_file_ = {};

	int _vidx = 0;
	int _aidx = 0;

	// video
	int _video_w = 0;
	int _video_h = 0;
	std::string _root = {};
	std::string _vtype = {};
	int _max_rec_minutes = 0;

	// serial
	std::string _port = {};
	int _baudrate = 9600;

	// language 
	std::string _lang = {};

public:

	static std::vector<std::pair<char, std::string>> list_removable_drives() {
		std::vector<std::pair<char, std::string>> v;
		for (char i = 'A'; i <= 'Z'; i++) {
			char x[4] = { i, ':' };
			UINT Type = GetDriveTypeA(x);
			if (Type == DRIVE_REMOVABLE) {
				x[2] = '\\';
				char lable[MAX_PATH] = {};
				if (GetVolumeInformationA(x, lable, sizeof(lable), nullptr, nullptr, nullptr, nullptr, 0)) {
					v.push_back(std::make_pair(i, std::string(lable)));
				}
			}
		}
		return v;
	}

	bool clear_root() const;

	std::string get_version() const;
	std::string get_remainder_space() const;

	static constexpr uintmax_t FACTOR = 1024;
	static constexpr uintmax_t KB = FACTOR;
	static constexpr uintmax_t MB = KB * FACTOR;
	static constexpr uintmax_t GB = MB * FACTOR;
	static constexpr uintmax_t TB = GB * FACTOR;
	static std::string format_space(uintmax_t bytes, uintmax_t* pfactor = nullptr, uintmax_t* integer = nullptr, uintmax_t* real=nullptr);

	void dump_amp() const;
	void dump_cam() const;
	void dump_mi() const;

	void set_mi(mi mi) { AUTO_LOG_FUNCTION; _mi = mi; dump_mi(); }
	mi get_mi() const { AUTO_LOG_FUNCTION;  dump_mi(); return _mi; }

	void set_procamp(procamp pa) { AUTO_LOG_FUNCTION; _procamp = pa;  dump_amp(); }
	procamp get_procamp() const { AUTO_LOG_FUNCTION; dump_amp(); return _procamp; }

	void set_camera(camera_set cs) { AUTO_LOG_FUNCTION; _camera = cs; dump_cam(); }
	camera_set get_camera() const { AUTO_LOG_FUNCTION; dump_cam(); return _camera; }


	std::string get_video_path() const;
	std::string get_capture_path() const;
	std::string create_new_video_path() const;
	std::string create_new_capture_path() const;
	std::string get_thumb_path() const;
	std::string create_new_thumb_path(const std::string& stem);
	std::string get_thumb_of_video(const std::string& vpath); // if thumb not exist, create it.
	std::string create_selected_pic_path(const std::string& stem);
	std::string get_selected_pic(const std::string& path);

#define declare_getter(type, val) type get##val() const { return val; }
#define declare_getter_int(val) declare_getter(int, val)

#define declare_setter(type, val) void set##val(const type& v) { if (val == v) return; val = v; save(); }
#define declare_setter_int(val) declare_setter(int, val)

#define declare_gs(type, val) declare_getter(type, val); declare_setter(type, val);
#define declare_gs_int(val) declare_gs(int, val)
#define declare_gs_string(val) declare_gs(std::string, val)



	declare_gs_int(_vidx);
	declare_gs_int(_aidx);
	declare_gs_int(_video_w);
	declare_gs_int(_video_h);
	declare_gs_string(_root);
	declare_gs_string(_port);
	declare_gs_int(_baudrate);
	declare_gs_string(_lang);
	declare_gs_string(_vtype);

	//declare_gs(mi, _mi);
	declare_gs_int(_max_rec_minutes);
};

