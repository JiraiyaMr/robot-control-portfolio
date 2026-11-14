#ifndef REFEREE_H
#define REFEREE_H

#include "main.h"

#include "protocol.h"

enum judge_robot_ID{
	hero_red       = 1,
	engineer_red   = 2,
	infantry3_red  = 3,
	infantry4_red  = 4,
	infantry5_red  = 5,
	plane_red      = 6,
	
	hero_blue      = 101,
	engineer_blue  = 102,
	infantry3_blue = 103,
	infantry4_blue = 104,
	infantry5_blue = 105,
	plane_blue     = 106,
};
typedef enum
{
    PROGRESS_UNSTART        = 0,
    PROGRESS_PREPARE        = 1,
    PROGRESS_SELFCHECK      = 2,
    PROGRESS_5sCOUNTDOWN    = 3,
    PROGRESS_BATTLE         = 4,
    PROGRESS_CALCULATING    = 5,
} game_progress_t;
typedef __packed struct //0001
{
  uint8_t game_type : 4;					//比赛类型
	uint8_t game_progress : 4;			//当前比赛阶段
	uint16_t stage_remain_time;			//当前阶段剩余时间 单位s
	uint64_t SyncTimeStamp;
} ext_game_state_t;

typedef __packed struct //0002
{
    uint8_t winner;
} ext_game_result_t;
/* ID: 0x0003  Byte:  32    比赛机器人血量数据 */
typedef __packed struct
{
    uint16_t red_1_robot_HP;
    uint16_t red_2_robot_HP;
    uint16_t red_3_robot_HP;
    uint16_t red_4_robot_HP;
    uint16_t red_5_robot_HP;
    uint16_t red_7_robot_HP;
    uint16_t red_base_HP;
    uint16_t blue_1_robot_HP;
    uint16_t blue_2_robot_HP;
    uint16_t blue_3_robot_HP;
    uint16_t blue_4_robot_HP;
    uint16_t blue_5_robot_HP;
    uint16_t blue_7_robot_HP;
    uint16_t blue_base_HP;
} ext_game_robot_HP_t;
typedef __packed struct //0101
{
    uint32_t event_type;
} ext_event_data_t;

typedef __packed struct //0x0102
{
  uint8_t reserved;
	uint8_t supply_robot_id;
	uint8_t supply_projectile_step;
	uint8_t supply_projectile_num;
} ext_supply_projectile_action_t;

/* ID: 0x0104  Byte: 3   裁判系统警告信息 */
typedef __packed struct
{
  uint8_t level;													//己方最后一次受到判罚的等级
	uint8_t offending_robot_id;							//己方最后一次受到判罚的违规机器人 ID。（如红 1 机器人 ID 为 1，蓝1 机器人 ID 为 101）
	uint8_t count;													//己方最后一次受到判罚的违规机器人对应判罚等级的违规次数
} ext_referee_warning_t;
typedef __packed struct //0x0201
{
    uint8_t robot_id;
	uint8_t robot_level;
	uint16_t current_HP;
	uint16_t maximum_HP;
	uint16_t shooter_barrel_cooling_value;
	uint16_t shooter_barrel_heat_limit;
	uint16_t chassis_power_limit;
	uint8_t power_management_gimbal_output : 1;
	uint8_t power_management_chassis_output : 1;
	uint8_t power_management_shooter_output : 1;
} ext_game_robot_state_t;

typedef __packed struct //0x0202
{
  uint16_t chassis_voltage;   									//底盘输出电压 mV
	uint16_t chassis_current;    									//底盘输出电流 mA
	float chassis_power;   												//底盘输出功率 
	uint16_t buffer_energy;												//底盘缓冲功率剩余
	uint16_t shooter_17mm_1_barrel_heat;					//第 1 个 17mm 发射机构的枪口热量
	uint16_t shooter_17mm_2_barrel_heat;  				//第 2 个 17mm 发射机构的枪口热量
	uint16_t shooter_42mm_barrel_heat; 						//42mm枪口目前热量
} ext_power_heat_data_t;

typedef __packed struct //0x0203
{
	float x;												//本机器人位置 x 坐标，单位：m
	float y;												//本机器人位置 y 坐标，单位：m
	float angle;										//本机器人测速模块朝向，单位：度。正北为 0 度
} ext_game_robot_pos_t;

typedef __packed struct //0x0204
{
  uint8_t recovery_buff;					//机器人回血增益（百分比，值为 10 意为每秒回复 10%最大血量）
	uint8_t cooling_buff;						//机器人枪口冷却倍率（直接值，值为 5 意味着 5 倍冷却）
	uint8_t defence_buff;						//机器人防御增益（百分比，值为 50 意为 50%防御增益）
	uint16_t attack_buff;						//机器人攻击增益（百分比，值为 50 意为 50%攻击增益）
} ext_buff_musk_t;

typedef __packed struct //0x0205
{
  uint8_t airforce_status;				//空中机器人状态（0 为正在冷却，1 为冷却完毕，2 为空中支援期间）
	uint8_t time_remain;						//此状态的剩余时间
} aerial_robot_energy_t;

typedef __packed struct //0x0206
{
  uint8_t armor_id : 4;
	uint8_t HP_deduction_reason : 4;
} ext_robot_hurt_t;

typedef __packed struct //0x0207
{
  uint8_t bullet_type;						//弹丸类型
	uint8_t shooter_number;					//发射机构ID
	uint8_t launching_frequency;		//弹丸射频（单位：Hz）
	float initial_speed;						//弹丸初速度（单位：m/s）
} ext_shoot_data_t;
/* ID: 0x0208  Byte:  6    子弹数量数据 */
typedef __packed struct
{
  uint16_t projectile_allowance_17mm;			//17mm 弹丸允许发弹量
	uint16_t projectile_allowance_42mm;			//42mm 弹丸允许发弹量
	uint16_t remaining_gold_coin;						//剩余金币数量
} ext_bullet_remaining_t;
/* ID: 0x0209  Byte:  4    FRID状态 */
typedef __packed struct 
{ 
	uint32_t rfid_status ;												//RFID卡状态
}rfid_status_t;
typedef __packed struct //0x0301
{
    uint16_t send_ID;
    uint16_t receiver_ID;
    uint16_t data_cmd_id;
    uint16_t data_len;
    uint8_t *data;
} ext_student_interactive_data_t;

typedef __packed struct
{
    float data1;
    float data2;
    float data3;
    uint8_t data4;
} custom_data_t;


typedef __packed struct
{
    uint8_t data[64];
} ext_up_stream_data_t;

typedef __packed struct
{
    uint8_t data[32];
} ext_download_stream_data_t;



extern void init_referee_struct_data(void);
extern void referee_data_solve(uint8_t *frame);

extern void get_chassis_power_and_buffer(fp32 *power, fp32 *buffer);

extern uint8_t get_robot_id(void);
extern float get_chassis_power_limit();
extern void get_shoot_heat0_limit_and_heat0(uint16_t *heat0_limit, uint16_t *heat0);
extern void get_shoot_heat1_limit_and_heat1(uint16_t *heat1_limit, uint16_t *heat1);
#endif
