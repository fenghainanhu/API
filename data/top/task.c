





17G05A  软件关机重启,充电调试 (这个是重点),17G05A的mmi测试
/*debug*/
{
软件关机重启:解决



mmi测试：加入充电测试选项


充电电流太小：标准充电器（1.9A） USB充电（500mA）
{

rt5081_pmu_irq_.c    
    2.rt5081_pmu_irq_handler
    中断回调函数

rt5081_pmu_charger.c
    1.rt5081_pmu_ovpctrl_uvp_d_evt_irq_handler
    这段走的是非type-c接口
    rt5081_pmu_ovpctrl_uvp_evt_irq_handler

    3.rt5081_pmu_attachi_irq_handler
    这个应该是rt5081接受到中断后，就是接口插入的

        4.rt5081_inform_psy_changed
        将检测到的信息上报到power_supply子系统

mtk_charger.c
        5.mtk_charger_int_handler
        mt_charger对应的中断回调函数

        6._wake_up_charger
        唤醒mt_charger

        /* Turn on USB charger detection */
        7.rt5081_enable_chgdet_flow
        进行USB充电使能的检测

        8.rt5081_set_usbsw_state
        设置USB的状态

        9.mtk_is_charger_on->mtk_is_charger_in->rt5081_plug_in
 
 rt5081_pmu_charger.c       
        10.rt5081_plug_in
        rt5081检测到充电器插入所要进行的动作

mt_charger.c
        11.battery_callback
        对电池状态变化上报给power_supply子系统

        11.rt5081_enable_charging
        充电使能，这个充电使能只是简单的对寄存器使用set或者clear，置位或者清空





charger_routine_thread这个是常用的电池状态检测线程

(rt5081_pmu_charger.c) rt5081_chg_ops  rt5081提供的函数接口


RT5081_PMU_REG_CHGCTRL2这个寄存器写1可以将充电线上的电直接提供给系统不给电池
1. TE (0x12, bit[4]) : If this bit is enabled, the power path
will be turned off, and the buck of the charger will keep
providing power to the system

}

















按键不管用


马达振动


电池曲线的导入    

}



G1605A  售后问题，功耗问题 ， 电池维护特性，healthd线程
/*debug*/
{
    售后问题：   电量计不准的机器
    这个还不知道怎么看，只是觉得电量跟电池电压偏差有点大，而且底层电量跟上层显示的电量差别也很大



    apk发包次数，谁发的(主从)，就是链接到哪了？？




    电池维护特性：
        机器有问题了，tp不好使怎么办，万一还不了QAQ
        正在验证

    

}

















