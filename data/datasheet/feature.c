/************************************************************************
                        添加新的特性
*************************************************************************/


/*可以设置充电电量的上下限*/
{
    ->笔记本电脑的一种充电方式
{
    如何调整笔记本电池充电最低百分比?
    http://www.3lian.com/edu/2013/05-13/69533.html



    就是设置两个充电阈值，低于多少开始充电，高于多少停止充电

    主要是针对那些频繁插拔充电器的，减少充电次数，延长电池寿命       

-->
   1、充电阈值设置，仅在自带或后期安装的电池管理软件运行条件下有效;

　　2、充电终止百分比至少要比充电起始百分比高4%;

　　3、由于电池老化，一般将充电阈值的设置比理想值高2%;

　　4、当充电停止百分比小于100%时，请每三个月对电池进行一次100%充电(平时应尽量避免对电池进行完全充电和完全放电)。

}



->所以这里的想法是如何实现设定充电的阈值
{
    在标准充电器插入的时候根据设定的阈值充电，USB充电的时候不执行这个策略

    1.数据结构里面添加设定的上限，下限

    2.初始化的时候设定一个默认的值

    3.判断充电器类型标准充电器还是非标充，就是说根据检测到的充电器的类型

    4.上层对设备节点的写0,1决定对上下限写入的权限




    扩展想法是如何创建一个内核线程，并运行控制

}


->相关的代码
{

/*****************************************************************************
            电池的数据结构
 ****************************************************************************/
    typedef struct {
        kal_bool bat_exist;
        kal_bool bat_full;
        signed int bat_charging_state;
        unsigned int bat_vol;
        kal_bool bat_in_recharging_state;
        unsigned int Vsense;
        kal_bool charger_exist;
        unsigned int charger_vol;
        signed int charger_protect_status;
        signed int ICharging;
        signed int IBattery;
        signed int temperature;
        signed int temperatureR;
        signed int temperatureV;
        unsigned int total_charging_time;
        unsigned int PRE_charging_time;
        unsigned int CC_charging_time;
        unsigned int TOPOFF_charging_time;
        unsigned int POSTFULL_charging_time;
        unsigned int charger_type;
        signed int SOC;
        signed int UI_SOC;
        signed int UI_SOC2;
        unsigned int nPercent_ZCV;
        unsigned int nPrecent_UI_SOC_check_point;
        unsigned int ZCV;
    } PMU_ChargerStruct;


**************************************************************************************************
--->system node
//Gionee LiLuBao 20170309 modify for switch charging value begin
#define GN_BATTERY_SWITCH_CHARGING
#if defined(GN_BATTERY_SWITCH_CHARGING)
kal_bool gn_switch_charging_State=KAL_FALSE;
static ssize_t show_Switch_Charging_Value(struct device *dev,struct device_attribute *attr, char *buf)
{
	battery_log(BAT_LOG_CRTI, "show gn_switch_charging_State = %d\n", gn_switch_charging_State);    
    return sprintf(buf, "%u\n", gn_switch_charging_State);
}

static ssize_t store_Switch_Charging_Value(struct device *dev,struct device_attribute *attr, const char *buf, size_t size)
{
	int rv;
	
	rv = kstrtouint(buf, 0, &gn_switch_charging_State);
	if (rv != 0)
	{
		battery_log(BAT_LOG_CRTI, "  bad argument, echo [enable] > Switch_Charging_Value! gn_switch_charging_State = %d\n", gn_switch_charging_State);
		return -EINVAL;
	}

	
	battery_log(BAT_LOG_CRTI, "store gn_switch_charging_State = %d\n", gn_switch_charging_State); 

	return size;
}

static DEVICE_ATTR(Switch_Charging_Value, 0664, show_Switch_Charging_Value, store_Switch_Charging_Value);
#endif
//Gionee LiLuBao 20170309 modify for switch charging value end


--->probe
//Gionee LiLuBao 20170309 modify for switch charging begin
#if defined(GN_BATTERY_SWITCH_CHARGING)
ret_device_file = device_create_file(&(dev->dev), &dev_attr_Switch_Charging_Value);
#endif
//Gionee LiLuBao 20170309 modify for switch charging end


--->init.mt6735.c
# Gionee LiLuBao 20170310 modify for switch charging value begin
chown system system /sys/devices/platform/battery/Switch_Charging_Value
# Gionee LiLuBao 20170310 modify for switch charging value end 




    typedef enum {
        KAL_FALSE = 0,
        KAL_TRUE  = 1,
    } kal_bool;


关于充电截止之后状态的改变

}


}

















/*type-C接口上添加OTG开关*/
{
OTG的引脚 ，怎么在设备树里面添加状态，哪个是控制OTG状态的，如何控制开关的逻辑，整个的思维过程
使用的是bq24196

    1.没有通过dtsi文件，那就是通过头文件，然后直接设置状态的

    pin脚对应的几个模式
    #define PI5USB_INT_PIN          GPIO_IO_USB_EINT_PIN
    #define PI5USB_INT_PIN_MODE     GPIO_IO_USB_EINT_PIN_M_EINT
    #define PI5USB_INT_NUM          CUST_EINT_IO_USB_NUM
    #define PI5USB_RESET_PIN        GPIO_IO_SW_EN_USB
    #define PI5USB_RESET_PIN_MODE   GPIO_IO_SW_EN_USB_M_GPIO

    对gpio的操作
    mt_set_gpio_mode(PI5USB_INT_PIN, PI5USB_INT_PIN_MODE);
    mt_set_gpio_dir(PI5USB_INT_PIN, GPIO_DIR_IN);
    mt_set_gpio_pull_enable(PI5USB_INT_PIN, GPIO_PULL_ENABLE);
    mt_set_gpio_pull_select(PI5USB_INT_PIN, GPIO_PULL_UP);

    读取寄存器上的状态
    static int current_mode = 0x4;
    先关闭中断使能，然后读取寄存器
    pi5usb_write(client, current_mode + 0x1);//Disable interrupt
	msleep(30);//Delay 20ms ~ 45ms
	pi5usb_read(client, reg_val, REG_COUNT);

    reg_val[2],0x01 or 0x00
    reg_val[3]=0x05,0x06 OTG insert


    current_mode = def_mode?
    current_mode :0x0,0x02,0x04
    def_mode=0x04




    if(1 == audio_accessory_flag || 1 == debug_accessory_flag)
    {
        msleep(100);
    }else if(repeattimes >= 3)
    {
        repeattimes = 0;
        printk("VBUS in, no CC \n");    ？？
    }
    else
    {
        repeattimes++;
        msleep(240);
        pi5usb_write(client, 0x1);
        msleep(100);
//		pi5usb_write(client, 0x4);
    }




    
    2.OTG的工作流程：
    (pi5usb30216.c)pi5usb_i2c_probe,在I2C总线上注册一个设备申请中断，创建一个内核线程pi5usb_event_handler->pi5usb_event_handler ,
    while(1),等待中断的到来，->pi5usb_process ,中断来了，就读写寄存器，根据寄存器的不同位判断处于什么状态->pi5usb_try_snk，当有设备插入的时候 
    就执行这个，按意思来说应该说是等待设备准备好状态，有延时同时读取不同的寄存器判断,是否有热插拔或者其他外设




    pi5usb type-C的操作流程
    1. INTB asserted LOW, indicating Type-C port status change.
    将电平拉低检测type-C引脚的状态
    2. Processor first masks PI5USB30216A interrupt by writing a ‘1’ to Bit 0 of Control Register(0x02). INTB returned
    Hi-Z.
    通过掩码返回中断的状态
    3. Delay 30ms.
    延时
    4. Processor then read Register(0x01), Control Register (0x02), Interrupt Register(0x03) and CC Status
    Register(0x04). Interrupt Register(0x03) indicates if an attach or detach event was detected. All interrupt flags in
    Interrupt Register will be cleared after the I2C read action. CC Status Register(0x04) is used to determine plugin
    details and charging profile. Processor can configure the power and USB channels according to information in
    CC Status Register.
    0x01:只读寄存器
    0x02:控制寄存器
    0x03:中断
    0x04:CC状态寄存器
    当读了I2C的值之后就会清零
    5. Processor unmask PI5USB30216A interrupt by writing a ‘0’ to Bit 0 of Address 0x02 before ending the interrupt
    service routine
    在执行中断工作函数的之后会操作掩码，然后结束中断工作函数

    控制type-C使能来使能OTG开关，主要思路应该是这个，就是将电平拉高关闭使能，同时I2C也不能控制
    PI5USB30216A is reset only by Power-On Reset (POR). When ENB is float or is pulled high, PI5USB30216 is
    disabled and its I2C is not accessible. Whenever users pull low ENB to enable part, PI5USB30216A should be reset
    through below I2C sequence:

    type-C的两种工作状态
    1. In DRP mode, PI5USB30216A is reset by writing Reg[0x02]=01h, then delay 30ms followed by writing
    Reg[0x02]=04h/0Ch/14h. Please see Figure 6a.
    DRP模式就是从主设备模式，下行口
    2. In Source/DFP mode, PI5USB30216A is reset by writing Reg[0x02]=01h, then delay 30ms followed by writing
    Reg[0x02]=02h/0Ah/12h. Please see Figure 6b.
    DFP双端口模式，主从设备可以切换
    3. In Sink/UFP mode, PI5USB30216A is reset by writing Reg[0x02]=05h, then delay 30ms followed by writing
    Reg[0x02]=00h/20h. Please see Figure 6c
    UFP：从设备模式，上行口




    所以这里有几个关键地方：
    1.是控制iddig引脚中断使能，还是上面讲的写寄存器，让中断跟I2C都失效
    中断是哪个？原始代码应该是USB检测的中断，
    usb20.h 里面定义
    #define IDDIG_EINT_PIN (GPIO_OTG_IDDIG_EINT_PIN & ~(0x80000000))
    
    2.一个是USB3.0，一个时USB2.0代码结构不一样
    USB2.0->usb20_host.c           USB3.0->xhci-mtk-driver.c
    3.设备节点的创建和控制
    4.完备的代码逻辑



    pinctrl_usbc, en_output0，en_output1，client_global
    中断42

    secure boot;签名版的相关操作；服务器版本本地单编替换
    
}