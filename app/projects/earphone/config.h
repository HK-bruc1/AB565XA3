/*****************************************************************************
 * Module    : Config
 * File      : config.h
 * Function  : SDK配置文件
 *****************************************************************************/

#ifndef USER_CONFIG_H
#define USER_CONFIG_H
#include "config_define.h"

/*****************************************************************************
 * Module    : Function选择相关配置
 *****************************************************************************/
#define FUNC_MUSIC_EN                   0   //是否打开MUSIC功能
#define FUNC_CLOCK_EN                   0   //是否打开时钟功能
#define FUNC_FMRX_EN                    0   //是否打开FM功能
#define FUNC_BT_EN                      1   //是否打开蓝牙功能
#define FUNC_BTHID_EN                   0   //是否打开独立自拍器模式
#define FUNC_AUX_EN                     0   //是否打开AUX功能
#define FUNC_USBDEV_EN                  0   //是否打开USB DEVICE功能
#define FUNC_SPEAKER_EN                 0   //是否打开Speaker模式
#define FUNC_IDLE_EN                    0   //是否打开IDLE功能

/*****************************************************************************
 * Module    : 系统功能选择配置
 *****************************************************************************/
#define BUCK_MODE_EN                    1                           //是否BUCK MODE
#define SYS_CLK_SEL                     (xcfg_cb.sys_clk + SYS_26M) //选择系统时钟
#define POWKEY_10S_RESET                xcfg_cb.powkey_10s_reset
#define SOFT_POWER_ON_OFF               xcfg_cb.soft_power_on_off   //是否使用软开关机功能
#define USB_SD_UPDATE_EN                0                           //是否支持UDISK/SD的离线升级
#define GUI_SELECT                      GUI_NO                      //GUI Display Select
#define FLASH_SIZE                      0x20000                     //128K Bytes
#define SPIFLASH_SPEED_UP_EN            0                           //SPI FLASH提速。
#define UART0_PRINTF_SEL                PRINTF_NONE                 //选择UART打印信息输出IO，或关闭打印信息输出
#define SYS_SUPPORT_DONGLE_EN           0                           //系统支持加密狗，注意还需自行修改xcfg.xm文件中osc_cap和soft_key

/*****************************************************************************
 * Module    : 音乐功能配置
 *****************************************************************************/
#define MUSIC_UDISK_EN                  0   //是否支持播放UDISK
#define MUSIC_SDCARD_EN                 0   //是否支持播放SDCARD

#define MUSIC_WAV_SUPPORT               0   //是否支持WAV格式解码
#define MUSIC_WMA_SUPPORT               0   //是否支持WMA格式解码
#define MUSIC_APE_SUPPORT               0   //是否支持APE格式解码
#define MUSIC_FLAC_SUPPORT              0   //是否支持FLAC格式解码
#define MUSIC_SBC_SUPPORT               0   //是否支持SBC格式解码

#define MUSIC_FOLDER_SELECT_EN          0   //文件夹选择功能
#define MUSIC_AUTO_SWITCH_DEVICE        0   //双设备循环播放
#define MUSIC_BREAKPOINT_EN             0   //音乐断点记忆播放
#define MUSIC_QSKIP_EN                  0   //快进快退功能
#define MUSIC_PLAYMODE_NUM              4   //音乐播放模式总数


/*****************************************************************************
 * Module    : 蓝牙功能配置
 *****************************************************************************/
#define BT_BACKSTAGE_EN                 0   //蓝牙后台管理（全模式使用蓝牙）
#define BT_BACKSTAGE_PLAY_DETECT_EN     0   //非蓝牙模式下检测到手机蓝牙播放音乐，则切换到蓝牙模式
#define BT_NAME_DEFAULT                 "AB565XA3-BOX"     //默认蓝牙名称（不超过31个字符）
#define BT_NAME_WITH_ADDR_EN            0   //蓝牙名称是否附加地址信息（调试用，例如：btbox-***）
#define BT_POWER_UP_RECONNECT_TIMES     3   //上电回连次数
#define BT_TIME_OUT_RECONNECT_TIMES     20  //掉线回连次数
#define BT_TWS_TIME_OUT_RECONNECT_TIMES 5   //TWS 超时断线回连次数
#define BT_NOR_RECONNECT_TIMES          5   //对耳回连手机次数
#define BT_TWS_RECONNECT_TIMES          5   //对耳TWS回连次数
#define BT_SIMPLE_PAIR_EN               1   //是否打开蓝牙简易配对功能（关闭时需要手机端输入PIN码）
#define BT_DISCOVER_CTRL_EN             0   //是否使用按键打开可被发现（按一下按键才能被连接配对）
#define BT_PWRKEY_5S_DISCOVER_EN        1   //是否使用长按5S开机进入可被发现(耳机长按开机功能)
#define BT_DISCOVER_TIMEOUT             100 //按键打开可被发现后，多久后仍无连接自动关闭，0不自动关闭，单位100ms
#define BT_ANTI_LOST_EN                 0   //是否打开蓝牙防丢报警
#define BT_DUT_TEST_EN                  1//0   //蓝牙DUT测试使能--------------------》配置工具没有配置进入DUT方式，也要使能DUT模块
#define BT_FCC_TEST_EN                  0   //蓝牙FCC测试使能
#define BT_LOCAL_ADDR                   1   //蓝牙是否使用本地地址，0使用配置工具地址
#define BT_PAIR_SLEEP_EN                xcfg_cb.bt_pair_sleep_en    //蓝牙未连接, 是否进入休眠模式
#define BT_MAX_NR_LINK_KEY              3   //最大手机linkkey记忆数, max: 8

#define BT_TWS_EN                       1   //是否支持TWS
#define BT_TWS_LR_EN                    0   //是否自动分配TWS左右声道
#define BT_2ACL_EN                      1   //是否支持连接两部手机（TWS不支持）
#define BT_2ACL_AUTO_SWITCH             1   //连接两部手机时是否支持点击播放切换到对应的手机（TWS不支持）
#define BT_A2DP_EN                      1   //是否打开蓝牙音乐服务
#define BT_HFP_EN                       1   //是否打开蓝牙通话服务
#define BT_SPP_EN                       1   //是否打开蓝牙串口服务
#define BT_HID_EN                       1   //是否打开蓝牙HID服务（自拍器）
#define BT_MAP_EN                       0   //是否打开蓝牙短信服务(用于获取设备时间，支持IOS/Android)
#define BT_HID_MENU_EN                  1   //蓝牙HID是否需要手动连接/断开
#define BT_HID_DISCON_DEFAULT_EN        1   //蓝牙HID服务默认不连接，需要手动进行连接。
#define BT_HID_VOL_SYNC_EN              1   //蓝牙HID音量同步功能
#define BT_HFP_CALL_PRIVATE_EN          1   //是否使能私密接听与蓝牙接听切换功能
#define BT_HFP_CALL_PRIVATE_FORCE_EN    1   //是否强制使用私密接听（手机端接听）
#define BT_HFP_RING_NUMBER_EN           1   //是否支持来电报号
#define BT_HFP_RINGS_BEFORE_NUMBER      xcfg_cb.bt_hfp_rings_before_number  //报号前先响铃几次
#define BT_HFP_INBAND_RING_EN           1   //是否支持手机来电铃声（部分android不支持，默认用本地RING提示音）
#define BT_HFP_BAT_REPORT_EN            1   //是否支持电量显示
#define BT_HFP_MSBC_EN                  1   //是否打开宽带语音功能
#define BT_A2DP_VOL_CTRL_EN             1   //是否支持A2DP音量与手机同步
#define BT_SCO_DBG_EN                   1   //是否打开无线调试通话参数功能
#define BT_AAC_AUDIO_EN                 1   //是否支持蓝牙AAC音频格式
#define BT_QUICK_TEST_EN                1   //蓝牙快测功能
#define BT_LINK_FILTER                  0   //是否过滤手机和电脑连接(连接蓝牙发射器专用)
#define BT_A2DP_AVRCP_PLAY_STATUS_FIX   0   //快速播放暂停同步状态
#define BT_TWS_RECONN_NOR_FIX           0   //修正TWS回连中调用bt_connect回连手机异常的问题

#define BT_TWS_AUTO_SWITCH              1
#define BT_TWS_TSCO_AUTO_SWITCH         1   //通话是否自动切换，在BT_TWS_AUTO_SWITCH为1时生效
#define BT_TWS_VBAT_AUTO_SWITCH         1   //是否根据电量自动主从切换
#define BT_TWS_DBG_EN                   1   //是否支持BT-Assistant工具分析信号质量

#define BT_A2DP_DEFAULT_VOLUME_EN       0   //是否支持设置第一次连接默认音量，可分别设置IOS和非IOS设备默认音量
#define BT_A2DP_IOS_DEFAULT_VOLUME_FIX  0   //IOS第一次连接设置默认音量，不需要点击播放
#define BT_A2DP_DEFAULT_VOLUME          VOL_MAX/2   //设置非IOS设备默认音量，根据系统音量级数设置，需要打开BT_A2DP_DEFAULT_VOLUME_EN
#define BT_A2DP_IOS_DEFAULT_VOLUME      VOL_MAX/2   //设置IOS设备默认音量，根据系统音量级数设置，需要打开BT_A2DP_DEFAULT_VOLUME_EN

#define BT_HFP_CALL_EARPHONE_EN         0   //是否默认耳机接听，苹果手机在手机端接听会转到耳机接听
#define BT_PLAY_PAUSE_SIRI_CTRL_DIS_EN  0   //是否关闭播放暂停的SIRI控制功能
#define BT_A2DP_STORE_VOL_EN            0   //是否支持A2DP音量记忆，兼容笔记本
#define BT_TWS_CUSTOMER_CONTROL_TWSSCAN_STA_EN   0   //客户控制tws scan状态使能
#define BT_RF_POWER_BALANCE_EN          0   //优化回连干扰导致的噪声，注意IO是否有冲突，回连的rf功率调节bt_set_reconnect_txpwr函数
#define BT_TWS_TIMEOUT_DELAY_TIME_CTRL_EN    0        //是否设置TWS超时断线回连时间
#define BT_RESOLVE_SIRI_STA_NOUPDATE_EN      0        //发XAPL带上siri标志，解决IOS开启siri后自动关闭不更新siri状态的问题,该修改会导致了小牛测控仪器无法获取电量
#define BT_HID_SHUTTER_OPPO_EN          0   //修复OPPO系手机无法拍照的问题
#define BT_A2DP_MUSIC_XIAOMI_FIX_EN     0   //修复回连部分小米手机小概率音乐无声问题
#define BT_TWS_STANDBY_SUPPORT_SWITCH_EN    0   //待机支持主从切换,只解决主耳超距，不会主从切换的问题
#define BT_LOW_LATENCY_QUICK_FIX        0   //解决快速操作低延时不切换的问题
#define BT_TWS_WARNING_SLAVE_BYPASS_FIX 0   //解决主耳播放提示音，副耳蓝牙音乐会被中断的问题
#define BT_A2DP_AAC_BITRATE_CONFIG_EN   0   //修改AAC码率使能
#define BT_TWS_SET_RECONN_NOR_EN        0   //主耳回连手机时入仓，副耳继续回连手机
#define BT_TWS_SLAVE_LOSTCONN_WARNING_EN    0   //TWS连接手机，副耳超距播放断开连接提示音
#define BT_CALL_RING_EXIT_FIX           0   //修正连接部分手机，来电自动退出后耳机不退出sfunc_bt_ring的问题
#define BT_TWS_SLAVE_CONN_NOR_FIX       0   //修正某些情况下，手机会连接到副耳的问题
#define BT_TWS_CONN_NOR_STOP_RECON_FIX_EN   0   //副耳连接手机，停止TWS回连
#define BT_SUP_TIME_OUT_RECON_FIX_EN    0   //解决小概率超距回连手机，回连次数被修改提前退出回连问题

#define BT_TWS_FIX_LR_SAME_CH_MAC_EN        0   //固定左右耳为同一个耳机的地址,使能该功能需要打开VUSB_TBOX_NEW_QTEST_EN,打开该功能必须保证左右耳有固定声道
#define BT_TWS_SET_MAC_CH                   1   //1为设置右耳地址为主耳地址，2为设置右耳为主耳地址
#define BT_DISP_LOW_VBAT_EN                 0   //手机端显示低电量耳机

//低音增强
#define BT_ALG_DBB_EN                   0   //是否打开低音增强算法
#define BT_ALG_DBB_LEVEL_MAX            20  //低音增强最大强度(最大值20)
#define BT_ALG_DBB_KEY_EN               0   //是否三击开关低音增强算法(仅调试使用，打开后会无效三击配置UI)

#define LE_EN                           1   //是否打开BLE功能
#define LE_LIGHTING_EN                  1   //是否打开BLE灯光控制服务
#define LE_MUSIC_CTRL_EN                0   //是否打开BLE音乐控制服务

/*****************************************************************************
 * Module    : 通话算法配置
 *****************************************************************************/
//通话参数
#define BT_PLC_EN                       1
#define BT_ANL_GAIN                     xcfg_cb.bt_anl_gain         //MIC模拟增益(0~31)
#define BT_DIG_GAIN                     xcfg_cb.bt_dig_gain         //MIC数字增益(0-31),且于0~3DB范围细调,步进3/32DB
#define BT_CALL_MAX_GAIN                xcfg_cb.bt_call_max_gain    //配置通话时DAC最大模拟增益

#define BT_AEC_EN                       1
#define BT_ECHO_LEVEL                   xcfg_cb.bt_echo_level       //回声消除级别（级别越高，回声衰减越明显，但通话效果越差）(0~15)
#define BT_FAR_OFFSET                   xcfg_cb.bt_far_offset       //远端补偿值(0~255)
#define BT_AEC_NLP_GAIN_NEW_EN          0                           //aec nlp gain是否改为软件实现，可以使用更低level0

#define BT_ALC_EN                       1                           //是否使能ALC
#define BT_ALC_FADE_IN_DELAY            26 //208ms xcfg_cb.bt_alc_in_delay     //近端淡入延时
#define BT_ALC_FADE_IN_STEP             2  //64ms  xcfg_cb.bt_alc_in_step      //近端淡入速度
#define BT_ALC_FADE_OUT_DELAY           2  //16ms  xcfg_cb.bt_alc_out_delay    //远端淡出延时
#define BT_ALC_FADE_OUT_STEP            32 //4ms   xcfg_cb.bt_alc_out_step     //远端淡出速度
#define BT_ALC_VOICE_THR                0x30000

//通话近端降噪算法(耳机MIC采集数据降噪)
#define BT_SCO_NR_EN					1	//是否打开AEC硬件的DNN降噪
#define BT_NR_NOISE_DYN_EQ_EN           0   //是否检测大噪声切换不同的MIC EQ

#define BT_SCO_AINS3_EN                 1   //通话近端AINS3降噪算法
#define BT_SCO_AINS3_LEVEL              10  //降噪等级

//通话远端降噪算法(接收远端手机的通话数据降噪)
#define BT_SCO_FAR_NR_EN                1   //是否打开远端降噪算法
#define BT_SCO_FAR_NOISE_LEVEL          1   //强度（0~15dB，越大降噪效果越好，音质越差）

#define BT_SCO_MAV_EN                   0   //是否打开蓝牙通话变声功能，开启后关闭远端降噪

#define BT_SCO_DUMP_EN                  1   //是否dump sco数据
#define SMIC_DBG_EN                     0   //是否打开单麦算法开关指令


/*****************************************************************************
* Module    : AUX功能配置
******************************************************************************/
#define AUX_CHANNEL_CFG                 CH_AUXL_PA7 //选择LINEIN通路
#define MIC_CHANNEL_CFG                 CH_MICL0                    //选择MIC的通路
#define AUX_2_SDADC_EN                  1   //AUX是否进SDADC, 否则直通DAC。进SDADC可以调AUX EQ, 及AUX录音等功能
#define LINEIN_DETECT_EN                1   //是否打开LINEIN检测
#define SDCMD_MUX_DETECT_LINEIN         0   //是否复用SDCMD检测LINEIN插入
#define LINEIN_2_PWRDOWN_EN             1   //是否插入Linein后直接软关机（大耳包功能）
#define LINEIN_2_PWRDOWN_TONE_EN        xcfg_cb.linein_2_pwrdown_tone_en   //插入Linein直接关机，是否播放关机提示音。

#define LINEIN_DETECT_INIT()        linein_detect_init()
#define LINEIN_IS_ONLINE()          linein_is_online()
#define IS_DET_LINEIN_BUSY()        is_detect_linein_busy()

/*****************************************************************************
* Module    : DAC配置控制
******************************************************************************/
#define DAC_CH_SEL                      xcfg_cb.dac_sel         //DAC_MONO ~ DAC_VCMBUF_DUAL
#define DAC_FAST_SETUP_EN               0                       //DAC快速上电，有噪声需要外部功放MUTE
#define DAC_MAX_GAIN                    xcfg_cb.dac_max_gain    //配置DAC最大模拟增益，默认设置为dac_vol_table[VOL_MAX]
#define PIANO_TONE_VOL                  xcfg_cb.piano_tone_vol  //设置PianoTone提示音的音量，SRC1数字音量
#define DAC_OUT_SPR                     xcfg_cb.dac_out_spr     //dac out sample rate
#define DAC_LDOH_SEL                    xcfg_cb.dac_ldoh_sel
#define DAC_DNR_EN                      1                       //是否使能动态降噪
#define DAC_MUTE_ISSUE_FIX              0                       //修正dac静音时改音量会解mute问题


/*****************************************************************************
* Module    : ANC配置控制
******************************************************************************/
#define ANC_EN                          0                       //是否使能ANC


/*****************************************************************************
 * Module    : 录音功能配置
 *****************************************************************************/
#define FUNC_REC_EN                     0   //录音功能总开关
#define FMRX_REC_EN                     0   //是否打开FM录音功能
#define AUX_REC_EN                      0   //是否打开AUX录音功能
#define MIC_REC_EN                      0   //是否打开MIC录音功能
#define REC_DIR_LFN                     0   //是否打开长文件名目录
#define REC_TYPE_SEL                    REC_SBC


/*****************************************************************************
 * Module    : User按键配置 (可以同时选择多组按键)
 *****************************************************************************/
#define USER_ADKEY                      1           //ADKEY的使用， 0为不使用
#define USER_ADKEY2                     0           //ADKEY2的使用，0为不使用
#define USER_PWRKEY                     1           //PWRKEY的使用，0为不使用
#define USER_IOKEY                      1           //IOKEY的使用， 0为不使用

#define USER_TKEY                       1           //TouchKEY的使用，0为不使用
#define USER_TKEY_SOFT_PWR_EN           1           //是否使用TouchKey进行软开关机, TKEY软开关只能使用WKO通路
#define USER_TKEY_INEAR                 1           //是否使用TouchKey的入耳检测功能
#define USER_TKEY_SLIDE_OLD             0           //是否使用旧版TouchKey的滑动触摸功能
#define USER_TKEY_DEBUG_EN              1           //仅调试使用，用于确认TKEY的参数
#define IS_TKEY_PRESS()			        (sys_cb.tkey_pwrdwn_en && tkey_get_key())
#define USER_TKEY_CH_SEL                0           //选择TouchKey的通道, 0: WKO, 1: PB4, 2: PB3, 3: PA7
#define USER_TKEY_PWRKEY_EN             0           //开启非WKO的TouchKey，允许WKO的PWRKEY按键功能

//滑动触摸
#define USER_TKEY_SLIDE                 0           //是否使用TouchKey的滑动触摸功能, 适用于两铜柱间距大的情况
#define USER_TKEY_SLIDE_DEBUG_EN        0           //仅调试使用，用于确认滑动触摸的参数

#define USER_TKEY_SHORT_SLIDE           0           //适用于两铜柱间距小的滑动触摸功能
#define USER_TKEY_SHORT_SLIDE_DEBUG_EN  0           //仅调试使用，用于确认滑动触摸的参数

//触摸通道控制
#define USER_TKEY_MULTI_EN              0           //触摸通道控制总开关
#define USER_TKEY_MULTI_DEBUG_EN        0           //仅调试使用，打印各触摸通道参数
#define USER_TKEY_PB5_DISABLE           0           //是否关闭PB5(WKO)的触摸功能
#define USER_TKEY_PB4_EN                0           //是否开启PB4的触摸功能
#define USER_TKEY_PB3_EN                0           //是否开启PB3的触摸功能
#define USER_TKEY_PA7_EN                0           //是否开启PA7的触摸功能

#define USER_MULTI_PRESS_EN             1           //按键多击检测使能
#define ADKEY_CH                        sys_cb.adkey_ch    //ADCCH_PE7
#define ADKEY2_CH                       ADCCH_PE6

#define IS_PWRKEY_PRESS()			    ((sys_cb.wko_pwrkey_en) && (0 == (RTCCON & BIT(19)))) //pwrkey是否按下？
#define PWRON_PRESS_TIME                (500*xcfg_cb.pwron_press_time)                  //长按PWRKEY多长时间开机？
#define PWROFF_PRESS_TIME               (3+3*xcfg_cb.pwroff_press_time)                 //长按PWRKEY多长时间关机？

#define USER_NTC                        1           //NTC ADC检测
#define NTC_ADCCH                       sys_cb.ntc_adc_ch

/*****************************************************************************
 * Module    : SD0配置
 *****************************************************************************/
#define SDCLK_MUX_DETECT_SD             0           //是否复用SDCLK检测SD卡
#define SD0_MAPPING                     SD0MAP_G1   //选择SD0 mapping

//不复用检测时，需要定义SD检测引脚
#if !SDCLK_MUX_DETECT_SD
#define SD_DETECT_INIT()
#define SD_IS_ONLINE()                  0
#endif

/*****************************************************************************
 * Module    : I2C配置
 *****************************************************************************/
#define I2C_EN                          0           //是否使能I2C功能
#define I2C_SCL_H()                     GPIOASET = BIT(5)
#define I2C_SCL_L()                     GPIOACLR = BIT(5)
#define I2C_SDA_SCL_OUT()               GPIOADIR &= ~(0x03 << 5)
#define I2C_SDA_SCL_H()                 GPIOASET = 0x03 << 5
#define I2C_SDA_IN()                    {GPIOADIR |= BIT(6); GPIOAPU |= BIT(6);}
#define I2C_SDA_OUT()                   GPIOADIR &= ~BIT(6)
#define I2C_SDA_H()                     GPIOASET = BIT(6)
#define I2C_SDA_L()                     GPIOACLR = BIT(6)
#define I2C_SDA_IS_H()                  (GPIOA & BIT(6))


/*****************************************************************************
 * Module    : 系统细节配置
 *****************************************************************************/
#define KARAOK_MIX_EN                   0           //是否打开K歌宝功能
#define PWM_RGB_EN                      0           //PWM RGB三色灯功能
#define SYS_PARAM_RTCRAM                1           //是否系统参数保存到RTCRAM
#define SYS_PARAM_EEPROM                1           //是否支持EEPROM记忆
#define PWRON_ENTER_BTMODE_EN           0           //是否上电默认进蓝牙模式
#define VBAT_DETECT_EN                  1           //电池电量检测功能
#define VBAT_DETECT_10BIT_EN                 1           //电池电量ADC检测改为10BIT使能
#define VBAT2_ADCCH                     ADCCH_VBAT  //ADCCH_VBAT为内部1/2电压通路，带升压应用需要外部ADC通路检测1/2电池电压
#define EQ_MODE_EN                      0           //是否调节EQ MODE (POP, Rock, Jazz, Classic, Country)
#define EQ_DBG_IN_UART                  1           //是否使能UART在线调节EQ
#define EQ_DBG_IN_SPP                   1           //是否使能SPP在线调节EQ
#define PWRKEY_2_HW_PWRON               xcfg_cb.pwrkey_2_hw_pwron   //用PWRKEY模拟硬开关
#define PWRON_FIRST_BAT_EN              xcfg_cb.pwron_frist_bat_en  //第一次接电池上电是否直接开机
#define SLEEP_DAC_OFF_EN                (!sys_cb.sleep_dac_en)      //sfunc_sleep是否关闭DAC， 大耳包有些应用方案不能关DAC。
#define SYS_INIT_VOLUME                 xcfg_cb.sys_init_vol        //系统默认音量
#define SYS_RECOVER_INITVOL             xcfg_cb.sys_recover_initvol //小于多少级音量后恢复成系统默认音量
#define LPWR_WARNING_VBAT               xcfg_cb.lpwr_warning_vbat   //低电提醒电压
#define LPWR_OFF_VBAT                   xcfg_cb.lpwr_off_vbat       //低电关机电压
#define BT_LIGHTNINIG_EN                0                           //是否打开蓝牙闪电头earphone方案功能
#define BT_MOTOR_EN                     1                           //是否打开马达振动功能
#define BT_HALL_SENSOR_EN               0                           //是否打开霍尔传感器开关机功能
#define BT_HALL_SENSOR_GPIO_SEL         IO_NONE                     //霍尔传感器GPIO选择
#define BT_EARIN_DETECT_EN              1                           //是否打开耳机入耳检测功能
#define BT_EARIN_USAGE_PP_EN            xcfg_cb.bt_earin_usage_pp_en   //入耳功能是否发送PLAY/PAUSE方式，可以兼容android。关掉后为3D的真入耳功能
#define MIC_MAXPOW_CALC_EN              0                           //是否允许MIC能量计算

/*****************************************************************************
 * Module    : LED指示灯配置
 *****************************************************************************/
#define LED_DISP_EN                     1           //是否使用LED指示灯(蓝灯)
#define LED_PWR_EN                      1           //充电及电源指示灯(红灯)
#define LED_BREATHE_EN                  1           //是否打开呼吸灯功能,TIMER2软件PWM实现。（请在配置项中打开相应状态的呼吸灯使能位）
#define PORT_2LED_EN                    xcfg_cb.port_2led_en        //1个IO推两个LED灯
#define PORT_2LED_FIX_EN                0           //修复一个IO推两灯抖动问题
#define RLED_LOWBAT_EN                  1           //电池低电是否闪红灯
#define BLED_LOW2ON_EN                  xcfg_cb.bled_low2on_en      //蓝灯是否低电平亮,主要用于三极管推频谱灯。
#define BLED_CHARGE_FULL                xcfg_cb.led_charge_fully_xcfg //充电满是否亮蓝灯
#define USER_LED_GPIO                   0                             //是否开启自定义修改LED的IO上拉电阻，会占用公共区
#define USER_LED_SLEEP_EN               0           //自定义休眠同步闪灯
#define BT_TWS_SLEEP_LED_SYNC_EN        0           //TWS休眠同步闪灯

#define USER_PWM_LED_EN                 0           //是否开启PWM控制LED，可以修改LED亮度
#define USER_PWM_LED_LEVEL              5           //修改PWM控制LED亮度，10级可调

#define LED_INIT()                      bled_func.port_init(&bled_gpio)
#define LED_SET_ON()                    bled_func.set_on(&bled_gpio)
#define LED_SET_OFF()                   bled_func.set_off(&bled_gpio)

#define LED_PWR_INIT()                  rled_func.port_init(&rled_gpio)
#define LED_PWR_SET_ON()                rled_func.set_on(&rled_gpio)
#define LED_PWR_SET_OFF()               rled_func.set_off(&rled_gpio)

#define LED_188LED_DISP_EN              0           //是否支持188LED电量指示灯
#define LED_188LED_DISP_OFF_DELAY       4000         //显示多久自动关闭(单位/5ms)
#define LED_188LED_DISP_CHARGE          800          //充电闪烁间隔(单位/ms)
#define LED_188LED_DISP_CHARGE_FULL     4000         //充满电后显示多久自动关闭(单位/5ms)

/*****************************************************************************
 * Module    : Loudspeaker mute及耳机检测配置
 *****************************************************************************/
#define EARPHONE_DETECT_EN              0           //是否打开耳机检测
#define SDCMD_MUX_DETECT_EARPHONE       0           //是否复用SDCMD检测耳机插入

#if !SDCMD_MUX_DETECT_EARPHONE
#define EARPHONE_DETECT_INIT()          {GPIOFDE |= BIT(2); GPIOFPU  |= BIT(2); GPIOFDIR |= BIT(2);}
#define EARPHONE_IS_ONLINE()            (!(GPIOF & BIT(2)))
#define IS_DET_EAR_BUSY()               (0)
#else
#define EARPHONE_DETECT_INIT()
#define EARPHONE_IS_ONLINE()            SD_CMD_MUX_IS_ONLINE()
#define IS_DET_EAR_BUSY()               SD_MUX_IS_BUSY()
#endif

#define LOUDSPEAKER_MUTE_EN             1           //是否使能功放MUTE
#define LOUDSPEAKER_MUTE_INIT()         loudspeaker_mute_init()
#define LOUDSPEAKER_MUTE_DIS()          loudspeaker_disable()
#define LOUDSPEAKER_MUTE()              loudspeaker_mute()
#define LOUDSPEAKER_UNMUTE()            loudspeaker_unmute()
#define LOUDSPEAKER_UNMUTE_DELAY        xcfg_cb.loudspeaker_unmute_delay     //UNMUTE延时配置，单位为5ms

#define AMPLIFIER_SEL_INIT()            //{GPIOEDE |= BIT(6); GPIOEDIR &= ~BIT(6); GPIOE |= BIT(6);}
#define AMPLIFIER_SEL_D()               //{GPIOEDIR &= ~BIT(6); GPIOE |= BIT(6);}
#define AMPLIFIER_SEL_AB()              //{GPIOEDIR &= ~BIT(6); GPIOE &= ~BIT(6);}


/*****************************************************************************
 * Module    : 充电功能选择
 *****************************************************************************/
#define CHARGE_EN                       1           //是否打开充电功能
#define CHARGE_TRICK_EN                 1           //是否打开涓流充电功能
#define CHARGE_DC_RESET                 xcfg_cb.charge_dc_reset         //是否打开DC插入复位功能
#define CHARGE_DC_NOT_PWRON             xcfg_cb.charge_dc_not_pwron     //DC插入，是否软开机。 1: DC IN时不能开机
#define CHARGE_DC_IN()                  charge_dc_detect()              //((RTCCON >> 20) & 0x01)
#define CHARGE_INBOX()                  ((RTCCON >> 22) & 0x01)

#define CHARGE_STOP_CURR                xcfg_cb.charge_stop_curr        //充电截止电流：0:2.5mA; 1:5mA; 2:7.5mA; 3:10mA; 4:12.5mA; 5:15mA; 6:17.5mA; 7:20mA
#define CHARGE_STOP_VOLT                xcfg_cb.charge_stop_volt        //充电截止电压：4.09V: 0, 4.12V: 1, 4.16V: 2, 4.20V: 3, 4.24V: 4, 4.28V: 5, 4.32V: 6, 4.35V: 7
#define CHARGE_CONSTANT_CURR            xcfg_cb.charge_constant_curr    //恒流充电电流（电池电压大于3.0v）
#define CHARGE_TRICKLE_CURR             xcfg_cb.charge_trickle_curr     //涓流充电电流（电池电压小于3.0v）
#define CHARGE_VOLT_FOLLOW              0                               //是否打开跟随快充：0: NONE; 1: 187.5mV; 2: 250mV; 3: 312.5mV; 4: 375mV
#define BT_TWS_CHARGE_DELAY_EN          0                               //延迟3秒进充电

//NTC充电控制
#define CHARGE_NTC_DETECT               0           //是否打开NTC充电控制功能，需要打开NTC功能，超出阈值停止充电
#define CHARGE_NTC_DETECT_VAL           70          //NTC充电检测阈值

//内部温度检测充电控制
#define CHARGE_TSEN_DETECT              0           //是否打开内部温度检测充电控制
#define CHARGE_TSEN_SLOW_CURR           2           //慢充充电电流, 0: 5mA, 1: 10mA, 2: 15mA ...
#define CHARGE_TSEN_STOP_VAL            1055        //内部温度检测停止充电阈值, 大于该值停止充电
#define CHARGE_TSEN_SLOW_VAL            950         //内部温度检测慢充阈值，小于该值缓慢充电

#define CHARGE_PULL_OUT_FILTER_EN       1           //有客户充电仓，耳机入仓会出现5v不稳，十几ms拉到4v（5us）的现象，该宏兼容这种仓

//入仓延迟开漏电流
#define CHARGE_DELAY_OPEN_LEAKAGE_EN    0           //入仓检测到INBOX，延迟开漏电流。解决IP5513仓，5v掉2.4v瞬间入仓，仓不起5v问题
#define CHARGE_DELAY_LEAKAGE_TICKS      400         //延迟2s开入仓漏电流，单位5ms

/*****************************************************************************
 * Module    : VUSB UART功能选择
 *****************************************************************************/
#define VUSB_HUART_DMA_EN               0           //VUSB使用高速串口
#define VUSB_TBOX_QTEST_EN              1           //VUSB快速测试盒测试
#define VUSB_TBOX_NEW_QTEST_EN          0           //新版快速测试盒测试,新增加配对码校验，程序校验对比, 快测关机支持船运关机等功能，搭配测试盒276以上版本
#define VUSB_TBOX_NEW_QTEST_RECORD_TWS_INFO_EN     0 //新快测记录tws信息到其他page，让客户手动清除配对后，调用bt_update_qtest_tws_info()可恢复快测的配对记录,返回true说明恢复成功。可使用bt_get_ext_link_info获取tws信息，使用bt_put_ext_link_info写入信息，写入时记得调用param_sync
#define VUSB_SMART_VBAT_HOUSE_EN        1           //智能充电仓 (非智能仓时关闭能省1.5KB程序空间)
#define VUSB_SMART_VHOUSE_ONELINE_EN    0           //智能仓左右耳共用一根通讯线
#define VUSB_SMART_VBAT_DELAY_DISC      0           //智能充电仓时，延迟断线进入充电（0=不延迟, n=延迟n毫秒）
#define IODM_TEST_MODE                  0           // IODM 测试 功能

/*****************************************************************************
 * Module    : 提示音 功能选择
 *****************************************************************************/
#define WARNING_TONE_EN                 1            //是否打开提示音功能, 总开关
#define WARNING_VOLUME                  xcfg_cb.warning_volume   //播放提示音的音量级数
#define WARNING_WAV_EN                  0            //是否播放wav提示音
#define WARING_WAV_VOL_OWN_EN           0            //是否固定音量播放WAV提示音，不跟随系统音乐音量变化
#define LANG_SELECT                     LANG_EN_ZH   //提示音语言选择
#define WAV_KEY_VOICE_QUICK_EN          0            //快速WAV按键提示音，播放更快，且可打断其他提示音，需要打开播放WAV提示音

#define WARNING_POWER_ON                1
#define WARNING_POWER_OFF               1
#define WARNING_FUNC_MUSIC              0
#define WARNING_FUNC_BT                 1
#define WARNING_FUNC_CLOCK              0
#define WARNING_FUNC_FMRX               0
#define WARNING_FUNC_AUX                1
#define WARNING_FUNC_USBDEV             0
#define WARNING_FUNC_SPEAKER            0
#define WARNING_FUNC_SPDIF              0
#define WARNING_LOW_BATTERY             1
#define WARNING_BT_WAIT_CONNECT         0
#define WARNING_BT_CONNECT              1
#define WARNING_BT_DISCONNECT           1
#define WARNING_BT_INCALL               1
#define WARNING_USB_SD                  0
#define WARNING_MAX_VOLUME              1
#define WARNING_MIN_VOLUME              1
#define WARNING_BT_HID_MENU             1            //BT HID MENU手动连接/断开HID Profile提示音
#define WARNING_BTHID_CONN              0            //BTHID模式是否有独立的连接/断开提示音
#define WARNING_BT_PAIR                 1            //BT PAIRING提示音

#include "config_extra.h"

#endif // USER_CONFIG_H
