
1.thransplant移植相关

2.power功耗

3.charging充电

4.thermal温升

5.USB&&OTG相关的

6.anthor其他问题




Fuel_gauge更换了充电芯片
以前只是廉价的替代芯片

送测有那几个版本？
CMCC ，CTA他们送的是T54版本的软件




transplant代码移植相关：
/*{

GNSPR#53551  	电源管理：关机充电动画改善（移植充电显示相关的，BBL7332的hd720图标，S8的代码）
代码不多，也没什么变化，主要是细节（图片的差异用beyond compare，rules.mk里图片加进去的数组下标要对应，
编译烧版本要知道烧哪了，是否编译进去了）

多用beyond compare找差异

1.移植需要注意的地方（首先应该明确关机充电流程，充电logo显示，切换，要不要把开机，关机动画也顺便修改一下）

	
  hd720充电的图标--->rules.mk要加进去编译成logo.bin
	rules.mk
	把这行注释了，并加到开关宏的后面
	#$(BOOT_LOGO_DIR)/$(BOOT_LOGO)/$(BOOT_LOGO)_kpoc_charge.raw
	然后就是图片目录要不要替换或者换个名字，把编译的名字也换一个


  lk阶段代码移植--->mt6735...编译脚本加进编译的开关宏
	
	两个条件宏GN_MTK_BSP_LK_CHARGE_GIONEELOGO，CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT,
	两个宏用了if-else条件编译
	platform.c,mt_logo.c

	gnbj6737t_66_m0.mk 但是BBL7332那两行是注释的
	#GioneeDrv LiLuBao 20161110 modify for logo change begin
	#DEFINES += GN_MTK_BSP_LK_CHARGE_GIONEELOGO
	CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT = yes
	DEFINES += CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT
	#GioneeDrv LiLuBao 20161110 modify for logo change end

	增加了G1605A.mk编译CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT


  libshowlogo目录下的代码--->G1605.mk总的编译脚本加进编译的宏
	在外部的vendor里面，改完之后还要在金立目录下提交
	lk目录下show_animation_common.c，
	external目录下show_animation_common.c，
	cust_display.h ，bootlogo.cpp  ，common.cpp，charging_animation.cpp

	gionee目录下有几个是要分层加编译选项的

	
	vendor/mediatek/proprietary/external/charger/Android.mk 脚本里面加了--->这个不需要
	#Gionee LiLuBao 20161110 modify for gioneelogo begin
	ifeq ($(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT), yes)
	LOCAL_CFLAGS += -DCONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT
	endif
	#Gionee LiLuBao 20161110 modify for gioneelogo begin

	编译的脚本改的有问题？改的一直打印不出来,这么简单的工作搞那么长时间....
	宏开关以外的也没打印？
	rules.mk	Android.mk	gnbj6737t_66_m0.mk	G1605A.mk

	单编有时候有问题，改的目录也不对，log的等级不够？
	其实是你烧错了地方

	
	a.代码相关
	CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT这个宏控制S8关机充电显示的图标

	G1605A上是这个宏GN_MTK_BSP_LK_CHARGE_GIONEELOGO			（kpoc？）

	BBL7332上相关的代码
	gionee/project/BBL7332A02_A_CTA/bootable/bootloader/lk/project/gionee6735_65c_l1.mk:#CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT = yes
	gionee/project/BBL7332A02_A_CTA/bootable/bootloader/lk/project/gionee6735_65c_l1.mk:#DEFINES += CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT
	gionee/project/BBL7332/bootable/bootloader/lk/project/gionee6735_66t_l1.mk:#CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT = yes
	gionee/project/BBL7332/bootable/bootloader/lk/project/gionee6735_66t_l1.mk:#DEFINES += CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT
	gionee/project/BBL7332/bootable/bootloader/lk/dev/logo/rules.mk~:ifeq ($(strip $(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)), yes)
	gionee/project/BBL7332/bootable/bootloader/lk/dev/logo/rules.mk:ifeq ($(strip $(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)), yes)
	gionee/project/BBL7332/bootable/bootloader/lk/platform/mt6735/platform.c:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/bootable/bootloader/lk/platform/mt6735/mt_logo.c:        #if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/bootable/bootloader/lk/platform/mt6735/mt_logo.c:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/bootable/bootloader/lk/platform/mt6735/mt_logo.c:        #if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.c~:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.c:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:    #ifndef CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT 
	gionee/project/BBL7332/vendor/mediatek/proprietary/external/libshowlogo/cust_display.h:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/project/BBL7332/vendor/mediatek/proprietary/external/charger/bootlogo.cpp:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT) 
	gionee/project/BBL7332/vendor/mediatek/proprietary/external/charger/common.cpp:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT) 
	gionee/project/BBL7332A02_A/bootable/bootloader/lk/project/gionee6735_65c_l1.mk:#CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT = yes
	gionee/project/BBL7332A02_A/bootable/bootloader/lk/project/gionee6735_65c_l1.mk:#DEFINES += CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT

	gionee/alps_drv/bootable/bootloader/lk/dev/logo/rules.mk:ifeq ($(strip $(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)), yes)
	gionee/alps_drv/bootable/bootloader/lk/platform/mt6735/platform.c:            #if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/bootable/bootloader/lk/platform/mt6735/mt_logo.c:        #if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/bootable/bootloader/lk/platform/mt6735/mt_logo.c:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/bootable/bootloader/lk/platform/mt6735/mt_logo.c:        #if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/bootable/bootloader/lk/lib/libshowlogo/show_animation_common.c:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.h:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)                        
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/show_animation_common.c:    #ifndef CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT 
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:    	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/charging_animation.cpp:		#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/vendor/mediatek/proprietary/external/libshowlogo/charging_animation.h:#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT)
	gionee/alps_drv/vendor/mediatek/proprietary/external/charger/bootlogo.cpp:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT) 
	gionee/alps_drv/vendor/mediatek/proprietary/external/charger/common.cpp:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT) 
	gionee/alps_drv/vendor/mediatek/proprietary/external/ipod/ipodcommon.cpp:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT) 
	gionee/alps_drv/vendor/mediatek/proprietary/external/ipod/bootlogo.cpp:	#if defined(CONFIG_GN_BSP_AMIGO_CHARGING_SUPPORT) 



	/*
	 * Check logo.bin address if valid, and get logo related info
	 * pinfo[0] : the number of all pictures in logo.bin
	 * pinf0[1] : the whole length of logo.bin
	 * pinf0[2] : the index = 0 picture's length
	 * pinfo[3+index] - pinfo[2+index] : means index length
	 */


	logo index 反应不同状态logo，数组下标
	// Common LOGO index
	#define BOOT_LOGO_INDEX   0 
	#define KERNEL_LOGO_INDEX   38 

	#define ANIM_V0_BACKGROUND_INDEX   1 
	#define ANIM_V1_BACKGROUND_INDEX   35
	 
	 
	#define LOW_BATTERY_INDEX   2 
	#define CHARGER_OV_INDEX   3 
	#define FULL_BATTERY_INDEX   37 


	这是后又来了一个问题就是patch，补丁？


3.总结，这次移植用了这么长时间，好心酸.....，最后发现图片压缩的一个脚本有问题
	
	a.有时间把整个流程走一遍？
	logo.bin怎么生成的，boot_logo,kernel_logo怎么调用的，从哪开始调用的，动画（应该是以一定频率播放图片），fstab分区，电量的百分比变化


	b.关机充电的流程，充电动画
	
		1、正常开机流程，按开机键。

		可大致分成三部分

		（1）OS_level：UBOOT、kenrel、init这三步完成系统启动；

		（2）Android_level：这部分完成android部的初始化；

		（3）Home Screen：这部分就是我们看到的launcher部分。
	
		2.关机状态下，只走到init阶段
	


	c.关机状态下主要涉及两个过程
	lk阶段：
		显示uboot的logo，这个过程有一个检测的过程，在platform_init()函数中进行一定的初始化包括LCM背光，对电池状态的检测，充电类型的检测
	，调用mt_disp_show_kpoc_charge_logo()进行关机充电在lk阶段的初始化调用，显示logo就是用framebuffer的方式对不同的像素点用RGB不同的比例显示出来
	然后按一定频率刷新屏幕，显示不同的图片。最终显示的画面是不同图片的组合
		
	system阶段
		对电池状态进行检测,电池处于什么样的状态，显示什么百分比，动画图标的变换


	这其中有这几个函数，涉及到最后显示出来的效果framebuffer的处理
/**********************************************************************************************************************************
 * Fill a screen size buffer with logo content
 *********************************************************************************************************************************/
void fill_animation_logo(unsigned int index, void *fill_addr, void * dec_logo_addr, void * logo_addr, LCM_SCREEN_T phical_screen)
{
    LOGO_PARA_T logo_info;
    int logo_width;
    int logo_height;
    int raw_data_size;
    if(check_logo_index_valid(index, logo_addr, &logo_info) != CHECK_LOGO_BIN_OK)
        return;

    raw_data_size = decompress_logo((void*)logo_info.inaddr, dec_logo_addr, logo_info.logolen, phical_screen.fb_size);
    //RECT_REGION_T rect = {0, 0, phical_screen.width, phical_screen.height};
    logo_width =phical_screen.width;
    logo_height = phical_screen.height;
    if (phical_screen.rotation == 270 || phical_screen.rotation == 90) {
        logo_width =phical_screen.height;
        logo_height = phical_screen.width;
    }
    if (0 == bits) {
        if (raw_data_size == logo_width*logo_height*4) {
            bits = 32;
        } else if (raw_data_size == logo_width*logo_height*2) {
            bits = 16;
        } else {
            LOG_ANIM("[show_animation_common: %s %d]Logo data error\n",__FUNCTION__,__LINE__);
            return;
        }
        LOG_ANIM("[show_animation_common: %s %d]bits = %d\n",__FUNCTION__,__LINE__, bits);
    }

    RECT_REGION_T rect = {0, 0, logo_width, logo_height};

    fill_rect_with_content(fill_addr, rect, dec_logo_addr, phical_screen, bits);


}
	

/***********************************************************************************************************************************
 * Draw a rectangle region  with logo content
 **********************************************************************************************************************************/
void fill_rect_with_content(void *fill_addr, RECT_REGION_T rect, void *src_addr, LCM_SCREEN_T phical_screen, unsigned int bits)
{
	SLOGD("[show_logo_common: %s %d]\n",__FUNCTION__,__LINE__);
    if (check_rect_valid(rect) != CHECK_RECT_OK)
        return;
    if (bits == 32) {
        if (phical_screen.fill_dst_bits == 16) {
            fill_rect_with_content_by_16bit_argb8888((unsigned short *)fill_addr, rect, (unsigned int *)src_addr, phical_screen);
        } else if (phical_screen.fill_dst_bits == 32){
            fill_rect_with_content_by_32bit_argb8888((unsigned int *)fill_addr, rect, (unsigned int *)src_addr, phical_screen, bits);
        } else {
            SLOGD("[show_logo_common %s %d]unsupported phical_screen.fill_dst_bits =%d\n",__FUNCTION__,__LINE__, phical_screen.fill_dst_bits );
        }
    } else {
        if (phical_screen.fill_dst_bits == 16) {
            fill_rect_with_content_by_16bit_rgb565((unsigned short *)fill_addr, rect, (unsigned short *)src_addr, phical_screen);
        } else if (phical_screen.fill_dst_bits == 32){
            fill_rect_with_content_by_32bit_rgb565((unsigned int *)fill_addr, rect, (unsigned short *)src_addr, phical_screen, bits);
        } else {
            SLOGD("[show_logo_common %s %d]unsupported phical_screen.fill_dst_bits =%d\n",__FUNCTION__,__LINE__, phical_screen.fill_dst_bits );
        }
}
	




	移植OTG开关到BBL7515上，USB协议是








}*/




















power待机功耗相关的
/*{

	基本的概念：
	1.Cell standby：手机搜索信号的稳定

	2.volte：
	VoLTE是基于IMS的语音业务。IMS由于支持多种接入和丰富的多媒体业务，成为全IP时代的核心网标准架构。
	经历了过去几年的发展成熟后，如今IMS已经跨越裂谷，成为固定话音领域VoBB、PSTN网改的主流选择，而且也被3GPP、GSMA确定为移动语音的标准架构
	。VoLTE即Voice over LTE，它是一种IP数据传输技术，无需2G/3G网，全部业务承载于4G网络上，可实现数据与语音业务在同一网络下的统一。
	换言之，4G网络下不仅仅提供高速率的数据业务，同时还提供高质量的音视频通话，后者便需要VoLTE技术来实现。


1.功耗要注意的几个地方

	a.首先明确是否有modem log，c2kmdlog文件，modem log 特别耗电，而且占空间然后察看modem log的时间变化。
	c2kmdlog文件应该是记录传输的数据，recycle_config，modem log会经常唤醒系统，导致系统没有睡下去，所以耗电量肯定高，
	开始那几个谁开的modem log耗电四五十的。

	modem常用的几个信道，哪些跟耗电密切相关的？
	{
		channel 10是AT交互
		channel 14是读写nvram
		channel 34是与WCN通信，告知wifi当前modem所使用频段。
		
		10是跟at command有关，一般都是网络变化
		55是跟volte相关
		
		基本上，上述3个channel的跟modem的网络唤醒有关。

		wakeup source:(2/42)
		42是modem log开关
		20是流量

	}

	
	b.抓取battersystats.log和wakeup_sources.log，以及导出mobile log；
	抓取方法：
	adb shell dumpsys batterystats > battersystats.log
	adb shell cat /sys/kernel/debug/wakeup_sources > wakeup_sources.log

	c.比较关键的两个log文件batterystats.log和kernel.log
	batterystats.log记录系统耗电的过程
	
	1.signal level 当前环境的信号质量
	信号质量差，modem一直在找信号，耗电量肯定大，所以网络环境变化在一定程度上也会造成耗电。

	2.wake lock 应用是否长时间占用锁
	wake up alarm, wake up by 
	以上两个都有一个限度，什么样才属于过度，什么样在合理范围内?

	3.系统是否被经常被应用唤醒
	在sys.log目录下，搜索alarm 
	应用推送业务，频繁唤醒。

	kernel部分的统计思路是：
	1. 搜索"wake up by"关键字，只看kworker或是system_server进程打印的
	2. 如果是EINT event的话，要接着搜索"is pending"关键字，找到是哪个EINT
	EINT是外部中断，GPT是什么？
	3. 如果CLDMA event的话，要接着搜索"wakeup source"关键字，找到是哪个channel			
	4. 计算时间的话，从wakeup event后面的第一个PM: suspend exit到后面第一个PM: suspend entry之间的时间差

	sys log部分的统计思路是：
	1. 搜索关键字"wakeup alarm"
	2. 只查找type为0和type为2的alarm
	3. 过滤package name做统计

	
	极致省电模式电流：5mA以上
	飞行模式待机电流：3——4mA
	未开数据待机电流：5——6mA
	开数据:10mA< x < 20mA
	开wifi待机电流：<20mA
	正常的待机电流小于20mA，所以超过20mA有异常
	这些只是一部分的待机电流

	待机功耗，待机电流就你们问题多



	功耗测试的标准：
	1.移动运营商的电流为9ma，电信运营商的电流为9mA，联通运营商的电流为16mA
	2.计算公式：电流（ma）×时间（H）=Mah
	  如联通卡待机12小时耗电量为：（16×12）÷6020=3%
	3.如果手机插双卡，就在单卡的基础上电流加2ma；
	  如果手机开wifi，就在待机基础上加5ma
	4.如果双卡中插了联通卡，就优先联通算法，在单卡的基础上加2mA
	5.如果手机开启飞行模式进行待机，直接以5ma的电流进行计算
	  如待机12小时的耗电量为：（5×12）÷6020≈1%
	6.耗电量算法根据实际项目要求为主，这里仅供参考。

	机~器-人 2016-11-21 11:28:32
	测试前注意事项：
	1.高通平台的手机，测试充电，耗电相关项目时，LOG只开前两项，一定不能开启QXDM LOG；
	2.MTK平台的手机，不能开启MODEM LOG,否则很耗电，不方便数据的统计，只开启MobileLog中的 Android Log、Kernel Log和第三项NetworkLog，开log后需要重启手机。
	3.次日早上来正常开启所有的LOG,包括QXDM LOG也要开启，然后重启手机，若是MTK平台的手机则将MODEM LOG重启开启，否则通话类的问题LOG无效；
	4.每次重启手机都要进入开发选项中确认USB调试是否开启；
	5.测试过程中要注意自己的LOG开启是否正确，高通平台的logs文件夹是5个,MTK平台的LOG开启时要注意当前LOG主页面的三项是否都是正常开启；
	6.耗电测试时，测试前若是清除过LOG，需要重启手机；
	7.测试耗电相关时，如果中途有换电池和升级新版本，需要充满一次电后再验证；
	8.验证第二三四六七晚的电量问题，都需截图，且第二天早来不能急着清理log，需得到组长确认后再清理log；

	
	待机电流问题：
	1.飞行模式下的底电流是否正常。
	2.屏幕熄灭，系统不会立刻睡下去，要等待一定的时间系统才能睡下去，同时测试的时间也不能太短
	3.通过log察看唤醒源是谁，谁唤醒的，同时要关闭应用推送业务，避免推送经常唤醒系统。
	4.modem搜信号会唤醒系统
	环境变化影响，一般的MODEM 待机测试，需要关闭应用推送业务，蓝牙／WIFI／GPS，最好关闭数据链接，
        因为应用推送业务瞬间一段时间会比较打，等待10-15 分钟稳定，测试20分钟左右


	
从我这边分析的结果看：没看到网路异常. 就是QQ/WEchat 定时起来跟网路做.

从alarm这部分的唤醒看，都是QQ 跟微信 5min起来一次跟服务器联网.
11-24 20:00:25.009802 1532 1746 D AlarmManager: wakeup alarm = Alarm{3bb42d2 type 0 when 1479988824257 com.tencent.mobileqq}; package = com.tencent.mobileqqneedGrouping = false
11-24 20:00:25.012924 1532 1746 D AlarmManager: wakeup alarm = Alarm{5839fa3 type 2 when 22724930 com.tencent.mm}; package = com.tencent.mmneedGrouping = false
11-24 20:00:25.016047 1532 1746 D AlarmManager: wakeup alarm = Alarm{69e59 type 0 when 1479988824940 com.gionee.cloud.gpe}; package = com.gionee.cloud.gpeneedGrouping = false
11-24 20:09:10.015841 1532 1746 D AlarmManager: wakeup alarm = Alarm{869acfc type 2 when 23056399 com.android.phone}; package = com.android.phoneneedGrouping = true
11-24 20:10:24.025028 1532 1746 D AlarmManager: wakeup alarm = Alarm{2de3d39 type 0 when 1479989424024 com.tencent.mobileqq}; package = com.tencent.mobileqqneedGrouping = true
11-24 20:15:10.028418 1532 1746 D AlarmManager: wakeup alarm = Alarm{b1307fb type 2 when 23324539 com.tencent.mm}; package = com.tencent.mmneedGrouping = true
11-24 20:15:10.030567 1532 1746 D AlarmManager: wakeup alarm = Alarm{aab1018 type 0 when 1479989424178 com.gionee.cloud.gpe}; package = com.gionee.cloud.gpeneedGrouping = true
11-24 20:15:10.035478 1532 1746 D AlarmManager: wakeup alarm = Alarm{ae42056 type 2 when 23325469 com.tencent.mm}; package = com.tencent.mmneedGrouping = true
11-24 20:15:10.039940 1532 1746 D AlarmManager: wakeup alarm = Alarm{94d15c4 type 2 when 23325472 com.tencent.mm}; package = com.tencent.mmneedGrouping = true
11-24 20:15:10.044497 1532 1746 D AlarmManager: wakeup alarm = Alarm{7347b73 type 2 when 23610512 com.android.phone}; package = com.android.phoneneedGrouping = true
11-24 20:20:24.093182 1532 1746 D AlarmManager: wakeup alarm = Alarm{da6d6bf type 2 when 23630547 com.tencent.mm}; package = com.tencent.mmneedGrouping = true
11-24 20:20:24.095583 1532 1746 D AlarmManager: wakeup alarm = Alarm{c31d18c type 0 when 1479990024066 com.gionee.cloud.gpe}; package = com.gionee.cloud.gpeneedGrouping = true
11-24 20:20:24.099179 1532 1746 D AlarmManager: wakeup alarm = Alarm{d5710d5 type 2 when 23924577 com.tencent.mm}; package = com.tencent.mmneedGrouping = true
11-24 20:21:48.014131 1532 1746 D AlarmManager: wakeup alarm = Alarm{194ce6d type 2 when 23925104 com.tencent.mm}; package = com.tencent.mmneedGrouping = true


Alarm 起来的时候，会伴随着连接网路的动作，CLDMA-10主要是反馈网路RRC的连接状态，用于分组处理.
11-24 20:00:25------->11-24 20:00:27, sleep_time = 26.9 , wake_up_time = 2.1 , wake_up_reason = alarm
11-24 20:05:55------->11-24 20:05:57, sleep_time = 328.7 , wake_up_time = 2.7 , wake_up_reason = alarm
11-24 20:06:16------->11-24 20:06:17, sleep_time = 17.6 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
11-24 20:09:10------->11-24 20:09:11, sleep_time = 172.7 , wake_up_time = 1.2 , wake_up_reason = alarm
11-24 20:10:24------->11-24 20:10:25, sleep_time = 72.8 , wake_up_time = 1.7 , wake_up_reason = alarm
11-24 20:14:09------->11-24 20:14:10, sleep_time = 223.3 , wake_up_time = 1.3 , wake_up_reason = alarm
11-24 20:14:29------->11-24 20:14:31, sleep_time = 19.0 , wake_up_time = 2.2 , wake_up_reason = CLDMA_MD-10
11-24 20:15:10------->11-24 20:15:14, sleep_time = 38.5 , wake_up_time = 5.0 , wake_up_reason = alarm
11-24 20:20:24------->11-24 20:20:29, sleep_time = 309.0 , wake_up_time = 5.7 , wake_up_reason = alarm
11-24 20:20:49------->11-24 20:20:50, sleep_time = 19.7 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
11-24 20:21:48------->11-24 20:21:49, sleep_time = 57.4 , wake_up_time = 1.7 , wake_up_reason = alarm
11-24 20:30:24------->11-24 20:30:26, sleep_time = 514.3 , wake_up_time = 2.1 , wake_up_reason = alarm
11-24 20:30:44------->11-24 20:30:45, sleep_time = 18.5 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
11-24 20:31:48------->11-24 20:31:49, sleep_time = 62.2 , wake_up_time = 1.7 , wake_up_reason = alarm
11-24 20:37:48------->11-24 20:37:50, sleep_time = 358.3 , wake_up_time = 2.7 , wake_up_reason = alarm
11-24 20:38:09------->11-24 20:38:10, sleep_time = 19.1 , wake_up_time = 1.2 , wake_up_reason = CLDMA_MD-10
11-24 20:40:25------->11-24 20:40:26, sleep_time = 134.1 , wake_up_time = 1.2 , wake_up_reason = alarm
11-24 20:41:48------->11-24 20:42:17, sleep_time = 81.8 , wake_up_time = 29.7 , wake_up_reason = alarm
11-24 20:42:21------->11-24 20:42:22, sleep_time = 3.3 , wake_up_time = 1.2 , wake_up_reason = alarm

从main log看，都是QQ 建立socket 连接.
Line 3118: 11-24 23:36:13.029622 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :80 
Line 3153: 11-24 23:37:18.034159 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :80 
Line 3188: 11-24 23:38:23.024540 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :80 
Line 3221: 11-24 23:39:27.023168 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :443 
Line 3256: 11-24 23:40:33.029502 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :443 
Line 3289: 11-24 23:41:38.028188 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :14000 
Line 3332: 11-24 23:42:43.028241 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :14000 
Line 3357: 11-24 23:42:45.721971 1734 2651 D Posix : [Posix_connect Debug]Process com.gionee.cloud.gpe :5222 
Line 3376: 11-24 23:43:47.020887 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :80 
Line 3415: 11-24 23:44:51.033229 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :80 
Line 3448: 11-24 23:45:55.019767 5041 5100 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq:MSF :80 
Line 3479: 11-24 23:45:56.217867 5012 13486 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq :14000 
Line 3487: 11-24 23:45:56.223832 5012 13487 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq :14000 
Line 3509: 11-24 23:45:56.302290 5012 8514 D Posix : [Posix_connect Debug]Process com.tencent.mobileqq :443
	
	
	
2.出现的相BUG


GNSPR#55512：
	现象：单卡电信数据wifi微信qq15h耗电11%

	经常被唤醒，未能睡眠下去wakeup source CLDMA
	  Start clock time: 2016-11-15-17-56-42
	  Screen on: 8m 3s 745ms (0.8%) 8x, Interactive: 8m 2s 529ms (0.8%)
	  Screen brightnesses:
	    dark 56s 557ms (11.7%)
	    bright 7m 7s 188ms (88.3%)
	  Connectivity changes: 3
	  Total full wakelock time: 1m 47s 277ms
	  Total partial wakelock time: 13m 13s 304ms
	  Mobile total received: 0B, sent: 0B (packets received 0, sent 0)
	  Phone signal levels:
	    none 18s 432ms (0.0%) 12x
	    great 33m 38s 7ms (3.5%) 27x
	  Signal scanning time: 18s 242ms
	  Radio types:
	    none 16h 9m 47s 328ms (100.0%) 1x
	  Mobile radio active time: 0ms (0.0%) 0x
	  Wi-Fi total received: 111.50MB, sent: 3.86MB (packets received 82222, sent 61884)
	  Wifi on: 5h 48m 8s 330ms (35.9%), Wifi running: 16h 9m 50s 595ms (100.0%)
	  Wifi states: (no activity)
	  Wifi supplicant states:
	    disconn 5ms (0.0%) 1x
	    scanning 4s 447ms (0.0%) 2x
	    associating 43ms (0.0%) 2x
	    associated 9ms (0.0%) 2x
	    4-way-handshake 320ms (0.0%) 2x
	    group-handshake 56ms (0.0%) 2x
	    completed 16h 9m 43s 681ms (100.0%) 2x
	  Wifi signal levels:
	    level(3) 12s 133ms (0.0%) 2x
	    level(4) 16h 9m 33s 984ms (100.0%) 3x
	  WiFi Idle time: 0ms (--%)
	  WiFi Rx time:   0ms (--%)
	  WiFi Tx time:   0ms (--%)
	  WiFi Power drain: 0mAh
	  Bluetooth Idle time: 0ms (--%)
	  Bluetooth Rx time:   0ms (--%)
	  Bluetooth Tx time:   0ms (--%)
	  Bluetooth Power drain: 0mAh


GNSPR#55283

	长时间占用锁
	  All kernel wake locks:
	  Kernel Wake lock ttyC0       : 1h 48m 0s 478ms (3279 times) realtime
	  Kernel Wake lock ccci_imsc   : 1h 3m 45s 858ms (3823 times) realtime
	  Kernel Wake lock PowerManagerService.WakeLocks: 57m 53s 239ms (11366 times) realtime
	  Kernel Wake lock ccci_fs     : 41m 52s 851ms (1342 times) realtime
	  Kernel Wake lock asc_rx_RxHdSdio: 30m 51s 64ms (1996 times) realtime
	  Kernel Wake lock cbp_rst     : 27m 22s 385ms (231 times) realtime
	  Kernel Wake lock ccci_monitor: 24m 0s 303ms (1041 times) realtime
	  Kernel Wake lock EINT wakelock: 21m 44s 679ms (1698 times) realtime
	  Kernel Wake lock asc_tx_TxHdCbp: 15m 47s 623ms (1661 times) realtime
	  Kernel Wake lock ccci_rpc_k  : 14m 11s 483ms (463 times) realtime


	/******/
	/* Wakeup Source */
	enum SPM_WAKE_SRC_LIST	{
		WAKE_SRC_SPM_MERGE = (1U << 0),  /* PCM timer, TWAM or CPU */
		WAKE_SRC_LTE_PTP = (1U << 1),
		WAKE_SRC_KP = (1U << 2),
		WAKE_SRC_WDT = (1U << 3),
		WAKE_SRC_GPT = (1U << 4),
		WAKE_SRC_EINT = (1U << 5),
		WAKE_SRC_CONN_WDT = (1U << 6),
		WAKE_SRC_CCIF0_MD = (1U << 7),
		WAKE_SRC_CCIF1_MD = (1U << 8),
		WAKE_SRC_LOW_BAT = (1U << 9),
		WAKE_SRC_CONN2AP = (1U << 10),
		WAKE_SRC_F26M_WAKE = (1U << 11),
		WAKE_SRC_F26M_SLEEP = (1U << 12),
		WAKE_SRC_PCM_WDT = (1U << 13),
		WAKE_SRC_USB_CD = (1U << 14),
		WAKE_SRC_USB_PDN = (1U << 15),
		WAKE_SRC_LTE_WAKE = (1U << 16),
		WAKE_SRC_LTE_SLEEP = (1U << 17),
		WAKE_SRC_SEJ = (1U << 18),
		WAKE_SRC_UART0 = (1U << 19),
		WAKE_SRC_AFE = (1U << 20),
		WAKE_SRC_THERM = (1U << 21),
		WAKE_SRC_CIRQ = (1U << 22),
		WAKE_SRC_MD1_VRF18_WAKE = (1U << 23),
		WAKE_SRC_SYSPWREQ = (1U << 24),
		WAKE_SRC_MD_WDT = (1U << 25),
		WAKE_SRC_C2K_WDT = (1U << 26),
		WAKE_SRC_CLDMA_MD = (1U << 27),
		WAKE_SRC_MD1_VRF18_SLEEP = (1U << 28),
		WAKE_SRC_CPU_IRQ = (1U << 29),
		WAKE_SRC_APSRC_WAKE = (1U << 30),
		WAKE_SRC_APSRC_SLEEP = (1U << 31)




GNSPR#54702:
	现象：单卡（移动3g 卡1），开启数据业务，未涉及项保持默认待机12小时，耗电26%
	
	终于回复了......

	{白卡没有正常睡眠下去，AP端一直在发送AT+ESIMAPP，导致手机无法休眠}?

	已经申请patch

	jinhao wu
	65440
	Thanks

	当前版本：alps-mp-m0.mp1-V2.140.3_gnbj6737t.66.m0_p23？
	怎么知道版本察看，这个在ProjectConfig.mk脚本里面有更新



GNSPR#54717:
	现象：T41版本手机待机一晚（12小时40分钟），实际耗电36%；单卡（联通）



GNSPR#53772
	现象：恢出厂清三方，开飞行模式关闭所有快捷开关，清后台待机14H，耗电6%、9%
	18：02->91%
	08:34->83%
	耗电8%，22.86mA
	这个eService关闭了


GNSPR#54515
	现象：
	恢出厂清三方，不开数据不开飞行关闭所有快捷开关，清后台待机11H，耗电8%  【2次】A3 A30
	
	20：00  ->68%
	08:13 ->60%
	耗电8%-->26mA
	
	有一段时间降的很快	

	20：58->94%
	08:12->86%
	耗电8%， 29mA



GNSPR#54713
	现象：
	单卡（电信 卡1），关闭数据业务，未开启飞行模式，未涉及项保持默认待机9.5小时，耗电7%



GNSPR#54241	恢出厂清三方，开数据关闭所有快捷开关，清后台待机11H，耗电16%、35%

这个跟modem相关

耗电比较大的局域是在上半夜，21点到0点这段时间，其中主要是集中在EINT212的唤醒，EINT212主要是C2K的ccci.
目前不是很确定是哪些channel出了问题，需要贵司进一步打开下面的log来做debug.

你先测试一下，保证有正常的log输出.

	/kernel-3.18/drivers/misc/mediatek/c2k_ccci/modem_sdio.c

	1.修改一.
	static void sdio_pio_intr_handler(struct sdio_func *func)
	..........
	..........
	if ((modem->msg->head.chanInfo == SDIO_AT_CHANNEL_NUM) ||
	(modem->msg->head.chanInfo == SDIO_AT2_CHANNEL_NUM) ||
	(modem->msg->head.chanInfo == EXCP_MSG_CH_ID) ||
	(modem->msg->head.chanInfo == EXCP_CTRL_CH_ID) ||
	(modem->msg->head.chanInfo == AGPS_CH_ID) ||
	(modem->msg->head.chanInfo == SDIO_AT3_CHANNEL_NUM)) {
	sdio_tx_rx_printk(modem->msg, 0);
	}
	.........
	.........
	修改成：
	sdio_tx_rx_printk(modem->msg, 0);
	也就是把if的条件去掉.

	2.修改二，默认把所有channel的信息打出来.
	同时修改：sdio_tx_rx_printk把LOGPRT(LOG_INFO, 修改成printk(
	/**
	*sdio_tx_rx_printk - print sdio tx and rx data, when log level is LOG_NOTICE or larger
	*@buf: the point of data buffer
	*@type: print type, 0:rx 1:tx
	*
	*no return
	*/
	static void sdio_tx_rx_printk(const void *buf, unsigned char type)
	{
	unsigned int count;
	const unsigned char *print_buf = (const unsigned char *)buf;
	const struct sdio_msg_head *head = NULL;
	/*int i; */
	/*return; */
	if (buf)
	head = (struct sdio_msg_head *)buf;
	count = calc_payload_len(head, NULL);
	if (type == 1)
	LOGPRT(LOG_INFO, "write %d to channel%d/[%d]>>",
	count, head->chanInfo, sdio_tx_cnt);
	else
	LOGPRT(LOG_INFO, "read %d from channel%d/[%d]<<",
	count, head->chanInfo, sdio_rx_cnt);
	/*if(count > RECORD_DUMP_MAX) */
	/*count = RECORD_DUMP_MAX; */
	LOGPRT
	(LOG_INFO, "%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x\n",
	*print_buf, *(print_buf + 1), *(print_buf + 2), *(print_buf + 3),
	*(print_buf + 4), *(print_buf + 5), *(print_buf + 6),
	*(print_buf + 7), *(print_buf + 8), *(print_buf + 9),
	*(print_buf + 10), *(print_buf + 11), *(print_buf + 12),
	*(print_buf + 13), *(print_buf + 14), *(print_buf + 15),
	*(print_buf + 16), *(print_buf + 17), *(print_buf + 18),
	*(print_buf + 19), *(print_buf + 20), *(print_buf + 21),
	*(print_buf + 22), *(print_buf + 23));
	/*
	for(i = 0; i < count + sizeof(struct sdio_msg_head); i++)
	{
	printk(KERN_CONT "%x-", *(print_buf+i));
	}
	printk(KERN_CONT "\n");
	*/
	}
	修改成：
	static void sdio_tx_rx_printk(const void *buf, unsigned char type)
	{
	unsigned int count;
	const unsigned char *print_buf = (const unsigned char *)buf;
	const struct sdio_msg_head *head = NULL;
	/*int i; */
	/*return; */
	if (buf)
	head = (struct sdio_msg_head *)buf;
	count = calc_payload_len(head, NULL);
	if (type == 1)
	printk("write %d to channel%d/[%d]>>",
	count, head->chanInfo, sdio_tx_cnt);
	else
	printk("read %d from channel%d/[%d]<<",
	count, head->chanInfo, sdio_rx_cnt);
	/*if(count > RECORD_DUMP_MAX) */
	/*count = RECORD_DUMP_MAX; */
	printk("%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x-%x\n",
	*print_buf, *(print_buf + 1), *(print_buf + 2), *(print_buf + 3),
	*(print_buf + 4), *(print_buf + 5), *(print_buf + 6),
	*(print_buf + 7), *(print_buf + 8), *(print_buf + 9),
	*(print_buf + 10), *(print_buf + 11), *(print_buf + 12),
	*(print_buf + 13), *(print_buf + 14), *(print_buf + 15),
	*(print_buf + 16), *(print_buf + 17), *(print_buf + 18),
	*(print_buf + 19), *(print_buf + 20), *(print_buf + 21),
	*(print_buf + 22), *(print_buf + 23));
	/*
	for(i = 0; i < count + sizeof(struct sdio_msg_head); i++)
	{
	printk(KERN_CONT "%x-", *(print_buf+i));
	}
	printk(KERN_CONT "\n");
	*/
	}
	先试一下把log是否可以打出来.




GNSPR #56384
	现象：【品质压力】恢复出厂设置》清除所有安装的三方应用,插入SIM卡-开关飞行模式,
	关闭后台所有后台运行/设置开关,电量为99%待机11H后电量为94%》耗电5% 2/40 C45、Q23、Q22

	



GNSPR#58813 送测待机电流偏高



};









charging充电相关的
/*{


需要注意的地方：

	1.OTG开关不能只开关中断，原因跟USB的热插拔相似，插入USB（插入模块，创建设备文件，节点），拔除USB（关闭中断，想上层上报信息，，关闭中断，卸载模块）。
	2.一些问题有待新版本测试，以前版本OTG和反向充电开关默认是关闭的
	3.fg_coulom库仑计的检测，变化？



GNSPR#54689	
	现象：测机放桌面，电量从9%直接掉0%自动关机后，连充电器自动开机，重启恢复（未做大量开关机动作）

	高优先级SSS	iphone电量经常这样跳变经常变成0%，然后关机

	19：55--->20:00--->20:18--->20:28
	9% -->中间自动关机了,电量跳变?-->7%-->2%-->3%?

	电量的0%检测？什么情况下会检测0%？
	mt_battery_0Percent_tracking_check()


	这可能的原因：电量计的偏差有关
		1.RTC记录的是上一次关机时候的电量，而中间电量几还没有来的急更新，又烧了新版本，
		或者插拔充电接口，电量变化了，但是电量计还没来的急更新，导致偏差变大
	
		2.电量计通常在100%或者0%更新，
		#define CUST_SYSTEM_OFF_VOLTAGE 3385 关机电压，而在9%左右电压已经低于关机电压了，
		可能是电量计偏差较大，或者负载较大的原因，将电压拉低了。
	<3>[37450.398205]  (2)[205:bat_routine_thr][kernel]AvgVbat 3434,bat_vol 3360, AvgI 0, I 0, VChr 0, AvgT 43, T 44, ZCV 3362, CHR_Type 0, SOC  10: 10:  9







GNSPR #53212 	 OTG：连接2A旅充头，简单操作测机10min，未充进电，充电前电量显示57%，充电后点电量显示57%（未做过大量重启、开关机操作）【随机一次】A29



GNSPR #52967	 OTG：G1605手机插上一拖一反充线给M6手机反充电，充电10分钟后，查看充电电流，其中有值低于800mA（附图）。



GNSPR #53732 	 OTG：设置中反向充电和ＭＭＩ反充充电，切换后，设置中开启反向充电和ＭＭＩ里面都不能反向充电。


这两个问题都是充电频繁断开，跟OTG底层接口有关
GNSPR #52977	 OTG：测试机G1605给另一个G1605和S8充电，两台被充电的手机一直很频繁充电/不充电（附视频）
GNSPR #52885	 OTG：使用G1605给S8反向充电，频繁充电过程出现中断，但很快又连接上的问题（附视频）




GNSPR#53131 
	现象：关机状态连充电器，长按电源键3s开机，10s后才有振动提示和开机画面 对比S9无【6台必现】A9

MTK提供的解决方法
	1.方案一：这种方法在于在握手之前先判断是否是长按了音量下（强制刷机）,个人觉得这种方案是可以的，刷机本来就是用组合键
	   Sure, you can add keypad check function before usb_handshake() as you point out.
	We already provided the keypad check API in preloader. For example, you can use: mtk_detect_key (MTK_PMIC_RST_KEY)
	We have found the cause as below. We will commit it soon, you can also take a try.
	bootable/bootloader/preloader / platform/mt6735/src/drivers/pmic.c
	Only when KCOL is enable(force download), it is possible to use the MTK download tool.
	We will modify that keypad check for mtk download tool is disable. (CFG_USB_DOWNLOAD :=0 / CFG_EMERGENCY_DL_SUPPORT :=0)
	Please modify that USB handshake function is possible when keypad is pressed,
	For example
		    #if CFG_USB_TOOL_HANDSHAKE
		    (Add keypad check function)
		    if (TRUE == usb_handshake(handler))
		           g_meta_com_type = META_USB_COM;
		           BOOTING_TIME_PROFILING_LOG("USB handshake");
		    #endif
		    #if CFG_
	disable “CFG_USB_TOOL_HANDSHAKE”, the above problems will not exist anymore.
	But you will not be able to use USB download and USB Meta tool via preloader.
	If you think that is ok, we don’t have other concerns.

	cust_bldr.mak

	alps\bootable\bootloader\preloader\custom\[project]\cust_bldr.mak
	CFG_UART_TOOL_HANDSHAKE :=0



		cust_bldr.mak:
		CFG_UART_TOOL_HANDSHAKE :=0

	default.mak
		MTK_PMIC_RST_KEY :=1

	main.c

	    #if CFG_USB_TOOL_HANDSHAKE
	    if( mtk_detect_key (MTK_PMIC_RST_KEY) ){
		if (TRUE == usb_handshake(handler))
                g_meta_com_type = META_USB_COM;
		    BOOTING_TIME_PROFILING_LOG("USB handshake");
	    }
	   //GioneeDrv LiLuBao 20161121 modify for fixed GNSPR#53131 end
            #endif


	2.方案二：修改检测延迟的时间timeout，不见效
	修改usb tool hanshake timour的时间为1s，这个修改可能是影响tool的连接使用，所以建议修改不能比1s少
	这里设置为1s
	#if !defined(CFG_USB_HANDSHAKE_TIMEOUT)
	-#define CFG_USB_HANDSHAKE_TIMEOUT (2500) /* 2.5s */
	+#define CFG_USB_HANDSHAKE_TIMEOUT (1000) /* 1s */
	#endif
	b>去掉uart hanshake
	/vendor/mediatek/proprietary/bootable/bootloader/preloader/platform/mt6755/default.mak
	CFG_UART_TOOL_HANDSHAKE :=0
	c>把dlpt初始化的位置挪到后面（从preloader移到lk阶段，但是刷机延迟时间就长了）

	#define CFG_USB_ENUM_TIMEOUT            (8000)           /* 8s */
	USB枚举时间超时，改变之后影响座充充电时间

	
	#define CFG_USB_HANDSHAKE_TIMEOUT       (2500)    	 /* 2.5s */
	USB握手时间超时，改变之后影响了用USB充电的时间



	3.方案三：
	将dlpt_init_inLK()移动到如下，在6s左右:如上传log：session--usb-44444.log
	#ifdef LK_PROFILING
	dprintf(CRITICAL, "[PROFILE] ------- show logo takes %d ms --------
	", (int)get_timer(time_show_logo));
	#endif
	/*add bebug by mtk ALPS02765977 20160624
	dlpt_init_inLK();




GNSPR#57824
	现象：手机低电量自动关机待机一晚上连接充电器》充电到7%以后长按power键开机》闪现开机画面后手机不开机》
	且从7%快速充到12%又快速掉到8%一直连着充电器【5分钟后长按power键恢复
	
	可能的原因：
		1.电量不准，电压低于开机电压
		之前电量计没有经过校准，自动关机后，关机充电到7%，启动系统，检测到电压未达到开机电压，然后系统启动不了。
		（系统的这些电量电压检测会在哪些时候调用）

		2.电量计未来的及初始化
		电量计未初始化（RTC相关）导致使用默认的值，电压为0，未达到开机电压，启动不了。

	





}*/














	







USB&&OTG相关的
/*{


	USB眼图




GNSPR#54386 	
	现象：测机连接USB至显示充电不弹选项，计算机查看无法识别USB，插拔数据线不恢复 ，重启恢复



GNSPR#57228
	现象：【自动化monkey 72h测试】手机adb连接异常，导致测试不通过。
		手机数据能连上，能充电，就是adb会断开，之前不进行monkey72h正常，但是进行了monkey之后部分机器会出现adb连击不上的
		情况
		（是否是应用软件冲突的原因-monkey72h，其他都能连接上，应该不是硬件的原因（硬件一般是什么原因会造成连接不上））	
		
		手机IMEI1和IMEI2默认设置为999999999999994（此操作目的：自动化测试服务器会把该IMEI号手机的USB调试默认为打开状态）
	
	手机adb链接出现问题的常见原因：
		adb：		
	首先adb的意思是手机打开USB调试并连接电脑后，在电脑上自动运行的一个进程，相当于驱动程序。
	出现上述情况的原因一般是驱动程序安装错误或者电脑及手机上有其他程序发生了冲突







}*/



	



















thermal温度控制相关的
/*{

	首先thermal.txt文件内的相关参数要知道什么意思，怎么改更合理，有效。

	相关的几个case(12时00分12秒)ALPS02710500 ，ALPS02796764 




在如下时间点系统发生cpu占用率高，导致系统弹出cpu占有率高的对话框，请驱动同事做进一步分析！

11-24 17:13:38.570567 <7>[25342.267555] (2)[509:AALLightSensor][name:aal_control&][ALS/AAL]Get als dat :40
11-24 17:13:38.718625 <3>[25342.415613] (0)[11158:kworker/0:2]<STEP_COUNTER> step_c data48
11-24 17:13:38.728131 <4>[25342.425119] (0)[11236:kworker/0:3][Power/CPU_Thermal]set_adaptive_cpu_power_limit 1621 T=63200,1734 T=62600,1884 T=61500,2010 T=61900,0 T=58700
11-24 17:13:38.728154 <4>[25342.425142] (0)[11236:kworker/0:3][Power/cpufreq] mt_cpufreq_thermal_protect(): limited_power = 1384
11-24 17:13:38.728170 <7>[25342.425158] (0)[11236:kworker/0:3][name:mt_cpufreq&][Power/cpufreq] @mt_cpufreq_thermal_protect: found = 1, limited_power_idx = 1, limited_max_freq = 1235000, limited_max_ncpu = 4
11-24 17:13:38.728319 <7>[25342.425307] (0)[11236:kworker/0:3][name:mt_cpufreq&][Power/cpufreq] @_mt_cpufreq_set_locked(): Vproc = 1112mv, freq = 1235000 KHz
11-24 17:13:38.728345 <7>[25342.425333] (0)[11236:kworker/0:3][name:mtk_cooler_atm&][Power/CPU_Thermal]set_adaptive_gpu_power_limit 600
11-24 17:13:38.770063 <7>[25342.467051] (0)[11236:kworker/0:3][name:mtk_ts_bts&][Power/BTS_Thermal] T_AP=51000
11-24 17:13:43.660714 <7>[25347.357702] (0)[507:AALServiceMain][name:ddp_pwm&][PWM] (latest= 5): 145( 823, 575) 144( 823, 592) 143( 823, 609) 141( 823, 643) 140( 823, 660)
11-24 17:13:43.728026 <4>[25347.425014] (0)[11158:kworker/0:2][Power/cpufreq] mt_cpufreq_thermal_protect(): limited_power = 1420
11-24 17:13:43.728049 <7>[25347.425037] (0)[11158:kworker/0:2][name:mt_cpufreq&][Power/cpufreq] @mt_cpufreq_thermal_protect: found = 1, limited_power_idx = 1, limited_max_freq = 1235000, 


温度升高导致系统启动温度预警，限制CPU频率，核的数量，最终导致CPU负载过大，系统卡顿

set_adaptive_cpu_power_limit（）

adaptive_limit：
1621 T=63200,
1734 T=62600,
1884 T=61500,
2010 T=61900,
0    T=58700

mt_cpufreq_thermal_protect()
limited_power = 1384

mt_cpufreq_thermal_protect（）
found = 1,
limited_power_idx = 1,
limited_max_freq = 1235000,

_mt_cpufreq_set_locked()
Vproc = 1112mv, 
freq = 1235000 KHz

set_adaptive_gpu_power_limit（）
600


T_AP=51000

[PWM] (latest= 5): 145( 823, 575) 144( 823, 592) 143( 823, 609) 141( 823, 643) 140( 823, 660)

mt_cpufreq_thermal_protect()
limited_power = 1420

mt_cpufreq_thermal_protect（）
found = 1, 
limited_power_idx = 1,
limited_max_freq = 1235000, 



#if THERMAL_HEADROOM
static int p_Tpcb_correlation;
static int Tpcb_trip_point;
static int thp_max_cpu_power;
static int thp_p_tj_correlation;
static int thp_threshold_tj;
#endif


static struct mt_cpu_dvfs cpu_dvfs[] = {
	[MT_CPU_DVFS_LITTLE] = {
		.name = __stringify(MT_CPU_DVFS_LITTLE),
		.cpu_id = MT_CPU_DVFS_LITTLE,	/* TODO: FIXME */
		.cpu_level = CPU_LEVEL_1,	/* FY segment */
		.ops = &dvfs_ops,
#ifdef CONFIG_CPU_DVFS_TURBO_MODE
		.turbo_mode = 0,
#endif
#ifdef CONFIG_CPU_DVFS_POWER_THROTTLING
		.idx_opp_tbl_for_pwr_thro = -1,
		.idx_pwr_thro_max_opp = 0,
#endif
	},
};


#elif defined(CONFIG_ARCH_MT6735M)
/* CPU LEVEL 0, 1.2GHz segment */
static struct mt_cpu_freq_info opp_tbl_e1_0[] = {
	OP(CPU_DVFS_FREQ0, 125000),
	OP(CPU_DVFS_FREQ1, 121875),
	OP(CPU_DVFS_FREQ4, 118750),
	OP(CPU_DVFS_FREQ6, 115000),
	OP(CPU_DVFS_FREQ7, 110000),
	OP(CPU_DVFS_FREQ8, 105000),
	OP(CPU_DVFS_FREQ9, 100000),
	OP(CPU_DVFS_FREQ10, 95000),
};

/* CPU LEVEL 1, 1GHz segment */
static struct mt_cpu_freq_info opp_tbl_e1_1[] = {
	OP(CPU_DVFS_FREQ2, 125000),
	OP(CPU_DVFS_FREQ3, 121875),
	OP(CPU_DVFS_FREQ5, 118750),
	OP(CPU_DVFS_FREQ6, 115000),
	OP(CPU_DVFS_FREQ7, 110000),
	OP(CPU_DVFS_FREQ8, 105000),
	OP(CPU_DVFS_FREQ9, 100000),
	OP(CPU_DVFS_FREQ10, 95000),
};

/* CPU LEVEL 2, 1.25GHz segment */
static struct mt_cpu_freq_info opp_tbl_e1_2[] = {
	OP(CPU_DVFS_FREQ0_1,125000),
	OP(CPU_DVFS_FREQ1,  121875),
	OP(CPU_DVFS_FREQ5,  118750),
	OP(CPU_DVFS_FREQ6,  115000),
	OP(CPU_DVFS_FREQ7,  110000),
	OP(CPU_DVFS_FREQ8,  105000),
	OP(CPU_DVFS_FREQ9,  100000),
	OP(CPU_DVFS_FREQ10, 95000),
};

/* CPU LEVEL 3, 1.1GHz segment */
static struct mt_cpu_freq_info opp_tbl_e1_3[] = {
	OP(CPU_DVFS_FREQ1_1,125000),
	OP(CPU_DVFS_FREQ2,  121875),
	OP(CPU_DVFS_FREQ5,  118750),
	OP(CPU_DVFS_FREQ6,  115000),
	OP(CPU_DVFS_FREQ7,  110000),
	OP(CPU_DVFS_FREQ8,  105000),
	OP(CPU_DVFS_FREQ9,  100000),
	OP(CPU_DVFS_FREQ10, 95000),
};

static struct opp_tbl_info opp_tbls[] = {
	[CPU_LEVEL_0] = {opp_tbl_e1_0, ARRAY_SIZE(opp_tbl_e1_0)},
	[CPU_LEVEL_1] = {opp_tbl_e1_1, ARRAY_SIZE(opp_tbl_e1_1)},
	[CPU_LEVEL_2] = {opp_tbl_e1_2, ARRAY_SIZE(opp_tbl_e1_2)},
	[CPU_LEVEL_3] = {opp_tbl_e1_3, ARRAY_SIZE(opp_tbl_e1_3)},
};




mtktscpu  (IC junction temperature)

ATM (adaptive thermal management)

overshooting 过辐射

opp ：operating performance point 操作性能点

 

收敛

/proc/driver/thermal/clatm_setting
0  2010 15 30 1 720 2010 600 1050 

0  	id
2010    firststep
15	theta(ja)fall
30	theta(js)rise	 （fall/rise 比值越小变化越快）	
1	minbudgetchange （每次最少要变化的大小）
720	minCPUpower(mW)
2010	maxCPUpower(mW)
600	minGPUpower(mW)
1050	maxGPUpower(mW)


/proc/driver/thermal/clctm
1 85000 66000 38999 41999 75000 60000 331980 6333 269995 5000   

1 	ctm on
85000	Target Tj0
66000	Target Tj2
38999	Tpcb1
41999	Tpcb2
75000	exit Tj0
60000	exit Tj2
331980	enter_a
6333	enter_b
269995	exit_a
5000	exit_b


thermal温度高想降频，但降频会导致系统卡顿，无响应，温升和降频是一对互斥的

温度容易升高，同时系统容易卡顿......

整理一下逻辑：温度升高，系统启动mt_cpufreq_thermal_protect，降压，降频，限制CPU核，导致系统无响应，降频过快还是跨度过大，还是CPU核减的太少


Cooler Name    ctm    ENABLE
Extra    ctm on    1
Extra    Target Tj 0    85000
Extra    Target Tj 2    66000
Extra    Tpcb 1    38999
Extra    Tpcb 2    41999
Extra    Exit Tj 0    75000
Extra    Exit Tj 2    60000
Extra    Enter_a    331980
Extra    Enter_b    6333
Extra    Exit_a    269995
Extra    Exit_b    5000

算法：
Tj = min(85, max(66, (Enter_a – Enter_b*bts_cur_temp/1000)/1000))

Exit tj = min(75, max(60, (Exit_a – Exit_b*bts_cur_temp/1000)/1000))



//通过这个函数获取CPU动态调节的策略
p = id_to_cpu_dvfs(_get_cpu_dvfs_id(policy->cpu));

[MT_CPU_DVFS_LITTLE/0]
[0] = { .cpufreq_khz = 1300000,	.cpufreq_ncpu = 4,	.cpufreq_power = 1379, },
[1] = { .cpufreq_khz = 1235000,	.cpufreq_ncpu = 4,	.cpufreq_power = 1271, },
[2] = { .cpufreq_khz = 1170000,	.cpufreq_ncpu = 4,	.cpufreq_power = 1157, },
[3] = { .cpufreq_khz = 1300000,	.cpufreq_ncpu = 3,	.cpufreq_power = 1034, },
[4] = { .cpufreq_khz = 1235000,	.cpufreq_ncpu = 3,	.cpufreq_power = 953, },
[5] = { .cpufreq_khz = 1040000,	.cpufreq_ncpu = 4,	.cpufreq_power = 935, },
[6] = { .cpufreq_khz = 1170000,	.cpufreq_ncpu = 3,	.cpufreq_power = 867, },
[7] = { .cpufreq_khz = 1040000,	.cpufreq_ncpu = 3,	.cpufreq_power = 701, },
[8] = { .cpufreq_khz = 1300000,	.cpufreq_ncpu = 2,	.cpufreq_power = 689, },
[9] = { .cpufreq_khz = 819000,	.cpufreq_ncpu = 4,	.cpufreq_power = 685, },
[10] = { .cpufreq_khz = 1235000,	.cpufreq_ncpu = 2,	.cpufreq_power = 635, },
[11] = { .cpufreq_khz = 1170000,	.cpufreq_ncpu = 2,	.cpufreq_power = 578, },
[12] = { .cpufreq_khz = 819000,	.cpufreq_ncpu = 3,	.cpufreq_power = 513, },
[13] = { .cpufreq_khz = 598000,	.cpufreq_ncpu = 4,	.cpufreq_power = 468, },
[14] = { .cpufreq_khz = 1040000,	.cpufreq_ncpu = 2,	.cpufreq_power = 467, },
[15] = { .cpufreq_khz = 598000,	.cpufreq_ncpu = 3,	.cpufreq_power = 351, },
[16] = { .cpufreq_khz = 1300000,	.cpufreq_ncpu = 1,	.cpufreq_power = 344, },
[17] = { .cpufreq_khz = 819000,	.cpufreq_ncpu = 2,	.cpufreq_power = 342, },
[18] = { .cpufreq_khz = 442000,	.cpufreq_ncpu = 4,	.cpufreq_power = 324, },
[19] = { .cpufreq_khz = 1235000,	.cpufreq_ncpu = 1,	.cpufreq_power = 317, },
[20] = { .cpufreq_khz = 1170000,	.cpufreq_ncpu = 1,	.cpufreq_power = 289, },
[21] = { .cpufreq_khz = 442000,	.cpufreq_ncpu = 3,	.cpufreq_power = 243, },
[22] = { .cpufreq_khz = 598000,	.cpufreq_ncpu = 2,	.cpufreq_power = 234, },
[23] = { .cpufreq_khz = 1040000,	.cpufreq_ncpu = 1,	.cpufreq_power = 233, },
[24] = { .cpufreq_khz = 299000,	.cpufreq_ncpu = 4,	.cpufreq_power = 209, },
[25] = { .cpufreq_khz = 819000,	.cpufreq_ncpu = 1,	.cpufreq_power = 171, },
[26] = { .cpufreq_khz = 442000,	.cpufreq_ncpu = 2,	.cpufreq_power = 162, },
[27] = { .cpufreq_khz = 299000,	.cpufreq_ncpu = 3,	.cpufreq_power = 156, },
[28] = { .cpufreq_khz = 598000,	.cpufreq_ncpu = 1,	.cpufreq_power = 117, },
[29] = { .cpufreq_khz = 299000,	.cpufreq_ncpu = 2,	.cpufreq_power = 104, },
[30] = { .cpufreq_khz = 442000,	.cpufreq_ncpu = 1,	.cpufreq_power = 81, },
[31] = { .cpufreq_khz = 299000,	.cpufreq_ncpu = 1,	.cpufreq_power = 52, },


mt_gpufreqs_power[0].gpufreq_khz = 448500
mt_gpufreqs_power[0].gpufreq_volt = 115000
mt_gpufreqs_power[0].gpufreq_power = 753

mt_gpufreqs_power[1].gpufreq_khz = 279500
mt_gpufreqs_power[1].gpufreq_volt = 115000
mt_gpufreqs_power[1].gpufreq_power = 487


mt_gpufreqs_power[2].gpufreq_khz = 279500
mt_gpufreqs_power[2].gpufreq_volt = 105000
mt_gpufreqs_power[2].gpufreq_power = 396










}*/









 


其他问题
/*
{
   

   
待机电流

签名版










}*/

	











another其他问题
/*
{


GNSPR#52112 MMI充电测试失败
	1.现象：
	a.充电测试失败
	无论是交流充电还是usb，充电电压是在增加，但是电流是负，且电量越来越少。
	b.充电芯片是正常工作的，起码充进去了电，但是充的比耗的多
	

	2.可能的原因：
	a.软件：pmic未工作，或者未挂上i2c总线
	b.硬件：电源管理芯片出现问题，或内部硬件短路，造成较大的功耗。
	c.我理解的可能原因，电池对外放电（电流为负）

	原因：充电芯片没有焊好


GNSPR#55817 MMI充电测试失败

	1.现象：
		机器插上充电器或者USB充电电流为负


	2.可能的原因：
		a.跟上次类似充电芯片未正常工作
		b.根据log,fg_coulomb的数值，i2c传输数据有问题 
			c.充电芯片未正常工作，或者充电接口出现问题（未充电时电流等其他都是正常的）

	原因：又是充电芯片没焊好导致的充电芯片没有正常工作





待解决的问题
   /*a.明确打印出来的那几个物理量的意思，充放电过程什么变化情况。*/
log:
	bat_routine_thr][kernel]AvgVbat 3806,bat_vol 3749, AvgI 0, I 0, VChr 0, AvgT 29, T 30, ZCV 3864, CHR_Type 0, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3804,bat_vol 3725, AvgI 505, I -775, VChr 5137, AvgT 29, T 29, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3800,bat_vol 3694, AvgI 465, I -655, VChr 5137, AvgT 29, T 30, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3796,bat_vol 3718, AvgI 427, I -586, VChr 5137, AvgT 29, T 29, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3793,bat_vol 3707, AvgI 385, I -706, VChr 5128, AvgT 29, T 29, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3788,bat_vol 3675, AvgI 344, I -706, VChr 5128, AvgT 29, T 30, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3784,bat_vol 3691, AvgI 306, I -586, VChr 5128, AvgT 29, T 31, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3781,bat_vol 3726, AvgI 264, I -706, VChr 5137, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3778,bat_vol 3717, AvgI 225, I -620, VChr 5516, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3775,bat_vol 3716, AvgI 184, I -689, VChr 5128, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3772,bat_vol 3719, AvgI 142, I -689, VChr 5128, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3768,bat_vol 3702, AvgI 102, I -655, VChr 5128, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54

	bat_routine_thr][kernel]AvgVbat 3765,bat_vol 3730, AvgI 65, I -568, VChr 5610, AvgT 29, T 32, ZCV 3864, CHR_Type 1, SOC  54: 54: 54


battery_log(BAT_LOG_CRTI,
	    "[kernel]AvgVbat %d,bat_vol %d, AvgI %d, I %d, VChr %d, AvgT %d, T %d, ZCV %d, CHR_Type %d, SOC %3d:%3d:%3d\n",
	    BMT_status.bat_vol, bat_vol, BMT_status.ICharging, ICharging,
	    BMT_status.charger_vol, BMT_status.temperature, temperature, BMT_status.ZCV,
	    BMT_status.charger_type, BMT_status.SOC, BMT_status.UI_SOC, BMT_status.UI_SOC2);


AvgVbat ：电池的平均电压 
	ret = battery_meter_ctrl(BATTERY_METER_CMD_GET_ADC_V_BAT_SENSE, &val)--->BMT_status.bat_vol =mt_battery_average_method(BATTERY_AVG_VOLT, 
		&batteryVoltageBuffer[0], ZCV, &bat_sum, batteryIndex);

bat_vol：电池的电压 ret = battery_meter_ctrl(BATTERY_METER_CMD_GET_ADC_V_BAT_SENSE, &val)

AvgI:平均充电电流（跟上面相似）

I   ：充电电流，以前是通过检测充电引脚正负极参数，做适当的补偿，算出充电的电流，现在是通过充电芯片，IC算出电流，然后读取寄存器的值

电压，电流的变化跟之前的充电过程一样，涓流->恒流->恒压->充满--->检测是否小于回充电压，是否打电话等条件，在判断是否要充电，充电还是放电主要看充电器电压，然后电池电压，
电流的变化，温度是否正常

VChr	：充电器的电压

AvgT	：电池的平均温度

T	：电池的温度

ZCV	：电池当前的开路电压，这个开路电压跟当前的温度有关，导入的温度曲线，通过线性插值法找到具体的温度对应的电压

CHR_Type：充电接口的类型

SOC	:底层的电量

UI_SOC	:上层的电量

   force_get_tbat()---》BattVoltToTemp（）上面的那几个运算，什么do_div。。。64位的除法    	


   /*b.pmic工作的过程，如何确定i2c是否正常工作*/
   安装了i2c_tools，还有什么dump register?上面有一个打印pmic寄存器的log，正常打印，i2c设备应该正常。



   /*c.充电升降压调节的控制具体怎么控制的，双充电芯片？*/
定义的输入输出电流开关的宏
CONFIG_MTK_SWITCH_INPUT_OUTPUT_CURRENT_SUPPORT

定义的开关标志位g_bcct_flag    g_bcct_input_flag，好像跟温度有关的

快充部分有相应的升压调节，快充部分的升压通过充电适配器升高电压，而快充的通信好像是循环升压，看是否变化，变化了，证明连接上了，多了次数的判断，满足条件，可以快充。
mtk_ta_retry_increase()->mtk_ta_increase()->control的那个宏，set_ta_current将电压逐渐升到12

而普通的升压跟充电模式相关，应该在各种模式中，调度算法里 select_charging_current()设置充电电流，涓流->恒流->恒压->充满--->检测是否小于回充电压，
是否打电话等条件，在判断是否要充电，充电还是放电主要看充电器电压，然后电池电压，电流的变化，温度是否正常

基于输入电压的动态电源管理 (VIN-DPM)：电源和充电器之间的电阻会阻碍充电器从电源获得最大功率，导致电源电压陡降，造成充电器欠压闭锁。
已经有了用来计算充电器所需最小电源电压的方程式，其可计算既定电源适配器的最大充电电流。此外， VIN-DPM 特性还能动态地降低充电器的输入电流限值，避免适配器电压陡降，
因而允许使用多种类型的适配器和/或电源连接

双充电芯片：
两个充电芯片同时为电池工作，可以降低充电的温度（热量分散到两个芯片上）
定义的这个宏：CONFIG_MTK_DUAL_INPUT_CHARGER_SUPPORT



   /*d.反向充电，升压，USB，otg*/

当设备检测到USB_ID信号为低时，表该设备应作为Host（主机，也称A设备）用。
当设备检测到USB_ID信号为高时，表示该设备作为Slave(外设，也称B设备）用。

switch_int_to_device   switch_int_to_host  ，根据变量的值判断进入device还是host，而这个值是是读取寄存器的值，iddig
pinctrl_select_state控制gpio引脚的状态，
反向充电应该跟USB_OTG有关，当检测到USB 的id引脚为0，otg模式，触发中断。当然逻辑好像改了，如果是OTG充电模式，上层要点击确定，使OTG中断使能，才可以反向充电。

is_enter_mmi_test 反向充电测试开关


开机充电，关机充电 ，低电量保护？

正常开机充电，一直再看。

关机充电：

       关机时充电器检测代码在lk部分，当接入充电器时，PMIC会通过ADC采样，检测Vchrg（charger/usb电源）电压，
判断Vchrg的值如果在这个区间：4.4v~6.5v，系统就会开始进入充电过程，这个过程包括show low battery logo，接着
显示充电电量logo。
相关代码：
mt6572/mediatek/platform/mt6572/lk/platform.c

查看函数void platform_init(void)：

充电器检测：
充电过程中，upmu_is_chr_det() 检测Vchrg状态，返回KAL_FALSE则power off。
#ifdef MTK_KERNEL_POWER_OFF_CHARGING
    if((g_boot_arg->boot_reason == BR_USB) && (upmu_is_chr_det() == KAL_FALSE))
    {
        printf("[%s] Unplugged Charger/Usb between Pre-loader and Uboot in Kernel Charging Mode, Power Off \n", __func__);
        mt6575_power_off();
    }
#endif

显示LCM背光：
#ifndef DISABLE_FOR_BRING_UP
    mt65xx_backlight_on(); //[TODO] workaround
#endif

boot模式选择，用于区分开机过程。
boot_mode_select();

显示low battery logo，点亮充电指示灯：

        if(kernel_charging_boot() == 1) //关机充电状态
        {
                mt_disp_power(TRUE);
                mt_disp_show_low_battery();
                mt_disp_wait_idle();
#ifndef DISABLE_FOR_BRING_UP
                mt65xx_leds_brightness_set(6, 110);
#endif
        }

kernel_charging_boot定义：
#if defined (MTK_KERNEL_POWER_OFF_CHARGING)
int kernel_charging_boot(void)
{
        if((g_boot_mode == KERNEL_POWER_OFF_CHARGING_BOOT || g_boot_mode == LOW_POWER_OFF_CHARGING_BOOT) && upmu_is_chr_det() == KAL_TRUE)
        {
                printf("[%s] Kernel Power Off Charging with Charger/Usb \n", __func__);
                return  1;
        }
        else if((g_boot_mode == KERNEL_POWER_OFF_CHARGING_BOOT || g_boot_mode == LOW_POWER_OFF_CHARGING_BOOT) && upmu_is_chr_det() == KAL_FALSE)
        {
                printf("[%s] Kernel Power Off Charging without Charger/Usb \n", __func__);
                return -1;
        }
        else
                return 0;
}
#endif


   /*e.分析那个电池曲线，出现的什么问题，自己想（虽然不是我解决的）*/
充电时间有点长

	

   /*f.ZCV电池曲线，导入，操作*/
	battery_profile_tX 几个数组

   /*g.充电类型的判断*/
	
   pmic和USB都要ready,PMU检测充电端是否正常工作


SS：高优先级--->温度的调控，处理算法，  h.thermal相关的内容还是不太清楚


待机状态下有些会定期唤醒
	1.modem不太了解，经常唤醒，耗电很大
	2.1800s定期唤醒（30min），防睡死，1%检测中断

充电状态下BAT_thread几个线程10s唤醒一次





}*/

