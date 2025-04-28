#ifndef __CONFIG_EXTRA_H__
#define __CONFIG_EXTRA_H__

/*****************************************************************************
 * Module    : 系统功能配置
 *****************************************************************************/

#if !AUX_2_SDADC_EN
#undef AUX_REC_EN
#define AUX_REC_EN                  0
#endif

#if IODM_TEST_MODE
#undef  VUSB_SMART_VBAT_HOUSE_EN
#undef  BT_DUT_TEST_EN
#undef  BT_FCC_TEST_EN
#define VUSB_SMART_VBAT_HOUSE_EN   1
#define BT_DUT_TEST_EN             1
#define BT_FCC_TEST_EN             1
#endif

#if !VUSB_TBOX_QTEST_EN
#undef VUSB_TBOX_NEW_QTEST_EN
#define VUSB_TBOX_NEW_QTEST_EN     0
#endif

#if !VUSB_TBOX_NEW_QTEST_EN
#undef VUSB_TBOX_NEW_QTEST_RECORD_TWS_INFO_EN
#define VUSB_TBOX_NEW_QTEST_RECORD_TWS_INFO_EN    0
#endif

#if VUSB_HUART_DMA_EN && VUSB_TBOX_QTEST_EN
#undef VUSB_TBOX_NEW_QTEST_EN
#define VUSB_TBOX_NEW_QTEST_EN      1
#endif



#if BT_TWS_FIX_LR_SAME_CH_MAC_EN
#undef VUSB_TBOX_NEW_QTEST_EN
#define VUSB_TBOX_NEW_QTEST_EN   1
#endif

#ifndef EQ_CHECK_RES_SEL
#define EQ_CHECK_RES_SEL        	   0
#endif

#if !BT_ALG_DBB_EN
#if (EQ_CHECK_RES_SEL == 2)
#undef EQ_CHECK_RES_SEL
#define EQ_CHECK_RES_SEL               0
#endif
#if (EQ_CHECK_RES_SEL == 3)
#undef EQ_CHECK_RES_SEL
#define EQ_CHECK_RES_SEL               1
#endif
#endif

/*****************************************************************************
 * Module    : 音乐功能配置
 *****************************************************************************/
#if !FUNC_MUSIC_EN
#undef  MUSIC_UDISK_EN
#undef  MUSIC_SDCARD_EN

#undef  MUSIC_WAV_SUPPORT
#undef  MUSIC_WMA_SUPPORT
#undef  MUSIC_APE_SUPPORT
#undef  MUSIC_FLAC_SUPPORT
#undef  MUSIC_SBC_SUPPORT

#define MUSIC_UDISK_EN              0
#define MUSIC_SDCARD_EN             0
#define MUSIC_WAV_SUPPORT           0
#define MUSIC_WMA_SUPPORT           0
#define MUSIC_APE_SUPPORT           0
#define MUSIC_FLAC_SUPPORT          0
#define MUSIC_SBC_SUPPORT           0
#endif // FUNC_MUSIC_EN




#if !CHARGE_EN
#undef  CHARGE_TRICK_EN
#undef  CHARGE_DC_NOT_PWRON
#undef CHARGE_VOLT_INSTABILITY_FIX_EN
#define CHARGE_TRICK_EN                         0
#define CHARGE_DC_NOT_PWRON                     0
#define CHARGE_VOLT_INSTABILITY_FIX_EN          0
#else
#ifndef CHARGE_VOLT_INSTABILITY_FIX_EN
#define CHARGE_VOLT_INSTABILITY_FIX_EN          0
#endif
#endif

#if !MUSIC_SDCARD_EN
#undef SDCLK_MUX_DETECT_SD
#undef SD_DETECT_INIT
#undef SD_IS_ONLINE
#undef IS_DET_SD_BUSY
#define SD_DETECT_INIT()
#define SDCLK_MUX_DETECT_SD         0
#define SD_IS_ONLINE()              0
#define IS_DET_SD_BUSY()            0
#endif

#if !EARPHONE_DETECT_EN
#undef SDCMD_MUX_DETECT_EARPHONE
#undef EARPHONE_DETECT_INIT
#undef EARPHONE_IS_ONLINE
#undef IS_DET_EAR_BUSY
#define EARPHONE_DETECT_INIT()
#define SDCMD_MUX_DETECT_EARPHONE   0
#define EARPHONE_IS_ONLINE()        0
#define IS_DET_EAR_BUSY()           0
#endif

#if !LINEIN_DETECT_EN
#undef SDCMD_MUX_DETECT_LINEIN
#undef LINEIN_DETECT_INIT
#undef LINEIN_IS_ONLINE
#undef IS_DET_LINEIN_BUSY

#define LINEIN_DETECT_INIT()
#define SDCMD_MUX_DETECT_LINEIN     0
#define LINEIN_IS_ONLINE()          0
#define IS_DET_LINEIN_BUSY()        0
#endif

#if !BUZZER_EN
#undef BUZZER_INIT
#define BUZZER_INIT()
#undef BUZZER_ON
#define BUZZER_ON()
#undef BUZZER_OFF
#define BUZZER_OFF()
#endif

#ifndef PORT_2LED_EN
#define PORT_2LED_EN                0
#endif // PORT_2LED_EN

#ifndef RLED_LOWBAT_EN
#define RLED_LOWBAT_EN              0
#endif // RLED_LOWBAT_EN

#if !LED_DISP_EN
#undef PORT_2LED_EN
#undef RLED_LOWBAT_EN
#undef BLED_FFT_EN
#undef BLED_LOW2ON_EN
#undef BLED_CHARGE_FULL
#define PORT_2LED_EN                0
#define RLED_LOWBAT_EN              0
#define BLED_FFT_EN                 0
#define BLED_LOW2ON_EN              0
#define BLED_CHARGE_FULL            0
#endif

#if !LED_PWR_EN
#undef LED_PWR_INIT
#define LED_PWR_INIT()
#undef LED_PWR_IS_ON
#define LED_PWR_IS_ON()             0
#undef LED_PWR_SET_ON
#define LED_PWR_SET_ON()
#undef LED_PWR_SET_OFF
#define LED_PWR_SET_OFF()
#endif

#if !LOUDSPEAKER_MUTE_EN
#undef LOUDSPEAKER_MUTE_INIT
#define LOUDSPEAKER_MUTE_INIT()
#undef LOUDSPEAKER_MUTE_DIS
#define LOUDSPEAKER_MUTE_DIS()
#undef LOUDSPEAKER_MUTE
#define LOUDSPEAKER_MUTE()
#undef LOUDSPEAKER_UNMUTE
#define LOUDSPEAKER_UNMUTE()
#undef LOUDSPEAKER_UNMUTE_DELAY
#define LOUDSPEAKER_UNMUTE_DELAY        0
#endif

#ifndef IR_INPUT_NUM_MAX
#define IR_INPUT_NUM_MAX                999         //最大输入数字9999
#endif // IR_INPUT_NUM_MAX

#if !USER_TKEY
#undef  USER_TKEY_INEAR
#undef  USER_TKEY_DEBUG_EN
#undef  USER_TKEY_SLIDE_OLD
#undef  USER_TKEY_SLIDE
#undef  USER_TKEY_SHORT_SLIDE
#undef  USER_TKEY_MULTI_EN
#define USER_TKEY_INEAR                 0
#define USER_TKEY_DEBUG_EN              0
#define USER_TKEY_SLIDE_OLD             0
#define USER_TKEY_SLIDE                 0
#define USER_TKEY_SHORT_SLIDE           0
#define USER_TKEY_MULTI_EN              0
#endif

#if USER_TKEY_SLIDE_OLD
#undef  USER_TKEY_SLIDE
#undef  USER_TKEY_MULTI_EN
#undef  USER_TKEY_SHORT_SLIDE
#define USER_TKEY_SLIDE                 0
#define USER_TKEY_SHORT_SLIDE           0
#define USER_TKEY_MULTI_EN              0
#endif

#if !USER_TKEY_SLIDE
#undef  USER_TKEY_SLIDE_DEBUG_EN
#define USER_TKEY_SLIDE_DEBUG_EN        0
#endif

#if !USER_TKEY_SHORT_SLIDE
#undef  USER_TKEY_SHORT_SLIDE_DEBUG_EN
#define USER_TKEY_SHORT_SLIDE_DEBUG_EN  0
#endif

#if !USER_TKEY_MULTI_EN
#undef  USER_TKEY_MULTI_DEBUG_EN
#undef  USER_TKEY_PB5_DISABLE
#undef  USER_TKEY_PB4_EN
#undef  USER_TKEY_PB3_EN
#undef  USER_TKEY_PA7_EN
#define USER_TKEY_MULTI_DEBUG_EN        0
#define USER_TKEY_PB5_DISABLE           0
#define USER_TKEY_PB4_EN                0
#define USER_TKEY_PB3_EN                0
#define USER_TKEY_PA7_EN                0
#endif


#ifndef SYS_SUPPORT_DONGLE_EN
#define SYS_SUPPORT_DONGLE_EN           0
#endif

/*****************************************************************************
 * Module    : 录音功能配置
 *****************************************************************************/
#if !FUNC_REC_EN
#undef  FMRX_REC_EN
#undef  AUX_REC_EN
#undef  MIC_REC_EN
#undef REC_TYPE_SEL

#define FMRX_REC_EN                 0
#define AUX_REC_EN                  0
#define MIC_REC_EN                  0
#define REC_TYPE_SEL                REC_NO
#endif //FUNC_REC_EN


/*****************************************************************************
 * Module    : 蓝牙相关配置
 *****************************************************************************/
#ifndef BT_A2DP_RECON_EN
#define BT_A2DP_RECON_EN            0
#endif

#if !LE_EN
#undef LE_LIGHTING_EN
#undef LE_MUSIC_CTRL_EN
#define LE_LIGHTING_EN              0
#define LE_MUSIC_CTRL_EN            0
#endif

#if LE_LIGHTING_EN || LE_MUSIC_CTRL_EN
    #define BT_DUAL_MODE_EN         1
#else
    #define BT_DUAL_MODE_EN         0
#endif

#if !BT_LIGHTNINIG_EN
#undef  SYS_PARAM_EEPROM
#undef  BT_QUICK_TEST_EN
#define SYS_PARAM_EEPROM            0
#define BT_QUICK_TEST_EN            0
#endif

#ifndef BT_AAC_AUDIO_EN
#define BT_AAC_AUDIO_EN             0
#endif // BT_AAC_AUDIO_EN

#if !BT_A2DP_EN
#undef  BT_AAC_AUDIO_EN
#define BT_AAC_AUDIO_EN             0
#endif

#if BT_SCO_MAV_EN
#undef BT_SCO_FAR_NR_EN
#define BT_SCO_FAR_NR_EN           0
#endif // BT_SCO_MAV_EN

#ifndef BT_TWS_CUSTOMER_CONTROL_TWSSCAN_STA_EN
#define BT_TWS_CUSTOMER_CONTROL_TWSSCAN_STA_EN    0   //客户控制tws scan状态使能
#endif

#if !BT_A2DP_DEFAULT_VOLUME_EN
#undef  BT_A2DP_IOS_DEFAULT_VOLUME_FIX
#define BT_A2DP_IOS_DEFAULT_VOLUME_FIX            0
#endif

/*****************************************************************************
 * Module    : usb device 功能配置
 *****************************************************************************/
#if !FUNC_USBDEV_EN
#undef  UDE_STORAGE_EN
#undef  UDE_SPEAKER_EN
#undef  UDE_HID_EN
#undef  UDE_MIC_EN
#undef  UDE_ENUM_TYPE

#define UDE_STORAGE_EN              0
#define UDE_SPEAKER_EN              0
#define UDE_HID_EN                  0
#define UDE_MIC_EN                  0
#define UDE_ENUM_TYPE               0
#endif

#if !FUNC_AUX_EN
#undef AUX_2_SDADC_EN
#undef LINEIN_DETECT_EN
#undef LINEIN_2_PWRDOWN_EN
#undef LINEIN_2_PWRDOWN_TONE_EN
#undef WARNING_FUNC_AUX

#define AUX_2_SDADC_EN              0
#define LINEIN_DETECT_EN            0
#define LINEIN_2_PWRDOWN_EN         0
#define LINEIN_2_PWRDOWN_TONE_EN    0
#define WARNING_FUNC_AUX            0
#endif

#if !LINEIN_DETECT_EN
#undef SDCMD_MUX_DETECT_LINEIN
#undef LINEIN_DETECT_INIT
#undef LINEIN_IS_ONLINE
#undef IS_DET_LINEIN_BUSY

#define LINEIN_DETECT_INIT()
#define SDCMD_MUX_DETECT_LINEIN     0
#define LINEIN_IS_ONLINE()          0
#define IS_DET_LINEIN_BUSY()        0
#endif


/*****************************************************************************
 * Module    : 提示音配置
 *****************************************************************************/
#if (!WARNING_TONE_EN)
#undef WARNING_POWER_ON
#undef WARNING_POWER_OFF
#undef WARNING_FUNC_MUSIC
#undef WARNING_FUNC_BT
#undef WARNING_FUNC_CLOCK
#undef WARNING_FUNC_FMRX
#undef WARNING_FUNC_AUX
#undef WARNING_FUNC_USBDEV
#undef WARNING_FUNC_SPEAKER
#undef WARNING_FUNC_SPDIF
#undef WARNING_LOW_BATTERY
#undef WARNING_BT_CONNECT
#undef WARNING_BT_DISCONNECT
#undef WARNING_BT_INCALL
#undef WARNING_USB_SD
#undef WARNING_BT_HID_MENU
#undef WARNING_BTHID_CONN
#undef WARNING_MAX_VOLUME
#undef WARNING_BT_PAIR

#define WARNING_POWER_ON               0
#define WARNING_POWER_OFF              0
#define WARNING_FUNC_MUSIC             0
#define WARNING_FUNC_BT                0
#define WARNING_FUNC_CLOCK             0
#define WARNING_FUNC_FMRX              0
#define WARNING_FUNC_AUX               0
#define WARNING_FUNC_USBDEV            0
#define WARNING_FUNC_SPEAKER           0
#define WARNING_FUNC_SPDIF             0
#define WARNING_LOW_BATTERY            0
#define WARNING_BT_CONNECT             0
#define WARNING_BT_DISCONNECT          0
#define WARNING_BT_INCALL              0
#define WARNING_USB_SD                 0
#define WARNING_BT_HID_MENU            0
#define WARNING_BTHID_CONN             0
#define WARNING_MAX_VOLUME             0
#define WARNING_BT_PAIR                0
#endif

#if ((!MUSIC_UDISK_EN) && (!MUSIC_SDCARD_EN))
#undef  USB_SD_UPDATE_EN
#define USB_SD_UPDATE_EN               0
#endif

#ifndef UPD_FILENAME
#define UPD_FILENAME                   "fw5000.upd"
#endif

#define USB_SUPPORT_EN                 (MUSIC_UDISK_EN | FUNC_USBDEV_EN)
#define SD_SUPPORT_EN                  (MUSIC_SDCARD_EN)
#define SDCMD_MUX_DETECT_EN            (SDCMD_MUX_DETECT_EARPHONE | SDCMD_MUX_DETECT_LINEIN)


#endif // __CONFIG_EXTRA_H__

