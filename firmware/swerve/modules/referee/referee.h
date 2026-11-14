//#ifndef REFEREE_H
//#define REFEREE_H

//#include "main.h"
//#include "protocol.h"
//#include "stdbool.h"

///***********************************命令码ID V1.6。1****************************************************/
//					
///* 

//	ID: 0x0001  Byte:  11   比赛状态数据       			  		固定1HZ频率发送      
//	ID: 0x0002  Byte:  1    比赛结果数据         					比赛结束后发送      
//	ID: 0x0003  Byte:  32   比赛机器人血量数据   					固定3HZ频率发送 
//	
//	ID: 0x0101  Byte:  4    场地事件数据   								固定1HZ频率发送
//	ID: 0x0102  Byte:  3    场地补给站动作标识数据  			触发发送 
//	ID: 0X0104  Byte:  2    裁判警告数据									己方判罚/判负时触发发送，其余时间以 1Hz 频率发送
//	ID: 0x0105  Byte:  1    飞镖发射相关数据							固定1HZ频率发送
//	
//	ID: 0X0201  Byte: 27    机器人性能体系数据						固定10HZ频率发送
//	ID: 0X0202  Byte: 16    实时功率热量数据   						固定50HZ频率发送     
//	ID: 0x0203  Byte: 16    机器人位置数据          			哨兵机器人10HZ，其余机器人1HZ
//	ID: 0x0204  Byte:  1    机器人增益数据          			固定3HZ频率发送
//	ID: 0x0205  Byte:  1    空中支援时间数据      				固定1HZ频率发送
//	ID: 0x0206  Byte:  1    伤害状态数据           				伤害发生后发送
//	ID: 0x0207  Byte:  7    实时射击数据           				子弹发射后发送
//	ID: 0x0208  Byte:  6    允许发弹量           					固定10HZ频率发送
//	ID: 0x0209  Byte:  4    机器人RFID状态								固定3HZ频率发送
//	ID: 0x020A  Byte:  6    飞镖选手端指令数据						飞镖闸门上线后固定3HZ频率发送
//	ID: 0x020B  Byte: 40    地面机器人位置数据      			固定1HZ频率发送
//	ID: 0x020C  Byte:  6    雷达标记进度数据      				固定1HZ频率发送
//	ID: 0x020D  Byte:  4    哨兵自主决策相关信息同步  		固定3HZ频率发送
//	ID: 0x020E  Byte:  1    雷达自主决策信息同步      		固定1HZ频率发送
//	
//	ID: 0x0301  Byte:128    机器人间交互数据          		发送方触发发送，频率上限为 10Hz 
//	ID: 0x0302  Byte: 30    自定义控制器与机器人交互数据  发送方触发发送，频率上限为 30Hz
//	ID: 0x0303  Byte: 15    选手端小地图交互数据      		选手端触发发送
//	ID: 0x0304  Byte: 12    键鼠遥控数据									固定 30Hz 频率发送      				
//	ID: 0x0305  Byte: 10    选手端小地图接收雷达数据			频率上限为 10Hz
//	ID: 0x0306  Byte:  8    自定义控制器与选手端交互数据	发送方触发发送，频率上限为 30Hz
//	ID: 0x0307  Byte:103    选手端小地图接收哨兵数据			频率上限为 1Hz
//	ID: 0x0308  Byte: 34    选手端小地图接收机器人消息		频率上限为 3Hz 
//*/
///*定义底盘类型 默认是血量优先 0是血量优先 1是功率优先*/
//#define Chassis_Attributes  0
///*定义枪口类型 默认是冷却优先 0是冷却优先 1是爆发优先*/
//#define Shoot_Attributes    0
//typedef enum
//{
//    PROGRESS_UNSTART        = 0,
//    PROGRESS_PREPARE        = 1,
//    PROGRESS_SELFCHECK      = 2,
//    PROGRESS_5sCOUNTDOWN    = 3,
//    PROGRESS_BATTLE         = 4,
//    PROGRESS_CALCULATING    = 5,
//} game_progress_t;

//typedef enum
//{
//	ID_game_state      											= 0x0001,//比赛状态数据       			  		固定1HZ频率发送
//	ID_game_result      										= 0x0002,//比赛结果数据         					比赛结束后发送
//	ID_robot_health_status      						= 0x0003,//比赛机器人血量数据   					固定3HZ频率发送 
//	
//	ID_event_data  													= 0x0101,//场地事件数据   								固定1HZ频率发送
//	ID_supply_station_action   							= 0x0102,//场地补给站动作标识数据  			  触发发送
//	ID_supply_warn 													= 0x0104,//裁判警告数据									  己方判罚/判负时触发发送，其余时间以 1Hz 频率发送
//	ID_missile_shoot_time										= 0x0105,//飞镖发射相关数据							  固定1HZ频率发送
//	
//	ID_game_robot_state    									= 0x0201,//机器人性能体系数据							固定10HZ频率发送
//	ID_power_heat_data    									= 0x0202,//实时功率热量数据   						固定50HZ频率发送 
//	ID_game_robot_pos        								= 0x0203,//机器人位置数据          				哨兵机器人10HZ，其余机器人1HZ
//	ID_buff_musk														= 0x0204,//机器人增益数据          				固定3HZ频率发送
//	ID_aerial_support_time									= 0x0205,//空中支援时间数据      				  固定1HZ频率发送
//	ID_robot_hurt														= 0x0206,//伤害状态数据           				伤害发生后发送
//	ID_shoot_data														= 0x0207,//实时射击数据           				子弹发射后发送
//	ID_bullet_remaining         						= 0x0208,//允许发弹量           					固定10HZ频率发送
//	ID_rfid_status													= 0x0209,//机器人RFID状态									固定3HZ频率发送
//	ID_dart_player_command        					= 0x020A,//飞镖选手端指令数据							飞镖闸门上线后固定3HZ频率发送
//	ID_ground_robot_position								= 0x020B,//地面机器人位置数据      				固定1HZ频率发送
//	ID_radar_marking_progress								=	0x020C,//雷达标记进度数据      					固定1HZ频率发送
//	ID_sentinel_autonomous_decision_sync		= 0x020D,//哨兵自主决策相关信息同步  			固定3HZ频率发送
//	ID_radar_autonomous_decision_sync				=	0x020E,//雷达自主决策信息同步      			固定1HZ频率发送
//	
//	ID_robot_interactive_header_data				= 0x0301,//机器人间交互数据          			发送方触发发送，频率上限为 10Hz
//	ID_controller_interactive_header_data 	= 0x0302,//自定义控制器与机器人交互数据   发送方触发发送，频率上限为 30Hz
//	ID_map_interactive_header_data        	= 0x0303,//选手端小地图交互数据      			选手端触发发送
//	ID_keyboard_information               	= 0x0304,//键鼠遥控数据										固定 30Hz 频率发送             				
//	ID_player_minimap_radar_data						= 0x0305,//选手端小地图接收雷达数据				频率上限为 10Hz
//	ID_custom_controller_player_interaction = 0x0306,//自定义控制器与选手端交互数据		发送方触发发送，频率上限为 30Hz
//	ID_player_minimap_sentinel_data					= 0x0307,//选手端小地图接收哨兵数据				频率上限为 1Hz
//	ID_player_minimap_robot_messages				= 0x0308,//选手端小地图接收机器人消息			频率上限为 3Hz 
//}CmdID;

///* ID: 0x0001  Byte:  11    比赛状态数据 */

//typedef __packed struct 
//{ 
//	uint8_t game_type : 4;					//比赛类型
//	uint8_t game_progress : 4;			//当前比赛阶段
//	uint16_t stage_remain_time;			//当前阶段剩余时间 单位s
//	uint64_t SyncTimeStamp;
//} game_status_t; 

///* ID: 0x0002  Byte:  1    比赛结果数据 */
//typedef __packed struct 
//{ 
//	uint8_t winner;
//} game_result_t;
///* ID: 0x0003  Byte:  32    比赛机器人血量数据 */
//typedef __packed struct 
//{ 
//	uint16_t red_1_robot_HP;   
//	uint16_t red_2_robot_HP;  
//	uint16_t red_3_robot_HP;  
//	uint16_t red_4_robot_HP;  
//	uint16_t red_5_robot_HP;  
//	uint16_t red_6_robot_HP;  
//	uint16_t red_7_robot_HP;  
//	uint16_t red_outpost_HP; 
//  uint16_t red_base_HP; 
//	
//	uint16_t blue_1_robot_HP;   
//	uint16_t blue_2_robot_HP;   
//	uint16_t blue_3_robot_HP;   
//	uint16_t blue_4_robot_HP;   
//	uint16_t blue_5_robot_HP;   
//	uint16_t blue_6_robot_HP;   
//	uint16_t blue_7_robot_HP; 

//	uint16_t blue_outpost_HP; 
//  uint16_t blue_base_HP;   
//} game_robot_HP_t; 

///* ID: 0x0101  Byte:  4    场地事件数据 */
//typedef __packed struct 
//{ 
//	uint32_t event_data;										//都需要使用 后续与UI相连
//} event_data_t;

///* ID: 0x0102  Byte:  4    场地补给站动作标识数据 */
//typedef __packed struct 
//{ 
//	uint8_t supply_robot_id;								//当前补弹机器人ID
//	uint8_t supply_projectile_step;					//补给站出弹口状态
//	uint8_t supply_projectile_num;					//补弹数目
//} ext_supply_projectile_action_t; 

///* ID: 0x0104  Byte: 3   裁判系统警告信息 */
//typedef __packed struct
//{
//	uint8_t level;													//己方最后一次受到判罚的等级
//	uint8_t offending_robot_id;							//己方最后一次受到判罚的违规机器人 ID。（如红 1 机器人 ID 为 1，蓝1 机器人 ID 为 101）
//	uint8_t count;													//己方最后一次受到判罚的违规机器人对应判罚等级的违规次数
//} referee_warning_t;

///* ID: 0x0105  Byte:3  飞镖发射口倒计时 */
//typedef __packed struct 
//{ 
//	uint8_t dart_remaining_time; 						//15s倒计时
//	uint8_t dart_info;									//飞镖目标状态
//} dart_info_t;  

///* ID: 0X0201  Byte: 6    机器人状态数据 */
//typedef __packed struct
//{
//	uint8_t robot_id;															//本机器人ID
//	uint8_t robot_level;                          //机器人等级
//	uint16_t current_HP;													//机器人当前血量
//	uint16_t maximum_HP;                          //机器人血量上限
//	uint16_t shooter_barrel_cooling_value;        //机器人枪口热量每秒冷却值
//  uint16_t shooter_barrel_heat_limit;           //机器人枪口热量上限
//  uint16_t chassis_power_limit;                 //机器人底盘功率上限
//	uint8_t power_management_gimbal_output : 1;		//gimbal  口输出
//	uint8_t power_management_chassis_output : 1;	//chassis 口输出
//	uint8_t power_management_shooter_output : 1;	//shoot   口输出
//}robot_status_t;
///* ID: 0X0202  Byte: 16    实时功率热量数据 */
//typedef __packed struct 
//{ 
//	uint16_t chassis_voltage;   									//底盘输出电压 mV
//	uint16_t chassis_current;    									//底盘输出电流 mA
//	float chassis_power;   												//底盘输出功率 
//	uint16_t buffer_energy;												//底盘缓冲功率剩余
//	uint16_t shooter_17mm_1_barrel_heat;					//第 1 个 17mm 发射机构的枪口热量
//	uint16_t shooter_17mm_2_barrel_heat;  				//第 2 个 17mm 发射机构的枪口热量
//	uint16_t shooter_42mm_barrel_heat; 						//42mm枪口目前热量
//} power_heat_data_t; 

///* ID: 0x0203  Byte: 12    机器人位置数据 */
//typedef __packed struct
//{
//	float x;												//本机器人位置 x 坐标，单位：m
//	float y;												//本机器人位置 y 坐标，单位：m
//	float angle;										//本机器人测速模块朝向，单位：度。正北为 0 度
//}robot_pos_t;

///* ID: 0x0204  Byte:  5    机器人增益数据 */
//typedef __packed struct
//{
//	uint8_t recovery_buff;					//机器人回血增益（百分比，值为 10 意为每秒回复 10%最大血量）
//	uint8_t cooling_buff;						//机器人枪口冷却倍率（直接值，值为 5 意味着 5 倍冷却）
//	uint8_t defence_buff;						//机器人防御增益（百分比，值为 50 意为 50%防御增益）
//	uint8_t vulnerability_buff;     //机器人负防御增益（百分比，值为30表示-30%防御增益）
//	uint16_t attack_buff;						//机器人攻击增益（百分比，值为 50 意为 50%攻击增益）
//}buff_t;
///* ID: 0x0205  Byte:  1    空中支援数据 */
//typedef __packed struct
//{
//	uint8_t airforce_status;				//空中机器人状态（0 为正在冷却，1 为冷却完毕，2 为空中支援期间）
//	uint8_t time_remain;						//此状态的剩余时间
//}air_support_data_t; 

///* ID: 0x0206  Byte:  1    伤害状态数据 */
//typedef __packed struct
//{
//	uint8_t armor_id : 4;
//	uint8_t HP_deduction_reason : 4;
//}hurt_data_t;
///* ID: 0x0207  Byte:  7    实时射击数据 */
//typedef __packed struct
//{
//	uint8_t bullet_type;						//弹丸类型
//	uint8_t shooter_number;					//发射机构ID
//	uint8_t launching_frequency;		//弹丸射频（单位：Hz）
//	float initial_speed;						//弹丸初速度（单位：m/s）
//}shoot_data_t;

///* ID: 0x0208  Byte:  6    子弹数量数据 */
//typedef __packed struct
//{
//	uint16_t projectile_allowance_17mm;			//17mm 弹丸允许发弹量
//	uint16_t projectile_allowance_42mm;			//42mm 弹丸允许发弹量
//	uint16_t remaining_gold_coin;						//剩余金币数量
//}projectile_allowance_t;
///* ID: 0x0209  Byte:  4    FRID状态 */
//typedef __packed struct 
//{ 
//	uint32_t rfid_status ;												//RFID卡状态
//}rfid_status_t;
///* ID: 0x020A  Byte:  6    飞镖机器人客户端指令数据*/
//typedef __packed struct
//{
//	uint8_t dart_launch_opening_status;			//当前飞镖发射站的状态：1 关闭 0 开启
//	uint8_t reserved;
//	uint16_t target_change_time;						//切换打击目标时的比赛剩余时间，单位：s
//	uint16_t latest_launch_cmd_time;				//最后一次操作手确定发射指令时的比赛剩余时间，单位：s
//}dart_client_cmd_t;
///* ID: 0x020B  Byte:   40  各机器人位置信息 
//场地围挡在红方补给站附近的交点为坐标原点，沿场地长边向蓝方为 X 轴正方向，沿场地短边
//向红方停机坪为 Y 轴正方向。
//*/
//typedef __packed struct
//{
//	float hero_x;							//英雄X坐标
//	float hero_y;							//英雄Y坐标
//	float engineer_x;					//工程X坐标
//	float engineer_y;					//工程Y坐标
//	float standard_3_x;				//3号步兵X坐标
//	float standard_3_y;				//3号步兵Y坐标
//	float standard_4_x;				//4号步兵X坐标
//	float standard_4_y;				//4号步兵Y坐标
//	float standard_5_x;				//5号步兵X坐标
//	float standard_5_y;				//5号步兵Y坐标
//}ground_robot_position_t;

///* ID: 0x020C  Byte:  6    对方机器人被标记进度 */
//typedef __packed struct
//{
//	uint8_t mark_hero_progress;							//对方英雄机器人被标记进度：0—120
//	uint8_t mark_engineer_progress;					//对方工程机器人被标记进度：0—120
//	uint8_t mark_standard_3_progress;				//对方 3 号步兵机器人被标记进度：0—120
//	uint8_t mark_standard_4_progress;				//对方 4 号步兵机器人被标记进度：0—120
//	uint8_t mark_standard_5_progress;				//对方 5 号步兵机器人被标记进度：0—120
//	uint8_t mark_sentry_progress;						//对方哨兵机器人被标记进度：0—120
//}radar_mark_data_t;

///* ID: 0x020D  Byte:  4    哨兵兑换的发弹量数据 */
//typedef __packed struct
//{
//																					//bit0-10：哨兵已经成功兑换的发弹量值
//	uint32_t sentry_info;										//bit 11-14：哨兵成功远程兑换发弹量的次数
//																					//bit 15-18：哨兵成功远程兑换血量的次数
//} sentry_info_t;

///* ID: 0x020E  Byte:  1    雷达触发双倍易伤信息 */
//typedef __packed struct
//{
//	uint8_t radar_info;	  //bit 0-1：雷达是否拥有触发双倍易伤的资格 bit2：0：对手未被触发双倍易伤	1：对手正在被触发双倍易伤 ? 
//}radar_info_t;


///* 
//	
//	交互数据，包括一个统一的数据段头结构，
//	包含了内容 ID，发送者以及接受者的 ID 和内容数据段，
//	整个交互数据的包总共长最大为 128 个字节，
//	减去 frame_header,cmd_id,frame_tail 以及数据段头结构的 6 个字节，
//	故而发送的内容数据段最大为 113。
//	整个交互数据 0x0301 的包上行频率为 10Hz。

// 1：红方英雄机器人
// 2：红方工程机器人
// 3/4/5：红方步兵机器人（与机器人 ID 3~5 对应）
// 6：红方空中机器人
// 7：红方哨兵机器人
// 8：红方飞镖
// 9：红方雷达
// 10：红方前哨站
// 11：红方基地
// 101：蓝方英雄机器人
// 102：蓝方工程机器人
// 103/104/105：蓝方步兵机器人（与机器人 ID 3~5 对应）
// 106：蓝方空中机器人
// 107：蓝方哨兵机器人
// 108：蓝方飞镖
// 109：蓝方雷达
// 110：蓝方前哨站
// 111：蓝方基地
//选手端 ID 如下所示：
// 0x0101：红方英雄机器人选手端
// 0x0102：红方工程机器人选手端
// 0x0103/0x0104/0x0105：红方步兵机器人选手端（与机器人 ID 3~5 对应）
// 0x0106：红方空中机器人选手端
// 0x0165：蓝方英雄机器人选手端
// 0x0166：蓝方工程机器人选手端
// 0x0167/0x0168/0x0169：蓝方步兵机器人选手端（与机器人 ID 3~5 对应）
// 0x8080：裁判系统服务器（用于哨兵和雷达自主决策指令）
//*/

///* ID: 0x301   Byte:  7    机器人交互数据使用常规链路发送*/
//typedef __packed struct
//{
//	uint16_t data_cmd_id;			//需为开放的子内容 ID
//	uint16_t sender_id;				//需与自身 ID 匹配
//	uint16_t receiver_id;			//接收者 ID
//  uint8_t user_data[113];			//内容数据段  X 最大为113
//}robot_interaction_data_t;

//enum judge_robot_ID{
//	hero_red       = 1,
//	engineer_red   = 2,
//	infantry3_red  = 3,
//	infantry4_red  = 4,
//	infantry5_red  = 5,
//	plane_red      = 6,
//	
//	hero_blue      = 101,
//	engineer_blue  = 102,
//	infantry3_blue = 103,
//	infantry4_blue = 104,
//	infantry5_blue = 105,
//	plane_blue     = 106,
//};
//typedef struct{
//	uint16_t teammate_hero;
//	uint16_t teammate_engineer;
//	uint16_t teammate_infantry3;
//	uint16_t teammate_infantry4;
//	uint16_t teammate_infantry5;
//	uint16_t teammate_plane;
//	uint16_t teammate_sentry;
//	
//	uint16_t client_hero;
//	uint16_t client_engineer;
//	uint16_t client_infantry3;
//	uint16_t client_infantry4;
//	uint16_t client_infantry5;
//	uint16_t client_plane;
//} ext_interact_id_t;

//typedef struct judge_info_struct {
//	unpack_data_t										FrameHeader;							// 								帧头信息
//	
//	game_status_t 									GameState;								// 0x0001         比赛状态数据
//	game_result_t 									GameResult;								// 0x0002         比赛结果数据
//	game_robot_HP_t 								GameRobotHP;							// 0x0003         机器人血量数据
//	
//	event_data_t										EventData;								// 0x0101         场地事件数据
//	ext_supply_projectile_action_t	SupplyProjectileAction;		// 0x0102 				补给站动作标识
//	referee_warning_t								RefereeWarning;						// 0x0104         裁判警告信息
//	ext_dart_remaining_time_t				dart_remaining_time;			// 0x0105         飞镖发射口倒计时
//	
//	robot_status_t									GameRobotStat;						// 0x0201         比赛机器人状态
//	power_heat_data_t								PowerHeatData;						// 0x0202         实时功率热量数据
//	robot_pos_t											GameRobotPos;							// 0x0203         机器人位置
//	buff_t													Buff;											// 0x0204     		机器人增益
//	air_support_data_t							AerialRobotEnergy;				// 0x0205         空中机器人能量状态
//	hurt_data_t											RobotHurt;								// 0x0206         伤害状态
//	shoot_data_t										ShootData;								// 0x0207         实时射击信息(射频  射速 子弹信息)
//	projectile_allowance_t					bullet_remaining;					// 0x0208	        子弹剩余发射数
//	rfid_status_t										rfid_status;							// 0x0209	        RFID信息
//	dart_client_cmd_t           		dart_client;        			// 0x020A         飞镖客户端
//	ground_robot_position_t         GroundRobotPosition;			// 0x020B					各机器人位置信息
//	radar_mark_data_t								RadarMarkData;						// 0x020C					对方机器人被标记进度
//	sentry_info_t										SentryInfo;								// 0x020D					哨兵兑换的发弹量数据
//	radar_info_t										RadarInfo;								// 0x020E					雷达触发双倍易伤信息 
//	
//	ext_interact_id_t								ids;											//								与本机交互的机器人id
//	uint16_t                        self_client;        			//								本机客户端
//	bool	 													IF_REF_ONL;

//} Referee_info_t;


///* 
//	学生机器人间通信 cmd_id 0x0301，内容 ID:0x0200~0x02FF
//	交互数据 机器人间通信：0x0301。
//	发送频率：上限 10Hz  

//	字节偏移量 	大小 	说明 			备注 
//	0 			2 		数据的内容 ID 	0x0200~0x02FF 
//										可以在以上 ID 段选取，具体 ID 含义由参赛队自定义 
//	
//	2 			2 		发送者的 ID 	需要校验发送者的 ID 正确性， 
//	
//	4 			2 		接收者的 ID 	需要校验接收者的 ID 正确性，
//										例如不能发送到敌对机器人的ID 
//	
//	6 			n 		数据段 			n 需要小于 113 

//*/
//typedef __packed struct 
//{ 
//	uint8_t data[113]; //数据段,n需要小于113
//} robot_interactive_data_t;

///* 
//	学生机器人间通信 cmd_id 0x0301，内容 data_ID:0x0200~0x02FF
//	交互数据 机器人间通信：0x0301。
//	发送频率：数据上下行合计带宽不超过 5000 Byte。 上下行发送频率分别不超过30Hz。
// * +------+------+-------------+------------------------------------+
// * | byte | size |    breif    |            note                    |
// * |offset|      |             |                                    |
// * +------+------+-------------+------------------------------------+
// * |  0   |  2   | data_cmd_id | 0x0200~0x02FF,可以在这些 ID 段选取 |
// * |      |      |             | 具体ID含义由参赛队自定义           |
// * +------|------|-------------|------------------------------------|
// * |  2   |  2   | 	sender_ID  | 需要校验发送者的 ID 正确性					|
// * +------|------|-------------|------------------------------------|
// * |  4   |  2   | receiver_ID | 需要校验接收者的 ID 正确性					|
// * |      |      |             | 例如不能发送到敌对机器人的ID				| 
// * +------|------|-------------|------------------------------------|
// * |  6   |  n   | user_data[x]| x 需要小于 113 										|
// * +------+------+-------------+------------------------------------+
//*/

///******************************客户端交互数据**************************************/
//enum
//{
//	//0x200-0x02ff 	队伍自定义命令 格式  INTERACT_ID_XXXX
//	INTERACT_ID_delete_graphic 							= 0x0100,	/*客户端删除图形		*/
//	INTERACT_ID_draw_one_graphic 						= 0x0101,	/*客户端绘制1个图形	*/
//	INTERACT_ID_draw_two_graphic 						= 0x0102,	/*客户端绘制2个图形	*/
//	INTERACT_ID_draw_five_graphic 					= 0x0103,	/*客户端绘制5个图形	*/
//	INTERACT_ID_draw_seven_graphic 					= 0x0104,	/*客户端绘制7个图形	*/
//	INTERACT_ID_draw_char_graphic 				 	= 0x0110,	/*客户端绘制字符图形*/
//	INTERACT_ID_sentry_autonomous_decision 	= 0x0120, /*哨兵自主决策指令  */
//	INTERACT_ID_radar_autonomous_decision		= 0x0121, /*雷达自主决策指令  */
//	INTERACT_ID_bigbome_num									= 0x02ff
//};
////单位（字节）
//enum
//{
//	LEN_INTERACT_delete_graphic     				= 8,  //删除图层 2(数据内容ID)+2(发送者ID)+2（接收者ID）+2（数据内容）  
//	LEN_INTERACT_draw_one_graphic   				= 21, //同上2+2+2+15
//	LEN_INTERACT_draw_two_graphic   				= 36, //6+15*2
//	LEN_INTERACT_draw_five_graphic  				= 81, //6+15*5
//	LEN_INTERACT_draw_seven_graphic 				= 111,//6+15*7
//	LEN_INTERACT_draw_char_graphic  				= 51, //6+45（字符串内容）
//	LEN_INTERACT_sentry_autonomous_decision	=	10,	//6+4
//	LEN_INTERACT_radar_autonomous_decision	=	7,	//6+1
//};
////****************************绘图的数据段内容****************************/
//typedef __packed struct
//{
//	uint8_t figure_name[3];		//图形名  在图形删除，修改等操作中，作为索引
//	uint32_t operate_tpye:3;	//bit 0-2：图形操作： 0：空操作1：增加2：修改 3：删除
//	uint32_t figure_tpye:3;		//bit 3-5：图形类型：0：直线1：矩形2：正圆3：椭圆4：圆弧5：浮点数6：整型数7：字符
//	uint32_t layer:4;					//bit 6-9：图层数（0~9）
//	uint32_t color:4;					//bit 10-13：颜色：0：红/蓝（己方颜色）1：黄色2：绿色3：橙色4：紫红色5：粉色6：青色7：黑色8：白色
//	uint32_t details_a:9;			//bit 14-31：依绘制的不同图形含义不同，详见下表
//	uint32_t details_b:9;			//bit 14-31：依绘制的不同图形含义不同，详见下表
//	uint32_t width:10;				//图形配置 2 bit 0-9：线宽，建议字体大小与线宽比例为 10：1
//	uint32_t start_x:11;			//bit 10-20：起点/圆心 x 坐标
//	uint32_t start_y:11;			//bit 21-31：起点/圆心 y 坐标
//	uint32_t details_c:10;		//依绘制的不同图形含义不同，详见下表
//	uint32_t details_d:11;		//依绘制的不同图形含义不同，详见下表
//	uint32_t details_e:11;		//依绘制的不同图形含义不同，详见下表
//}interaction_figure_t;
///*
//类型 	details_a    details_b    details_c     details_d      details_e
//直线     -            -            -        终点 x 坐标    终点 y 坐标
//矩形 		 - 						- 					 - 				对角顶点 x 坐标 对角顶点 y 坐标
//正圆 		 - 						- 					半径					- 							-
//椭圆 		 - 						- 					 - 					x 半轴长度 		y 半轴长度
//圆弧 起始角度 		终止角度 				 -          x 半轴长度    y 半轴长度
//浮点数 字体大小    无作用 				 该值除以 1000 即实际显示值
//整型数 字体大小 		- 							32 位整型数，int32_t
//字符 字体大小 		字符长度 				 - 						- 							-

//角度值含义为：0°指 12 点钟方向，顺时针绘制；
//屏幕位置：（0,0）为屏幕左下角（1920，1080）为屏幕右上角；
//浮 点 数 ： 整 型 数 均 为 32 位 ， 对 于 浮 点 数 ， 实 际 显 示 的 值 为 输 入 的 值 /1000 ， 如 在
//details_c,details_d,details_e 对应的字节输入 1234，选手端实际显示的值将为 1.234。
//即使发送的数值超过对应数据类型的限制，图形仍有可能显示，但此时不保证显示的效果。
//*/
//typedef __packed struct//浮点数
//{                          
//	uint8_t graphic_name[3]; 
//	uint32_t operate_tpye:3; 
//	uint32_t graphic_tpye:3; 
//	uint32_t layer:4;        
//	uint32_t color:4;        
//	uint32_t start_angle:9;  
//	uint32_t end_angle:9;    
//	uint32_t width:10;       
//	uint32_t start_x:11;    
//	uint32_t start_y:11;     
//  float number;       
//} Float_data_struct_t;
//typedef __packed struct//整型数
//{                          
//	uint8_t graphic_name[3]; 
//	uint32_t operate_tpye:3; 
//	uint32_t graphic_tpye:3; 
//	uint32_t layer:4;        
//	uint32_t color:4;        
//	uint32_t start_angle:9;  
//	uint32_t end_angle:9;    
//	uint32_t width:10;       
//	uint32_t start_x:11;    
//	uint32_t start_y:11;     
//  int number;       
//} Int_data_struct_t;

///* ID:0x0100   Byte:  2     删除图层操作结构体*/
//typedef __packed struct
//{
//	uint8_t delete_type;			//0：空操作 1：删除图层 2：删除所有
//	uint8_t layer;						//图层数：0~9
//}interaction_layer_delete_t;

//typedef enum
//{
//	NONE_delete    = 0,
//	GRAPHIC_delete = 1,
//	ALL_delete     = 2
//}delete_Graphic_Operate;//ext_client_custom_graphic_delete_t：uint8_t operate_type
///*图层删除操作*/

////bit 0-2
//typedef enum
//{
//	NONE   = 0,/*空操作*/
//	ADD    = 1,/*增加图层*/
//	MODIFY = 2,/*修改图层*/
//	DELETE = 3,/*删除图层*/
//}Graphic_Operate;//graphic_data_struct_t：uint32_t operate_tpye
///*图层操作*/
////bit3-5
//typedef enum
//{
//	LINE      = 0,//直线
//	RECTANGLE = 1,//矩形
//	CIRCLE    = 2,//正圆
//	OVAL      = 3,//椭圆
//	ARC       = 4,//圆弧
//	FLOAT     = 5,//浮点数
//	INT       = 6,//整型数
//	CHAR      = 7 //字符
//}Graphic_Type;
///*图层类型*/
////bit 6-9图层数 最大为9，最小0
////bit 10-13颜色
//typedef enum
//{
//	RED_BLUE  = 0,//红蓝主色	
//	YELLOW    = 1,
//	GREEN     = 2,
//	ORANGE    = 3,
//	FUCHSIA   = 4,	/*紫红色*/
//	PINK      = 5,
//	CYAN_BLUE = 6,	/*青色*/
//	BLACK     = 7,
//	WHITE     = 8
//}Graphic_Color;
///*图层颜色类型*/
////bit 14-31 角度 [0,360]
///**********************************客户端绘图************************************************/
////删除图层
//typedef __packed struct
//{
//	frame_header_struct_t txFrameHeader;			
//	uint16_t  CmdID;										
//	robot_interaction_data_t   dataFrameHeader;
//	interaction_layer_delete_t clientData;		
//	uint16_t	FrameTail;								
//}ext_deleteLayer_data_t;

////绘字符串
//typedef __packed struct
//{
//	interaction_figure_t grapic_data_struct;
//	uint8_t data[30];
//} ext_client_string_t;


////固定数据段长度数据包
//typedef __packed struct
//{
//	frame_header_struct_t txFrameHeader;			//帧头
//	uint16_t  CmdID;										//命令码
//	robot_interaction_data_t   dataFrameHeader;//数据段头结构
//	ext_client_string_t clientData;//数据段
//	uint16_t	FrameTail;								//帧尾
//}ext_charstring_data_t;
////绘象形图
//typedef __packed struct
//{
//	frame_header_struct_t txFrameHeader;			//帧头
//	uint16_t  CmdID;										//命令码
//	robot_interaction_data_t   dataFrameHeader;//数据段头结构
//	ext_client_string_t clientData;		//数据段
//	uint16_t	FrameTail;								//帧尾
//}ext_graphic_one_data_t;
//typedef __packed struct
//{
//	frame_header_struct_t txFrameHeader;			
//	uint16_t  CmdID;										
//	robot_interaction_data_t   dataFrameHeader;
//	ext_client_string_t clientData[2];		
//	uint16_t	FrameTail;								
//}ext_graphic_two_data_t;
//typedef __packed struct
//{
//	frame_header_struct_t txFrameHeader;			
//	uint16_t  CmdID;										
//	robot_interaction_data_t   dataFrameHeader;
//	ext_client_string_t clientData[5];		
//	uint16_t	FrameTail;								
//}ext_graphic_five_data_t;
//typedef __packed struct
//{
//	frame_header_struct_t txFrameHeader;			
//	uint16_t  CmdID;										
//	robot_interaction_data_t   dataFrameHeader;
//	ext_client_string_t clientData[7];		
//	uint16_t	FrameTail;								
//}ext_graphic_seven_data_t;
////绘制浮点型
//typedef __packed struct
//{
//	frame_header_struct_t txFrameHeader;			
//	uint16_t  CmdID;										
//	robot_interaction_data_t   dataFrameHeader;
//	Float_data_struct_t clientData[2];		
//	uint16_t	FrameTail;								
//}ext_float_two_data_t;
//typedef __packed struct
//{
//	frame_header_struct_t txFrameHeader;			
//	uint16_t  CmdID;										
//	robot_interaction_data_t   dataFrameHeader;
//	Float_data_struct_t clientData[7];		
//	uint16_t	FrameTail;								
//}ext_float_seven_data_t;
////绘制整型
//typedef __packed struct
//{
//	frame_header_struct_t txFrameHeader;			
//	uint16_t  CmdID;										
//	robot_interaction_data_t   dataFrameHeader;
//	Int_data_struct_t clientData[2];		
//	uint16_t	FrameTail;								
//}ext_int_two_data_t;
//typedef __packed struct
//{
//	frame_header_struct_t txFrameHeader;			
//	uint16_t  CmdID;										
//	robot_interaction_data_t   dataFrameHeader;
//	Int_data_struct_t clientData[7];		
//	uint16_t	FrameTail;								
//}ext_int_seven_data_t;



//extern void init_referee_struct_data(void);
//extern void referee_data_solve(uint8_t *frame);

//extern void get_chassis_power_and_buffer(fp32 *power, fp32 *buffer);

//extern uint8_t get_robot_id(void);

//extern void get_shoot_heat1_limit_and_heat1(uint16_t *heat1_limit, uint16_t *heat1);
//extern void get_shoot_heat2_limit_and_heat2(uint16_t *heat2_limit, uint16_t *heat2);
//extern void get_shoot_heat42_limit_and_heat42(uint16_t *heat42_limit ,uint16_t *heat42);

//extern void get_chassis_power_limit(uint16_t *chassis_power_limit);
//extern uint16_t get_robot_level(void);

//extern void UI_send_init(void);
//extern void UI_send_update(void);
//extern void Determine_ID(void);
//#endif

