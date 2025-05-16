#ifndef _API_BTSTACK_H
#define _API_BTSTACK_H

//工作模式
#define MODE_NORMAL                     0       //正常工作模式，关闭DUT测试
#define MODE_NORMAL_DUT                 1       //正常工作模式，启用DUT测试
#define MODE_FCC_TEST                   2       //FCC测试模式，通过串口通信
#define MODE_BQB_RF_BREDR               3       //BR/EDR DUT测试模式，通过串口通信
#define MODE_BQB_RF_BLE                 4       //BLE DUT测试模式，通过串口通信
#define MODE_IODM_TEST                  5       //IODM测试模式，通过串口通信

//蓝牙配置
#define PROF_A2DP                       0x0007  //音乐播放功能
#define PROF_HID                        0x0018  //蓝牙键盘功能
#define PROF_HFP                        0x0060  //蓝牙通话功能
#define PROF_SPP                        0x0080  //蓝牙串口功能
#define PROF_PBAP                       0x0100  //蓝牙电话本功能
#define PROF_HSP                        0x0200
#define PROF_MAP                        0x0400  //蓝牙消息功能
#define PROF_TWS                        0x8000  //TWS功能

#define PROF_AVDTP_SIGNAL               0x0001
#define PROF_AVDTP_MEDIA                0x0002
#define PROF_AVCTP                      0x0004
#define PROF_AVDTP                      (PROF_AVDTP_SIGNAL|PROF_AVDTP_MEDIA)

//通话功能
#define HFP_BAT_REPORT                  0x01    //是否支持手机电量显示
#define HFP_3WAY_CALL                   0x02    //是否支持三通电话
#define HFP_INBAND_RING_TONE            0x04    //是否支持IOS来电铃声
#define HFP_CALL_PRIVATE                0x08    //是否强制私密接听
#define HFP_SIRI_CMD                    0x10    //是否有siri语音命令
#define HFP_EC_AND_NR                   0x20    //是否让手机回声和降噪
#define HFP_RING_NUMBER_EN              0x40    //是否支持来电报号
#define A2DP_AVRCP_VOL_CTRL             0x01    //是否支持手机音量与耳机同步
#define A2DP_RECON_CTRL                 0x02    //是否有A2DP控制键重连
#define A2DP_AVRCP_PLAY_STATUS          0x04    //是否支持手机播放状态同步（可加快播放暂停响应速度，但部分台机不兼容）
#define A2DP_RESTORE_PLAYING            0x08    //是否支持断线后恢复播放
#define A2DP_AVDTP_DELAY_REPORT         0x10    //是否支持AVDTP delay report功能
#define A2DP_IOS_FLAG                   0x80    //是否ios系统

#define HID_CONN_IOS_ONLY               0x01    //是否支持HID只连接IOS系统的功能

//扩展标志
#define BT_FLAG0_TWS_SCAN           0x01
#define BT_FLAG0_CLK_ADVANCE        0x02
#define BT_FLAG0_TWS_M_RECON        0x04
#define BT_FLAG0_TWS_FAST_RECON     0x08        //快速按TWS配对
#define BT_FLAG0_TWS_MENU_PAIR      0x10        //手动配对tws功能
#define BT_FLAG0_TWS_FAST_PAIR      0x20        //快速TWS配对，直接进入配对状态
#define BT_FLAG0_POPUP_3BAT         0x40
#define BT_FLAG0_NAME_MENU          0x80

#define BT_TWS_SYNC_TO              50          //BT同步等待（会延迟到一个50ms的250ms超时）

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
#define BT_FEAT_NEGO_LINK_TICKS 0x0040              //当有snoop link时同步ticks（这是一个feat, 兼容之前老版本TWS打印）
#define BT_FEAT_NEGO_TICKS      0x0080
#define BT_FEAT_NEGO_PUBLIC_ADDR 0x0100             //TWS连接后是否使用公共地址给手机
#define BT_FEAT_NEGO_CHK1       0x0200   //res
#define BT_FEAT_NEGO_CHK2       0x0400   //res
#define BT_FEAT_NEGO_CHK3       0x0800   //res
#define BT_FEAT_NEGO_RECONNECT	0x4000				//是否在发起连接手机

typedef struct {
    u8 pwrup_connect_times;                     //不包含TWS功能，上电连手机次数
    u8 sup_to_connect_times;                    //远程超时后重连次数
    u8 nor_connect_times;                       //自动连手机次数
    u8 tws_connect_times;                       //自动TWS配对次数
    u8 tws_sup_to_connect_times;                //TWS远程超时后重连次数
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

    u16 tws_fast_con_en     : 1;                //TWS快速配对功能
    u16 remote_name_req_en  : 1;
    u16 sdp_client_avrcp_en : 1;
//#if BT_QUICK_TEST_EN
    u16 bt_quick_test       : 1;
//#endif
} bt_cfg_t;
extern bt_cfg_t bt_cfg;

//蓝牙状态
enum {
    BT_STA_OFF,                                 // 蓝牙模块已关闭
    BT_STA_INITING,                             // 初始化中
    BT_STA_IDLE,                                // 蓝牙模块打开，未连接

    BT_STA_SCANNING,                            // 扫描中
    BT_STA_DISCONNECTING,                       // 断开中
    BT_STA_CONNECTING,                          // 连接中

    BT_STA_CONNECTED,                           // 已连接
    BT_STA_PLAYING,                             // 播放中
    BT_STA_INCOMING,                            // 来电振铃
    BT_STA_OUTGOING,                            // 正在拨号
    BT_STA_INCALL,                              // 通话中
};

//通话状态
enum {
    BT_CALL_IDLE,                               //空闲
    BT_CALL_INCOMING,                           //来电振铃
    BT_CALL_OUTGOING,                           //正在拨号
    BT_CALL_ACTIVE,                             //通话中
    BT_CALL_3WAY_CALL,                          //三通电话（与手机通话）
};

//蓝牙通知
enum {
    BT_NOTICE_INIT_FINISH,                      //蓝牙初始化完成
    BT_NOTICE_CONNECT_START,                    //开始连接手机, param[0]=nouse,  param[6:1]=bd_addr
    BT_NOTICE_CONNECT_FAIL,                     //连接手机失败, param[0]=reason, param[6:1]=bd_addr
    BT_NOTICE_DISCONNECT,                       //蓝牙断开,     param[0]=reason, param[6:1]=bd_addr
    BT_NOTICE_LOSTCONNECT,                      //蓝牙连接丢失, param[0]=nouse,  param[6:1]=bd_addr
    BT_NOTICE_CONNECTED,                        //蓝牙连接成功, param[0]=feat,index,  param[6:1]=bd_addr
    BT_NOTICE_CONNECT_STATUS,
    BT_NOTICE_DISCONNECT_STATUS,
    BT_NOTICE_RECON_FINISH,                     //蓝牙重连完成
    BT_NOTICE_SCO_SETUP,
    BT_NOTICE_SCO_KILL,
    BT_NOTICE_INCOMING,                         //来电
    BT_NOTICE_RING,                             //来电振铃
    BT_NOTICE_OUTGOING,                         //去电
    BT_NOTICE_CALL,                             //正在通话
    BT_NOTICE_SET_SPK_GAIN,                     //设置通话音量
    BT_NOTICE_MUSIC_PLAY,                       //蓝牙音乐开始播放
    BT_NOTICE_MUSIC_STOP,                       //蓝牙音乐停止播放
    BT_NOTICE_MUSIC_CHANGE_VOL,                 //改变蓝牙音乐音量
    BT_NOTICE_MUSIC_SET_VOL,                    //设置蓝牙音乐音量
    BT_NOTICE_HID_CONN_EVT,                     //HID连接相关事件
    BT_NOTICE_SET_QTEST_CH,
    BT_NOTICE_FIRST_CONNECT,                    //第一次连接手机, param[5:0]=bd_addr
    BT_NOTICE_A2DP_VOL_CTRL,

    BT_NOTICE_TWS_SEARCH_TIMEOUT = 0x40,        //搜索TWS超时
    BT_NOTICE_TWS_CONNECT_START,                //开始连接TWS, param[0]=nouse,  param[6:1]=bd_addr
    BT_NOTICE_TWS_CONNECT_FAIL,                 //TWS连接失败, param[0]=reason, param[6:1]=bd_addr
    BT_NOTICE_TWS_DISCONNECT,                   //TWS已断开,   param[0]=reason, param[6:1]=bd_addr
    BT_NOTICE_TWS_LOSTCONNECT,                  //TWS连接丢失, param[0]=nouse,  param[6:1]=bd_addr
    BT_NOTICE_TWS_CONNECTED,                    //TWS连接成功, param[0]=feat,index,  param[6:1]=bd_addr
    BT_NOTICE_TWS_SET_VOL,                      //设置TWS音乐音量
    BT_NOTICE_TWS_HID_SHUTTER,                  //远程TWS拍照键
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

//蓝牙消息
enum bt_msg_t {
    BT_MSG_OFF          = 0,                    //关闭蓝牙
    BT_MSG_ON,                                  //打开蓝牙
    BT_MSG_RES0,
    BT_MSG_RES1,
    BT_MSG_CONNECT,                             //蓝牙连接
    BT_MSG_DISCONNECT,                          //断开蓝牙
    BT_MSG_PLAY_PAUSE,                          //切换播放、暂停
    BT_MSG_VOL_CHANGE,                          //音量改变
    BT_MSG_CALL_REDIAL,                         //重播电话（最后一次通话）
    BT_MSG_CALL_ANSWER_INCOM,                   //接听来电
    BT_MSG_CALL_TERMINATE,                      //挂断通话来电
    BT_MSG_CALL_SWAP,                          //切换三通电话
    BT_MSG_HFP_REPORT_BAT,                     //电量回调电
    BT_MSG_HFP_SPK_GAIN,                       //设置通话扬声器音量
    BT_MSG_HFP_MIC_GAIN,                       //设置通话麦克风音量
    BT_MSG_HFP_PRIVATE_SWITCH,                 //切换私密通话
    BT_MSG_HID_CONNECT,                        //连接HID
    BT_MSG_HID_DISCONNECT,                     //断开HID
    BT_MSG_AUDIO_BYPASS,                       //关闭所有SBC/SCO AUDIO
    BT_MSG_AUDIO_ENABLE,                       //使能所有SBC/SCO AUDIO
    BT_MSG_LOW_LATENCY_EN,
    BT_MSG_LOW_LATENCY_DIS,
    BT_MSG_TWS_DISCONNECT,                     //断开TWS连接
    BT_MSG_TWS_CONNECT,                        //连接TWS连接
    BT_MSG_TWS_SEARCH_SLAVE,                   //主动搜索TWS连接
    BT_MSG_TWS_NOTICE_RING,
    BT_MSG_SWITCH_SIRI,
    BT_MSG_EAR_STA_CHANGE,
    BT_MSG_NR_STA_CHANGE,
    BT_MSG_TWS_NOTICE_INCOME_NUMBER,
    BT_MSG_HFP_AT_CMD,                         //重播电话（用hfp_get_outgoing_number获取号码）
    BT_MSG_SIRI_ON,
    BT_MSG_CALL_ANSWER_INCOM_REJ_OTHER,        //接听来电，三通时挂断当前通话
    BT_MSG_CONNECT_ADDRESS,
    BT_MSG_FCC_TEST_FUNC,
    BT_MSG_PLAY_PRIVATE_SWITCH,                //切换手机播放/耳机播放
    BT_MSG_TWS_SYNC_INFO,
    BT_MSG_NOR_CONNECT,
    BT_MSG_NOR_DISCONNECT,
    BT_MSG_2ACL_PALY_SWITCH,                   //一拖二播放切换
    BT_MSG_TWS_SET_BYPASS,
    BT_MSG_RESET_BT_NAME,
    BT_MSG_TWS_SWITCH,                         //主从切换
    BT_MSG_CALL_REDIAL_NUMBER,
    BT_MSG_MAX,

    BT_MSG_HID_KEY      = 0x30,                //HID按键
    BT_MSG_HID_CONSUMER,                       //CONSUMER按键
    BT_MSG_TWS_USER_KEY,                       //TWS用户自定义消息
    BT_MSG_HID_TOUCH_SCREEN,                   //触摸屏
    BT_MSG_PBAP,
    BT_MSG_BAT_HOUSE_CTRL,
    BT_MSG_HSP_SEND,
    BT_MSG_SYNC_INFO,                          //蓝牙端同步一些状态信息
    BT_MSG_TWS_SET_SCAN,
    BT_MSG_SET_SCAN,
    BT_MSG_START_WORK,
	BT_MSG_ATT_VA,
	BT_MSG_STORE_A2DP_VOLUME,                  //保存连接点同步音量
	BT_MSG_MAP,
	BT_MSG_ADV0_SET_EN,
    BT_MSG_PARAM_MAX,

    BT_MSG_PLAY         = AVC_PLAY,            //播放
    BT_MSG_PAUSE        = AVC_PAUSE,           //暂停
    BT_MSG_STOP         = AVC_STOP,            //停止
    BT_MSG_PREV         = AVC_BACKWARD,        //上一曲
    BT_MSG_NEXT         = AVC_FORWARD,         //下一曲
    BT_MSG_REWIND       = AVC_REWIND,          //开始快退
    BT_MSG_FAST_FORWARD = AVC_FAST_FORWARD,    //开始快进
    BT_MSG_REWIND_END       = AVC_REWIND|0x80, //结束快退
    BT_MSG_FAST_FORWARD_END = AVC_FAST_FORWARD|0x80, //结束快进
    BT_MSG_SPEED_DOUBLE     = AVC_SPEED_DOUBLE|0x80, //倍播*2
    BT_MSG_SPEED_HALF       = AVC_SPEED_HALF|0x80,   //倍播1/2

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
void bt_init(void);                             //初始化蓝牙协议栈
int bt_setup(void);                             //启动蓝牙模块
void bt_off(void);                              //关闭蓝牙模块
void bt_wakeup(void);                           //唤醒蓝牙模块
void bt_start_work(uint8_t opcode, uint8_t scan_en); //蓝牙开始工作，opcode: 0=正常, 1=配对中, 2=tws配对中,直到自动连接
void bt_send_msg(uint msg);                     //发送蓝牙消息，参考：bt_msg_t
void bt_audio_bypass(void);                     //蓝牙SBC/SCO通路关闭
void bt_audio_enable(void);                     //蓝牙SBC/SCO通路使能
void bt_connect_address(void);                  //根据地址连接 需要用 uint8_t bt_get_connect_addr(uint8_t *bd_addr);配合
void bt_set_sco_far_delay(void *buf, uint size, uint delay);

void bt_set_scan(uint8_t scan_en);              //开/关闭可被发现和可被连接, bit0=可被连接, bit1=可被发现
void bt_connect(void);                          //蓝牙设备连接
void bt_disconnect(void);                       //蓝牙设备断开
void bt_hid_connect(void);                      //蓝牙HID设备连接
void bt_hid_disconnect(void);                   //蓝牙HID设备断开
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
uint bt_get_siri_status(void);                  //获取SIRI当前状态, 0=SIRI已退出, 1=SIRI已激活
bool bt_is_calling(void);                       //判断是否正在通话
bool bt_is_playing(void);                       //判断是否正在播放
bool bt_is_ring(void);                          //判断是否正在振铃
bool bt_is_testmode(void);                      //判断当前蓝牙是否在测试模式
bool bt_is_sleep(void);                         //判断蓝牙是否在睡眠状态
bool bt_is_connected(void);                     //判断蓝牙是否有连接（TWS主从有连接，或者蓝牙与手机有连接）
bool bt_is_ios_device(void);                    //判断当前播放的是否ios设备
uint32_t bt_sleep_proc(void);
void bt_enter_sleep(void);
void bt_exit_sleep(void);
bool bt_is_silence(void);
bool bt_cur_dev_is_silence(void);
void bt_other_dev_silence_reset(void);
bool bt_sync_tick(void);                        //蓝牙端时钟同步（50ms对齐为5ms）

//tws
bool bt_tws_is_connected(void);                 //tws设备是否有连接
bool bt_tws_is_slave(void);                     //tws从机判断，注意要先判断tws是否有连接
uint8_t bt_tws_get_scan(void);                  //tws是否已打开可被发现可连接
void bt_tws_set_scan(uint8_t scan_en);          //开/关闭tws可被发现可连接, bit0=可被连接, bit1=可被发现
void bt_tws_disconnect(void);                   //断开tws设备
void bt_tws_connect(void);                      //连接tws设备
void bt_tws_search_slave(void);                 //搜索tws设备（主动搜索）
uint bt_tws_get_link_info(uint8_t *bd_addr);    //获取tws配对信息，返回为是否有过TWS配对记录
bool bt_tws_user_key(uint keycode);             //发送tws自定义按键, 10bit有效
bool bt_tws_sync_ring_tone(void);               //同步给tws来电铃声
void bt_tws_clr_ring_tone(void);                //
bool bt_tws_need_ring_sync(void);
void bt_tws_switch(void);                       //主从切换（在通话时，切换MIC）
bool tws_switch_is_need(void);                  //查看是否需要切换
void bt_tws_sync(uint sync_info);               //同步状态信息 sync_info_enum
void bt_tws_sync_run(uint ntick);               //同步运行ntick * 50ms
bool bt_nor_is_connected(void);                 //是否连接了手机
void bt_nor_connect(void);                      //连接手机
void bt_nor_disconnect(void);                   //断开手机
bool bt_nor_get_link_info(uint8_t *bd_addr);    //获取手机配对信息，bd_addr=NULL时，查询是否存在回连信息
uint8_t bt_connect_for_the_first_time(uint8_t *bd_addr);//检查连接的蓝牙地址是否为第一次连接。0:first_time, 1:not_first_time, 255:no record for given bd_addr
bool bt_nor_get_link_info_addr(uint8_t *bd_addr, uint8_t order);    //获取第n个手机配对信息，bd_addr=NULL时，查询是否存在回连信息
bool bt_tws_is_auto_switch(void);
bool bt_tws_get_auto_channel(void);
bool btstack_tws_atmos_led(void);
bool btstack_tws_qtest_channel(uint8_t ch);
void lm_set_name_buf(const char *bt_name, uint max_len);
bool bt_tws_get_link_rssi(int8_t *buffer, bool left_channel);
u8 bt_get_tws_remote_status(void);              //获取对端的状态

void bt_clr_all_link_info(u8 flag);             //删除全部配对信息
void bt_nor_clr_link_info(u8 flag);             //删除手机配对信息
void bt_tws_clr_link_info(u8 flag);             //删除tws配对信息


enum {
    TWS_RES_NUM_0,        // 数字 0 的提示音
    TWS_RES_NUM_1,        // 数字 1 的提示音
    TWS_RES_NUM_2,        // 数字 2 的提示音
    TWS_RES_NUM_3,        // 数字 3 的提示音
    TWS_RES_NUM_4,        // 数字 4 的提示音
    TWS_RES_NUM_5,        // 数字 5 的提示音
    TWS_RES_NUM_6,        // 数字 6 的提示音
    TWS_RES_NUM_7,        // 数字 7 的提示音
    TWS_RES_NUM_8,        // 数字 8 的提示音
    TWS_RES_NUM_9,        // 数字 9 的提示音
    TWS_RES_RING,         // 来电提示音（来电铃声）
    TWS_RES_LANGUAGE,     // 语言切换提示音
    TWS_RES_LOW_BATTERY,  // 低电量提示音
    TWS_RES_NOR_CONNECTED,// 普通连接提示音（非TWS连接）
    TWS_RES_CONNECTED,    // TWS连接成功提示音
    TWS_RES_DISCONNECT,   // TWS断开连接提示音
    TWS_RES_PAIRING,      // 配对提示音
    TWS_RES_CALL_HANGUP,  // 通话挂断提示音
    TWS_RES_CALL_REJECT,  // 拒绝来电提示音
    TWS_RES_REDIALING,    // 重拨电话提示音
    TWS_RES_MAX_VOL,      // 音量最大提示音
    TWS_RES_MIN_VOL,      // 音量最小提示音
    TWS_RES_MUSIC_MODE,   // 音乐模式提示音
    TWS_RES_GAME_MODE,    // 游戏模式提示音
    TWS_RES_NR_DISABLE,   // 降噪关闭提示音
    TWS_RES_ANC,          // ANC（主动降噪）提示音
    TWS_RES_TRANSPARENCY, // 通透模式提示音
    TWS_RES_TONE,         // 音效切换提示音-----------》具体表现为嘟的一声
    TWS_RES_WAV_TEST,     // WAV测试提示音

    TWS_RES_MAX,          // 提示音资源总数上限

    // 如果MSG有序列是前面提示音的
    TWS_RES_RING_STOP   = 50, // 来电结束提示音（挂断时播放）
    TWS_BRE_LED_START,        // 呼吸灯开始提示音
    TWS_BRE_LED_START1,       // 呼吸灯开始提示音（备用）
    TWS_BRE_LED_STOP,         // 呼吸灯结束提示音
    TWS_ATMOS_LED_ON,         // 氛围灯开启提示音
    TWS_ATMOS_LED_OFF,        // 氛围灯关闭提示音

    // 最大不超过63
};

void tws_res_init(void);                        //api
void tws_res_play(uint8_t index);               //api

void tws_mp3_res_play_kick(u32 addr, u32 len);  //开始播放TWS资源
u32 tws_res_sync(u32 limit);                    //同步TWS资源
void tws_mp3_res_play_kick(u32 addr, u32 len);  //开始播放TWS资源
u32 tws_res_sync(u32 limit);                    //同步TWS资源
void tws_res_add(u8 index, u32 ticks);          //添加TWS资源到播放列表
void tws_res_proc(void);                        //TWS资源处理过程
bool tws_res_play_sync(u32 flag);               //同步播放TWS资源
void tws_res_done(void);                        //TWS资源播放完成处理
void tws_res_reset(void);                       //重置TWS资源
u32 tws_res_get_ticks(u8 index);               //获取TWS资源的时间戳
bool tws_res_hssi_sync_wait(isr_t isr_callback); //等待TWS资源同步
bool tws_res_is_busy(void);                     //检查TWS资源是否忙碌

//蓝牙扫描控制
#define bt_scan_enable()                        bt_set_scan(0x03)                       //开启扫描
#define bt_scan_disable()                       bt_set_scan(0x00)                       //关闭扫描
#define bt_fcc_test_start()                     bt_send_msg(BT_MSG_FCC_TEST_FUNC)      //启动FCC测试

//音乐控制
#define bt_music_play()                         bt_send_msg(BT_MSG_PLAY)               //播放
#define bt_music_pause()                        bt_send_msg(BT_MSG_PAUSE)              //暂停
#define bt_music_play_pause()                   bt_send_msg(BT_MSG_PLAY_PAUSE)         //切换播放/暂停
#define bt_music_stop()                         bt_send_msg(BT_MSG_STOP)               //停止
#define bt_music_prev()                         bt_send_msg(BT_MSG_PREV)               //上一曲
#define bt_music_next()                         bt_send_msg(BT_MSG_NEXT)               //下一曲
#define bt_music_rewind()                       bt_send_msg(BT_MSG_REWIND)             //开始快退
#define bt_music_rewind_end()                   bt_send_msg(BT_MSG_REWIND_END)         //结束快退
#define bt_music_fast_forward()                 bt_send_msg(BT_MSG_FAST_FORWARD)       //开始快进
#define bt_music_fast_forward_end()             bt_send_msg(BT_MSG_FAST_FORWARD_END)   //结束快进
#define bt_low_latency_enble()                  bt_send_msg(BT_MSG_LOW_LATENCY_EN)     //启用低延时模式
#define bt_low_latency_disable()                bt_send_msg(BT_MSG_LOW_LATENCY_DIS)    //关闭低延时模式
#define bt_music_play_switch()                  bt_send_msg(BT_MSG_2ACL_PALY_SWITCH)   //双连接音乐切换
#define bt_music_play_private_switch()          bt_send_msg(BT_MSG_PLAY_PRIVATE_SWITCH) //切换手机播放/耳机播放音乐
bool bt_is_low_latency(void);                   //检查是否处于低延时模式

//通话控制
#define bt_call_redial_last_number()            bt_send_msg(BT_MSG_CALL_REDIAL)        //电话重拨最后一个通话号码
#define bt_call_answer_incoming()               bt_send_msg(BT_MSG_CALL_ANSWER_INCOM)   //接听来电
#define bt_call_answer_incom_rej_other()        bt_send_msg(BT_MSG_CALL_ANSWER_INCOM_REJ_OTHER) //接听来电并挂断当前通话
#define bt_call_terminate()                     bt_send_msg(BT_MSG_CALL_TERMINATE)     //挂断电话
#define bt_call_swap()                          bt_send_msg(BT_MSG_CALL_SWAP)          //切换三通电话
#define bt_call_private_switch()                bt_send_msg(BT_MSG_HFP_PRIVATE_SWITCH)  //切换私密通话
#define bt_siri_switch()                        bt_send_msg(BT_MSG_SWITCH_SIRI)        //切换SIRI状态
#define bt_siri_on()                            bt_send_msg(BT_MSG_SIRI_ON)            //打开SIRI

#define bt_ctl_ear_sta_change()                 bt_send_msg(BT_MSG_EAR_STA_CHANGE)     //耳机状态改变通知
#define bt_ctl_nr_sta_change()                  bt_send_msg(BT_MSG_NR_STA_CHANGE)      //降噪状态改变通知

enum {
    VA_STA_IDLE = 0,                           //语音助手空闲
    VA_STA_CONNECT,                            //语音助手已连接
    VA_STA_STOP,                               //语音助手停止
    VA_STA_START,                              //语音助手启动
    VA_STA_END,                                //语音助手结束
};

void bt_va_operate(u8 enble);                  //语音助手操作
u8 bt_va_is_connect(void);                     //检查语音助手是否连接

//蓝牙配置参数
extern uint8_t cfg_bt_work_mode;               //蓝牙工作模式
extern uint8_t cfg_bt_sw_ver_major;            //蓝牙软件主版本号
extern uint8_t cfg_bt_tws_slot_inc;            //TWS时隙增量
extern uint8_t cfg_bt_tws_limit_pair_time;     //TWS配对时间限制
extern uint8_t cfg_bt_a2dp_media_discard;      //A2DP媒体丢弃策略
extern uint32_t cfg_a2dp_aac_max_bitrate;      //AAC最大码率
extern uint16_t cfg_bt_tws_sniff_interval;     //TWS休眠间隔
extern uint16_t cfg_bt_tws_feat;               //TWS特性
extern uint8_t cfg_bt_hci_disc_only_spp;       //仅断开SPP连接
extern uint8_t cfg_bt_hid_feature;             //HID功能特性

uint bt_get_hfp_feature(void);                 //获取HFP功能特性
int bt_spp_tx(uint8_t *packet, uint16_t len);  //发送SPP数据
bool bt_spp_tx_ex(u8 idx, const char * cmd, u8 len); //带索引发送SPP数据

//HID控制
bool bt_hid_send_key(uint msg, uint keycode);  //发送HID按键
#define bt_hid_key(keycode)                     bt_hid_send_key(BT_MSG_HID_KEY, keycode)        //发送标准HID键
#define bt_hid_consumer(keycode)                bt_hid_send_key(BT_MSG_HID_CONSUMER, keycode)   //发送自定义HID键
/**
 * @brief 模拟触点函数
   注意:IOS 范围是-2047-2048，安卓是0-4096;
        IOS设备的x,y是相对位置，如向10,10是相对当前位置移动10,10;
        安卓设备的x、y是绝对位置，10,10是在手机10,10的位置上;
 * @param is_press	1按下，0抬起
 * @param x	模拟触点横坐标
 * @param y	模拟触点纵坐标
 **/
void bt_hid_point_pos(bool is_press, s16 x, s16 y);
#define bt_hid_finger_select_ios()              bt_send_msg(BT_MSG_HID_TOUCH_SCREEN | 1 << 8)   //触摸屏幕选择IOS系统
#define bt_hid_finger_select_andriod()          bt_send_msg(BT_MSG_HID_TOUCH_SCREEN | 2 << 8)   //触摸屏幕选择Android系统

//抖音控制
void bt_hid_douyin_down(void);                 //抖音向下滑动
void bt_hid_douyin_up(void);                   //抖音向上滑动
void bt_hid_douyin_p(void);                    //抖音点赞
void bt_hid_douyin_pp(void);                   //抖音暂停/播放
void bt_hid_douyin_ctrl(u8 ctrl);              //抖音控制

/*****************************************************************************
 * BLE API接口函数
 *****************************************************************************/
void le_set_adv_interval(u16 interval);        //设置BLE广播间隔

#endif //_API_BTSTACK_H
