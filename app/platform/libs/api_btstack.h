#ifndef _API_BTSTACK_H
#define _API_BTSTACK_H

//����ģʽ
#define MODE_NORMAL                     0       //��������ģʽ���ر�DUT����
#define MODE_NORMAL_DUT                 1       //��������ģʽ��ʹ��DUT����
#define MODE_FCC_TEST                   2       //FCC����ģʽ��ͨ������ͨ��
#define MODE_BQB_RF_BREDR               3       //BR/EDR DUT����ģʽ��ͨ������ͨ��
#define MODE_BQB_RF_BLE                 4       //BLE DUT����ģʽ��ͨ������ͨ��
#define MODE_IODM_TEST                  5       //IODM����ģʽ��ͨ������ͨ��

//��������
#define PROF_A2DP                       0x0007  //�������ֹ���
#define PROF_HID                        0x0018  //�������̹���
#define PROF_HFP                        0x0060  //����ͨ������
#define PROF_SPP                        0x0080  //�������ڹ���
#define PROF_PBAP                       0x0100  //�����绰������
#define PROF_HSP                        0x0200
#define PROF_MAP                        0x0400  //������Ϣ����
#define PROF_TWS                        0x8000  //TWS����

#define PROF_AVDTP_SIGNAL               0x0001
#define PROF_AVDTP_MEDIA                0x0002
#define PROF_AVCTP                      0x0004
#define PROF_AVDTP                      (PROF_AVDTP_SIGNAL|PROF_AVDTP_MEDIA)

//��������
#define HFP_BAT_REPORT                  0x01    //�Ƿ�֧���ֻ�������ʾ
#define HFP_3WAY_CALL                   0x02    //�Ƿ�֧����ͨ�绰
#define HFP_INBAND_RING_TONE            0x04    //�Ƿ�֧��IOS��������
#define HFP_CALL_PRIVATE                0x08    //�Ƿ��ǿ��˽�ܽ���
#define HFP_SIRI_CMD                    0x10    //�Ƿ��siri��������
#define HFP_EC_AND_NR                   0x20    //�Ƿ���ֻ��˻����ͽ���
#define HFP_RING_NUMBER_EN              0x40    //�Ƿ�֧�����籨��
#define A2DP_AVRCP_VOL_CTRL             0x01    //�Ƿ�֧���ֻ���������ͬ��
#define A2DP_RECON_CTRL                 0x02    //�Ƿ��A2DP���Ƽ�����
#define A2DP_AVRCP_PLAY_STATUS          0x04    //�Ƿ�֧���ֻ�����״̬ͬ�����ɼӿ첥����ͣ��Ӧ�ٶȣ�������̨�����
#define A2DP_RESTORE_PLAYING            0x08    //�Ƿ�֧�ֵ��߻�����ָ�����
#define A2DP_AVDTP_DELAY_REPORT         0x10    //�Ƿ�֧��AVDTP delay report����
#define A2DP_IOS_FLAG                   0x80    //�Ƿ�isoϵͳ

#define HID_CONN_IOS_ONLY               0x01    //�Ƿ�֧��HIDֻ����IOSϵͳ�Ĺ���

//��չ��־
#define BT_FLAG0_TWS_SCAN           0x01
#define BT_FLAG0_CLK_ADVANCE        0x02
#define BT_FLAG0_TWS_M_RECON        0x04
#define BT_FLAG0_TWS_FAST_RECON     0x08        //���ٰ�TWS����
#define BT_FLAG0_TWS_MENU_PAIR      0x10        //�ֶ�����tws����
#define BT_FLAG0_TWS_FAST_PAIR      0x20        //����TWS������ֱ�ӽ����������
#define BT_FLAG0_POPUP_3BAT         0x40
#define BT_FLAG0_NAME_MENU          0x80

#define BT_TWS_SYNC_TO              50          //BTͬ���ȴ������뵽һ��50ms��250ms��ʱ

//AVRCP��Ϣ
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

//�Զ����USER KEY
#define TWS_USER_KEY_RING_STOP      0x101
#define TWS_USER_KEY_ATMOS          0x102

#define TWS_USER_KEY_DOUYIN_CTRL    0x110           //0x110~0x120, ��������
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
#define BT_FEAT_NEGO_LINK_TICKS 0x0040              //����snoop linkʱͬ��ticks������һ��feat, ����֮ǰ�ɰ汾TWS��ӡ�
#define BT_FEAT_NEGO_TICKS      0x0080
#define BT_FEAT_NEGO_PUBLIC_ADDR 0x0100             //TWS���Ӻ�����ʹ�ù�����ַ�����ֻ�
#define BT_FEAT_NEGO_CHK1       0x0200   //res
#define BT_FEAT_NEGO_CHK2       0x0400   //res
#define BT_FEAT_NEGO_CHK3       0x0800   //res
#define BT_FEAT_NEGO_RECONNECT	0x4000				//�Ƿ��ڷ�������ֻ�

typedef struct {
    u8 pwrup_connect_times;                     //��������TWS���ܣ��ϵ�����ֻ�����
    u8 sup_to_connect_times;                    //Զ������߻�������
    u8 nor_connect_times;                       //�Զ������ֻ�����
    u8 tws_connect_times;                       //�Զ�TWS��������
    u8 tws_sup_to_connect_times;                //TWSԶ������߻�������
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

    u16 tws_fast_con_en     : 1;                //TWS�����������
    u16 remote_name_req_en  : 1;
    u16 sdp_client_avrcp_en : 1;
//#if BT_QUICK_TEST_EN
    u16 bt_quick_test       : 1;
//#endif
} bt_cfg_t;
extern bt_cfg_t bt_cfg;

//����״̬
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

//ͨ��״̬
enum {
    BT_CALL_IDLE,                               //
    BT_CALL_INCOMING,                           //��������
    BT_CALL_OUTGOING,                           //���ں���
    BT_CALL_ACTIVE,                             //ͨ����
    BT_CALL_3WAY_CALL,                          //��ͨ�绰�������ֻ�ͨ��
};

//����֪ͨ
enum {
    BT_NOTICE_INIT_FINISH,                      //������ʼ�����
    BT_NOTICE_CONNECT_START,                    //��ʼ�����ֻ�, param[0]=nouse,  param[6:1]=bd_addr
    BT_NOTICE_CONNECT_FAIL,                     //�����ֻ�ʧ��, param[0]=reason, param[6:1]=bd_addr
    BT_NOTICE_DISCONNECT,                       //�����Ͽ�,     param[0]=reason, param[6:1]=bd_addr
    BT_NOTICE_LOSTCONNECT,                      //�������Ӷ�ʧ, param[0]=nouse,  param[6:1]=bd_addr
    BT_NOTICE_CONNECTED,                        //�������ӳɹ�, param[0]=feat,index,  param[6:1]=bd_addr
    BT_NOTICE_CONNECT_STATUS,
    BT_NOTICE_DISCONNECT_STATUS,
    BT_NOTICE_RECON_FINISH,                     //������������
    BT_NOTICE_SCO_SETUP,
    BT_NOTICE_SCO_KILL,
    BT_NOTICE_INCOMING,                         //����
    BT_NOTICE_RING,                             //��������
    BT_NOTICE_OUTGOING,                         //ȥ��
    BT_NOTICE_CALL,                             //����ͨ��
    BT_NOTICE_SET_SPK_GAIN,                     //����ͨ������
    BT_NOTICE_MUSIC_PLAY,                       //�������ֿ�ʼ����
    BT_NOTICE_MUSIC_STOP,                       //��������ֹͣ����
    BT_NOTICE_MUSIC_CHANGE_VOL,                 //�ı�������������
    BT_NOTICE_MUSIC_SET_VOL,                    //����������������
    BT_NOTICE_HID_CONN_EVT,                     //HID���������¼�
    BT_NOTICE_SET_QTEST_CH,
    BT_NOTICE_FIRST_CONNECT,                    //��һ�������ֻ�, param[5:0]=bd_addr
    BT_NOTICE_A2DP_VOL_CTRL,

    BT_NOTICE_TWS_SEARCH_TIMEOUT = 0x40,        //����TWS��ʱ
    BT_NOTICE_TWS_CONNECT_START,                //��ʼ����TWS, param[0]=nouse,  param[6:1]=bd_addr
    BT_NOTICE_TWS_CONNECT_FAIL,                 //TWS����ʧ��, param[0]=reason, param[6:1]=bd_addr
    BT_NOTICE_TWS_DISCONNECT,                   //TWS���Ͽ�,   param[0]=reason, param[6:1]=bd_addr
    BT_NOTICE_TWS_LOSTCONNECT,                  //TWS���Ӷ�ʧ, param[0]=nouse,  param[6:1]=bd_addr
    BT_NOTICE_TWS_CONNECTED,                    //TWS���ӳɹ�, param[0]=feat,index,  param[6:1]=bd_addr
    BT_NOTICE_TWS_SET_VOL,                      //����TWS��������
    BT_NOTICE_TWS_HID_SHUTTER,                  //Զ��TWS���ռ�
    BT_NOTICE_TWS_INFO,                         //TWSͬ����Ϣ
    BT_NOTICE_TWS_USER_KEY,                     //TWS�Զ��尴��
    BT_NOTICE_TWS_SEARCH_STATUS,
    BT_NOTICE_TWS_TO_STATUS,
    BT_NOTICE_TWS_GET_MAP_TIME,
    BT_NOTICE_TWS_SET_OPERATION,
    BT_NOTICE_TWS_RES_PLAY,
    BT_NOTICE_TWS_PLAY_STA_SYNC,
    BT_NOTICE_TWS_EQ_SYNC,
    BT_NOTICE_TWS_RES7,
};

//������Ϣ
enum bt_msg_t {
    BT_MSG_OFF          = 0,                    //�ر�����
    BT_MSG_ON,                                  //������
    BT_MSG_RES0,
    BT_MSG_RES1,
    BT_MSG_CONNECT,                             //��������
    BT_MSG_DISCONNECT,                          //�Ͽ�����
    BT_MSG_PLAY_PAUSE,                          //�л����š���ͣ
    BT_MSG_VOL_CHANGE,                          //��������
    BT_MSG_CALL_REDIAL,                         //�ز��绰�����һ��ͨ����
    BT_MSG_CALL_ANSWER_INCOM,                   //��������
    BT_MSG_CALL_TERMINATE,                      //�Ҷ�ͨ��������
    BT_MSG_CALL_SWAP,                           //�л���ͨ�绰
    BT_MSG_HFP_REPORT_BAT,                      //�����ص���
    BT_MSG_HFP_SPK_GAIN,                        //����ͨ������������
    BT_MSG_HFP_MIC_GAIN,                        //����ͨ����˷�����
    BT_MSG_HFP_PRIVATE_SWITCH,                  //�л�˽��ͨ��
    BT_MSG_HID_CONNECT,                         //����HID
    BT_MSG_HID_DISCONNECT,                      //�Ͽ�HID
    BT_MSG_AUDIO_BYPASS,                        //��������SBC/SCO AUDIO
    BT_MSG_AUDIO_ENABLE,                        //ʹ������SBC/SCO AUDIO
    BT_MSG_LOW_LATENCY_EN,
    BT_MSG_LOW_LATENCY_DIS,
    BT_MSG_TWS_DISCONNECT,                      //�Ͽ�TWS����
    BT_MSG_TWS_CONNECT,                         //����TWS����
    BT_MSG_TWS_SEARCH_SLAVE,                    //����������TWS����
    BT_MSG_TWS_NOTICE_RING,
    BT_MSG_SWITCH_SIRI,
    BT_MSG_EAR_STA_CHANGE,
    BT_MSG_NR_STA_CHANGE,
    BT_MSG_TWS_NOTICE_INCOME_NUMBER,
    BT_MSG_HFP_AT_CMD,                          //�ز��绰����hfp_get_outgoing_number��ȡ���룩
    BT_MSG_SIRI_ON,
    BT_MSG_CALL_ANSWER_INCOM_REJ_OTHER,         //�������磬��ͨʱ�Ҷϵ�ǰͨ��
    BT_MSG_CONNECT_ADDRESS,
    BT_MSG_FCC_TEST_FUNC,
    BT_MSG_PLAY_PRIVATE_SWITCH,                 //�л��ֻ�����/��������
    BT_MSG_TWS_SYNC_INFO,
    BT_MSG_NOR_CONNECT,
    BT_MSG_NOR_DISCONNECT,
    BT_MSG_2ACL_PALY_SWITCH,                    //һ�϶������л�
    BT_MSG_TWS_SET_BYPASS,
    BT_MSG_RESET_BT_NAME,
    BT_MSG_TWS_SWITCH,                          //�����л�
    BT_MSG_CALL_REDIAL_NUMBER,
    BT_MSG_MAX,

    BT_MSG_HID_KEY      = 0x30,                 //HID����
    BT_MSG_HID_CONSUMER,                        //CONSUMER����
    BT_MSG_TWS_USER_KEY,                        //TWS�û��Զ�����Ϣ
    BT_MSG_HID_TOUCH_SCREEN,                    //������
    BT_MSG_PBAP,
    BT_MSG_BAT_HOUSE_CTRL,
    BT_MSG_HSP_SEND,
    BT_MSG_SYNC_INFO,                           //������ͬ��һЩ״̬��Ϣ
    BT_MSG_TWS_SET_SCAN,
    BT_MSG_SET_SCAN,
    BT_MSG_START_WORK,
	BT_MSG_ATT_VA,
	BT_MSG_STORE_A2DP_VOLUME,                   //�������ӵ�ͬ������
	BT_MSG_MAP,
	BT_MSG_ADV0_SET_EN,
    BT_MSG_PARAM_MAX,

    BT_MSG_PLAY         = AVC_PLAY,             //����
    BT_MSG_PAUSE        = AVC_PAUSE,            //��ͣ
    BT_MSG_STOP         = AVC_STOP,             //ֹͣ
    BT_MSG_PREV         = AVC_BACKWARD,         //��һ��
    BT_MSG_NEXT         = AVC_FORWARD,          //��һ��
    BT_MSG_REWIND       = AVC_REWIND,           //��ʼ����
    BT_MSG_FAST_FORWARD = AVC_FAST_FORWARD,     //��ʼ���
    BT_MSG_REWIND_END       = AVC_REWIND|0x80,  //��������
    BT_MSG_FAST_FORWARD_END = AVC_FAST_FORWARD|0x80, //�������
    BT_MSG_SPEED_DOUBLE     = AVC_SPEED_DOUBLE|0x80, //�첥*2
    BT_MSG_SPEED_HALF       = AVC_SPEED_HALF|0x80,   //����1/2

    BT_MSG_NO           = 0xff,
};

enum sync_info_enum {
    TWS_SYNC_INFO_LEDCNT,               //ͬ�� led cnt
    TWS_SYNC_INFO_EQ,

    TWS_SYNC_TWS_RES0 = 0x40,           //0x40~0x7f for tws_res
    TWS_SYNC_TWS_RES_MAX = 0x7f,

    TWS_SYNC_RUN = 0x80,                //ͬ������
};

//control
void bt_init(void);                             //��ʼ����������
int bt_setup(void);                             //������ģ��
void bt_off(void);                              //�ر�����ģ��
void bt_wakeup(void);                           //��������ģ��
void bt_start_work(uint8_t opcode, uint8_t scan_en); //������ʼ������opcode: 0=����, 1=������, 2=tws������,ֱ���Զ����
void bt_send_msg(uint msg);                     //����������Ϣ���������bt_msg_t
void bt_audio_bypass(void);                     //����SBC/SCOͨ·�ر�
void bt_audio_enable(void);                     //����SBC/SCOͨ·ʹ��
void bt_connect_address(void);                  //�������ݵ�ַ���� ��Ҫ�� uint8_t bt_get_connect_addr(uint8_t *bd_addr);������
void bt_set_sco_far_delay(void *buf, uint size, uint delay);

void bt_set_scan(uint8_t scan_en);              //��/�رտɱ����ֺͿɱ�����, bit0=�ɱ�����, bit1=�ɱ�����
void bt_connect(void);                          //�����豸����
void bt_disconnect(void);                       //�����豸�Ͽ�
void bt_hid_connect(void);                      //����HID�������
void bt_hid_disconnect(void);                   //����HID����Ͽ�
int bt_hid_is_connected(void);
bool bt_hid_is_ready_to_discon(void);
void ble_popup_ctrl(uint opcode);
bool sco_is_connected(void);
bool bt_sco_is_msbc(void);

//status
uint bt_get_disp_status(void);                  //��ȡ�����ĵ�ǰ��ʾ״̬, V060
uint bt_get_status(void);                       //��ȡ�����ĵ�ǰ״̬
uint8_t bt_get_scan(void);                      //�жϵ�ǰ�ɱ����ӿɱ�ɨ��״̬
uint bt_get_call_indicate(void);                //��ȡͨ���ĵ�ǰ״̬
uint bt_get_siri_status(void);                  //��ȡSIRI��ǰ״̬, 0=SIRI���˳�, 1=SIRI�ѻ���
bool bt_is_calling(void);                       //�ж��Ƿ�����ͨ��
bool bt_is_playing(void);                       //�ж��Ƿ����ڲ���
bool bt_is_ring(void);                          //�ж��Ƿ���������
bool bt_is_testmode(void);                      //�жϵ�ǰ�����Ƿ��ڲ���ģʽ
bool bt_is_sleep(void);                         //�ж������Ƿ��������״̬
bool bt_is_connected(void);                     //�ж������Ƿ������ӣ�TWS���������ӣ����������ֻ������ӣ�
bool bt_is_ios_device(void);                    //�жϵ�ǰ���ŵ��Ƿ�ios�豸
uint32_t bt_sleep_proc(void);
void bt_enter_sleep(void);
void bt_exit_sleep(void);
bool bt_is_silence(void);
bool bt_cur_dev_is_silence(void);
void bt_other_dev_silence_reset(void);
bool bt_sync_tick(void);                        //��������ʱ��ͬ����50ms�����Ϊ5ms

//tws
bool bt_tws_is_connected(void);                 //tws�豸�Ƿ�������
bool bt_tws_is_slave(void);                     //tws�����жϣ�ע��Ҫ���ж�tws�Ƿ�������
uint8_t bt_tws_get_scan(void);                  //tws�Ƿ��Ѵ򿪿ɱ�����������
void bt_tws_set_scan(uint8_t scan_en);          //��/�ر�tws�ɱ�����������, bit0=�ɱ�����, bit1=�ɱ�����
void bt_tws_disconnect(void);                   //�Ͽ�tws�豸
void bt_tws_connect(void);                      //����tws�豸
void bt_tws_search_slave(void);                 //����tws�豸������������
uint bt_tws_get_link_info(uint8_t *bd_addr);    //��ȡtws�����Ϣ������Ϊ�Ƿ���Թ�TWS���ж�
bool bt_tws_user_key(uint keycode);             //����tws�Զ��尴��, 10bit��Ч
bool bt_tws_sync_ring_tone(void);               //ͬ����tws�������
void bt_tws_clr_ring_tone(void);                //
bool bt_tws_need_ring_sync(void);
void bt_tws_switch(void);                       //�����л�������ͨ��ʱ������MIC
bool tws_switch_is_need(void);                  //�鿴�Ƿ���Ҫ�л�
void bt_tws_sync(uint sync_info);               //ͬ��״̬��Ϣ sync_info_enum
void bt_tws_sync_run(uint ntick);               //ͬ����ntick * 50ms
bool bt_nor_is_connected(void);                 //�Ƿ��������ֻ�
void bt_nor_connect(void);                      //�����ֻ�
void bt_nor_disconnect(void);                   //�Ͽ��ֻ�
bool bt_nor_get_link_info(uint8_t *bd_addr);    //��ȡ�ֻ������Ϣ��bd_addr=NULLʱ����ѯ�Ƿ���ڻ�����Ϣ
uint8_t bt_connect_for_the_first_time(uint8_t *bd_addr);//��������������ַ�Ƿ�Ϊ��һ�����ӡ�0:first_time, 1:not_first_time, 255:no record for given bd_addr
bool bt_nor_get_link_info_addr(uint8_t *bd_addr, uint8_t order);    //��ȡ��n���ֻ������Ϣ��bd_addr=NULLʱ����ѯ�Ƿ���ڻ�����Ϣ
bool bt_tws_is_auto_switch(void);
bool bt_tws_get_auto_channel(void);
bool btstack_tws_atmos_led(void);
bool btstack_tws_qtest_channel(uint8_t ch);
void lm_set_name_buf(const char *bt_name, uint max_len);
bool bt_tws_get_link_rssi(int8_t *buffer, bool left_channel);
u8 bt_get_tws_remote_status(void);              //��ȡ�Զ���״̬

void bt_clr_all_link_info(u8 flag);             //ɾ��ȫ�������Ϣ
void bt_nor_clr_link_info(u8 flag);             //ɾ���ֻ������Ϣ
void bt_tws_clr_link_info(u8 flag);             //ɾ��tws�����Ϣ


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

    //����MSG����ϵ�ǰ���ŵ���ʾ��
    TWS_RES_RING_STOP   = 50,
    TWS_BRE_LED_START,
    TWS_BRE_LED_START1,
    TWS_BRE_LED_STOP,
    TWS_ATMOS_LED_ON,
    TWS_ATMOS_LED_OFF,

    //���ֻ��63
};

void tws_res_init(void);                        //api
void tws_res_play(uint8_t index);               //api

void tws_mp3_res_play_kick(u32 addr, u32 len);  //tws_res��ʼ����
u32 tws_res_sync(u32 limit);                    //tws_resͬ��
void tws_res_add(u8 index, u32 ticks);          //tws_res�����б�
void tws_res_proc(void);                        //tws_res���̴���
bool tws_res_play_sync(u32 flag);
void tws_res_done(void);
void tws_res_reset(void);
u32 tws_res_get_ticks(u8 index);
bool tws_res_hssi_sync_wait(isr_t isr_callback);
bool tws_res_is_busy(void);

//��������
#define bt_scan_enable()                        bt_set_scan(0x03)                       //��ɨ��
#define bt_scan_disable()                       bt_set_scan(0x00)
#define bt_fcc_test_start()                     bt_send_msg(BT_MSG_FCC_TEST_FUNC)   //fcc kick

//��������
#define bt_music_play()                         bt_send_msg(BT_MSG_PLAY)            //����
#define bt_music_pause()                        bt_send_msg(BT_MSG_PAUSE)           //��ͣ
#define bt_music_play_pause()                   bt_send_msg(BT_MSG_PLAY_PAUSE)      //�л�����/��ͣ
#define bt_music_stop()                         bt_send_msg(BT_MSG_STOP)            //ֹͣ
#define bt_music_prev()                         bt_send_msg(BT_MSG_PREV)            //��һ��
#define bt_music_next()                         bt_send_msg(BT_MSG_NEXT)            //��һ��
#define bt_music_rewind()                       bt_send_msg(BT_MSG_REWIND)          //��ʼ����
#define bt_music_rewind_end()                   bt_send_msg(BT_MSG_REWIND_END)      //��������
#define bt_music_fast_forward()                 bt_send_msg(BT_MSG_FAST_FORWARD)    //��ʼ���
#define bt_music_fast_forward_end()             bt_send_msg(BT_MSG_FAST_FORWARD_END)//�������
#define bt_low_latency_enble()                  bt_send_msg(BT_MSG_LOW_LATENCY_EN)      //����ʹ�ܵ���ʱ
#define bt_low_latency_disable()                bt_send_msg(BT_MSG_LOW_LATENCY_DIS)     //�����رյ���ʱ
#define bt_music_play_switch()                  bt_send_msg(BT_MSG_2ACL_PALY_SWITCH)
#define bt_music_play_private_switch()          bt_send_msg(BT_MSG_PLAY_PRIVATE_SWITCH) //�л��ֻ�����/������������
bool bt_is_low_latency(void);

//����ͨ��
#define bt_call_redial_last_number()            bt_send_msg(BT_MSG_CALL_REDIAL)     //�绰�ز������һ��ͨ����
#define bt_call_answer_incoming()               bt_send_msg(BT_MSG_CALL_ANSWER_INCOM) //�����绰����ͨʱ����ǰͨ��
#define bt_call_answer_incom_rej_other()        bt_send_msg(BT_MSG_CALL_ANSWER_INCOM_REJ_OTHER)	//�����绰����ͨʱ�Ҷϵ�ǰͨ��
#define bt_call_terminate()                     bt_send_msg(BT_MSG_CALL_TERMINATE)  //�Ҷϵ绰
#define bt_call_swap()                          bt_send_msg(BT_MSG_CALL_SWAP)       //�л���ͨ�绰
#define bt_call_private_switch()                bt_send_msg(BT_MSG_HFP_PRIVATE_SWITCH)       //�л���ͨ�绰
#define bt_siri_switch()                        bt_send_msg(BT_MSG_SWITCH_SIRI)     //����SIRI, android��Ҫ�����������д򿪡�������������������, ios��Ҫ��siri����
#define bt_siri_on()                            bt_send_msg(BT_MSG_SIRI_ON)         //����������

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
int bt_spp_tx(uint8_t *packet, uint16_t len);           //����SPP����
bool bt_spp_tx_ex(u8 idx, const char * cmd, u8 len);    //�����ȵķ���SPP����

//hid
bool bt_hid_send_key(uint msg, uint keycode);
#define bt_hid_key(keycode)                     bt_hid_send_key(BT_MSG_HID_KEY, keycode)        //��׼HID��, ��Enter
#define bt_hid_consumer(keycode)                bt_hid_send_key(BT_MSG_HID_CONSUMER, keycode)   //�Զ���HID��, ��VOL+ VOL-
/**
 * @brief ģ�ⴥ�㺯��
   ע��:IOS ��Χ��-2047-2048 ����׿��0-4096;
        IOS�豸��x,y�����λ�ã�����10,10����Ե�ǰλ���ƶ�10,10;
        ��׿�豸��x��y�Ǿ���λ�ã�10,10�����ֻ�10,10��λ����;
 * @param is_press	1���£�0̧��
 * @param x	ģ�ⴥ�������
 * @param y	ģ�ⴥ��������
 **/
void bt_hid_point_pos(bool is_press, s16 x, s16 y);
#define bt_hid_finger_select_ios()              bt_send_msg(BT_MSG_HID_TOUCH_SCREEN | 1 << 8)                //������Ƶѡ��IOSϵͳ, ��Ҫ��bt����ǰ��������
#define bt_hid_finger_select_andriod()          bt_send_msg(BT_MSG_HID_TOUCH_SCREEN | 2 << 8)                //������Ƶѡ��andriodϵͳ, ��Ҫ��bt����ǰ��������

//��������
void bt_hid_douyin_down(void);
void bt_hid_douyin_up(void);
void bt_hid_douyin_p(void);
void bt_hid_douyin_pp(void);
void bt_hid_douyin_ctrl(u8 ctrl);


/*****************************************************************************
 * BLE API�����ӿ�
 *****************************************************************************/
void le_set_adv_interval(u16 interval);

#endif //_API_BTSTACK_H
