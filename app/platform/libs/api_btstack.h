#ifndef _API_BTSTACK_H
#define _API_BTSTACK_H

//工作模式
#define MODE_NORMAL                     0       //正常连接模式，关闭DUT测试
#define MODE_NORMAL_DUT                 1       //正常连接模式，使能DUT测试
#define MODE_FCC_TEST                   2       //FCC测试模式，通过串口通信
#define MODE_BQB_RF_BREDR               3       //BR/EDR DUT测试模式，通过蓝牙通信
#define MODE_BQB_RF_BLE                 4       //BLE DUT测试模式，通过串口通信
#define MODE_IODM_TEST                  5       //IODM测试模式，通过串口通信

//蓝牙功能
#define PROF_A2DP                       0x0007  //蓝牙音乐功能
#define PROF_HID                        0x0018  //蓝牙键盘功能
#define PROF_HFP                        0x0060  //蓝牙通话功能
#define PROF_SPP                        0x0080  //蓝牙串口功能
#define PROF_PBAP                       0x0100  //蓝牙电话本功能
#define PROF_HSP                        0x0200
#define PROF_MAP                        0x0400  //蓝牙短息功能
#define PROF_TWS                        0x8000  //TWS功能

#define PROF_AVDTP_SIGNAL               0x0001
#define PROF_AVDTP_MEDIA                0x0002
#define PROF_AVCTP                      0x0004
#define PROF_AVDTP                      (PROF_AVDTP_SIGNAL|PROF_AVDTP_MEDIA)

//蓝牙特性
#define HFP_BAT_REPORT                  0x01    //是否支持手机电量显示
#define HFP_3WAY_CALL                   0x02    //是否支持三通电话
#define HFP_INBAND_RING_TONE            0x04    //是否支持IOS来电铃声
#define HFP_CALL_PRIVATE                0x08    //是否打开强制私密接听
#define HFP_SIRI_CMD                    0x10    //是否打开siri控制命令
#define HFP_EC_AND_NR                   0x20    //是否打开手机端回音和降噪
#define HFP_RING_NUMBER_EN              0x40    //是否支持来电报号
#define A2DP_AVRCP_VOL_CTRL             0x01    //是否支持手机音量控制同步
#define A2DP_RECON_CTRL                 0x02    //是否打开A2DP控制键回连
#define A2DP_AVRCP_PLAY_STATUS          0x04    //是否支持手机播放状态同步，可加快播放暂停响应速度，蓝牙后台建议打开
#define A2DP_RESTORE_PLAYING            0x08    //是否支持掉线回连后恢复播放
#define A2DP_AVDTP_DELAY_REPORT         0x10    //是否支持AVDTP delay report功能
#define A2DP_IOS_FLAG                   0x80    //是否iso系统

#define HID_CONN_IOS_ONLY               0x01    //是否支持HID只连接IOS系统的功能

//扩展标志
#define BT_FLAG0_TWS_SCAN           0x01
#define BT_FLAG0_CLK_ADVANCE        0x02
#define BT_FLAG0_TWS_M_RECON        0x04
#define BT_FLAG0_TWS_FAST_RECON     0x08        //快速版TWS回连
#define BT_FLAG0_TWS_MENU_PAIR      0x10        //手动控制tws连接
#define BT_FLAG0_TWS_FAST_PAIR      0x20        //开机TWS不回连直接进入搜索配对
#define BT_FLAG0_POPUP_3BAT         0x40
#define BT_FLAG0_NAME_MENU          0x80

#define BT_TWS_SYNC_TO              50          //BT同步等待，对齐到一个50ms，250ms超时

//AVRCP消息
#define AVC_VOLUME_UP               0x41
#define AVC_VOLUME_DOWN             0x42
#define AVC_MUTE                    0x43
#define AVC_PLAY                    0x44
#define AVC_STOP                    0x45
#define AVC_PAUSE                   0x46
#define AVC_RECORD                  0x47
#define AVC_REWIND                  0x48
#define AVC_FAST_FORWARD            0x49
#define AVC_EJECT                   0x4a
#define AVC_FORWARD                 0x4b
#define AVC_BACKWARD                0x4c
#define AVC_SPEED_DOUBLE            0x4d
#define AVC_SPEED_HALF              0x4e

//自定义的USER KEY
#define TWS_USER_KEY_RING_STOP      0x101
#define TWS_USER_KEY_ATMOS          0x102

#define TWS_USER_KEY_DOUYIN_CTRL    0x110           //0x110~0x120, 抖音控制
#define TWS_USER_KEY_DOUYIN_DOWN    0x110
#define TWS_USER_KEY_DOUYIN_UP      0x111
#define TWS_USER_KEY_DOUYIN_PP      0x112
#define TWS_USER_KEY_DOUYIN_P       0x113

//TWS FEAT
#define BT_FEAT_NEGO_TSCO       0x0001
#define BT_FEAT_NEGO_RING       0x0002
#define BT_FEAT_NEGO_MSBC       0x0004
#define BT_FEAT_NEGO_AAC        0x0008
#define BT_FEAT_NEGO_SW         0x0010
#define BT_FEAT_NEGO_WARNING    0x0020
#define BT_FEAT_NEGO_LINK_TICKS 0x0040              //建立snoop link时同步ticks。增加一个feat, 兼容之前旧版本TWS组队。
#define BT_FEAT_NEGO_TICKS      0x0080
#define BT_FEAT_NEGO_PUBLIC_ADDR 0x0100             //TWS连接后主耳使用公共地址连接手机
#define BT_FEAT_NEGO_CHK1       0x0200   //res
#define BT_FEAT_NEGO_CHK2       0x0400   //res
#define BT_FEAT_NEGO_CHK3       0x0800   //res
#define BT_FEAT_NEGO_RECONNECT	0x4000				//是否在发起回连手机

typedef struct {
    u8 pwrup_connect_times;                     //单耳（无TWS功能）上电回连手机次数
    u8 sup_to_connect_times;                    //远距离掉线回连次数
    u8 nor_connect_times;                       //对耳回连手机次数
    u8 tws_connect_times;                       //对耳TWS回连次数
    u8 tws_sup_to_connect_times;                //TWS远距离掉线回连次数
    u16 profile;
    u8 max_acl_link;
    u8 max_nor_link;
    u8 a2dp_feature;
    u8 hfp_feature;
    u8 vx_set;
    u8 max_nr_link_key;
    u16 scan_timeout;
    u16 supervision_to_dft;                     //SUPERVISION_TO_DFT
    u8 new_conn_timeout_add;
    u8 res[2];

    u16 dual_mode           : 1;
    u16 tws_mode            : 1;
    u16 simple_pair_mode    : 1;
    u16 scan_ctrl           : 1;
    u16 sco_plc_en          : 1;
    u16 sco_msbc_en         : 1;

    u16 tws_fast_con_en     : 1;                //TWS快速配对连接
    u16 remote_name_req_en  : 1;
    u16 sdp_client_avrcp_en : 1;
//#if BT_QUICK_TEST_EN
    u16 bt_quick_test       : 1;
//#endif
} bt_cfg_t;
extern bt_cfg_t bt_cfg;

//蓝牙状态
enum {
    BT_STA_OFF,                                 //蓝牙模块已关闭
    BT_STA_INITING,                             //初始化中
    BT_STA_IDLE,                                //蓝牙模块打开，未连接

    BT_STA_SCANNING,                            //扫描中
    BT_STA_DISCONNECTING,                       //断开中
    BT_STA_CONNECTING,                          //连接中

    BT_STA_CONNECTED,                           //已连接
    BT_STA_PLAYING,                             //播放
    BT_STA_INCOMING,                            //来电响铃
    BT_STA_OUTGOING,                            //正在呼出
    BT_STA_INCALL,                              //通话中
};

//通话状态
enum {
    BT_CALL_IDLE,                               //
    BT_CALL_INCOMING,                           //来电响铃
    BT_CALL_OUTGOING,                           //正在呼出
    BT_CALL_ACTIVE,                             //通话中
    BT_CALL_3WAY_CALL,                          //三通电话或两部手机通话
};

//蓝牙通知
enum {
    BT_NOTICE_INIT_FINISH,                      //蓝牙初始化完成
    BT_NOTICE_CONNECT_START,                    //开始回连手机, param[0]=nouse,  param[6:1]=bd_addr
    BT_NOTICE_CONNECT_FAIL,                     //回连手机失败, param[0]=reason, param[6:1]=bd_addr
    BT_NOTICE_DISCONNECT,                       //蓝牙断开,     param[0]=reason, param[6:1]=bd_addr
    BT_NOTICE_LOSTCONNECT,                      //蓝牙连接丢失, param[0]=nouse,  param[6:1]=bd_addr
    BT_NOTICE_CONNECTED,                        //蓝牙连接成功, param[0]=feat,index,  param[6:1]=bd_addr
    BT_NOTICE_CONNECT_STATUS,
    BT_NOTICE_DISCONNECT_STATUS,
    BT_NOTICE_RECON_FINISH,                     //蓝牙回连结束
    BT_NOTICE_SCO_SETUP,
    BT_NOTICE_SCO_KILL,
    BT_NOTICE_INCOMING,                         //来电
    BT_NOTICE_RING,                             //来电响铃
    BT_NOTICE_OUTGOING,                         //去电
    BT_NOTICE_CALL,                             //建立通话
    BT_NOTICE_SET_SPK_GAIN,                     //设置通话音量
    BT_NOTICE_MUSIC_PLAY,                       //蓝牙音乐开始播放
    BT_NOTICE_MUSIC_STOP,                       //蓝牙音乐停止播放
    BT_NOTICE_MUSIC_CHANGE_VOL,                 //改变蓝牙音乐音量
    BT_NOTICE_MUSIC_SET_VOL,                    //设置蓝牙音乐音量
    BT_NOTICE_HID_CONN_EVT,                     //HID服务连接事件
    BT_NOTICE_SET_QTEST_CH,
    BT_NOTICE_FIRST_CONNECT,                    //第一次连接手机, param[5:0]=bd_addr
    BT_NOTICE_A2DP_VOL_CTRL,

    BT_NOTICE_TWS_SEARCH_TIMEOUT = 0x40,        //搜索TWS超时
    BT_NOTICE_TWS_CONNECT_START,                //开始回连TWS, param[0]=nouse,  param[6:1]=bd_addr
    BT_NOTICE_TWS_CONNECT_FAIL,                 //TWS回连失败, param[0]=reason, param[6:1]=bd_addr
    BT_NOTICE_TWS_DISCONNECT,                   //TWS牙断开,   param[0]=reason, param[6:1]=bd_addr
    BT_NOTICE_TWS_LOSTCONNECT,                  //TWS连接丢失, param[0]=nouse,  param[6:1]=bd_addr
    BT_NOTICE_TWS_CONNECTED,                    //TWS连接成功, param[0]=feat,index,  param[6:1]=bd_addr
    BT_NOTICE_TWS_SET_VOL,                      //设置TWS音乐音量
    BT_NOTICE_TWS_HID_SHUTTER,                  //远端TWS拍照键
    BT_NOTICE_TWS_INFO,                         //TWS同步信息
    BT_NOTICE_TWS_USER_KEY,                     //TWS自定义按键
    BT_NOTICE_TWS_SEARCH_STATUS,
    BT_NOTICE_TWS_TO_STATUS,
    BT_NOTICE_TWS_GET_MAP_TIME,
    BT_NOTICE_TWS_SET_OPERATION,
    BT_NOTICE_TWS_RES_PLAY,
    BT_NOTICE_TWS_PLAY_STA_SYNC,
    BT_NOTICE_TWS_EQ_SYNC,
    BT_NOTICE_TWS_RES7,
};

//控制消息
enum bt_msg_t {
    BT_MSG_OFF          = 0,                    //关闭蓝牙
    BT_MSG_ON,                                  //打开蓝牙
    BT_MSG_RES0,
    BT_MSG_RES1,
    BT_MSG_CONNECT,                             //连接蓝牙
    BT_MSG_DISCONNECT,                          //断开蓝牙
    BT_MSG_PLAY_PAUSE,                          //切换播放、暂停
    BT_MSG_VOL_CHANGE,                          //音量调整
    BT_MSG_CALL_REDIAL,                         //回拨电话（最后一次通话）
    BT_MSG_CALL_ANSWER_INCOM,                   //接听来电
    BT_MSG_CALL_TERMINATE,                      //挂断通话或来电
    BT_MSG_CALL_SWAP,                           //切换三通电话
    BT_MSG_HFP_REPORT_BAT,                      //报告电池电量
    BT_MSG_HFP_SPK_GAIN,                        //设置通话扬声器音量
    BT_MSG_HFP_MIC_GAIN,                        //设置通话麦克风音量
    BT_MSG_HFP_PRIVATE_SWITCH,                  //切换私密通话
    BT_MSG_HID_CONNECT,                         //连接HID
    BT_MSG_HID_DISCONNECT,                      //断开HID
    BT_MSG_AUDIO_BYPASS,                        //忽略蓝牙SBC/SCO AUDIO
    BT_MSG_AUDIO_ENABLE,                        //使能蓝牙SBC/SCO AUDIO
    BT_MSG_LOW_LATENCY_EN,
    BT_MSG_LOW_LATENCY_DIS,
    BT_MSG_TWS_DISCONNECT,                      //断开TWS连接
    BT_MSG_TWS_CONNECT,                         //建立TWS连接
    BT_MSG_TWS_SEARCH_SLAVE,                    //搜索并建立TWS连接
    BT_MSG_TWS_NOTICE_RING,
    BT_MSG_SWITCH_SIRI,
    BT_MSG_EAR_STA_CHANGE,
    BT_MSG_NR_STA_CHANGE,
    BT_MSG_TWS_NOTICE_INCOME_NUMBER,
    BT_MSG_HFP_AT_CMD,                          //回拨电话（从hfp_get_outgoing_number获取号码）
    BT_MSG_SIRI_ON,
    BT_MSG_CALL_ANSWER_INCOM_REJ_OTHER,         //接听来电，三通时挂断当前通话
    BT_MSG_CONNECT_ADDRESS,
    BT_MSG_FCC_TEST_FUNC,
    BT_MSG_PLAY_PRIVATE_SWITCH,                 //切换手机播放/蓝牙播放
    BT_MSG_TWS_SYNC_INFO,
    BT_MSG_NOR_CONNECT,
    BT_MSG_NOR_DISCONNECT,
    BT_MSG_2ACL_PALY_SWITCH,                    //一拖二播放切换
    BT_MSG_TWS_SET_BYPASS,
    BT_MSG_RESET_BT_NAME,
    BT_MSG_TWS_SWITCH,                          //主从切换
    BT_MSG_CALL_REDIAL_NUMBER,
    BT_MSG_MAX,

    BT_MSG_HID_KEY      = 0x30,                 //HID按键
    BT_MSG_HID_CONSUMER,                        //CONSUMER按键
    BT_MSG_TWS_USER_KEY,                        //TWS用户自定义消息
    BT_MSG_HID_TOUCH_SCREEN,                    //触摸屏
    BT_MSG_PBAP,
    BT_MSG_BAT_HOUSE_CTRL,
    BT_MSG_HSP_SEND,
    BT_MSG_SYNC_INFO,                           //主副耳同步一些状态消息
    BT_MSG_TWS_SET_SCAN,
    BT_MSG_SET_SCAN,
    BT_MSG_START_WORK,
	BT_MSG_ATT_VA,
	BT_MSG_STORE_A2DP_VOLUME,                   //保存连接的同步音量
	BT_MSG_MAP,
	BT_MSG_ADV0_SET_EN,
    BT_MSG_PARAM_MAX,

    BT_MSG_PLAY         = AVC_PLAY,             //播放
    BT_MSG_PAUSE        = AVC_PAUSE,            //暂停
    BT_MSG_STOP         = AVC_STOP,             //停止
    BT_MSG_PREV         = AVC_BACKWARD,         //上一曲
    BT_MSG_NEXT         = AVC_FORWARD,          //下一曲
    BT_MSG_REWIND       = AVC_REWIND,           //开始快退
    BT_MSG_FAST_FORWARD = AVC_FAST_FORWARD,     //开始快进
    BT_MSG_REWIND_END       = AVC_REWIND|0x80,  //结束快退
    BT_MSG_FAST_FORWARD_END = AVC_FAST_FORWARD|0x80, //结束快进
    BT_MSG_SPEED_DOUBLE     = AVC_SPEED_DOUBLE|0x80, //快播*2
    BT_MSG_SPEED_HALF       = AVC_SPEED_HALF|0x80,   //慢放1/2

    BT_MSG_NO           = 0xff,
};

enum sync_info_enum {
    TWS_SYNC_INFO_LEDCNT,               //同步 led cnt
    TWS_SYNC_INFO_EQ,

    TWS_SYNC_TWS_RES0 = 0x40,           //0x40~0x7f for tws_res
    TWS_SYNC_TWS_RES_MAX = 0x7f,

    TWS_SYNC_RUN = 0x80,                //同步运行
};

//control
void bt_init(void);                             //初始化蓝牙变量
int bt_setup(void);                             //打开蓝牙模块
void bt_off(void);                              //关闭蓝牙模块
void bt_wakeup(void);                           //唤醒蓝牙模块
void bt_start_work(uint8_t opcode, uint8_t scan_en); //蓝牙开始工作，opcode: 0=回连, 1=不回连, 2=tws不回连,直接自动配对
void bt_send_msg(uint msg);                     //蓝牙控制消息，参数详见bt_msg_t
void bt_audio_bypass(void);                     //蓝牙SBC/SCO通路关闭
void bt_audio_enable(void);                     //蓝牙SBC/SCO通路使能
void bt_connect_address(void);                  //蓝牙根据地址链接 需要把 uint8_t bt_get_connect_addr(uint8_t *bd_addr);函数打开
void bt_set_sco_far_delay(void *buf, uint size, uint delay);

void bt_set_scan(uint8_t scan_en);              //打开/关闭可被发现和可被连接, bit0=可被发现, bit1=可被连接
void bt_connect(void);                          //蓝牙设备回连
void bt_disconnect(void);                       //蓝牙设备断开
void bt_hid_connect(void);                      //蓝牙HID服务回连
void bt_hid_disconnect(void);                   //蓝牙HID服务断开
int bt_hid_is_connected(void);
bool bt_hid_is_ready_to_discon(void);
void ble_popup_ctrl(uint opcode);
bool sco_is_connected(void);
bool bt_sco_is_msbc(void);

//status
uint bt_get_disp_status(void);                  //获取蓝牙的当前显示状态, V060
uint bt_get_status(void);                       //获取蓝牙的当前状态
uint8_t bt_get_scan(void);                      //判断当前可被连接可被扫描状态
uint bt_get_call_indicate(void);                //获取通话的当前状态
uint bt_get_siri_status(void);                  //获取SIRI当前状态, 0=SIRI已退出, 1=SIRI已唤出
bool bt_is_calling(void);                       //判断是否正在通话
bool bt_is_playing(void);                       //判断是否正在播放
bool bt_is_ring(void);                          //判断是否正在响铃
bool bt_is_testmode(void);                      //判断当前蓝牙是否处于测试模式
bool bt_is_sleep(void);                         //判断蓝牙是否进入休眠状态
bool bt_is_connected(void);                     //判断蓝牙是否已连接（TWS副耳被连接，或主耳与手机已连接）
bool bt_is_ios_device(void);                    //判断当前播放的是否ios设备
uint32_t bt_sleep_proc(void);
void bt_enter_sleep(void);
void bt_exit_sleep(void);
bool bt_is_silence(void);
bool bt_cur_dev_is_silence(void);
void bt_other_dev_silence_reset(void);
bool bt_sync_tick(void);                        //根据蓝牙时钟同步到50ms，误差为5ms

//tws
bool bt_tws_is_connected(void);                 //tws设备是否已连接
bool bt_tws_is_slave(void);                     //tws主从判断，注意要先判断tws是否已连接
uint8_t bt_tws_get_scan(void);                  //tws是否已打开可被搜索与连接
void bt_tws_set_scan(uint8_t scan_en);          //打开/关闭tws可被搜索与连接, bit0=可被发现, bit1=可被连接
void bt_tws_disconnect(void);                   //断开tws设备
void bt_tws_connect(void);                      //回连tws设备
void bt_tws_search_slave(void);                 //搜索tws设备，并建立连接
uint bt_tws_get_link_info(uint8_t *bd_addr);    //获取tws配对信息，可作为是否配对过TWS的判断
bool bt_tws_user_key(uint keycode);             //发送tws自定义按键, 10bit有效
bool bt_tws_sync_ring_tone(void);               //同步等tws来电号码
void bt_tws_clr_ring_tone(void);                //
bool bt_tws_need_ring_sync(void);
void bt_tws_switch(void);                       //主从切换，例如通话时切主副MIC
bool tws_switch_is_need(void);                  //查看是否需要切换
void bt_tws_sync(uint sync_info);               //同步状态信息 sync_info_enum
void bt_tws_sync_run(uint ntick);               //同步到ntick * 50ms
bool bt_nor_is_connected(void);                 //是否已连接手机
void bt_nor_connect(void);                      //回连手机
void bt_nor_disconnect(void);                   //断开手机
bool bt_nor_get_link_info(uint8_t *bd_addr);    //获取手机配对信息，bd_addr=NULL时仅查询是否存在回连信息
uint8_t bt_connect_for_the_first_time(uint8_t *bd_addr);//检查给定的蓝牙地址是否为第一次连接。0:first_time, 1:not_first_time, 255:no record for given bd_addr
bool bt_nor_get_link_info_addr(uint8_t *bd_addr, uint8_t order);    //获取第n个手机配对信息，bd_addr=NULL时仅查询是否存在回连信息
bool bt_tws_is_auto_switch(void);
bool bt_tws_get_auto_channel(void);
bool btstack_tws_atmos_led(void);
bool btstack_tws_qtest_channel(uint8_t ch);
void lm_set_name_buf(const char *bt_name, uint max_len);
bool bt_tws_get_link_rssi(int8_t *buffer, bool left_channel);
u8 bt_get_tws_remote_status(void);              //获取对耳的状态

void bt_clr_all_link_info(u8 flag);             //删除全部配对信息
void bt_nor_clr_link_info(u8 flag);             //删除手机配对信息
void bt_tws_clr_link_info(u8 flag);             //删除tws配对信息


enum {
    TWS_RES_NUM_0,
    TWS_RES_NUM_1,
    TWS_RES_NUM_2,
    TWS_RES_NUM_3,
    TWS_RES_NUM_4,
    TWS_RES_NUM_5,
    TWS_RES_NUM_6,
    TWS_RES_NUM_7,
    TWS_RES_NUM_8,
    TWS_RES_NUM_9,
    TWS_RES_RING,
    TWS_RES_LANGUAGE,
    TWS_RES_LOW_BATTERY,
    TWS_RES_NOR_CONNECTED,
    TWS_RES_CONNECTED,
    TWS_RES_DISCONNECT,
    TWS_RES_PAIRING,
    TWS_RES_CALL_HANGUP,
    TWS_RES_CALL_REJECT,
    TWS_RES_REDIALING,
    TWS_RES_MAX_VOL,
    TWS_RES_MIN_VOL,
    TWS_RES_MUSIC_MODE,
    TWS_RES_GAME_MODE,
    TWS_RES_NR_DISABLE,
    TWS_RES_ANC,
    TWS_RES_TRANSPARENCY,
    TWS_RES_TONE,
    TWS_RES_WAV_TEST,
    TWS_RES_MAX,

    //下面MSG不打断当前播放的提示音
    TWS_RES_RING_STOP   = 50,
    TWS_BRE_LED_START,
    TWS_BRE_LED_START1,
    TWS_BRE_LED_STOP,
    TWS_ATMOS_LED_ON,
    TWS_ATMOS_LED_OFF,

    //最大只能63
};

void tws_res_init(void);                        //api
void tws_res_play(uint8_t index);               //api

void tws_mp3_res_play_kick(u32 addr, u32 len);  //tws_res开始播放
u32 tws_res_sync(u32 limit);                    //tws_res同步
void tws_res_add(u8 index, u32 ticks);          //tws_res加入列表
void tws_res_proc(void);                        //tws_res流程处理
bool tws_res_play_sync(u32 flag);
void tws_res_done(void);
void tws_res_reset(void);
u32 tws_res_get_ticks(u8 index);
bool tws_res_hssi_sync_wait(isr_t isr_callback);
bool tws_res_is_busy(void);

//蓝牙连接
#define bt_scan_enable()                        bt_set_scan(0x03)                       //打开扫描
#define bt_scan_disable()                       bt_set_scan(0x00)
#define bt_fcc_test_start()                     bt_send_msg(BT_MSG_FCC_TEST_FUNC)   //fcc kick

//蓝牙音乐
#define bt_music_play()                         bt_send_msg(BT_MSG_PLAY)            //播放
#define bt_music_pause()                        bt_send_msg(BT_MSG_PAUSE)           //暂停
#define bt_music_play_pause()                   bt_send_msg(BT_MSG_PLAY_PAUSE)      //切换播放/暂停
#define bt_music_stop()                         bt_send_msg(BT_MSG_STOP)            //停止
#define bt_music_prev()                         bt_send_msg(BT_MSG_PREV)            //上一曲
#define bt_music_next()                         bt_send_msg(BT_MSG_NEXT)            //下一曲
#define bt_music_rewind()                       bt_send_msg(BT_MSG_REWIND)          //开始快退
#define bt_music_rewind_end()                   bt_send_msg(BT_MSG_REWIND_END)      //结束快退
#define bt_music_fast_forward()                 bt_send_msg(BT_MSG_FAST_FORWARD)    //开始快进
#define bt_music_fast_forward_end()             bt_send_msg(BT_MSG_FAST_FORWARD_END)//结束快进
#define bt_low_latency_enble()                  bt_send_msg(BT_MSG_LOW_LATENCY_EN)      //蓝牙使能低延时
#define bt_low_latency_disable()                bt_send_msg(BT_MSG_LOW_LATENCY_DIS)     //蓝牙关闭低延时
#define bt_music_play_switch()                  bt_send_msg(BT_MSG_2ACL_PALY_SWITCH)
#define bt_music_play_private_switch()          bt_send_msg(BT_MSG_PLAY_PRIVATE_SWITCH) //切换手机播放/蓝牙播放音乐
bool bt_is_low_latency(void);

//蓝牙通话
#define bt_call_redial_last_number()            bt_send_msg(BT_MSG_CALL_REDIAL)     //电话回拨（最后一次通话）
#define bt_call_answer_incoming()               bt_send_msg(BT_MSG_CALL_ANSWER_INCOM) //接听电话，三通时挂起当前通话
#define bt_call_answer_incom_rej_other()        bt_send_msg(BT_MSG_CALL_ANSWER_INCOM_REJ_OTHER)	//接听电话，三通时挂断当前通话
#define bt_call_terminate()                     bt_send_msg(BT_MSG_CALL_TERMINATE)  //挂断电话
#define bt_call_swap()                          bt_send_msg(BT_MSG_CALL_SWAP)       //切换三通电话
#define bt_call_private_switch()                bt_send_msg(BT_MSG_HFP_PRIVATE_SWITCH)       //切换三通电话
#define bt_siri_switch()                        bt_send_msg(BT_MSG_SWITCH_SIRI)     //开关SIRI, android需要在语音助手中打开“蓝牙耳机按键启动”, ios需要打开siri功能
#define bt_siri_on()                            bt_send_msg(BT_MSG_SIRI_ON)         //打开语音助手

#define bt_ctl_ear_sta_change()                 bt_send_msg(BT_MSG_EAR_STA_CHANGE)
#define bt_ctl_nr_sta_change()                  bt_send_msg(BT_MSG_NR_STA_CHANGE)

enum {
    VA_STA_IDLE = 0,
    VA_STA_CONNECT,
    VA_STA_STOP,
    VA_STA_START,
    VA_STA_END,
};

void bt_va_operate(u8 enble);
u8 bt_va_is_connect(void);


extern uint8_t cfg_bt_work_mode;
extern uint8_t cfg_bt_sw_ver_major;
extern uint8_t cfg_bt_tws_slot_inc;
extern uint8_t cfg_bt_tws_limit_pair_time;
extern uint8_t cfg_bt_a2dp_media_discard;
extern uint32_t cfg_a2dp_aac_max_bitrate;
extern uint16_t cfg_bt_tws_sniff_interval;
extern uint16_t cfg_bt_tws_feat;
extern uint8_t cfg_bt_hci_disc_only_spp;
extern uint8_t cfg_bt_hid_feature;


uint bt_get_hfp_feature(void);
int bt_spp_tx(uint8_t *packet, uint16_t len);           //发送SPP命令
bool bt_spp_tx_ex(u8 idx, const char * cmd, u8 len);    //带长度的发送SPP命令

//hid
bool bt_hid_send_key(uint msg, uint keycode);
#define bt_hid_key(keycode)                     bt_hid_send_key(BT_MSG_HID_KEY, keycode)        //标准HID键, 如Enter
#define bt_hid_consumer(keycode)                bt_hid_send_key(BT_MSG_HID_CONSUMER, keycode)   //自定义HID键, 如VOL+ VOL-
/**
 * @brief 模拟触点函数
   注意:IOS 范围是-2047-2048 ，安卓是0-4096;
        IOS设备，x,y是相对位置，比如10,10是相对当前位置移动10,10;
        安卓设备，x，y是绝对位置，10,10是在手机10,10的位置上;
 * @param is_press	1按下，0抬起
 * @param x	模拟触点横坐标
 * @param y	模拟触点纵坐标
 **/
void bt_hid_point_pos(bool is_press, s16 x, s16 y);
#define bt_hid_finger_select_ios()              bt_send_msg(BT_MSG_HID_TOUCH_SCREEN | 1 << 8)                //抖音视频选择IOS系统, 需要在bt连接前调用区分
#define bt_hid_finger_select_andriod()          bt_send_msg(BT_MSG_HID_TOUCH_SCREEN | 2 << 8)                //抖音视频选择andriod系统, 需要在bt连接前调用区分

//抖音神器
void bt_hid_douyin_down(void);
void bt_hid_douyin_up(void);
void bt_hid_douyin_p(void);
void bt_hid_douyin_pp(void);
void bt_hid_douyin_ctrl(u8 ctrl);


/*****************************************************************************
 * BLE API函数接口
 *****************************************************************************/
void le_set_adv_interval(u16 interval);

#endif //_API_BTSTACK_H
