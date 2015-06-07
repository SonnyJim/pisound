#define CFG_VOLUME "volume="
#define CFG_MAXVOICES "max_voices="
#define CFG_AUDIO_CHANNELS "channels="
#define CFG_AUDIO_BUFFERS "audio_buffers="
#define CFG_AUDIO_FORMAT "audio_format="
#define CFG_AUDIO_RATE "audio_rate="
#define CFG_GFX_ENGINE "gfx_enabled="
#define CFG_AUDIO_ENGINE "audio_enabled="
#define CFG_UDP_ENGINE "udp_enabled="
#define CFG_GPIO_ENGINE "gpio_enabled="
#define CFG_SHOW_LOGO   "show_logo="
#define CFG_VERBOSE     "verbose="
#define CFG_FULLSCREEN  "fullscreen="
#define CFG_LISTEN  "listen="

#define DEFAULT_SOUND_FILE "sounds.cfg"
#define DEFAULT_MUSIC_FILE "music.cfg"
#define DEFAULT_CFG_FILE "pisound.cfg"

int cfg_gfx_engine;
int cfg_audio_engine;
int cfg_udp_engine;
int cfg_gpio_engine;
int cfg_fullscreen;
char *cfg_server_host;


int cfg_show_fps;
int cfg_show_logo;

int getopts (int argc, char *argv[]);

int cfg_load_audio (void);
int cfg_load (void);
