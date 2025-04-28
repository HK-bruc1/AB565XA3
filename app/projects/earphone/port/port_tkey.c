#include "include.h"

#if USER_TKEY
AT(.rodata.tkey)
const tkey_ch_t tkey0 = {
    .cdpr = CDPR,
    .type = 1,
    .ctrim = 0,
    .itrim = 0x14,
};

//AT(.rodata.tkey)
//const tkey_ch_t tkey1 = {
//    .cdpr = CDPR,
//    .type = 1,
//    .ctrim = 0,
//    .itrim = 0x14,
//};

AT(.rodata.tkey)
const tkey_ch_t tkey_inear = {
    .cdpr = EAR_CDPR,
    .type = 2,
    .ctrim = 0,
    .itrim = 0x14,
};

//calibration专用
AT(.rodata.tkey)
const tkey_ch_t tkey_temp_cali_ch = {
    .cdpr = 180,
    .type = 0,
    .ctrim = 0x3f,
    .itrim = 0x14,
};

AT(.rodata.tkey)
const tkey_cfg_t tkey_cfg = {
    .key[0] = &tkey0,               //WKO
    .key[1] = NULL,                 //PB4
    .key[2] = NULL,                 //PB3
    .key[3] = NULL,                 //PA7

    //TKTMR Register
    .fil_low = FIL_LOW,
    .fil_high = FIL_HIGH,
    .fil_except = FIL_EXCEPT,
    .fil_val = FIL_VAL,
    .to_except = TO_EXCEPT,

    //TKPTHD Register
    .tkpthd = TKPTHRESH,
    .tkrthd = TKRTHRESH,
    .pto_except = PTO_EXCEPT,

    //TKETHD Register
    .tksthd = SMALLTHD,
    .tklthd = LARGETHD,

    //TKVARI Register
    .tkvthd = 32,

    //TKVARITHD Register
    .tkarthd = TKRTHRESH-1,
    .tkaethd = SMALLTHD,
    .tkvfil  = FIL_EXCEPT,
    .tkbadd  = SMALLTHD,

    //TKCON2 Register
     .press_vari_en = 0,
     .rels_vari_en = 0,
     .press_fil_sel = 1,
     .rels_fil_sel = 1,
     .press_vari_thd = 10,
     .rels_vari_thd = 10,
     .to_bcnt_thd = 10,

    //TETMR Register
    .ear_fil_low = EAR_FIL_LOW,
    .ear_fil_high = EAR_FIL_HIGH,
    .ear_fil_except = EAR_FIL_EXCEPT,
    .ear_fil_val = EAR_FIL_VAL,
    .tkpwup = TKPWUP,

    //TEPTHD Register
    .tepthd = TEPTHRESH,
    .terthd = TERTHRESH,
    .tefathd = TEFATHD,

    //TEETHD Register
    .testhd = TE_SMALLTHD,
    .telthd = TE_LARGETHD,
};

#endif
