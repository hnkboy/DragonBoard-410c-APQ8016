/* list1.c
 *  * http://www.linuxjournal.com/article/6735?page=0,1
 *   * 20150911
 *    * display some PCM types and formats
 *     */
#include "alsa/asoundlib.h"

int main()
{

	int val;
	snd_pcm_t *handle;

	printf("ALSA library version: %s\n", SND_LIB_VERSION_STR);

	printf("\n***PCM stream types***:\n");
	for (val = 0;val <= SND_PCM_STREAM_LAST;val++)
		printf("%s\n", snd_pcm_stream_name((snd_pcm_stream_t)val));

	printf("\n***PCM access types***:\n");
	for (val = 0; val <= SND_PCM_ACCESS_LAST; val++)
		printf("%s\n", snd_pcm_access_name((snd_pcm_access_t)val));
		printf("\n***PCM format***:\n");
	for (val = 0; val <= SND_PCM_FORMAT_LAST; val++)
		if (snd_pcm_format_name((snd_pcm_format_t)val) != NULL)
			printf("%s (%s)\n", snd_pcm_format_name((snd_pcm_format_t)val),
								snd_pcm_format_description((snd_pcm_format_t)val));

	printf("\n***PCM subformat***:\n");
	for (val = 0; val <= SND_PCM_SUBFORMAT_LAST; val++)
	printf("%s (%s)\n", snd_pcm_subformat_name((snd_pcm_subformat_t)val),
						snd_pcm_subformat_description((snd_pcm_subformat_t)val));

	printf("\n***PCM states***:\n");
	for (val = 0;val < SND_PCM_STATE_LAST;val++)
		printf("%s\n", snd_pcm_state_name((snd_pcm_state_t)val));

	return 0;
}

