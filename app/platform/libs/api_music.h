#ifndef _API_MUSIC_H
#define _API_MUSIC_H

typedef enum {
    NONE_TYPE = 0,
    WAV_TYPE,
    MP3_TYPE,
    SBC_TYPE,
    APE_TYPE,
    FLAC_TYPE,
    WMA_TYPE,
    ESBC_TYPE,
    AAC_TYPE,
} enum_msc_type_t;

enum {
    MUSIC_STOP = 0,
    MUSIC_PAUSE,
    MUSIC_PLAYING,
};

enum {
    //decode msg
    MUSIC_MSG_STOP = 0,
    MUSIC_MSG_PAUSE,
    MUSIC_MSG_PLAY,
};
typedef struct {
    volatile u8  sta;
    u8  type;
} music_cb_t;
extern music_cb_t music_cb;

typedef struct sbc_struct {
	unsigned long flags;

	uint8_t frequency;
	uint8_t blocks;
	uint8_t subbands;
	uint8_t mode;
	uint8_t allocation;
	uint8_t bitpool;

	struct sbc_priv *priv;
} sbc_t;

typedef struct {
    u8 sbc_type;    //0:sbc  1:msbc 2:bt_msbc
    sbc_t sbc;
    u8 buf[256];
} sbc_dec_t;
extern sbc_dec_t sbc_dec;

extern bool (*sbcdec_frame)(void);


void music_control(u8 msg);
u8 get_music_dec_sta(void);
void music_sbc_play(u8 msg);
bool music_is_playing(void);

int sbc_dec_init(void);
bool sbc_dec_frame(void);
void sbc_decode_init(void);
void bt_decode_exit(void);

int sbc_decode(sbc_t *sbc, const void *input, size_t input_len);
void sbcdec_end(void);
void bt_set_sbc_playing(u8 enable);

void esbc_init(const u8 *ptr, u32 size);
u8 res_play_start(uint index);
u8 res_play_start_do(uint index);
void res_play_stop(u8 dnr_sta);
int esbc_res_play(u8 index, const u8 *ptr, const u8 *sptr, u32 size, u32 tws_sync);
bool bt_ring_tone_play(u32 tws_sync);
bool bt_ring_tone_stop(u32 force);

void tws_wav_res_play_kick(u32 addr, u32 len);
void wav_res_stop(void);
void wav_res_play_kick(u32 addr, u32 len, bool tws_sync);
void wav_res_dec_process(void);
bool wav_res_is_play(void);

#endif // _API_MUSIC_H
