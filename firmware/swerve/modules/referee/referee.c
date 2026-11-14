//#include "referee.h"
//#include "string.h"
//#include "stdio.h"
//#include "CRC8_CRC16.h"
//#include "protocol.h"

//frame_header_struct_t referee_receive_header;
//frame_header_struct_t referee_send_header;

//game_status_t 									GameState;						
//game_result_t 									GameResult;						
//game_robot_HP_t 								GameRobotHP;					

//event_data_t										EventData;						
//ext_supply_projectile_action_t	SupplyProjectileAction;
//referee_warning_t								RefereeWarning;				
//ext_dart_remaining_time_t				dart_remaining_time;	

//robot_status_t									GameRobotStat;				
//power_heat_data_t								PowerHeatData;				
//robot_pos_t											GameRobotPos;					
//buff_t													Buff;									
//air_support_data_t							AerialRobotEnergy;		
//hurt_data_t											RobotHurt;						
//shoot_data_t										ShootData;						
//projectile_allowance_t					bullet_remaining;			
//rfid_status_t										rfid_status;					
//dart_client_cmd_t           		dart_client;        	
//ground_robot_position_t         GroundRobotPosition;	
//radar_mark_data_t								RadarMarkData;				
//sentry_info_t										SentryInfo;						
//radar_info_t										RadarInfo;			

//robot_interaction_data_t        RobotIntereactionData;
//Referee_info_t 	REF;

//void init_referee_struct_data(void)//为各个结构体数据分配对应大小的空间
//{
//    memset(&referee_receive_header, 0, sizeof(frame_header_struct_t));
//    memset(&referee_send_header, 0, sizeof(frame_header_struct_t));

//    memset(&GameState, 0, sizeof(game_status_t));
//    memset(&GameResult, 0, sizeof(game_result_t));
//    memset(&GameRobotHP, 0, sizeof(game_robot_HP_t));


//    memset(&EventData, 0, sizeof(event_data_t));
//    memset(&SupplyProjectileAction, 0, sizeof(ext_supply_projectile_action_t));
//    memset(&RefereeWarning, 0, sizeof(referee_warning_t));
//		memset(&dart_remaining_time, 0, sizeof(ext_dart_remaining_time_t));


//    memset(&GameRobotStat, 0, sizeof(robot_status_t));
//    memset(&PowerHeatData, 0, sizeof(power_heat_data_t));
//		memset(&GameRobotPos, 0, sizeof(robot_pos_t));
//    memset(&Buff, 0, sizeof(buff_t));
//		memset(&AerialRobotEnergy, 0, sizeof(air_support_data_t));
//    memset(&RobotHurt, 0, sizeof(hurt_data_t));
//		memset(&ShootData, 0, sizeof(shoot_data_t));
//    memset(&bullet_remaining, 0, sizeof(projectile_allowance_t));
//		memset(&rfid_status, 0, sizeof(rfid_status_t));
//    memset(&dart_client, 0, sizeof(dart_client_cmd_t));
//		memset(&GroundRobotPosition, 0, sizeof(ground_robot_position_t));
//    memset(&RadarMarkData, 0, sizeof(radar_mark_data_t));
//		memset(&SentryInfo, 0, sizeof(sentry_info_t));
//    memset(&RadarInfo, 0, sizeof(radar_info_t));
//		
//    memset(&RobotIntereactionData, 0, sizeof(robot_interaction_data_t));
//}

//void referee_data_solve(uint8_t *frame)
//{
//    uint16_t cmd_id = 0;

//    uint8_t index = 0;

//    memcpy(&referee_receive_header, frame, sizeof(frame_header_struct_t));

//    index += sizeof(frame_header_struct_t);

//    memcpy(&cmd_id, frame + index, sizeof(uint16_t));
//    index += sizeof(uint16_t);

//    switch (cmd_id)
//    {
//				case ID_game_state:
//						{
//								memcpy(&GameState, frame + index, sizeof(game_status_t));
//						}
//						break;
//				case ID_game_result:
//						memcpy(&GameResult, frame + index, sizeof(game_result_t));
//						break;

//				case ID_robot_health_status:
//						memcpy(&GameRobotHP, frame + index, sizeof(game_robot_HP_t));
//						break;

//				case ID_event_data:
//						memcpy(&EventData, frame + index, sizeof(event_data_t));
//						break;

//				case ID_supply_station_action:
//						memcpy(&SupplyProjectileAction, frame + index, sizeof(ext_supply_projectile_action_t));
//						break;

//				case ID_supply_warn:
//						memcpy(&RefereeWarning, frame + index, sizeof(referee_warning_t));
//						break;

//				case ID_missile_shoot_time:
//						memcpy(&dart_remaining_time, frame + index, sizeof(ext_dart_remaining_time_t));
//						break;

//				case ID_game_robot_state:
//						memcpy(&GameRobotStat, frame + index, sizeof(robot_status_t));
//						break;

//				case ID_power_heat_data:
//						memcpy(&PowerHeatData, frame + index, sizeof(power_heat_data_t));
//						break;

//				case ID_game_robot_pos:
//						memcpy(&GameRobotPos, frame + index, sizeof(robot_pos_t));
//						break;

//				case ID_buff_musk:
//						memcpy(&Buff, frame + index, sizeof(buff_t));
//						break;

//				case ID_aerial_support_time:
//						memcpy(&AerialRobotEnergy, frame + index, sizeof(air_support_data_t));
//						break;

//				case ID_robot_hurt:
//						memcpy(&RobotHurt, frame + index, sizeof(hurt_data_t));
//						break;

//				case ID_shoot_data:
//						memcpy(&ShootData, frame + index, sizeof(shoot_data_t));
//						break;

//				case ID_bullet_remaining:
//						memcpy(&bullet_remaining, frame + index, sizeof(projectile_allowance_t));
//						break;

//				case ID_rfid_status:
//						memcpy(&rfid_status, frame + index, sizeof(rfid_status_t));
//						break;

//				case ID_dart_player_command:
//						memcpy(&dart_client, frame + index, sizeof(dart_client_cmd_t));
//						break;

//				case ID_ground_robot_position:
//						memcpy(&GroundRobotPosition, frame + index, sizeof(ground_robot_position_t));
//						break;

//				case ID_radar_marking_progress:
//						memcpy(&RadarMarkData, frame + index, sizeof(radar_mark_data_t));
//						break;

//				case ID_sentinel_autonomous_decision_sync:
//						memcpy(&SentryInfo, frame + index, sizeof(sentry_info_t));
//						break;

//				case ID_radar_autonomous_decision_sync:
//						memcpy(&RadarInfo, frame + index, sizeof(radar_info_t));
//						break;

//				case ID_robot_interactive_header_data:
//						memcpy(&RobotIntereactionData, frame + index, sizeof(robot_interaction_data_t));
//						break;

//        break;
//        default:
//        {
//            break;
//        }
//    }
//}

//void UI_send_init()//用于初次绘制UI
//{
//	Determine_ID();
//}

//void UI_send_update()//实时更新UI画面
//{
//	
//}

//void get_chassis_power_and_buffer(fp32 *power, fp32 *buffer)
//{
//    *power = PowerHeatData.chassis_power;
//    *buffer = PowerHeatData.buffer_energy;
//}

//uint8_t get_robot_id(void)
//{
//    return GameRobotStat.robot_id;
//}

//void get_shoot_heat1_limit_and_heat1(uint16_t *heat1_limit, uint16_t *heat1)
//{
//		uint8_t  robot_level = 0;
//		robot_level = get_robot_level();
//	
//	  if(Shoot_Attributes==0)//冷却优先
//		{
//			if(robot_level<10)
//				*heat1_limit = 15+35*robot_level;
//			if(robot_level==10)
//				*heat1_limit = 400;
//		}
//		if(Shoot_Attributes==1)//爆发优先
//		{
//				*heat1_limit = 150+50*robot_level;
//		}
//   
//    *heat1 = PowerHeatData.shooter_17mm_1_barrel_heat;
//}

//void get_shoot_heat2_limit_and_heat2(uint16_t *heat2_limit, uint16_t *heat2)
//{
//    uint8_t  robot_level = 0;
//		robot_level = get_robot_level();
//	
//	  if(Shoot_Attributes==0)//冷却优先
//		{
//			if(robot_level<10)
//				*heat2_limit = 15+35*robot_level;
//			if(robot_level==10)
//				*heat2_limit = 400;
//		}
//		if(Shoot_Attributes==1)//爆发优先
//		{
//				*heat2_limit = 150+50*robot_level;
//		}
//   
//    *heat2 = PowerHeatData.shooter_17mm_2_barrel_heat;
//}

//void get_shoot_heat42_limit_and_heat42(uint16_t *heat42_limit,uint16_t *heat42)
//{
//		 uint8_t  robot_level = 0;
//		 robot_level = get_robot_level();
//	
//		 if(robot_level<8)
//			 *heat42_limit = 170+30*robot_level;
//		 if(robot_level==8)
//			 *heat42_limit = 420;
//		 if(robot_level>8)
//			 *heat42_limit = 50*robot_level;
//		
//	  *heat42=PowerHeatData.shooter_42mm_barrel_heat;
//}

//uint16_t get_robot_level(void)
//{
//	uint16_t robot_exp   = 0;
//	uint8_t  robot_level = 0;
//	
//	robot_exp=GameRobotStat.current_exp;
//	
//	if(robot_exp<=2000)
//	{
//		robot_level=robot_exp%250+1;
//	}
//	if(robot_exp>=2500)
//	{
//		robot_level=10;
//	}
//	return robot_level;
//}

//void get_chassis_power_limit(uint16_t *chassis_power_limit)
//{
//	  uint16_t robot_id = 0;
//		uint8_t  robot_level = 0;
//		
//	
//		robot_id = get_robot_id();
//		robot_level = get_robot_level();
//	
//	if(robot_id ==hero_red||robot_id ==hero_blue)//英雄
//	{
//		if(Chassis_Attributes==0)//血量优先
//		{
//			if(robot_level<9)
//				*chassis_power_limit=50+5*robot_level;	
//			if(robot_level==9)
//				*chassis_power_limit=100;
//			if(robot_level==10)
//				*chassis_power_limit=120;
//		}
//		if(Chassis_Attributes==1)//功率优先
//		{
//			if(robot_level<10)
//				*chassis_power_limit=65+5*robot_level;	
//			if(robot_level==10)
//				*chassis_power_limit=120;
//		}
//	}
//	if(robot_id ==infantry3_red||robot_id ==infantry4_red||robot_id ==infantry5_red||robot_id ==infantry3_blue||robot_id ==infantry4_blue||robot_id ==infantry5_blue)//步兵
//	{
//		if(Chassis_Attributes==0)//血量优先
//		{
//			if(robot_level<8)
//				*chassis_power_limit=40+5*robot_level;	
//			if(robot_level>=8)
//				*chassis_power_limit=10*robot_level;
//		}
//		else										//功率优先
//		{
//			if(robot_level<10)
//				*chassis_power_limit=55+5*robot_level;	
//			if(robot_level==10)
//				*chassis_power_limit=100;
//		}
//	}
//}


//void Determine_ID(void)//判断自己是哪个队伍
//{
//	if(REF.GameRobotStat.robot_id < 10)//本机器人的ID，红方
//	{ 
//		REF.ids.teammate_hero 		 	= 1;
//		REF.ids.teammate_engineer  = 2;
//		REF.ids.teammate_infantry3 = 3;
//		REF.ids.teammate_infantry4 = 4;
//		REF.ids.teammate_infantry5 = 5;
//		REF.ids.teammate_plane		 	= 6;
//		
//		
//		REF.ids.client_hero 		 	= 0x0101;
//		REF.ids.client_engineer  = 0x0102;
//		REF.ids.client_infantry3 = 0x0103;
//		REF.ids.client_infantry4 = 0x0104;
//		REF.ids.client_infantry5 = 0x0105;
//		REF.ids.client_plane			= 0x0106;
//		
//		if     (REF.GameRobotStat.robot_id == hero_red)//不断刷新放置在比赛中更改颜色
//			REF.self_client = REF.ids.client_hero;
//		else if(REF.GameRobotStat.robot_id == engineer_red)
//			REF.self_client = REF.ids.client_engineer;
//		else if(REF.GameRobotStat.robot_id == infantry3_red)
//			REF.self_client = REF.ids.client_infantry3;
//		else if(REF.GameRobotStat.robot_id == infantry4_red)
//			REF.self_client = REF.ids.client_infantry4;
//		else if(REF.GameRobotStat.robot_id == infantry5_red)
//			REF.self_client = REF.ids.client_infantry5;
//		else if(REF.GameRobotStat.robot_id == plane_red)
//			REF.self_client = REF.ids.client_plane;
//	}
//	else //蓝方
//	{
//		REF.ids.teammate_hero 		 	= 101;
//		REF.ids.teammate_engineer  = 102;
//		REF.ids.teammate_infantry3 = 103;
//		REF.ids.teammate_infantry4 = 104;
//		REF.ids.teammate_infantry5 = 105;
//		REF.ids.teammate_plane		 	= 106;
//		
//		REF.ids.client_hero 		 	= 0x0165;
//		REF.ids.client_engineer  = 0x0166;
//		REF.ids.client_infantry3 = 0x0167;
//		REF.ids.client_infantry4 = 0x0168;
//		REF.ids.client_infantry5 = 0x0169;
//		REF.ids.client_plane			= 0x016A;
//		
//		if     (REF.GameRobotStat.robot_id == hero_blue)
//			REF.self_client = REF.ids.client_hero;
//		else if(REF.GameRobotStat.robot_id == engineer_blue)
//			REF.self_client = REF.ids.client_engineer;
//		else if(REF.GameRobotStat.robot_id == infantry3_blue)
//			REF.self_client = REF.ids.client_infantry3;
//		else if(REF.GameRobotStat.robot_id == infantry4_blue)
//			REF.self_client = REF.ids.client_infantry4;
//		else if(REF.GameRobotStat.robot_id == infantry5_blue)
//			REF.self_client = REF.ids.client_infantry5;
//		else if(REF.GameRobotStat.robot_id == plane_blue)
//			REF.self_client = REF.ids.client_plane;
//		
//	}

//}


