extern "C" {

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>

}

#include <SDL.h>
#include <SDL_thread.h>

#include <iostream>

#include "PacketQueue.h"
#include "Audio.h"
#include "Media.h"
#include "VideoDisplay.h"
using namespace std;

bool quit = false;

int main(int argv, char* argc[])
{
	av_register_all();

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER);

	char* filename = "E:\\Wildlife.wmv";
	//char* filename = "F:\\test.rmvb";
	MediaState media(filename);

	if (media.openInput())
		SDL_CreateThread(decode_thread, "", &media); // ���������̣߳���ȡpacket�������л���

	media.audio->audio_play(); // create audio thread

	media.video->video_play(&media); // create video thread

	AVStream *audio_stream = media.pFormatCtx->streams[media.audio->stream_index];
	AVStream *video_stream = media.pFormatCtx->streams[media.video->stream_index];

	double audio_duration = audio_stream->duration * av_q2d(audio_stream->time_base);
	double video_duration = video_stream->duration * av_q2d(video_stream->time_base);

	cout << "audioʱ����" << audio_duration << endl;
	cout << "videoʱ����" << video_duration << endl;

	SDL_Event event;
	while (true) // SDL event loop
	{
		SDL_WaitEvent(&event);
		switch (event.type)
		{
		case FF_QUIT_EVENT:
		case SDL_QUIT:
			quit = 1;
			SDL_Quit();

			return 0;
			break;

		case FF_REFRESH_EVENT:
			video_refresh_timer(&media);
			break;

		default:
			break;
		}
	}

	getchar();
	
	return 0;
}