








/*要处理的问题*/
{


    pmi8952,pmi8940,pmi8937,ti的bq系列电量计，以及精度对比
    {
        三个相关的文档对比
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
            
            
            2.linux_android_pmic_fuel_gauge_user_guide
            {
            	电量计对于电池充电接口的影响取决于
            	1.允许到充电截止的时间
            	2.当电池电量低于软件设定的阈值的时候，要关闭OTG功能
            
            	电量计读电流通过采样电阻，读电池电压通过连接电池正负极的BATT_P，BATT_N
            	放电读正极，其他时候读负极
            	
            	对于BAT_ID，通常用于检测电池是否存在，以及对于电池的识别，而这个识别的过程
            	是反复的提高基础电流知道发现匹配的为止(5->15->150uA)
            	
            	smart battery? 智能电池
            	
            	battery temperature
            	一是为了保障给电池充电可以安全的使用，二是为了更准确的计算电量
            
            	配置电量计中thermistor相关的参数(80-VT310-123).
            	qcom,thermal-coefficients这是一个位数组，通过这组数据，可以读NTC电阻的一些转换信息
            	{
            		这个有两个地方可以setting
            		一个是在kernel device tree里面
            		一个是在SBL里面，但是这里要晚1.5s因为要启动SRAM，在pm_config_target.c
            		里面设置
            			
            	}


				电池等效串联电阻ESR，这个值最好是实时的，则样才最准确
				等效电阻跟temperature有很大关系，同时也影响到电池的剩余电量
				power_supply 子系统是这个POWER_SUPPLY_PROP_RESISTANCE
				而这个数据的采集是在电量计每隔90s发一个脉冲，然后同步同一时间的电压和电流
				而这个脉冲会引起设备底电流的升高，所以当设备suspend的时候会关闭这个使能
				
				
				系统截止电流
				显示100%，系统截止电流，充电截止电流
				qcom,fg-iterm-ma = <150>; //这个是显示100%的截止电流？

				
				电量计的截止电流
				qcom,fg-chg-iterm-ma = <100>;	//这个是充电截止电流？
				
				
				系统关机电压
				这个影响0%的计算，也就是人为设定的一个0%电压应该是多少
				qcom, fg-cutoff-voltage-mv = <3000>; 
				
				
				开机的时候估计电池电压，如果超过阈值就重新检测导入对当前电量的计算
				qcom, vbat-estimate-diff-mv = <30>;

			
				恒流到恒压充电阈值
				qcom, fg-cc-cv-threshold-mv = <4340>;
				如果开启了动态电压调节充电这个值应该设定的接近vfloat voltage
				
				
				电量低于多少开启回充
				qcom,resume-soc = <95>;


				关于电池老化的检测是通过检测ESR来计算的
				一是通过ESR，
				一是通过电池电量学习算法
				{
					只是产权不开放
					跟温度，通过学习电池充电循环时间的增减，来学习充电循环
					电池开始的电量
				}


				Charge cycle count
				充电循环次数
				

				代码相关的位置
				drivers/power/qpnp-fg.c
				arch/arm/boot/dts/qcom/msm-pmi895x.dtsi
				Documentation/devicetree/bindings/power/qpnp-fg.txt

				
				debug
				{
					打开调试的log
					/*all debug types bit turned on */
					echo 0xff > /sys/module/qpnp_fg/parameters/debug_mask
					echo 8 > /proc/sys/kernel/printk
					dmesg > debug_output_filename

					还有一个dump sram信息的脚本，但是不管用					
				}

            }
            
            
            qpnp-fg.c
            {
            	为什么高通的单个结构体都有那么多成员，还有那么多工作函数
            	
            	fg_batt_profile_init 电量计相关参数的初始化 ，导入客制化profile
            	
            	然后重启电量计fg_do_restart
            
            }
            
            
            
            
            
        }
        
        
        
        

        给高通提case
    }    










    利用17G16A项目熟悉高通代码
    {
        充电smbcharger
        {   
            dtsi配置文件
            ./arch/arm/boot/dts/qcom/msm-pmi8940.dtsi

            kernel代码
            qpnp-smbcharger.c
            pmic-voter.c    



            (qpnp-smbcharger.c) 这个是高通充电用的主要的源代码，smbchg_init初始化模块 -> spmi_driver_register注册设备驱动 -> smbchg_probe将设备驱动跟设备挂钩，
            建立bind -> 从dtsi文件中获取一些参数 ->  create_votable创建了好几个投票变量 (fcc_votable,usb_icl_votable,dc_icl_votable,usb_suspend_votable,
            dc_suspend_votable,battchg_suspend_votable,hw_aicl_rerun_disable_votable,hw_aicl_rerun_enable_indirect_votable,aicl_deglitch_short_votable,
            hvdcp_enable_votable)



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


        fuelgauge
        {


        }




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

















