
#ifndef VIDEO_H
#define VIDEO_H

#include "SDL.h"
#include "SDL_video.h"

#include "PacketQueue.h"
#include "FrameQueue.h"
#include "Media.h"

struct MediaState;
/**
 * ������Ƶ��������ݷ�װ
 */
struct VideoState
{
	PacketQueue* videoq;        // �����video packet�Ķ��л���

	int stream_index;           // index of video stream
	AVCodecContext *video_ctx;  // have already be opened by avcodec_open2
	AVStream *stream;           // video stream

	FrameQueue frameq;          // ���������ԭʼ֡����
	AVFrame *frame;
	AVFrame *displayFrame;

	double frame_timer;         // Sync fields
	double frame_last_pts;
	double frame_last_delay;
	double video_clock;

	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Texture *bmp;
	SDL_Rect rect;

	void video_play(MediaState *media);

	double synchronize(AVFrame *srcFrame, double pts);
	
	VideoState();

	~VideoState();
};


int decode(void *arg); // ��packet���룬����������Frame����FrameQueue������


#endif