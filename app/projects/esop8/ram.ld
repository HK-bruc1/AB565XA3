ENTRY(_start)

/* Define the flash max size */
__max_flash_size = 256k;

__base = 0x10000000;

/* Define the area */
__comm_vma = 0x64000;
__max_comm_size = 0x1200;
__bcomm_vma = 0x18C00;
__max_bcomm_size = 0x1400;
__raminit_vma = 0x17000;
__aram_vma = 0x50000;
__cram_vma = 0x18000;
__dram_vma = 0x58000;
__nram_vma = 0x40000;

/* 专用overlap area */
__acomm_vma = 0x50000;
__max_acomm_size = 0x6e00;
__sleep_comm_vma = 0x52000;
__max_sleep_comm_size = 0x1000;
__pwrdwn_comm_vma = 0x52000;
__max_pwrdwn_comm_size = 0x1000;
__dbb_comm_vma = 0x41400;
__max_dbb_comm_size = 0x400;

MEMORY
{
    init            : org = __base,             len = 512
    flash(rx)       : org = __base + 512,       len = __max_flash_size
    comm(rx)        : org = __comm_vma,         len = __max_comm_size
    bcomm(rx)       : org = __bcomm_vma,        len = __max_bcomm_size
    acomm(rx)       : org = __acomm_vma,        len = __max_acomm_size
    sleep_comm(rx)  : org = __sleep_comm_vma,   len = __max_sleep_comm_size
    pwrdwn_comm(rx) : org = __pwrdwn_comm_vma,  len = __max_pwrdwn_comm_size
    dbb_comm(rx)    : org = __dbb_comm_vma,     len = __max_dbb_comm_size
    raminit(rx)     : org = __raminit_vma,      len = 512

    data            : org = 0x18500,            len = 0x700
    stack           : org = 0x63C00,            len = 1k
    aram            : org = __aram_vma,         len = 27k + 0x200
    cram            : org = __cram_vma,         len = 0x500
    dram            : org = __dram_vma,         len = 0x4500
    nram            : org = __nram_vma,         len = 6k
}

SECTIONS
{
    .init : {
        *(.reset)
        *(.init_text)
        . = ALIGN(512);
    } > init

    .bcomm : {
        *(.bt_voice*)
        *(.text.sleep.rf)
        *(.com_text.led_disp)
        *(.com_text.ret.bt21.isr*)
        *(.com_text.rf*)
        *(.com_text.mav)
        *(.com_text.mute)
        *(.com_text.mictrim)
        *(.com_text.sbccode)
        *(.com_text.aec*)
        *(.com_text.tkey*)
        *(.com_text.bsp.tkey*)
        *(.com_text.bsp.key*)
        *(.com_text.bb.*)
        *(.com_text.bt.*)
        . = ALIGN(512);
    } > bcomm AT > flash

    .comm : {
        KEEP(*(.vector))
        *(.plt)
        *(.com_text*)
        *(.com_rodata*)
        *(.srodata*)
        *debug.o(.rodata*)
        *(.data*)
        *(.sdata*)
        . = ALIGN(512);
    } > comm AT > flash

    .raminit : {
        *(.ramint.text*)
        . = ALIGN(512);
    } > raminit AT > flash

    /* 专用overlap area, 请勿修改 */
    .acomm : {
        *(.text.opus.asm)
        *(.text.opus.proc)
        *(.text.opus.ec_tell)
        *(.text.opus.entenc)
        *(.text.opus.mathops)
        *(.text.opus.metric)
        *(.text.opus.compute_mdcts)
        *(.text.opus.mdct)
        *(.text.opus.bands)
        *(.text.opus.kissfft)
        *(.text.opus.comb)
        *(.text.opus.vq)
        *(.rodata.opus*)
        . = ALIGN(512);
    } > acomm AT > flash

    .sleep_comm : {
        *(.text.sleep_com*)
        *(.sleep_text*)
        . = ALIGN(512);
    } > sleep_comm AT > flash

    .pwrdwn_comm : {
        *(.text.pwrdwn*)
        . = ALIGN(512);
    } > pwrdwn_comm AT > flash

    .dbb_comm : {
        *(.text.dbb*)
        . = ALIGN(512);
    } > dbb_comm AT > flash

    .flash : {
        *(.text.bb.tws.sw_nanos)
        . = ALIGN(512);
        *(.text.bfunc.bt)
        *(.text.bfunc.sleep)
        *(.text.bfunc.process)
        *(.text.bfunc.warning)
        *(.text.bfunc.vhouse)
        *(.text.bfunc*)
        *(.text.opus*)
        *(.text.ains3*)
        *(.rodata.ains3*)
        *(.text*)
        *(.rodata*)
        *(.rela*)
        . = ALIGN(512);
    } > flash

    .bss (NOLOAD):
    {
        __bss_start = .;
        *(.bss*)
        *(.sbss*)
        *(COMMON)
        *(.buf*)
        *(.wavres.buf)
        *(.anc.*)
        *(.anc_buf*)
        *(.ble_buf*)
        *(.ble_cache*)
        __bss_end = .;
    } > data
    __bss_size = __bss_end - __bss_start;

    .stack (NOLOAD) : {
        __irq_stack_start = .;
        . = 0x400;
        __irq_stack = .;
    } > stack
    __irq_stack_size = __irq_stack - __irq_stack_start;

    .aram_fcc __aram_vma (NOLOAD) : {
        . = 0x3350;
        *(.fcc_buf)
    } > aram

    /* 复用硬件DNN降噪的ram */
    .ains3_ram __aram_vma (NOLOAD) : {
        . = 0x5830;
        *(.ains3_buf.*)
        . = 0x6990;
    } > aram

    /* fcc huart使用buf, 复用mrom buff*/
    .cram_fcc __cram_vma (NOLOAD) : {
        *(.huart_buf)
    } > cram


    /* 进入快测模式后使用的buf*/
    .cram_qtest __cram_vma (NOLOAD) : {
        *(.qtest_buf)
    } > cram


    nram_opus __nram_vma (NOLOAD) : {
        *(.siri*)
        *(.opus_buf.buf)
        *(.att_siri.buf)
        *(.mavsco_buf)
        . = 0x1714;

        . = 0x1800;
    } > nram

    nram_dbb __nram_vma (NOLOAD) : {
        *(.codecs_buf.pcm)
        *(.codecs_pcm*)
        . = 0x1400;
    } > nram

    .dram_opus __dram_vma (NOLOAD) : {
        *(.opus_buf*)
        . = 0x4500;
    }  > dram

}

/* Calc the lma */
__comm_lma = LOADADDR(.comm);
__comm_size = SIZEOF(.comm);

__bcomm_lma = LOADADDR(.bcomm);
__bcomm_size = SIZEOF(.bcomm);

__acomm_lma = LOADADDR(.acomm);
__acomm_size = SIZEOF(.acomm);

__sleep_comm_lma = LOADADDR(.sleep_comm);
__sleep_comm_size = SIZEOF(.sleep_comm);

__pwrdwn_comm_lma = LOADADDR(.pwrdwn_comm);
__pwrdwn_comm_size = SIZEOF(.pwrdwn_comm);

__raminit_lma = LOADADDR(.raminit);
__raminit_size = SIZEOF(.raminit);

__dbb_comm_lma = LOADADDR(.dbb_comm);
__dbb_comm_size = SIZEOF(.dbb_comm);


