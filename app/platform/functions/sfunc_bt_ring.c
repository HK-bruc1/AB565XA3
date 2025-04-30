#include "include.h"
#include "func.h"
#include "func_bt.h"

#if FUNC_BT_EN

f_bt_ring1_cb f_bt_ring1;

#if WARNING_BT_INCALL

void bt_ring_var_init(void)
{
    f_bt_ring.len = 0;
}

void bt_ring_stop(void)
{
    f_bt.ring_stop = 1;
    bt_tws_user_key(TWS_USER_KEY_RING_STOP);
}


#endif

bool bt_ring_esbc_stop(u8 force)
{
    if(f_bt_ring1.esbc_flag == 0){
        return false;
    }
    if(force || (music_cb.sta == MUSIC_STOP) ){
        if(music_cb.sta != MUSIC_STOP) {            //避免来电响铃/报号未完成，影响get_music_dec_sta()状态
            music_control(MUSIC_MSG_STOP);
        }
        sys_clk_free(INDEX_RES_PLAY);
        f_bt_ring1.esbc_flag = 0;
        return true;
    }
    return false;
}

int bt_ring_esbc_play(u8 index, const u8 *ptr, const u8 *sptr, u32 size, u32 tws_sync)
{
    if ((size == 0) || (ptr == 0)) {
        return -1;
    }
    if (ptr[0] != 0x8B) {                       //头不匹配
        return -2;
    }
    sys_clk_req(INDEX_RES_PLAY, SYS_52M);

    if(music_cb.sta != MUSIC_STOP) {            //避免来电响铃/报号未完成，影响get_music_dec_sta()状态
        music_control(MUSIC_MSG_STOP);
    }
    f_bt_ring1.esbc_flag = 1;

    esbc_init(ptr, size);
    res_play_start_do(index);
    if (tws_sync) {
        tws_res_play_sync(1);
    } else {
        music_control(MUSIC_MSG_PLAY);
    }

    return 0;
}


/**
 * @brief 蓝牙来电状态处理函数
 *
 * 该函数负责处理蓝牙来电过程中的铃声播放、状态更新和TWS同步等功能。
 * 主要功能：
 * 1. 计算铃声播放间隔时间
 * 2. 检测来电状态变化并更新ring_sta状态
 * 3. 处理来电号码播报功能
 * 4. 处理手机铃声(inband ring)和本地铃声播放
 * 5. 控制振动马达功能
 */
void sfunc_bt_ring_process(void)
{
    // 计算铃声播放间隔时间，根据配置工具设置的铃声间隔选择
    u32 ticks = 50 + 500 * (u32)xcfg_cb.bt_ring_ticks_sel;

    // 首次播放铃声使用较短的延时(300ms)
    if (f_bt_ring.play.ring_first) {
        f_bt_ring.play.ring_first = 0;
        ticks = 300;
    }

    // 执行系统周期性处理和蓝牙子系统处理
    func_process();
    func_bt_sub_process();

    // 检测来电状态是否已改变，如果不再是来电状态或不在蓝牙模式，则准备退出
    if (f_bt.disp_status != BT_STA_INCOMING || func_cb.sta != FUNC_BT) {
        if (bt_tws_is_connected()) {
            // TWS已连接，需要处理同步退出
            if (tone_is_playing()) {
                // 如果正在播放提示音，需要等待提示音播放完成
                if (f_bt.ring_sta == 1) {
                    // 主机发送停止铃声命令给从机
                    if (!bt_tws_is_slave()) {
                        tws_res_play(TWS_RES_RING_STOP);
                    }
                    // 设置状态为等待提示音同步结束
                    f_bt.ring_sta = 3;
                }
            }
#if !BT_CALL_RING_EXIT_FIX
            // 如果音乐解码器还在工作，设置状态为等待音乐停止
            else if (get_music_dec_sta() != MUSIC_STOP) {
                f_bt.ring_sta = 2;
            }
#endif
            // 如果提示音已停止且音乐解码器已停止，可以直接退出
            else {
                f_bt.ring_sta = 0;
            }
        } else {
            // 非TWS模式，直接退出
            f_bt.ring_sta = 0;
        }
    }

    // 检测铃声是否播放完一次，如果是则更新计时器和计数器
    if (bt_ring_tone_stop(0) || bt_ring_esbc_stop(0) || bt_ring_sbc_stop(0)) {
        // 重置铃声计时器
        f_bt_ring.play.ring_tick = tick_get();
        // 铃声播放次数加1
        f_bt_ring.play.ring_cnt++;
    }

#if WARNING_BT_INCALL
    // 检查是否需要播放来电提示音
    if (bt_is_ring() && f_bt.ring_sta == 1 && !f_bt.ring_stop) {
    #if BT_HFP_RING_NUMBER_EN
        // 来电号码播报功能：在铃声响过指定次数后播报来电号码
        if ((xcfg_cb.bt_hfp_ring_number_en) && (f_bt_ring.play.ring_cnt >= BT_HFP_RINGS_BEFORE_NUMBER) && (f_bt_ring.play.cur < f_bt_ring.len)) {
            // 确保音乐解码器已停止，TWS资源空闲，且距离上次播放已超过500ms
            if (get_music_dec_sta() == MUSIC_STOP && !tws_res_is_busy() && tick_check_expire(f_bt_ring.play.ring_tick, 500)) {
                // 播放当前数字的语音提示
                if (f_bt_ring.buf[f_bt_ring.play.cur] != 0xff) {
                    // 主机负责播放，从机同步
                    if (!bt_tws_is_slave()) {
                        tws_res_play(f_bt_ring.buf[f_bt_ring.play.cur] + TWS_RES_NUM_0);
                    }
                }
                // 移动到下一个数字
                f_bt_ring.play.cur++;
                // 重置计时器
                f_bt_ring.play.ring_tick = tick_get();
            }
        } else if (get_music_dec_sta() == MUSIC_STOP)  // 等待号码播报完成
    #endif
        {
    #if BT_HFP_INBAND_RING_EN
            // 手机铃声(inband ring)功能
            if (xcfg_cb.bt_hfp_inband_ring_en) {
                if (sco_is_connected()) {
                    // SCO已连接，可以播放手机铃声
                    if (f_bt_ring.play.inband_ring_sta != 2) {
                        // 设置状态为已建立SCO连接
                        f_bt_ring.play.inband_ring_sta = 2;
                        // 启用蓝牙音频
                        bt_audio_enable();
                        // 解决某些iOS设备来电铃声音量过小的问题
                        if (sys_cb.hfp_vol < 6) {
                            sys_cb.hfp_vol = 6;
                            bsp_change_volume(bt_hfp_get_sys_vol(sys_cb.hfp_vol));
                        }
                        // 音频淡入
                        dac_fade_in();
                    }
                } else if (!tick_check_expire(f_bt_ring.play.ios_ring_tick, f_bt_ring.play.w4_sco_tick)) {
                    // SCO未连接但尚未超时，等待SCO建立
                    if (f_bt_ring.play.inband_ring_sta != 2) {
                        f_bt_ring.play.inband_ring_sta = 1;  // 等待状态
                    }
                } else {
                    // 等待SCO超时，切换到本地铃声
                    if (f_bt_ring.play.inband_ring_sta == 1) {
                        f_bt_ring.play.inband_ring_sta = 0;  // 清除等待状态
                    }
                }
            }
    #endif
            // 播放本地铃声：当没有使用手机铃声、没有播放提示音、TWS资源空闲且距离上次播放已超过设定时间
            if (!f_bt_ring.play.inband_ring_sta && !tone_is_playing() && !tws_res_is_busy() && tick_check_expire(f_bt_ring.play.ring_tick, ticks)) {
                // 播放本地铃声资源
                if (PIANO_BT_RING && !bt_tws_is_slave()) {
                    tws_res_play(TWS_RES_RING);
                    f_bt_ring.play.ring_tick = tick_get();
                }
            }
        }

        // 处理振动马达功能
        motor_ring_process();
    }
#endif
}

/**
 * @brief 蓝牙来电状态初始化函数
 *
 * 该函数负责初始化蓝牙来电环境，包括设置铃声参数、音频状态和TWS同步等。
 * 主要功能：
 * 1. 初始化来电相关变量和结构体
 * 2. 设置音频参数和铃声播放时间
 * 3. 根据设备类型(iOS/Android)调整等待SCO连接的时间
 * 4. 配置TWS同步铃声播放
 * 5. 处理来电号码播报功能
 */
void sfunc_bt_ring_enter(void)
{
#if WARNING_BT_INCALL
    // 初始化来电播放控制结构体
    memset(&f_bt_ring.play, 0, sizeof(t_play_cb));
    memset(&f_bt_ring1, 0, sizeof(f_bt_ring1));
    sbc_res_var_init();

    // 保存当前音量增益偏移值，用于退出时恢复
    f_bt_ring.play.gain_offset = sys_cb.gain_offset;

    // 设置iOS设备铃声时间戳和等待SCO连接的超时时间
    f_bt_ring.play.ios_ring_tick = tick_get();
    f_bt_ring.play.w4_sco_tick = 3000;  // 默认等待3秒
    if (bt_is_ios_device()) {
        f_bt_ring.play.w4_sco_tick = 6500;  // iOS设备等待6.5秒
    }

    // 清除铃声停止标志
    f_bt.ring_stop = 0;

    // 设置DAC模拟偏移为0
    dac_set_anl_offset(0);

    // 切换到蓝牙音频旁路模式，准备播放铃声
    bt_audio_bypass();

    // 停止当前可能正在播放的音乐
    music_control(MUSIC_MSG_STOP);

    // 初始化铃声计时器和首次播放标志
    f_bt_ring.play.ring_tick = tick_get();
    f_bt_ring.play.ring_first = 1;  // 标记为首次播放铃声
#endif // WARNING_BT_INCALL

#if BT_HFP_CALL_EARPHONE_EN
    // 启用通话切换到耳机功能
    sys_cb1.call_switch = 1;
#endif

    // 设置铃声状态为正常播放状态
    f_bt.ring_sta = 1;

    if (bt_tws_is_connected()) {
        // TWS已连接，尝试同步铃声
        if (!bt_tws_sync_ring_tone()) {
            // 同步失败，重置铃声计时器
            f_bt_ring.play.ring_tick = tick_get();
        }
    } else {
        // 非TWS模式，处理来电号码播报功能
        if (xcfg_cb.bt_hfp_ring_number_en) {
            // 等待来电号码获取，最多等待约3.5秒(70*10*5ms)
            int i;
            for(i = 0; i < 70; i++) {
                func_bt_status();
                // 如果已获取到号码，或不再是来电状态，则退出等待
                if((f_bt_ring.len != 0) || !bt_is_ring() || (f_bt.disp_status != BT_STA_INCOMING)) {
                    break;
                }
                delay_5ms(10);
            }
        }
    }

    // 重置TWS资源，准备新的铃声播放
    tws_res_reset();

#if DAC_DNR_EN
    // 保存当前DAC降噪状态，并临时关闭降噪功能
    f_bt_ring.dac_sta = dac_dnr_get_sta();
    dac_dnr_set_sta(0);
#endif
}

/**
 * @brief 蓝牙来电状态退出函数
 *
 * 该函数负责清理来电状态的资源，恢复系统设置，并准备进入下一个状态。
 * 主要功能：
 * 1. 清除来电状态标志
 * 2. 停止所有铃声播放
 * 3. 清理TWS同步资源
 * 4. 关闭振动马达
 * 5. 恢复音频设置和音量
 */
void sfunc_bt_ring_exit(void)
{
    // 清除来电状态标志
    f_bt.ring_sta = 0;

    // 强制停止所有类型的铃声播放
    bt_ring_tone_stop(1);
    bt_ring_esbc_stop(1);
    bt_ring_sbc_stop(1);

    // 清理TWS铃声同步资源
    bt_tws_clr_ring_tone();

#if BT_HFP_INBAND_RING_EN
    // 如果正在使用手机铃声(inband ring)，关闭它并切换到音频旁路模式
    if (f_bt_ring.play.inband_ring_sta == 2) {
        f_bt_ring.play.inband_ring_sta = 0;
        bt_audio_bypass();
    }
#endif

#if WARNING_BT_INCALL
    // 关闭振动马达
    motor_disable();

    // 清除来电号码缓冲区
    f_bt_ring.len = 0;

    // 确保音乐播放已停止
    music_control(MUSIC_MSG_STOP);

#if DAC_DNR_EN
    // 恢复DAC降噪状态
    dac_dnr_set_sta(f_bt_ring.dac_sta);
#endif

    // 恢复音量增益偏移值
    sys_cb.gain_offset = f_bt_ring.play.gain_offset;

    // 应用系统音量设置
    bsp_change_volume(sys_cb.vol);

    // 启用蓝牙音频，准备进入下一个状态
    bt_audio_enable();
#endif
}

AT(.text.func.btring)
void sfunc_bt_ring(void)
{
    /**
     * @brief 蓝牙来电状态处理函数
     *
     * 该函数在检测到蓝牙来电时被调用，负责处理来电铃声播放、按键响应和状态转换等功能。
     * 整个来电处理流程包括：初始化来电环境、循环处理来电状态、退出来电状态。
     *
     * 主要功能：
     * 1. 调用sfunc_bt_ring_enter()初始化来电环境，设置铃声参数和状态变量
     * 2. 在循环中处理来电状态，直到f_bt.ring_sta变为0（来电结束）
     * 3. 在循环中调用sfunc_bt_ring_process()处理铃声播放和状态更新
     * 4. 在循环中调用sfunc_bt_ring_message()处理用户按键消息（如接听/拒接）
     * 5. 来电结束后调用sfunc_bt_ring_exit()清理资源
     *
     * 状态控制：
     * - f_bt.ring_sta=1：正常来电铃声播放状态
     * - f_bt.ring_sta=2：TWS模式下等待音乐停止
     * - f_bt.ring_sta=3：TWS模式下等待提示音同步结束
     * - f_bt.ring_sta=0：来电状态结束
     */
    printf("%s\n", __func__);

    // 初始化来电环境，设置铃声参数和状态变量
    sfunc_bt_ring_enter();

    // 循环处理来电状态，直到f_bt.ring_sta变为0（来电结束）
    while (f_bt.ring_sta) {
        // 处理铃声播放和状态更新
        sfunc_bt_ring_process();
        // 处理用户按键消息（如接听/拒接）
        //这里会获取队列中的消息，传递给内层的消息处理函数
        sfunc_bt_ring_message(msg_dequeue());
//        func_bt_display();
    }

    // 来电结束后清理资源
    sfunc_bt_ring_exit();
}

#endif //FUNC_BT_EN
