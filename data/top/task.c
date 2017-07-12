





/*要处理的问题*/
{

	G1605B功耗问题,89268,89269,89270,89272,89273
	{
		这几个bug跟modem 的channel 10/34 网络一直变化有关
		还有可能跟开启了gpslog有关
	
	}

	17G10A p1试产待解决问题
	{
		电量显示不正确
		{
			PowerManagerService -> BatteryService -> BatteryProperties.java
			->BatteryProperties.cpp 
		
			framework/base/services/core/java/com/android/server/power/PowerManagerService.java
			
			上报电池相关数据
			private void updateIsPoweredLocked(int dirty) 
			
			获取电池电量
			mBatteryLevel = mBatteryManagerInternal.getBatteryLevel();
			
			
			frameworks/base/services/core/java/com/android/server/BatteryService.java
			
			public int getBatteryLevel() {
            synchronized (mLock) {
                return mBatteryProps.batteryLevel;
            }
      	
      		
      		frameworks/base/core/java/android/os/BatteryProperties.java
      	
			./native/services/batteryservice/BatteryProperties.cpp

      	
		
		}
	
	
		过压测试：NG，10V时电压不不截止，还有380ma~440ma，无电压过高提醒
	
	
		充电电流偏小，关机充电有问题
	
	
		插入充电器后的I2C时序或者说波形有问题，涉及到I2C通信相关的内容
		{
			请问一下，17g10的充电I2C通信速率是多少（clk频率），目前情况是fh->s->fh，之后没
			有通信数据了。是否没有调好呢？
			
			因为有出现三种频率，分别对应I2C的标准模式，快速模式和高速模式的频率
			
			标准模式和高速模式对应的地址也正确，快速模式频率400K是转换到高速模式之前的出现的
			
			最后一笔数据是高速模式的，在后面就只有空闲模式，没有I2C信号数据了
			
			且高速模式下对应的时序测试结果有异常
		
		}
		
		
		
		
		底电流偏高
		
		
		
		快速关机
		
		
		
		电池曲线的导入
		
		
		
		USB眼图：NG
		
		
	
	}
	





	底层电量跟上层显示差别较大，然后触发电量跳变问题	
	已提交eservices长时间未解决
	{
		大概写了一下流程，请过目：
		底层电量跟上层偏差过大导致电量跳变的判断
	
		fg_result：
		0：电量触发跳变的
		2：rtc=1的情况下，把rtc记录的电量赋给电量计；软件电量超过最大电量的情况
		4：rtc记录的电量超过10，把电量计记录的初始化的值赋给电量计
		5：其他情况下也是把电量计记录的初始化的值赋给电量计
	
		if (soc_flow == HW_FG || soc_flow == SW_FG) {
		    if ( (fg_plugout_status==0 || (boot_reason == BR_2SEC_REBOOT)) && (charger_exist != true)){
		        if (g_rtc_fg_soc == 0) {
		            fg_capacity_by_v = fg_capacity_by_v_init;
		            fg_result = 0;
		        } else {
		            if (g_rtc_fg_soc == 1) {
		                fg_capacity_by_v = g_rtc_fg_soc;
		                fg_result = 2;

		            } else if ( fg_sw_soc >= max_swocv ) {
		                fg_capacity_by_v = g_rtc_fg_soc;
		                fg_result = 2;
		            } else if (g_rtc_fg_soc > 10) {
		                fg_capacity_by_v = fg_capacity_by_v_init;
		                set_rtc = 2;
		                fg_result = 4;
		            } else {
		                fg_capacity_by_v = fg_capacity_by_v_init;
		                set_rtc = 1;
		                fg_result = 5;
		            }
		        }
		    } 
		    
		    else {	//rtc电量-硬件电量  超过  hw_rtc的阈值 同时  软件电量-rtc电量 大于 硬件 -  软件电量
		        if (((abs(g_rtc_fg_soc - fg_hw_soc)) > difference_hwocv_rtc)
		            && (abs(fg_sw_soc - g_rtc_fg_soc) > abs(fg_hw_soc - fg_sw_soc))) {
		            /* compare with hw_ocv & sw_ocv, check if less than or equal to 5% tolerance */
		            if (abs(fg_sw_soc - fg_hw_soc) > difference_hwocv_swocv) {
		                fg_capacity_by_v = fg_capacity_by_v_init;
		                fg_result = 0;
		            }
		        } else {
		            if (abs(fg_sw_soc - g_rtc_fg_soc) > (difference_swocv_rtc + batterypseudo1)
		                && ( abs(fg_sw_soc - g_rtc_fg_soc) > abs(fg_sw_soc - fg_vbat_soc) ) ) {
		                fg_capacity_by_v = fg_capacity_by_v_init;
		                fg_result = 0;
		            } else {
		                if (g_rtc_fg_soc == 0) {
		                    fg_capacity_by_v = fg_capacity_by_v_init;
		                    fg_result = 0;
		                } else {
		                    if (g_rtc_fg_soc == 1) {
		                        fg_capacity_by_v = g_rtc_fg_soc;
		                        fg_result = 2;

		                    } else if ( fg_sw_soc >= max_swocv ) {
		                        fg_capacity_by_v = g_rtc_fg_soc;
		                        fg_result = 2;
		                    } else if (g_rtc_fg_soc > 10) {
		                        fg_capacity_by_v = fg_capacity_by_v_init;
		                        set_rtc = 2;
		                        fg_result = 4;
		                    } else {
		                        fg_capacity_by_v = fg_capacity_by_v_init;
		                        set_rtc = 1;
		                        fg_result = 5;
		                    }
		                }
		            }
		        }
		    }
		}

		// modify g_booting_vbat

		if (fg_capacity_by_v == 0 && charger_exist == true) {
		    fg_capacity_by_v = 1;
		    fg_result = 3;
		    FGLOG_NOTICE("[FGADC] fg_capacity_by_v=%d\n", fg_capacity_by_v);
		}

		if (set_rtc == 1) {
		    fg_capacity = g_rtc_fg_soc;
		} else if (set_rtc == 2){
		    fg_capacity = g_rtc_fg_soc - 1;
		} else {
		    fg_capacity = fg_capacity_by_v;
		}
		fg_dod0 = 100 - fg_capacity_by_v;
		fg_capacity_by_c_init = fg_capacity;
		fg_capacity_by_c = fg_capacity;
		fg_dod0_init = fg_dod0;
		fg_dod1 = fg_dod0;
		set_fg_soc(fg_capacity_by_v);
		ui_soc=fg_capacity_by_c_init; 
	
	}	












	高通项目的bring up
	{
		1.相关的工作流程，代码框架
		
		2.项目脚本，涉及到哪些宏
		
		3.gpio，device tree,rpm ,ldo的配置
		
		4.充电的工作流程，充电器的检测，数据的上报
		
		5.电量计相关的计算
		
		6.ti的替换方案
		
	
	}

	
	1.msm8917
	{
		
		/*电量计*/
		{
			电量计的算法依赖于库仑计，以电压为基础的技术
			确保短期的线性变化(也就是说电量短时间变化不能太突然)
			同时长时间的精确性
	
			不需要一个完整的充放电循环来校准电量计的准确度
	
			开路电压计算初始值，库仑计的积分算法计算电量的变化	
	
			Disable USB on-the-go (OTG) functionality when battery SoC falls below a 		 programmable  threshold
	
			电量计的电流通过读取10m欧的采样电阻，读取充进电池的电量多少，电压通过读取adc转换的
			引脚的值，1500ms读取一次
		}


		/*相关概念*/
		{
		
			BAT_ID:
			通过BAT_ID检测电池的存在与否，这个检测过程内嵌在fg算法中，重复的升高基础电流（5uA->15uA->150uA）直到发现匹配的
			智能电池调控通常是提供5uA的灌电流去识别，因为5uA可以通过软件去识别然后设置使能标志位
			(Battery Serial Interface (BSI) module)
				电量计读电流通过采样电阻，读电池电压通过连接电池正负极的BATT_P，BATT_N
		放电读正极，其他时候读负极


			thermal：		
				电池温度检测，有些是将温度检测的电阻嵌入到电池内(NTC电阻)，然后通过一个thermal pin去连接，读取数值	
				thermal 相关参数的设置都是一些十六进制，总要说说是什么意思吧？pm_config_target.c
				这个文件在哪，这个文件在modem侧
				modem_qc_n_qrd8920_cs/BOOT.BF.3.3/boot_images/core/systemdrivers/pmic/config/mdm9x45/pm_config_target.c
		
			配置电量计中thermistor相关的参数(80-VT310-123).
			qcom,thermal-coefficients这是一个位数组，通过这组数据，可以读NTC电阻的一些转换信息
			{
				这个有两个地方可以setting
				一个是在kernel device tree里面
				一个是在SBL里面，但是这里要晚1.5s因为要启动SRAM，在pm_config_target.c
				里面设置
		
			}


			ESR(电池等效电阻):
				ESR可以提高电量计的准确性，对其他的也有影响，但是ESR在低电流的状态下是不需要的，而且在系统suspend的时候是无法使能的
				电池等效串联电阻ESR，这个值最好是实时的，则样才最准确等效电阻跟temperature有很大关系，同时也影响到电池的剩余电量
			power_supply 子系统是这个POWER_SUPPLY_PROP_RESISTANCE而这个数据的采集是在电量计每隔90s发一个脉冲，然后同步同一时间的
			电压和电流而这个脉冲会引起设备底电流的升高，所以当设备suspend的时候会关闭这个使能



			System termination current(系统的截止电流)：
				可以客制化电池100%状态的上报，
				qcom,fg-chg-iterm-ma这个是设置充电截止电流
				这里应该讲的是系统满电的状态，充电电流，电池电压
				显示100%，系统截止电流，充电截止电流


			FG termination current（这个使用电量计充电截止电流来判断停止充电的条件）：
			也就是说充电截止电流可以通过qcom,fg-chg-iterm-ma，或者qcom,chg-term-ua(不用)来设定
			充电截止的条件
				qcom,fg-iterm-ma = <150>; //这个是显示100%的截止电流？

				电量计的截止电流
				qcom,fg-chg-iterm-ma = <100>;	//这个是充电截止电流？


			System cutoff voltage：系统0%电量对应的电压
			qcom, fg-cutoff-voltage-mv = <3000>; /*example 3V */


			Estimate voltage at bootup(启动阶段估计电压)：
				Reload the profile for a better SoC estimate if the vbat_current_predicted differs from the actual vbat by more than a set threshold.
			如果启动阶段检测到的电压跟实际的电压偏差超过多少就重新载入数据


			CC_to_CV threshold set point
				恒流到恒压模式转变的阈值：
				qcom, fg-cc-cv-threshold-mv = <4340>;
				当然这里还有一个浮动电压，比上面的阈值高10V
				Specify the CC_CV set point for PMI895x FG to 4340 mV (currently default), which is 10 mV lower than the float voltage configured (currently default is 4350 mV). This is needed for EoC to be notified properly.


			Resume charging based on SoC
				qcom,resume-soc = <95>;
			当电量低于多少的时候，触发回充，recharging


			External/internal current sense
				这个应该是并行充电要添加的
				qcom,ext-sense-type


			IRQ_volt_empty：
				低电中断至少要比system cutoff voltage高50mV
				Recommended setting/default = System Cutoff voltage – 50 mV (may need more headroom than 50 mV value, depending on the system)
				因为上层读到0%的时候会立刻执行shutdown流程，避免电压低于UVLO
				irq-volt-empty-mv


			Battery age detection：
				1.通过等效电阻来估计电池的容量
				2.软件上通过库仑计来学习电池的容量


				The Battery Learning Capacity algorithm takes into account the following:
				1.Temperature
				2.Qualified starting point of the SoC of the battery
				3.Allowable increment and decrement of each charge cycle to qualify for each learning cycle
				这里应该是每次充满电循环所需要的时间，一次来估计电池的容量，和电池老化


				Charge cycle count
				当电量低于low-soc的时候触发算法，电量高于high-soc的时候counter增加，只有走一个完整的过程counter才会增加，如果移除电池的话数据会被清除，因为数据存储在FG SRAM

		}
		
		
		
		调用流程
		{
	        (qpnp-smbcharger.c) 这个是高通充电用的主要的源代码，smbchg_init初始化模块 -> spmi_driver_register注册设备驱动 -> smbchg_probe将设备驱动跟设备挂钩，
	        
			建立bind -> 从dtsi文件中获取一些参数 ->  create_votable创建了好几个投票变量 (fcc_votable,usb_icl_votable,dc_icl_votable,usb_suspend_votable,

			dc_suspend_votable,battchg_suspend_votable,hw_aicl_rerun_disable_votable,hw_aicl_rerun_enable_indirect_votable,aicl_deglitch_short_votable,hvdcp_enable_votable)
		
		
		
			高通的充电基本上都叫smbcharger
			(qpnp-smbcharger.c) smbchg_init 模块初始化->smbchg_probe	从dtsi文件中获取参数，上面还有一个投票制度貌似很重要，还有一些工作 -> smbchg_usb_update_online_work,
			
			判断usb是否插入,并上报状态到power_supply子系统 -> smbchg_parallel_usb_en_work并行充电使能，对并行充电的条件进行判断 aicl是否稳定，没有电池的条件下不允许改变
			
			并行充电的状态 -> smbchg_parallel_usb_enable -> smbchg_vfloat_adjust_work动态充电电压调节
		
		}
		
		
		
		/*debug*/
		{
		
			qpnp-smbcharger.c
		    {   
		        
		        smbcharger里面涉及到的投票变量都是干什么的
		        {
		            1.fcc_votable:这个是设置快充充电电流的，根据投票结果决定并行充电的充电电流是多少
		               并行充电好像跟taper_irq_en这个中断有关，但是这个中断是干什么的？

		               static int set_fastchg_current_vote_cb(struct device *dev,
							int fcc_ma,
							int client,
							int last_fcc_ma,
							int last_client)
		                {
		                    struct smbchg_chip *chip = dev_get_drvdata(dev);
		                    int rc;

		                    if (chip->parallel.current_max_ma == 0) {
		                        /*设置快速充电的充电电流 fcc_ma*/
		                        rc = smbchg_set_fastchg_current_raw(chip, fcc_ma);
		                        if (rc < 0) {
		                            pr_err("Can't set FCC fcc_ma=%d rc=%d\n", fcc_ma, rc);
		                            return rc;
		                        }
		                    }
		                    /*
		                    * check if parallel charging can be enabled, and if enabled,
		                    * distribute the fcc
		                    */
		                    /*高通的快充利用的是并行充电方案，主从charger*/
		                    smbchg_parallel_usb_check_ok(chip);
		                    return 0;
		                }

		            
		            2.usb_icl_votable：
		                设置usb充电电流的限制，可能要根据系统温升的情况决定
		                如果是快充充电器的会停止并行充电
		                static int set_usb_current_limit_vote_cb(struct device *dev,
							int icl_ma,
							int client,
							int last_icl_ma,
							int last_client)    
		                {
		                    struct smbchg_chip *chip = dev_get_drvdata(dev);
		                    int rc, aicl_ma, effective_id;

		                    effective_id = get_effective_client_id_locked(chip->usb_icl_votable);

		                    /* disable parallel charging if HVDCP is voting for 300mA */
		                    if (effective_id == HVDCP_ICL_VOTER)
		                        smbchg_parallel_usb_disable(chip);

		                    if (chip->parallel.current_max_ma == 0) {
		                        rc = smbchg_set_usb_current_max(chip, icl_ma);
		                        if (rc) {
		                            pr_err("Failed to set usb current max: %d\n", rc);
		                            return rc;
		                        }
		                    }

		                    /* skip the aicl rerun if hvdcp icl voter is active */
		                    if (effective_id == HVDCP_ICL_VOTER)
		                        return 0;

		                    aicl_ma = smbchg_get_aicl_level_ma(chip);
		                    if (icl_ma > aicl_ma)
		                        smbchg_rerun_aicl(chip);
		                    smbchg_parallel_usb_check_ok(chip);
		                    return 0;
		                }


		            3.dc_icl_votable
		            /*
		            * set the dc charge path's maximum allowed current draw
		            * that may be limited by the system's thermal level
		            */
		            设置直流充电器最大的充电电流，遍历数组寻找最匹配的值，写进寄存器
		            static int set_dc_current_limit_vote_cb(struct device *dev,
		                                    int icl_ma,
		                                    int client,
		                                    int last_icl_ma,
		                                    int last_client)
		            {
		                struct smbchg_chip *chip = dev_get_drvdata(dev);

		                return smbchg_set_dc_current_max(chip, icl_ma);
		            }                    

		            下面这几个大部分都是操作寄存器
		            4.usb_suspend_votable       将usb挂起
		            5.dc_suspend_votable        dc充电挂起
		            6.battchg_suspend_votable   关闭充电使能

		            7.hw_aicl_rerun_disable_votable     关闭rerun高电压aicl策略
		            8.smbchg_aicl_deglitch_config_cb    配置aicl去抖
		            9.hvdcp_enable_votable              允许高电压充电
		        }

		        smbcharger里面的几个工作函数
		        {
		            1.smbchg_usb_update_online_work 
		              检测usb的状态然后上报给power_supply子系统
		            2.smbchg_parallel_usb_en_work
		              设置并行充电使能
		            3.smbchg_vfloat_adjust_work
		              根据充电能路，目标充电电压，不断步进升压
		            4.smbchg_hvdcp_det_work
		              高压充电器j      
		        }
		    }
		

			qpnp-fg.c
            {
            	为什么高通的单个结构体都有那么多成员，还有那么多工作函数？
            	
            	
				fg_probe将电量计device跟driver挂钩,建立bind
				
				log_bat_status这个打印很多电池相关的信息
				都要从get_sram_prop_now这个接口里面去根据下标去读取数据
				fg_mem_data_index
				
				
				常用函数接口：
				充电使能
						rc = set_prop_enable_charging(chip, true);
				
				
				开始的时候初始化很多wakeup source，创建了很多work
				{
					wakeup source:需要了解机制
					
					work：
						update_jeita_setting 这是跟一个温度设置相关的
						
						update_sram_data_work 这个跟fg相关参数存储在sram中相关
							然后有的数据上报都是从sram从读取数组数据，但是这些数据是在哪写的
							利用哪些接口获取的
							
						update_temp_data 上报电池温度相关信息
						
						check_empty_work 这个是上报没电 0%？如果有vbatt-low interrupt这个  中						断触发就会调用这个函数
						
						batt_profile_init 电池相关的配置文件初始化
							
						check_sanity_work
						
				}
            }
		}
		
		
		
		
		/*调试方式*/
		{
			FG MEM_INTF access	
				adb shell "echo 0xXXX > /sys/kernel/debug/fg_memif/address"
				adb shell "echo 0xXX > /sys/kernel/debug/fg_memif/count"
				adb shell "cat /sys/kernel/debug/fg_memif/data"

			FG SRAM dump
				1.需要dumper.sh
	
				2.adb 命令
					adb root
					adb wait-for-devices
					adb push dumper.sh  /data/
					adb shell chmod 777 /data/dumper.sh
	
				3.运行脚本，将数据写到文件内
				好像不管用
				/data/dumper.sh > /data/kmsg.txt
	
				4.adb root
				  adb wait-for-devices
				  adb pull /data/kmsg.txt
				  
			FG debug logging
				调整qpnp-fg.c的log等级
		
				echo 0xff > /sys/module/qpnp_fg/parameters/debug_mask
				echo 8 > /proc/sys/kernel/printk
				dmesg > debug_output_filename
		}
		
		
		
		
		/*充电涉及到的代码和参考*/
		{
			./msm-3.18/drivers/power/qpnp-fg.c
			./msm-3.18/arch/arm/boot/dts/qcom/msm-pmi895x.dtsi
			./msm-3.18/Documentation/devicetree/bindings/power/qpnp-fg.txt
			
			
			 dtsi配置文件
		        ./arch/arm/boot/dts/qcom/msm-pmi8940.dtsi

	        kernel代码
	        ./msm-3.18/drivers/power/qpnp-smbcharger.c
	        ./msm-3.18/drivers/power/pmic-voter.c  
		}
		
	}






	ti的替换方案
    {

        1.bq27428
        {
        	The bq27426 battery fuel gauge uses the patented Impedance TrackTM algorithm for fuel gauging
        	bq27428利用的是阻抗追踪法来进行电量计算，提供相关的电池信息


			相关参数的精确度(温度范围是-40~100)： 
			电压 -0.1% ~ -0.2%
			温度 -13%  ~ 13%    这个偏差很大
			电流 0 ~ 0.6%
        
        	电量计通过读取流过采样电阻的电压来估计充电和放电时候的电池信息
        	
        	当电池接上了fuelgauge的时候。电池的阻抗是根据流进电池的电流，电池的开路电压
        	以及有负载情况下的电池电压
        	
        	P10 I2C通信要注意的地方
        	
        	fuelgauge提供四种模式：initilization,normal,sleep,shutdown
        
        	这里面应该要注意的一个是I2C通信，还有电量计的一些内容
        	
        }
 
    }    




















 	1.定时器相关的内容
 	{
 		有两个kthread function:hrtimer,fgtimer这是两个定时器相关的操作
		可以适当分析时钟相关的框架
		charger_kthread_hrtimer_func
		charger_kthread_fgtimer_func
 	}






	2.battery ID相关的内容，根据ID选择电池曲线
	{
		battery_id  ，研读相关代码
		读battery_id 电压，匹配电池曲线
		
		int g_fg_battery_id;

		#ifdef MTK_GET_BATTERY_ID_BY_AUXADC
		void fgauge_get_profile_id(void)
	
	}

	

	


   4.USB  pid，vid添加到驱动中
   {
   		gionee_usb_uid_pid 
   		不同功能对应不同的pid
   
   }
   
   
   5.这几个关键字的代码逻辑
   {
   	
		pmic_throttling_dlpt

   		FGADC_D0		开机初始化的电流，电压，电量
		fg_current_avg  平均电流
		fg_current_act	消耗电量
		
		打开fg log
		adb shell setpro persist.mediatek.fg.log.enable 1
		
		dlpt_notify_thr
		
		
	   //这应该是跟低电保护策略有关
	   DLPT_FEATURE_SUPPORT  
	   {
	   		#if defined(DLPT_FEATURE_SUPPORT)

			if (g_boot_mode != META_BOOT && g_boot_mode != FACTORY_BOOT && g_boot_mode != ATE_FACTORY_BOOT) {
				/*pmic_set_register_value(PMIC_BATON_TDET_EN, 1);*/
	
				pmic_set_register_value(PMIC_RG_BATON_EN, 1);
				if (pmic_get_register_value(PMIC_RGS_BATON_UNDET) == 1) {

					dprintf(CRITICAL, "[BATTERY] No battry plug-in. Power Off.");
					mt6575_power_off();
				}
			}

			pchr_turn_on_charging(KAL_FALSE);
			/* disable SW charger power path */

			switch_charger_power_path_enable(KAL_FALSE);
			mdelay(50);

			get_dlpt_imix_r();
			/* after get imix, re-enable SW charger power path */

			switch_charger_power_path_enable(KAL_TRUE);
			mdelay(50);

			check_bat_protect_status();
			if (is_charging == 1) {
				pchr_turn_on_charging(KAL_TRUE);
				dprintf(CRITICAL, "turn on charging \n\r");
			}
			#endif //#if defined(DLPT_FEATURE_SUPPORT)
	   }   
	   
	   
	   hps_main是干什么的
   }
   


	7.17G10A底电流偏高问题
	{
		这里主要是android系统的睡眠唤醒机制，然后为了便于分析功耗问题
		
	
		1.中断持锁EINT wakelock次数很多导致系统一直没有睡下去
		lk里面sc卡配置有问题，导致频繁上报中断
		
		还有一个不同阶段的gpio配置
		
		
		2.打印中断号，地址，wakeup.c
		在有中断持锁的时候知道是哪些中断
		
		
		3.系统的休眠，唤醒流程
		{
			"mem", "standby", "freeze" and "disk"
		
		}
		
		
		4.如何注掉一些驱动
		
		
		5.系统进入suspend 还有一些低功耗模式，一些流程
		
		
		5.分析思路

	}






	按键驱动以及工作流程，input子系统
	{
		按键工作主要包含哪些目录文件
		aw9523b.kl	
	
		
	
	}

	
    fuelgauge 3.0的问题,文档+邮件
    {


    }

}























/*17G10A当前存在的问题*/
{
	充电时序的关机充电和放电状态			
	{
		放电的1%位置还是有问题
		1%开始03：00 vbat=3.62V
		  结束03：52 vbat=3.3V

		1%可能相当于4%左右的电量，而且时间很长  
		1%电压还是很高
	}

	底层电量跟上层电量差别很大，但是这个没有开机和刚开是的充电的log		


	马达刷机第一次震动太弱，之后震动正常？	


	键盘按键有问题，mmi测试不通过，测键不管用


	mmi测试的调用节点好像还有问题
	{
		mmi测试读取的节点信息
		sys/class/power_supply/battery/

		充电可能读取的节点数据有问题
			几个电流，电压，电池电量，电池电压都有问题
			充电电流上不去
			{
				这个好像是主板温度过高，虽然显示的是电池温度50+，但是电池明显不热
				[    9.896490] <6>.(4)[258:charger_thread][name:mtk_charger&]Vbat=3775,I=-4446,VChr=9,T=49,Soc=0:0,CT:0:0

				[   12.371394] <6>.(6)[258:charger_thread][name:mtk_battery_hal&][fgauge_read_current] final current=5051 (ratio=950)
				[   12.372785] <6>.(6)[258:charger_thread][name:mtk_charger&]Vbat=3771,I=-5051,VChr=9,T=50,Soc=0:0,CT:0:0
				[   12.375494] <4>.(4)[258:charger_thread][name:pmic_auxadc&][mt6355_get_auxadc_value] ch = 2, reg_val = 0x1d3, adc_result = 410
				[   12.377141] <4>.(4)[258:charger_thread][name:mtk_battery_hal&][fgauge_read_current] final current=4735 (ratio=950)
				[   12.378491] [name:mtk_charger&][BATTERY] Battery over Temperature or NTC fail 50 50!!

				这个是电池的pin脚有问题
			}

		键盘mmi测试不能通过
			键盘的工作，以及涉及哪些目录下的文件
	}

        
	计步器，sd卡相关内容？
	{
		set_counter.c
		sd.c
	}


    从满电放电到关机的情况是否有异常


    计步器，sd卡相关内容？
    {
        set_counter.c
        sd.c
    }


	快充升压问题


	去掉OTG中断


	电池曲线的导入
}





/*G1605A  售后问题，功耗问题 healthd线程*/
{
    售后问题：   电量计不准的机器


    这个还不知道怎么看，只是觉得电量跟电池电压偏差有点大，而且底层电量跟上层显示的电量差别也很大


    apk发包次数，谁发的(主从)，就是链接到哪了？？
        那个脚本怎么用
}

















