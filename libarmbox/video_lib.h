#ifndef __VIDEO_LIB_H__
#define __VIDEO_LIB_H__

#include <linux/dvb/video.h>
#include "cs_types.h"
#include "dmx_hal.h"

typedef struct cs_vs_format_t
{
	char format[16];
} cs_vs_format_struct_t;

#if BOXMODEL_VUPLUS_ARM || BOXMODEL_DM820 || BOXMODEL_DM900
typedef enum
{
	HDMI_COLORIMETRY_AUTO,
	HDMI_COLORIMETRY_BT709,
	HDMI_COLORIMETRY_BT470
} HDMI_COLORIMETRY;
#else
typedef enum
{
	HDMI_COLORIMETRY_AUTO,
	HDMI_COLORIMETRY_BT2020NCL,
	HDMI_COLORIMETRY_BT2020CL,
	HDMI_COLORIMETRY_BT709
} HDMI_COLORIMETRY;
#endif

typedef enum
{
	VIDEO_FORMAT_MPEG2 = 0,
	VIDEO_FORMAT_MPEG4_H264,
	VIDEO_FORMAT_VC1,
	VIDEO_FORMAT_JPEG,
	VIDEO_FORMAT_GIF,
	VIDEO_FORMAT_PNG,
	VIDEO_FORMAT_MPEG4_H265,
	VIDEO_FORMAT_AVS = 16
} VIDEO_FORMAT;

typedef enum
{
	VIDEO_FRAME_RATE_23_976 = 0,
	VIDEO_FRAME_RATE_24,
	VIDEO_FRAME_RATE_25,
	VIDEO_FRAME_RATE_29_97,
	VIDEO_FRAME_RATE_30,
	VIDEO_FRAME_RATE_50,
	VIDEO_FRAME_RATE_59_94,
	VIDEO_FRAME_RATE_60
} VIDEO_FRAME_RATE;

typedef enum
{
	DISPLAY_AR_1_1,
	DISPLAY_AR_4_3,
	DISPLAY_AR_16_9,
	DISPLAY_AR_20_9,
	DISPLAY_AR_RAW
} DISPLAY_AR;

typedef enum
{
	DISPLAY_AR_MODE_PANSCAN = 0,
	DISPLAY_AR_MODE_LETTERBOX,
	DISPLAY_AR_MODE_NONE
} DISPLAY_AR_MODE;

typedef enum
{
	VIDEO_STD_NTSC,
	VIDEO_STD_PAL,
	VIDEO_STD_SECAM,
	VIDEO_STD_480P,
	VIDEO_STD_576P,
	VIDEO_STD_720P50,
	VIDEO_STD_720P60,
	VIDEO_STD_1080I25,
	VIDEO_STD_1080I50,
	VIDEO_STD_1080I60,
	VIDEO_STD_1080P24,
	VIDEO_STD_1080P25,
	VIDEO_STD_1080P30,
	VIDEO_STD_1080P50,
	VIDEO_STD_1080P60,
	VIDEO_STD_2160P24,
	VIDEO_STD_2160P25,
	VIDEO_STD_2160P30,
	VIDEO_STD_2160P50,
	VIDEO_STD_2160P60,
	VIDEO_STD_AUTO,
	VIDEO_STD_MAX = VIDEO_STD_AUTO
} VIDEO_STD;

typedef enum
{
	VIDEO_HDMI_CEC_MODE_OFF = 0,
	VIDEO_HDMI_CEC_MODE_TUNER = 3,
	VIDEO_HDMI_CEC_MODE_RECORDER = 1
} VIDEO_HDMI_CEC_MODE;

typedef enum
{
	VIDEO_HDMI_CEC_VOL_OFF = 0,
	VIDEO_HDMI_CEC_VOL_AUDIOSYSTEM = 1,
	VIDEO_HDMI_CEC_VOL_TV = 2
} VIDEO_HDMI_CEC_VOL;

typedef enum
{
	VIDEO_CONTROL_BRIGHTNESS = 0,
	VIDEO_CONTROL_CONTRAST,
	VIDEO_CONTROL_SATURATION,
	VIDEO_CONTROL_HUE,
	VIDEO_CONTROL_SHARPNESS,
	VIDEO_CONTROL_BLOCK_NOISE_REDUCTION,
	VIDEO_CONTROL_MOSQUITO_NOISE_REDUCTION,
	VIDEO_CONTROL_DIGITAL_CONTOUR_REMOVAL,
	VIDEO_CONTROL_AUTO_FLESH,
	VIDEO_CONTROL_GREEN_BOOST,
	VIDEO_CONTROL_BLUE_BOOST,
	VIDEO_CONTROL_DYNAMIC_CONTRAST,
	VIDEO_CONTROL_SCALER_SHARPNESS,
	VIDEO_CONTROL_ZAPPING_MODE,
	VIDEO_CONTROL_MAX = VIDEO_CONTROL_SHARPNESS
} VIDEO_CONTROL;

class cDemux;
class cPlayback;

class cVideo
{
		friend class cPlayback;
		friend class cDemux;
	private:
		/* video device */
		int fd;
		bool fdd;
		unsigned int devnum;
		/* apparently we cannot query the driver's state
		   => remember it */
		video_play_state_t playstate;
		int /*vidDispMode_t*/ croppingMode;
		int /*vidOutFmt_t*/ outputformat;

		VIDEO_FORMAT StreamType;
		DISPLAY_AR DisplayAR;
		DISPLAY_AR_MODE ARMode;
		DISPLAY_AR PictureAR;
		VIDEO_FRAME_RATE FrameRate;
		int video_standby;
		int brightness;
		int contrast;
		int saturation;
		int hue;
		int sharpness;
		int block_noise_reduction;
		int mosquito_noise_reduction;
		int digital_contour_removal;
		int auto_flesh;
		int green_boost;
		int blue_boost;
		int dynamic_contrast;
		int scaler_sharpness;
		int zapping_mode;
		int blank_mode;

		/* used internally by dmx */
		int64_t GetPTS(void);
	public:
		/* constructor & destructor */
		cVideo(int mode, void *, void *, unsigned int unit = 0);
		~cVideo(void);

		/* used internally by playback */
		void openDevice(void);
		void closeDevice(void);

		void open_AVInput_Device(void);
		void close_AVInput_Device(void);

		void setAVInput(int val);

		void *GetTVEnc()
		{
			return NULL;
		};
		void *GetTVEncSD()
		{
			return NULL;
		};

		/* aspect ratio */
		int getAspectRatio(void);
		void getPictureInfo(int &width, int &height, int &rate);
		int setAspectRatio(int aspect, int mode);

		/* cropping mode */
		int setCroppingMode(int x = 0 /*vidDispMode_t x = VID_DISPMODE_NORM*/);

		/* get play state */
		int getPlayState(void);

		/* blank on freeze */
		int getBlank(void);
		int setBlank(int enable);

		/* change video play state. Parameters are all unused. */
		int Start(void *PcrChannel = NULL, unsigned short PcrPid = 0, unsigned short VideoPid = 0, void *x = NULL);
		int Stop(bool blank = true);
		bool Pause(void);

		/* get video system infos */
		int GetVideoSystem(void);
		/* when system = -1 then use current video system */
		void GetVideoSystemFormatName(cs_vs_format_t *format, int system);

		/* set video_system */
		int SetVideoSystem(int video_system, bool remember = true);
		int SetStreamType(VIDEO_FORMAT type);
		void SetSyncMode(AVSYNC_TYPE mode);
		bool SetCECMode(VIDEO_HDMI_CEC_MODE);
		void SetCECAutoView(bool);
		void SetCECAutoStandby(bool);
		int  GetAudioDestination();
		void SetAudioDestination(int audio_dest);
		bool ShowPicture(const char *fname);
		void StopPicture();
		void Standby(unsigned int bOn);
		void ShowPig(int _x);
		void Pig(int x, int y, int w, int h, int osd_w = 1064, int osd_h = 600, int startx = 0, int starty = 0, int endx = 1279, int endy = 719);
		void SetControl(int, int);
		void setContrast(int val);
		void QuadPiP(bool active = false, int _x = 0, int _y = 0, int _w = 360, int _h = 288);

		void SetAudioHandle(void *)
		{
			return;
		};
		int  OpenVBI(int)
		{
			return 0;
		};
		int  CloseVBI(void)
		{
			return 0;
		};
		int  StartVBI(unsigned short)
		{
			return 0;
		};
		int  StopVBI(void)
		{
			return 0;
		};
		void SetDemux(cDemux *dmx);
		void SetHDMIColorimetry(HDMI_COLORIMETRY hdmi_colorimetry);
		bool GetScreenImage(unsigned char*&data, int &xres, int &yres, bool get_video = true, bool get_osd = false, bool scale_to_video = false);
};

#endif // __VIDEO_LIB_H__
