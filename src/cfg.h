#define CFG_VOLUME "volume="
#define CFG_MAXVOICES "max_voices="
#define CFG_AUDIO_CHANNELS "channels="
#define CFG_AUDIO_BUFFERS "audio_buffers="
#define CFG_AUDIO_FORMAT "audio_format="
#define CFG_AUDIO_RATE "audio_rate="
#define CFG_GFX_ENGINE "gfx_engine="


#define DEFAULT_SOUND_FILE "sounds.cfg"
#define DEFAULT_MUSIC_FILE "music.cfg"
#define DEFAULT_CFG_FILE "pisound.cfg"



int cfg_gfx_engine;
int cfg_show_fps;
int cfg_show_logo;

int getopts (int argc, char *argv[]);
