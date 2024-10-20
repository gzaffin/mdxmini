#include <stdio.h>
#include <string.h>
#include <signal.h>
#ifdef _MSC_VER
#include <SDL.h>
#endif // _MSC_VER
#ifdef __GNUC__
#include <SDL.h>
#include <unistd.h>
#include <stdlib.h>
#endif // __GNUC__
#include <getopt.h>


#ifdef USE_ICONV
#ifdef _MSC_VER
#include <windows.h>
#endif // _MSC_VER

#include <iconv.h>
#endif

#include "mdxmini.h"

#include "nlg.h"

NLGCTX *nlgctx;

#define MDXMINI_VERSION __DATE__

int g_viewnote = 0;
int g_verbose = 0;

#define PCM_BLOCK 512
#define PCM_BYTE_PER_SAMPLE 2
#define PCM_CH  2
#define PCM_NUM_BLOCKS 4

#define PCM_BLOCK_SAMPLES_CHANNELS (PCM_BLOCK * PCM_CH)
#define PCM_BUFFER_SAMPLES_CHANNELS (PCM_BLOCK_SAMPLES_CHANNELS * PCM_NUM_BLOCKS)
#define PCM_BLOCK_BYTES (PCM_BLOCK_SAMPLES_CHANNELS * PCM_BYTE_PER_SAMPLE)

static struct pcm_struct
{
    int on;
    int stop;

    int write_pos;
    int read_pos;

    int count;

    short buffer[PCM_BUFFER_SAMPLES_CHANNELS];
} pcm;

#include "wavwrite.h"
#include "fade.h"

/*
// prototypes
*/
static void audio_callback(void *param, Uint8 *data, int len);
static int audio_sdl_init(void);
static int audio_init(int freq);
static void audio_free(void);
static void audio_sig_handle(int sig);
static void audio_info(t_mdxmini *data, int sec, int len);
static int audio_poll_event(void);
#ifdef USE_ICONV
static int conv_with_iconv(char *title_orig, char *title_locale, const char *fromcode);

#endif
static void audio_disp_title(t_mdxmini *data);
//static int split_dir(const char *file , char *dir);
static void audio_loop(t_mdxmini *data, int freq, int len, int nloops);
static void audio_loop_file(t_mdxmini *data, const char *file, int freq , int len, int nloops);
static void usage(void);
int audio_main(int argc, char *argv[]);

/*
// audio_callback
*/

static void audio_callback(void *param, Uint8 *data, int len)
{
    int i;

    short *audio_buffer = (short *)data;

    if (!pcm.on)
    {
        memset(data, 0, len);
        len = 0; /* statement cuts off following for loop */
    }

    for(i = 0; i < len / 2; i++)
    {
        audio_buffer[i] = pcm.buffer[pcm.read_pos++];
        if (pcm.read_pos >= PCM_BUFFER_SAMPLES_CHANNELS)
        {
            pcm.read_pos = 0;
        }
    }
}

/*
// audio_sdl_init
*/

static int audio_sdl_init(void)
{

    if (SDL_Init(SDL_INIT_AUDIO))
    {
        printf("Failed to Initialize!!\n");
        return 1;
    }

    return 0;
}

/*
// audio_init
*/

static int audio_init(int freq)
{
    SDL_AudioSpec af;

    af.freq = freq;
    af.format = AUDIO_S16;
    af.channels = PCM_CH;
    af.samples = PCM_BLOCK;
    af.callback = audio_callback;
    af.userdata = NULL;

    if (SDL_OpenAudio(&af,NULL) < 0)
    {
        printf("Audio open error!!\n");
        return 1;
    }

    memset(&pcm, 0, sizeof(pcm));

    SDL_PauseAudio(0);
    return 0;
}

/*
// audio_free
*/

static void audio_free(void)
{
    SDL_CloseAudio();
    SDL_Quit();
}

/*
// audio_sig_handle
*/

static void audio_sig_handle(int sig)
{
    pcm.stop = 1;
}

/*
// audio_info
*/

static void audio_info(t_mdxmini *data, int sec, int len)
{
    int i,n;
    int notes[32];

    if (g_viewnote)
    {
        n = mdx_get_tracks ( data );

        mdx_get_current_notes( data , notes , n );

        for ( i = 0; i < 8; i ++ )
        {
            printf("%02x " , ( notes[i] & 0xff ) );
        }
        printf(" ");
    }

    printf("Time : %02d:%02d / %02d:%02d\r\r", \
           sec / 60, sec % 60, len / 60, len % 60);
    fflush(stdout);
}

/*
// audio_poll_event
*/

static int audio_poll_event(void)
{
    SDL_Event evt;
    int resVal = 0;

    while ( SDL_PollEvent( &evt ) )
    {
        switch ( evt.type )
        {
            case SDL_QUIT:
                resVal = -1;
            break;
        }
    }

    return resVal;
}

#ifdef USE_ICONV
/*
// conv_with_iconv
*/

static int conv_with_iconv(char *title_orig, char *title_locale, const char *fromcode)
{
    iconv_t icd = iconv_open("UTF-8", fromcode);

    if (icd != (iconv_t)(-1))
    {
        char *srcstr = title_orig;
        char *deststr = title_locale;

        size_t srclen = (NULL != srcstr) ? strlen(srcstr) + 1 : 0;
        size_t destlen = 1024;
        size_t wrtBytes = 0;

        (void)iconv(icd, NULL, NULL, NULL, NULL); // reset conversion state

        wrtBytes = iconv(icd, &srcstr, &srclen, &deststr, &destlen);
        if (wrtBytes == (size_t)-1)
        {
            /*printf("error iconv\n");*/
            return -1;
        }

        iconv_close(icd);
    }
    else
    {
        /*printf("error iconv_open\n");*/
        return -1;
    }

    return 0;
}

#endif

/*
// audio_disp_title
*/

static void audio_disp_title(t_mdxmini *data)
{
    char *title;

    char title_orig[1024] = { 0, };

    title = title_orig;

    mdx_get_title(data, title_orig);

#ifdef USE_ICONV
    char title_locale[1024] = { 0, };

    if (0 == conv_with_iconv(title_orig, title_locale, "SHIFT-JIS"))
    {
        title = title_locale;
    }
    else if (0 == conv_with_iconv(title_orig, title_locale, "CP932"))
    {
        title = title_locale;
    }
    else
    {
        ;
    }
#endif

    if (!g_viewnote)
    {
#ifdef _MSC_VER
        UINT oldCodePage;
        oldCodePage = GetConsoleOutputCP();
        if (!SetConsoleOutputCP(65001)) {
            printf("error\n");
        }
        printf("Title:");
        fwrite(title, 1, strlen(title) + 1, stdout);
        printf("\n");

        SetConsoleOutputCP(oldCodePage);

#else // _MSC_VER
        printf("Title:%s\n", title);

#endif // _MSC_VER

    }
}

/*
// path splitter
*/

//static int split_dir(const char *file , char *dir)
//{
//	char *p;
//	int len = 0;

#ifdef _MSC_VER
//	p = strrchr ((char*)file, '\\');
#else
//	p = strrchr ((char*)file, '/');
#endif

//	if ( p )
//	{
//		len = (int)( p - file );
//		strncpy ( dir , file , len );
//	}
//	dir[ len ] = 0;

//	return len;
//}

/*
// audio_loop
*/

static void audio_loop(t_mdxmini *data, int freq, int len, int nloops)
{

    int total;
    int sec;
    int sec_sample;
    int loop_sec;

    mdx_set_max_loop(data, 1);
    sec = mdx_get_length(data);
    mdx_set_max_loop(data, 2);
    loop_sec = mdx_get_length(data);
    loop_sec -= sec;
    if (loop_sec < 0)
    {
        loop_sec = 0;
    }
    sec -= loop_sec;
    if (-1 == nloops)
    {
        nloops = 1000;
    }
    if (nloops > 1)
    {
        mdx_set_max_loop(data, nloops - 1);
    }
    else
    {
        mdx_set_max_loop(data, 1);
    }

    // playtime
    if (len < 0)
    {
        len = sec + (nloops * loop_sec);
    }
    else
    {
        int loop_sec_prime = (sec + (nloops * loop_sec));
        if (loop_sec_prime < len)
        {
            nloops = len / loop_sec;
            mdx_set_max_loop(data, nloops);
        }
    }
    printf("Play time : %02d:%02d s", len / 60, len % 60);
    len += 5; // added fade-out time
    if (loop_sec > 0)
    {
        if (nloops > 0)
        {
            printf("  loop time : %02d:%02d s  %02d loops\n", loop_sec / 60, loop_sec % 60, nloops);
        }
        else
        {
            printf("  loop time : %02d:%02d s\n", loop_sec / 60, loop_sec % 60);
        }
    }
    else
    {
        printf("\n");
    }
    fflush(stdout);

    fade_init();

    total = sec = sec_sample = 0;

    // put title info
    audio_disp_title(data);

    // put time
    audio_info(data, sec, len);

    do
    {
        if ( ( ( pcm.read_pos < pcm.write_pos ) && ( ( PCM_BUFFER_SAMPLES_CHANNELS - pcm.write_pos + pcm.read_pos ) >= PCM_BLOCK_SAMPLES_CHANNELS ) )
        ||
        ( ( pcm.read_pos > pcm.write_pos ) && ( ( pcm.read_pos - pcm.write_pos ) >= PCM_BLOCK_SAMPLES_CHANNELS ) ) )
        {
            if ( pcm.write_pos >= PCM_BUFFER_SAMPLES_CHANNELS )
            {
                pcm.write_pos = 0;
            }

            // calculate samples
            mdx_calc_sample(data, pcm.buffer + pcm.write_pos, PCM_BLOCK);
            if (is_fade_run())
            {
                fade_stereo(pcm.buffer + pcm.write_pos, PCM_BLOCK);
            }

            pcm.write_pos += PCM_BLOCK_SAMPLES_CHANNELS;
            pcm.count += PCM_BLOCK_SAMPLES_CHANNELS;

            total += PCM_BLOCK;
            sec_sample += PCM_BLOCK;

            // if sec_samples > 1sec
            while (sec_sample >= freq)
            {
                sec_sample -= freq;
                sec++;

                if (sec >= (len - 5)) // 5 is default fade-out time
                {
                    if (!is_fade_run())
                    {
                        fade_start(freq, 1);
                    }
                }
                if (!g_viewnote)
                {
                    audio_info(data, sec, len);
                }
            }

        }
        else // waiting for next block
        {
            if (audio_poll_event() < 0)
            {
                SDL_PauseAudio(1);
            }
            SDL_Delay(1);
        }

        if (g_viewnote)
        {
            audio_info(data, sec, len);
        }

    } while ( ( sec < len ) && ( !pcm.stop ) );

    printf("\n");

    SDL_PauseAudio(1);
}

static void audio_loop_file(t_mdxmini *data, const char *file, int freq, int len, int nloops)
{
    FILE *fp = NULL;

    int sec;
//    int last_sec;
    int loop_sec;

    mdx_set_max_loop(data, 1);
    sec = mdx_get_length(data);
    mdx_set_max_loop(data, 2);
    loop_sec = mdx_get_length(data);
    loop_sec -= sec;
    if (loop_sec < 0)
    {
        loop_sec = 0;
    }
    sec -= loop_sec;
    if (-1 == nloops)
    {
        nloops = 1000;
    }
    if (nloops > 1)
    {
        mdx_set_max_loop(data, nloops - 1);
    }
    else
    {
        mdx_set_max_loop(data, 1);
    }

    int frames;
    int total_frames;

    short pcm_buffer[PCM_BLOCK_SAMPLES_CHANNELS];

    // put title info
    audio_disp_title(data);

    // playtime
    if (len < 0)
    {
        len = sec + (nloops * loop_sec);
    }
    else
    {
        int loop_sec_prime = (sec + (nloops * loop_sec));
        if (loop_sec_prime < len)
        {
            nloops = len / loop_sec;
            mdx_set_max_loop(data, nloops);
        }
    }
    printf("Play time : %02d:%02d s", len / 60, len % 60);
    len += 5; // added fade-out time
    if (loop_sec > 0)
    {
        if (nloops > 0)
        {
            printf("  loop time : %02d:%02d s  %02d loops\n", loop_sec / 60, loop_sec % 60, nloops);
        }
        else
        {
            printf("  loop time : %02d:%02d s\n", loop_sec / 60, loop_sec % 60);
        }
    }
    else
    {
        printf("\n");
    }
    fflush(stdout);

    fade_init();

    sec = \
    frames = \
    total_frames = 0;

    if (file)
    {
        fp = fopen(file, "wb");
    }

    if (file && fp == NULL)
    {
        printf("Can't write a PCM file!\n");
        return;
    }

    audio_write_wav_header(fp, freq, 0);

    audio_info(data, sec, len);

    do
    {
        // calculate samples
        if (fp)
            mdx_calc_sample(data, pcm_buffer, PCM_BLOCK);
        else
            mdx_calc_log(data, pcm_buffer, PCM_BLOCK);

        if (is_fade_run())
            fade_stereo(pcm_buffer, PCM_BLOCK);

        if (fp)
            fwrite(pcm_buffer, PCM_BLOCK_BYTES, 1, fp);

        // increase pointer
        frames += PCM_BLOCK;
        total_frames += PCM_BLOCK;

        /* increase seconds */
        while(frames >= freq)
        {
            frames -= freq;
            sec++;
            audio_info(data, sec, len);

            // start fader
            if (sec >= (len - 5)) // 5 is default fade-out time
            {
                if (!is_fade_run())
                {
                    fade_start(freq, 1);
                }
            }
        }

    } while ( ( sec < len ) && ( !pcm.stop ) );

    audio_write_wav_header(fp, freq, (total_frames * PCM_CH * PCM_BYTE_PER_SAMPLE));

    printf("\n");

    if (fp)
    {
        fclose(fp);
    }
}

/*
// usage
*/

static void usage(void)
{
    printf(
           "Usage mdxplay [options ...] <file> [files ...]\n"
           "\n"
           " Options ...\n"
           " -n nloops : Set loop repetition number (preset = 0, forever = -1 (i.e. nloops == 1000)) \n"
           " -l time   : Set playtime in second\n"
           " -s rate   : Set playback rate\n"
           " -q dir    : Set PCM path\n"
           " -o file   : Generate an Wave file(PCM)\n"
           " -p        : NULL PCM mode.\n"
           " -r file   : Record a NLG\n"
           " -b        : Record a NLG without sound\n"
           " -w        : Set verbose mode\n"
           " -x        : View note mode\n"
           " -h        : Help (this)\n"
           "\n"
           );
}

#define NLG_NORMAL 1
#define NLG_SAMEPATH 2

#ifdef _MSC_VER
    #define _PATH_SEP ";"
#else
    #define _PATH_SEP ":"
#endif

/*
// audio_main
*/

int audio_main(int argc, char *argv[])
{
    t_mdxmini mini;

    char pcmpath_mem[1024];
    char nlgpath_mem[1024];

    char *nlgfile = NULL;
    char *pcmpath = NULL;
    char *wavfile = NULL;

    int opt;

    int rate = 44100;
    int nosound = 0;
    int nlg_log = 0;
    int nloops = 0;
    int len = -1;

#ifdef _WIN32
#if !defined (__MINGW32__)
    (void)freopen("CON", "wt", stdout);
    (void)freopen("CON", "wt", stderr);
#endif
#endif

    audio_sdl_init();
    signal(SIGINT, audio_sig_handle);

    printf("MDXPLAY on SDL Version %s\n", MDXMINI_VERSION);

    if (argc < 2)
    {
        usage();
        return 1;
    }

    while ((opt = getopt(argc, argv, "n:q:l:r:s:o:bpwhx")) != -1)
    {
        switch (opt)
        {
            case 'n': // length
                nloops = atoi(optarg);
                break;
            case 'q': // pcm path
                pcmpath = optarg;
                break;
            case 'l': // length
                len = atoi(optarg);
                break;
            case 'b': // log without path(no arguments)
                nlg_log = NLG_SAMEPATH;
                nlgfile = NULL;
                nosound = 1;
                break;
            case 'r': // log with path
                nlg_log = NLG_NORMAL;
                nlgfile = optarg;
                break;
            case 'p': // no sound(no arguments)
                nosound = 1;
                break;
            case 's': // rate
                rate = atoi(optarg);
                break;
            case 'o': // output to wav file
                wavfile = optarg;
                break;
            case 'w': // verbose mode(no arguments)
                g_verbose = 1;
                break;
            case 'x': // view note mode(no arguments)
                g_viewnote = 1;
                break;
            case 'h':
            default:
                usage();
                return 1;
        }
    }

    if (rate < 8000)
    {
        rate = 8000;
    }

    if (audio_init(rate))
    {
        printf("Failed to initialize audio\n");

        return 0;
    }

    pcm.on = 1;

    mdx_set_rate(rate);

    if (!pcmpath)
    {
        pcmpath_mem[0] = 0;

#ifdef _MSC_VER
        char *home = getenv("USERPROFILE");
#else
        char *home = getenv("HOME");
#endif
        if (home)
        {
            pcmpath = pcmpath_mem;
            strcpy(pcmpath, home);
#ifdef _MSC_VER
            strcat(pcmpath,"\\");
#else
            strcat(pcmpath,"/");
#endif
            strcat(pcmpath, ".mdxplay");
#ifdef _MSC_VER
            strcat(pcmpath,"\\");
#else
            strcat(pcmpath,"/");
#endif
/*            strcat(pcmpath,_PATH_SEP);*/
        }
    }

    // play files
    for(;optind < argc; optind++)
    {

        char *playfile = argv[optind];

        // make NLG log
        if (nlg_log)
        {
            // NLG filename is not given
            if (!nlgfile)
            {
                nlgpath_mem[0] = 0;
                nlgfile = nlgpath_mem;

                strcpy(nlgfile, playfile);

                char *p = strrchr(nlgfile, '.');

                if (p)
                    strcpy(p,".NLG");
                else
                    strcat(nlgfile,".NLG");
            }

            printf("CreateNLG:%s\n",nlgfile);
            nlgctx = CreateNLG(nlgfile);
        }

        // open mdx
        if (mdx_open(&mini, playfile, pcmpath))
        {
            printf("File open error: %s\n", playfile);
            CloseNLG(nlgctx);
            nlgctx = NULL;
            return 0;
        }
        if (mini.mdx->haspdx)
        {
            char pdx_lcl_name[1024];
            pdx_lcl_name[0] = '\0';
            mdx_get_pdxfilename( &mini, pdx_lcl_name );
            if ('\0' != pdx_lcl_name[0])
            {
                printf("PDX File : %s\n", pdx_lcl_name);
            }
        }
        else
        {
          if (NULL == mini.pdx)
          {
              char pdx_lcl_name[1024];
              pdx_lcl_name[0] = '\0';
              mini.mdx->haspdx = 1; /* forced value trying to print not found PDX */
              mdx_get_pdxfilename( &mini, pdx_lcl_name );
              if ('\0' != pdx_lcl_name[0])
              {
                  printf("PDX File : %s NOT FOUND\n", pdx_lcl_name);
              }
              mini.mdx->haspdx = 0; /* restored value after use */
          }
        }

        if (nosound || wavfile)
        {
            audio_loop_file(&mini, wavfile, rate, len, nloops);
        }
        else
        {
            audio_loop(&mini, rate, len, nloops);
        }

        CloseNLG(nlgctx);
        nlgfile = NULL;
        nlgctx = NULL;

        // close mdx
        mdx_close(&mini);
    }

    audio_free();

    return 0;
}

// disable SDLmain for win32 console app
#ifdef _WIN32
#undef main
#endif

/*
// main
*/

int main(int argc, char *argv[])
{
    int ret = audio_main(argc, argv);

#ifdef DEBUG
    getch();
#endif

    return ret;
}
