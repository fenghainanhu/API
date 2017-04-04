H每日计划:
    1.每天有疑问的内容尽可能当天掌握，不把问题留给明天
    2.工作计划要高质量的完成，追求效率
    3.定期的检查，调整
    4.核心内容是“实践”，在实践中掌握知识，技能
    5.每周定期总结，处理遗留问题




2017.2.4
必须：
    1.整理好相关的文档（API/my_data） 														90%
    2.emacs，编辑器的了解，使用，vim编辑器相关的插件,emacs ,vim ,gedit三个先用好其中一个		50%(使用gedit)
    3.公积金																					0%
    4.在csdn和开源中国社区看到的内容深度操作系统的源码											0%
    5.git revert回滚操作，回撤（这个命令还有疑问，相关内容）									0%
     git commit --amend  																	50%
     	
     

待定：   
	1.移植17W05A电量计算部分(重点)，battery_common_fg_20.c ,battery_meter_fg_20.c ,switch_charging.c跟原始代码对比
    2.RT5081的文档，及使用（重点）
    3.搭建android开发环境，编译原生代码
    4.正则表达式这个是需要练习的
    
    
    
2017.2.5
必须：
	1.移植17W05A电量计算部分(重点，重中之重)，battery_common_fg_20.c ,battery_meter_fg_20.c ,switch_charging.c		90%
	跟原始代码对比，理清思路，代码工作的流程
	2.RT5081的文档，要仔细阅读(重点)																					0%
	3.完成昨天遗留的必须任务																							0%
	
	
	
待定：
	1.移植fuelgauge部分，这部分可能需要很大的工作量	
    2.研读RT5081文档和txt
    
    
    
2017.2.6
必须：
	1.验证移植的fuelgauge GM2.0是否正确，并且提交上去											100%
	2.研读RT5081的文档，代码(重点，重中之重)													30%
	
待定：
	1.完成遗留的任务	    
	
	
	
2017.2.7
必须：
	1.RT5081文档，代码通过log理清工作流程(重点)												20%
	2.读完俞闵宏那本书(全是讲道理，文采一般般)													0%

	
待定：
	1.解决一些遗留的任务
	

备注：
	17G05A开发板
	
	
2017.2.8
必须：
	1.调试17G05A开发板																		1%(因为不清楚要干什么)
	2.看代码，看文档	，看log																	20%(看完另外文档)
	3.串口线能用																				100%


待定：	
	1.解决一些遗留的任务
	2.用C语言和C++分别写my_string的库和类，感觉基本的东西都不记得了，好菜...
	
	
	读完俞闵宏那本书(全是讲道理，文采一般般)													60%(全篇的讲道理，语言重复冗杂)
	
	
	
	
2017.2.9
必须:
	1.调试17G05A开发板，弄清楚要干什么														10%(抓log追踪代码)
		对比rt5081_pmu_charger_gn20.c 与rt5081_pmu_charger.c
		看看lk，preloader ,kernel三个阶段的dws配置
	2.看代码，看文档，看log	
	3.读完那本书，计划下一本看什么书，还有大学幕课也要准备看些什么								90%(书看完了，但是后面的计划还没有)
	

待定:
	1.解决一些遗留的任务
	2.调整状态，调整心情	
	
	
2017.2.10
必须：
	1.调试17G05A开发板，抓log，追踪代码，改代码										10%(感觉写了一天的BUG，要努力)
	2.自动化脚本看看怎么写的															0%
	3.鸟哥的私房菜这本书，定好计划，大学幕课上(c++，数据结构)有点准备					0%
	
	
待定：
	1.下载win7镜像，做好启动盘
	2.双系统安装，grub操作(不太理解)，最主要的是系统安装，硬盘分区方面	
	

备注：制作好了win7启动盘，有win7，centos，ubuntu的镜像		
	

2017.2.11
必须：
	1.安装好双硬盘双系统，grub分区怎么回事											50%(系统是安装好了，但是不是双系统)
	2.搭建好android开发环境，能编译android											90%
	3.写一个c语言的string库，C++的string类											0%

待定：
	1.看鸟哥的私房菜和今天到的几本书
	2.K&R版的c语言，静下心好好研习这本书，提高自己的能力
	
	
2017.2.12
必须：
	1.移植完17W05A，功能正常															100%
	2.17G05A为什么充电电流这么小，USB又不充电，找原因，改好							10%
	
	
待定：
	1.搭建好android开发环境，能编译android，相关的软件也安装好							80%(软件没装好)
	source_insight,VMware_workstation
	2.旧iphone和新iphone																100%		
	
	
	
	
	
	
*******************************************************************************************************************************************	
	
	
	
	
2017.2.13
必须：
	1.找出17G05A充电电流小的原因														100%(原因是用的假电)
	2.修改代码，使充电电流正常														100%(直接换真电池就行了)
	3.抓log，追踪代码，哪些密切相关的要注意											10%
		a.关机充电
	4.试一试monkey那个脚本管不管用													100%(管用，但是手机里面应该有个monkey的二进制文件)
		
	
待定：																
	1.公开课准备，K&R的C语言
	2.梦的解析这本书，有时间看看	
    
    
2017.2.14
必须：
	1.找出USB不充电的原因															100%(USB是充电的，只是充电电流很小)
	2.对比Type-C修改为micro USB代码修改的部分											90%(注释了几个宏)
	3.慕课网C课程																	100%(C语言的基础部分)
	
待定：
	1.K&R的C语言 | KMP算法 | git rebase用法											100%
	2.梦的解析	    																0%(没看，估计是没有一个好的开始)
	
	
2017.2.15
必须：
	1.调整充电器的充电电流和电压														50%
	2.调整好心情，调整好节奏															0%
		效率不高，还很受罪，心态放宽，放松思维
	3.慕课网C课程(C基础篇)															100%
		
		
待定：
	1.K&R的C语言
	2.多实践，敢去做去实践
		
		
2017.2.16
必须：
	1.调整充电电压，电流																100%
	2.提高工作效率																	80%(比之前一天好点)
	3.慕课网，中国大学慕课的课程														100%(幕课网C课程，数组)
	
待定：
	1.实践，静下心不要浮躁		

	
2017.2.17
必须：
	1.rt5081的充电流程，电池参数，及怎么调整											20%
	2.K&RC语言，数组，幕课网C课程														0%
	3.多实践，敢于尝试，不懂多思考，多去问											20%
	(效率不高)
	
待定：
	1.静下心不要浮躁，每天要有收获	

	
2017.2.18
		放松了一天，看了一部电影(剧情有点狗血)，应该算是无聊的一天。其实无聊的时候应该给自己找点事情干
	不要整天无所事事的，每天要有进步，不能停滞不前，努力让自己变得更好，变得更优秀。发现问题要敢于面对，敢于立刻去解决处理，不能拖。

	
2017.2.19
必须：
	1.移植OTG开关到7337项目上														50%(一直反复重启....)
	2.慕课网C语言课程(函数)，中国大学慕课的课程										100%(有个递归调用，还要自己写写看)
	
待定：
	1.不要彷徨，	每天有目标，有动力，让自己变得优秀


************************************************************************************************************************************

	
2017.2.20
必须：
	1.17G05A电流是否能升到1.9A														90%(可能达不到1.9A)
	2.移植OTG开关到BBL7337项目														80%(还差调试)
	3.慕课网C语言课程，K&R的C语言函数部分												0%
	
待定：
	1.相信自己，对自己要有信心
	2.想开点，让自己变得优秀
	
	
2017.2.21
必须：
	1.移植完OTG开关到BBL7337上(其实更应该清楚OTG开关的工作流程)						80%(但是OTG开关的工作流程还是不清楚，应该是函数)
	2.提交17G05A提高充电代码到服务器上												100%
	3.慕课网C语言课程，中国大学慕课的课程												100%
	
待定：
	1.K&R的C语言函数部分				
	2.手机抓dump	
	
	
2017.2.22
必须：
	1.弄清楚17G05A，从init开始的工作流程(先里清楚pep20那几个)							50%
	2.慕课网C语言课程																0%
	3.梦的解析																		100%
	
待定：
	1.K&R的C语言函数部分		
	
2017.2.23
必须：
	1.解决17W05A的那个bug															100%
	2.17G05A用快充充电器的修改，不支持快充											100%
	3.慕课网C语言课程																0%
	
待定：
	1.梦的解析		
	
2017.2.24
必须：
	1.KMP算法，字符串匹配															50%
	2.堆栈的C语言实现																0%
	3.慕课网C语言课程																0%
		
待定：
	1.梦的解析
	2.搬家

		
2017.2.25
遛了一天，跑到了中关村，颐和园，有点累，新地址还没装修好，鼻炎又犯了QAQ


2017.2.26
必须：
	1.百年航母看完																	100%
	2.慕课网C语言.指针部分															0%
	3.中国大学慕课网，第二周课程														0%
	
待定：
	1.英语学习计划，制定，坚持
	2.梦的解析	
	
	
*********************************************************************************************************************

2017.2.27
必须：
	1.解决17W05A的那个bug，logo显示中文，应该是英文的									10%(找到了图片但是代码编译的有问题)
	2.17G05A的快充，宏还是那个变量生效												0%
	3.慕课网C语言课程，指针部分														0%
	
待定：
	1.背单词，听英语																	
	2.梦的解析					
	

2017.2.28
必须：
	1.解决昨天遗留的任务，一定要完成													0%
	2.慕课网C语言课程																0%
	
待定：
	1.被单词，听英语																	0%
	2.简单，量少但一定要完成															0%
	
这两天的执行力，效率太低，必须要改变，强化执行任务的能力



2017.3.1	
必须：
	1.修改完17W05A的关机充电logo														100%
	2.熟悉17G05A的代码流程															50%
	3.提高工作效率																	0%
	
待定：
	1.看书，梦的解析，一定要静下心，不要浮躁
	2.听听英语，听听历史，慢慢来	
	
要有一种危机感，时刻提升自己才能向前	


2017.3.2
必须：
	1.熟悉17G05A的代码流程															20%
	2.分析昨天因为mmi测试导致的充电失败的原因											80%
	3.提高工作效率
	
待定：
	1.中国大学慕课的课程，数据结构和算法，感觉很有收获，要坚持							100%
	2.看看书，听听故事或者英语	


2017.3.3
必须：
	1.分析mmi测试导致充电失败，两个log，代码调用的流程									0%
	2.能不能提高17G05A的充电电流，库仑计等相关参数是在哪调用读取的						0%
	3.中国大学慕课网，数据结构，K&R的C语言，指针数组部分								0%

待定：
	1.提高工作效率
	2.听听英语，听听历史
	
2017.3.4
好吧又是休息了一天，傍晚还被人叫去加班QAQ	



2017.3.5
必须：
	1.BBL7337 OTG开关为什么不管用，17W05A的两个patch怎么打进去						80%
	2.两份log，mmi测试导致充电失败的log，关机重启的log，这两个log都要重点分析			0%
	3.中国大学慕课网两个课程的编程作业												0%
	4.一定要打起精神，努力完成今天的计划												20%
	
待定：
	1.K&R的C语言，指针数组部分	
	
************************************************************************************************************************	

2017.3.6
必须：
	1.mmi测试导致充电失败的log以及修改的方案											100%
	2.关机重启bug的log，以及关机，重启的整个流程										30%
	3.两个patch合入17W05A项目验证													0%
	4.完成上周的作业																	0%

待定：
	1.要有危机意识，打起精神工作
	2.C语言画椭圆那个算法以及代码实现
	
	
2017.3.7
必须：
	1.分析启动，关机，重启的log流程，如何抓上层的sytem，main log						50%
	2.合入patch，前提知道是哪些是跟fuelgauge相关的									0%
	3.中国大学慕课网C语言课程，K&R的C语言指针数组部分									0%
	
待定：
	1.打起精神，努力工作，提高自己的能力是最重要的，只为追求卓越	
	

2017.3.8
必须：
	1.分析关机，重启的log，走的什么流程												100%
	2.熟悉G1605A的代码，想想那个特性应该怎么写										0%
	3.调整工作时间，不然有些任务一直无法完成，这样下去不行								50%
	
待定：
	1.提高工作效率，必须有危机意识，不能放松
	2.中国大学慕课网C语言，K&R的C语言指针数组部分，一定要着手看了，不要只是计划		
	
	
	
2017.3.9
必须：
	1.相关的文档，代码分类整理好													 	100%
	2.熟悉G1605A的代码流程，想想特性应该怎么写										100%
	3.中国大学慕课网数据结构，那几个代码怎么写											80%
	
待定：
	1.提高工作效率，每天要有进步	
	

2017.3.10
必须：
	1.在G1605A上实现电池维护特性													80%(有待完善)
	2.中国大学慕课网，C语言最基本的部分，一定要动手写，思考							 0%
	3.阅读，熟悉17G05A或者G1602A的代码											0%
	
待定：
	1.放宽心态，不要对一些琐事斤斤计较，要大度
	2.追求卓越		


2017.3.11
爬长城爬了一天，有点累。



2017.3.12

必须：
	1.看一两部电影，遇见大咖														100%
	2.中国大学慕课网，可以定要看完，对应的C语言的书(这个任务今天必须完成)				  100%	
	3.回顾这一周的事情，反思才能提高												  100%

待定：
	1.提高看代码，写代码的能力	




××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××

2017.3.13
必须：
	1.完善电池维护代码的逻辑，功能要实现												70%
	2.熟悉G1602的代码，烧写                                                       0%															
	3.慕课网C语言，K&R的C语言指针数组部分(要有执行力度)					              0%

待定：	
	1.看的懂代码，也要能写出来
	2.追求卓越


2017.3.14
必须：
	1.熟悉G1602A的相关代码，烧写													5%
	2.处理一些琐碎的事，一些基本的东西												100%
	3.先把慕课网的C语言指针看完，然后是中国大学慕课									0%

待定：
	1.把电池维护的工作移到周末，不然总是不能完成，同时影响其他工作进度
	2.工作要有效率，理解能力，分析问题能力要加强	


2017.3.15
必须：	
	1.熟悉BBL7505A的代码，理清思路，计划怎么添加这个功能								 80%
	2.17W05A的充电测试，做事要细心													20%
	3.慕课网的C语言指针课程，K&R的C语言指针数组部分									  100%

待定：
	1.提高工作效率	


2017.3.16
必须：
	1.熟悉BBL7505A的代码，今天晚上编译吧											 100%
	2.调整心态，我觉的最近的心态很有问题											   50%
	3.有些事想要做的要去做，不要拖延，不要害怕，不要在乎别人的想法，要关乎自己			  80%

待定：
	1.计划数据结构这本书的学习计划，不然这样拖下去，都不知道要学到什么时候





2017.3.17
必须：
	1.BBL7505A项目的OTG开关，基本功能要实现										50%
	理清了一些细节
	2.数据结构第一章结束，上面的一个一页的想法要实现，开始第二章						 20%
	3.就是关于绩效今天要写完了													0%

待定：	
	1.中国大学慕课网，数据结构的课程	



	数据结构这周的计划
	{
		最基本的要求：
		1.课程内容要看完，上面的例子要掌握
		2.数据结构这本书上的线性表部分，这部分内容是很基础但是也非常重要，基本的C语言怎么实现要会
		3.完善自己写的库，自己写的代码要清楚，怎样更完善，更好的调用，使用
		4.任务以完成量为准，不能以时间为准

		day1
		1.链表及其实现		
		->算法书的第一章
	}


2017.3.18
去圆明园玩了大半天，话说圆明园真是有点大，古代帝王的宫殿，真是可惜



2017.3.19
必须：
	1.今天最重要的任务是在BBL7505A上做好OTG开关，这是今天必须要完成的							30%
	2.关于绩效的内容尽量在今天完成														0%
	3.思考一下C，数据结构，英语这三个的学习计划，做事情要专一，专注							  0%

待定：	
	1.就是努力提高效率，完成自己设定的任务
	2.回顾这周的工作，哪些是比较重要，哪些是未完成的




一周总结：
	首先想学的有很多，但是每样都只学点皮毛，这点是没有什么用的，你想追求卓越，追求更好的自己，必须学会专注，对一件事情
	做好，做精，做出自己的特色，在一个领域要想有所建树必须深入的学习。

	所以当前阶段很重要的并且是很重要的任务就是学好C语言，而且英语这种东西积累是很重要的
	C语言：
		K&R的C语言书 + 慕课网的视频

	英语：
		每天坚持听英语 + 看英文的文章	

****************************************************************************************************	
***关于C语言，这周最基本的要完成指针部分的学习

2017.3.20

	1.绩效考评完成							***												100%
	2.BBL7505A的OTG开关功能			 		*****											 80%(基本功能可以实现，还差一点逻辑判断)
	3.C语言指针部分							**												100%	
	4.英语阅读，听力在于平时多积累，多听，多看    **											   100%


2017.3.21		
	1.type-c协议，熟悉相关内容				  		  ***										100%
	2.完善BBL7505A的OTG开关的逻辑，对比总结			   ******									  90%
	3.C语言指针，字符串，数组							***										   90%
	4.坚持阅读英语新闻，被单词，句子					 **											 80%


2017.3.22
	1.算法爱好者上的那篇文章							**										  100%
	2.BBL7505A的OTG开关的一些问题，确定是否能提交上去	   ****										 100%
	3.完善电池维护代码逻辑							 ***									   90%
	4.C语言指针，字符串，数组等课程的内容				***										  90%						
	5.坚持看英语新闻，背单词，句子					  **										100%


2017.3.23
	1.I2C两篇博客看完								   ***									  100%
	2.G1602A的USB/OTG，根据log熟悉代码				 ****									  100%
	3.K&R C语言指针部分，理解书上的源码，练习怎么写		 ***								  0%
	4.看英语新闻，看懂意思，背单词，句子				   ** 									  80%
	
	
2017.3.24
	1.根据log熟悉G1602A的代码						****									 100%	
	2.理清W919要负责哪些内容							***										0%
	3.C语言指针部分，强化将理解能力，思维能力			***										 0%
	4.看英语新闻，拼图95飞机(好激动)					**										 100%


2017.3.25
基本上花了一天的时间拼完了图95轰炸机，虽然比较大，但是细节不是很好，恩就是这样
然后因为代码没有merge进去，有跑到公司来QAQ，做事情一定要认真，仔细，负责		


2017.3.26	
	1.理清G1602A的开机启动log，分析功耗问题，G1602A的代码					***					 100%
	2.W919USB充电电流较小的问题要解决，马达，按键的驱动在哪	  			 ****				  20%
	3.C语言指针，数组部分											  ***					0%
	4.看英语新闻，怎么顺手怎么看，但是能力要提高							  **					0%
	5.总结这周的工作													*****				 100%	




改版后的daily_plan效率好了一点，要不断的总结，完善，养成合理的生活习惯，敢于改变，想做什么不要拖延，敢于做自己向左的
****************************************************************************************************************

2017.3.27
	1.熟悉G1602A的代码，首先是功耗问题，高通之前的几个case				 *****					100%
	2.读书，静下心												   ***					  100%
	3.看英语新闻，听英语												 **						100%



2017.3.28
	1.熟悉G1602A的代码，追踪代码，重点提高对功耗问题的分析能力			  *****					 100%
	2.BBL7505A的OTG开关功能验证									  ***					 100%
	3.读书，每天都要读书，提高自己的修养								    ***					   100%
	4.看英语新闻，听英语												 **						80%


2017.3.29
	1.无线充电那些文档，跟type-c的文档								  ***					 100%
	2.熟悉G1602A的代码，追踪代码，重点提高对功耗问题的分析能力			  ****					 100%
	3.读书，坚持下去												  ***					 0%
	4.看英语，考虑要不要背背单词，英语学习计划要修改完善					  ***					 0%


	有点迷茫。有被人打击了QAQ


2017.3.30
	1.G1602A的OTG工作流程及相关的代码								   *****				   60%
	2.高通的关于pmic的主要文档										***						0%
	3.调整好心态，不被打击怎么有动力，要坚强							  ****					  40%
	4.看书，每天坚持给自己充电，多读书，多思考问题，不要浪费在无意义的事情上  ***					   80%
	5.坚持看英语，水平太菜，不前进就是被淘汰							  **					  80%



2017.3.31
	1.处理完功耗问题												  ***					  100%
	2.熟悉G1602A的代码，高通的几个pmic文档				    		  ****					  0%
	3.坚持看书，多思考，有自己的想法，敢于实现，敢于做					   ***					   0%
	4.坚持看英语，不要想太多，最舒服的办法，寻找最有效的办法				 **ni						 80%



2017.4.1
	1.处理完功耗问题，总结相关的内容								     ****					 80%
	2.提高效率，认真完成工作内容										   ***					  40%
	3.着眼提高，每天要有进步，对这方面要有明确的计划						****					40%
	4.坚持看英语，看书，每天看一点，看能走马观花，要走心					   ***				      0%


2017.4.2
今天相当于完了半天，起了半天的车，绕了半个北京城；
给有些东西分好类了；
看了乘风破浪这部电影，虽然更喜欢颖宝了，好喜欢，但是整部电影，韩寒想干什么，还没想明白



总结这周工作
效率一般，有待提高
要看书，一本一本看，静下心，不能太浮躁，谁的青春不迷茫看完
下一步是操作系统，鸟哥的私房菜，还是linux内核，但是我更想提高自己的C语言功底
英语一个是单词，关于句子的分析，还有最后一步是阅读和听力
×××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××××

2017.4.3
	1.调试W919+，主要是关闭软件关机，USB充电，mmi测试（其他项目是怎么加的）			*****				20%
	2.整理好功耗问题，原因，分析，后续完善分析处理办法							    ****				0%
	3.电池维护代码完善														  ***				  0%
	4.坚持看英语，看书，运动健身												   ***					80%
	5.制定英语的作战计划														 ****				  0%

真是，整一天都没有整出来，上面那三条作为下周额外工作的核心，在没有紧急情况下，有限完成


2017.4.4
	1.看一部电影，美丽人生，写写观后感，自己之前看过的电影，不做旁观者，有些自己的想法跟看法				×××
	2.看书，谁的青春不迷茫，有些句子还是比较好的，可以适当摘下来，英语背单词计划，做自己的事不要想太多		 ××××
	3.中国大学慕课网，操作系统课程，看看怎么样，如果不错，就要确定一步做什么，看什么					   ××××
	4.最后一点应该是执行力的问题，做事要果断，有效率，有质量										   ×××