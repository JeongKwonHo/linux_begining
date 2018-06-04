#include <stdio.h>
#include <fcntl.h>
#include <limits.h>
#include <math.h>
#include <sys/ioctl.h>
#include <alsa/asoundlib.h>

#include "wavFile.h"
WAVHEADER wavheader;


int main(int argc, char** argv)
{
	int fd = -1;
	int rc, buf_size, dir;
	int channels, format;
	long loops, count;
	unsigned int val;
	char *buffer;

	snd_pcm_t *handle;
	snd_pcm_hw_params_t *params;
	snd_pcm_uframes_t frames;



	fd = open(argv[1], O_RDONLY);

	count = read(fd, &wavheader, sizeof(WAVHEADER));

	if(snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0 ))
	{
		return -1;
	}


	if(snd_pcm_hw_params_malloc(&params)<0){
		return -1;
	}

	if(snd_pcm_hw_params_any(handle, params) < 0){
		return -1;
	}

	channels = wavheader.nChannels-1;
	printf("Wave Channel Mode : %s \n", (channels)? "Stereo" : "Mono");
	snd_pcm_hw_params_set_channels(handle, params, channels);

	printf("Wave Bytes : %d\n", wavheader.nblockAlign);
	switch(wavheader.nblockAlign) {
		case 1:
			format = SND_PCM_FORMAT_U8;
			break;
		case 2:
			format = (!channels)? SND_PCM_FORMAT_S16_LE : SND_PCM_FORMAT_U8;
			break;
		case 4:
			printf("Stereo Wave file\n");
			format = SND_PCM_FORMAT_S16_LE;
			break;
		default:
			printf("Unknown byte rate for sound\n");
			break;
	}; 
	if(snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED) <0){
		return -1;
	}
	if(snd_pcm_hw_params_set_format(handle, params, format)<0){
		return -1;
	}

	printf("Wave Sampling Rate : 0x%d\n", wavheader.sampleRate);
	val = wavheader.sampleRate;
	if(snd_pcm_hw_params_set_rate_near(handle, params, &val, &dir) < 0){
		return -1;
	}

	frames = 32;
	if(snd_pcm_hw_params_set_period_size_near(handle, params, &frames, &dir) <0){
		return -1;
	}
	if(snd_pcm_hw_params(handle, params)<0){
		return -1;
	}
	snd_pcm_hw_params_get_period_size(params, &frames, &dir);

	buf_size = frames*channels*((format == SND_PCM_FORMAT_S16_LE)?2:1);
	buffer = (char*)malloc(buf_size);
	
	snd_pcm_hw_params_get_period_time(params, &val, &dir);

	do{
		if((count = read(fd, buffer,buf_size)) <=0) break;
		rc = snd_pcm_writei(handle, buffer, frames);
		if(rc == -EPIPE) {
			fprintf(stderr, "Underrun occurred\n");
			snd_pcm_prepare(handle);
		} else if(rc < 0){
			fprintf(stderr, "error from write : %s\n", snd_strerror(rc));
		} else if(rc != (int)frames) {
			fprintf(stderr, "short write, write %d frames \n", rc);
		}
	} while(count == buf_size);

end:
	close(fd);

	snd_pcm_drain(handle);

	snd_pcm_close(handle);
	
	free(buffer);


	return 0;
}


