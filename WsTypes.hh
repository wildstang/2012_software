#ifndef __WsTypes_hh__
#define __WsTypes_hh__

#include "vxWorks.h"

#define WS_FIRST_TARGET_BASKET_IDX WsTypes::WS_TOP_BASKET_INDEX
#define WS_LAST_TARGET_BASKET_IDX  WsTypes::WS_BASKET_COUNT - 1

class WsTypes
{
    public:
        typedef enum
        {
            WS_ESCALATOR_MOTOR_STATE_OFF = 0,
            WS_ESCALATOR_MOTOR_STATE_UP,
            WS_ESCALATOR_MOTOR_STATE_DOWN,
            WS_ESCALATOR_MOTOR_STATE_INVALID
        } WsEscalatorMotorStateType;
        
        typedef enum
        {
            WS_ELEVATOR_MOTOR_STATE_OFF = 0,
            WS_ELEVATOR_MOTOR_STATE_UP,
            WS_ELEVATOR_MOTOR_STATE_DOWN,
            WS_ELEVATOR_MOTOR_STATE_INVALID
        } WsElevatorMotorStateType;
        
        typedef enum
        {
            WS_ACCUMULATOR_MOTOR_STATE_OFF = 0,
            WS_ACCUMULATOR_MOTOR_STATE_PULLIN,
            WS_ACCUMULATOR_MOTOR_STATE_PUSHOUT,
            WS_ACCUMULATOR_MOTOR_STATE_INVALID
        } WsAccumulatorMotorStateType;
        
        typedef enum
        {
            WS_SHOOTER_CYLINDER_OPEN = 0,
            WS_SHOOTER_CYLINDER_CLOSED,
            WS_SHOOTER_CYLINDER_INVALID
        } WsShooterCylinderStateType;
        
        typedef enum
        {
            WS_HOOD_CYLINDER_UP = 0,
            WS_HOOD_CYLINDER_DOWN,
            WS_HOOD_CYLINDER_INVALID
        } WsHoodCylinderStateType;
        
        typedef enum
        {
            WS_POGO_STICK_CYLINDER_UP = 0,
            WS_POGO_STICK_CYLINDER_DOWN,
            WS_POGO_STICK_CYLINDER_INVALID
        } WsPogoStickCylinderStateType;
        
        typedef enum
        {
            WS_BRIDGE_BRAKE_CYLINDER_DISENGAGED = 0,
            WS_BRIDGE_BRAKE_CYLINDER_ENGAGED,
            WS_BRIDGE_BRAKE_CYLINDER_INVALID
        } WsBridgeBrakeCylinderStateType;
        
        typedef enum
        {
            WS_FLYWHEEL_OFF,
            WS_FLYWHEEL_ON,
            WS_FLYWHEEL_AUTO
        } WsFlywheelStateType;
        
        typedef enum
        {
            WS_COMPRESSOR_ON,
            WS_COMPRESSOR_OFF
        } WsCompressorStateType;
        
        typedef enum
        {
            WS_BASKET_INDEX_INVALID = -1,
            WS_TOP_BASKET_INDEX = 0,
            WS_LEFT_BASKET_INDEX,
            WS_RIGHT_BASKET_INDEX,
            WS_BOTTOM_BASKET_INDEX,
            WS_BASKET_COUNT
        } WsTargetBasketIdxType;
        
        typedef enum
        {
            WS_RANKING_CRITERIA_PARTICLE_AREA = 0,
            WS_RANKING_CRITERIA_COUNT
        } WsTargetRankingCriteriaType;
        
        typedef struct WsTargetImageInfoStruct
        {
            int img_width;
            int img_height;
            
            // Center of mass
            int cm_x;
            int cm_y;
            int target_area;
            float ratio_particle_area_to_image;
            float compactness;
            float quality;
            
            // Bounding Rectangle
            int br_left;
            int br_top;
            int br_width;
            int br_height;
            
            bool has_target;
        } WsTargetImageInfoType;
        
        typedef struct
        {
            float k_p;
            float k_i;
            float k_d;
            
            float error_epsilon;
            float static_epsilon;
            
            float max_integral;
            float error_increment; // Max increment to error sum each call
            float integral_error_thresh;
            float differentiator_band_limit;
            
            float max_output; // Ceiling on calculation output
            float min_output;
            float max_input; // Ceiling on calculation output
            float min_input;
            
            float min_on_target_time; // Minimum time in epsilon range to be done bool a_allowStaticEpsilon;
        } WsPidConfigurationDataT;
        
        typedef struct
        {
            float fender_distance;      // H-distance to the bottom target from the fender position
            float fender_fws;           // Ideal flywheel speed when shooting from the fender position
            float fender_speed;         // Ideal speed controller speed for desired flywheel rotatioal speed
            float key_bottom_distance;  // H-distance to the bottom target from the bottom of the key position
            float key_bottom_fws;       // Ideal flywheel speed when shooting from the bottom of the key position
            float key_bottom_speed;     // Ideal speed controller speed for desired flywheel rotatioal speed
            float key_middle_distance;  // H-distance to the bottom target from the middle of the key position
            float key_middle_fws;       // Ideal flywheel speed when shooting from the middle of the key position
            float key_middle_speed;     // Ideal speed controller speed for desired flywheel rotatioal speed
            float key_top_distance;     // H-distance to the bottom target from the top of the key position
            float key_top_fws;          // Ideal flywheel speed when shooting from the top of the key position
            float key_top_speed;        // Ideal speed controller speed for desired flywheel rotatioal speed
        } WsLocationSpeedMappingType;
        
        typedef struct
        {
            float shoot_state_prime_fly_1_time;
            float shoot_state_spin_fly_time_1;
            float shoot_state_shoot_1_time;
            float shoot_state_prime_fly_2_time;
            float shoot_state_spin_fly_time_2;
            float shoot_state_shoot_2_time;
            
        } WsAutoShootProgramConfigType;
        
        typedef enum
        {
            WS_AUTO_PROGRAM_SLEEPER = 0,
            WS_AUTO_PROGRAM_SHOOT_3,
            WS_AUTO_PROGRAM_SHOOT_6,
            WS_AUTO_PROGRAM_SHOOT_TIP,
            WS_AUTO_PROGRAM_TIP_SHOOT,
            WS_AUTO_PROGRAM_FENDER_RIGHT,
            WS_AUTO_PROGRAM_DRIVE,
            WS_AUTO_PROGRAM_FENDER_LEFT,
            WS_AUTO_PROGRAM_EMPTY,
            
            // Calibration Auto Programs
            WS_AUTO_PROGRAM_VICTOR_CALIBRATION = 13,
            WS_AUTO_PROGRAM_FLYWHEEL_LINEARIZER,
            WS_AUTO_PROGRAM_VICTOR_LINEARIZER,
            
            // Add Autonomous Program Id's above this line
            WS_AUTO_PROGRAM_NUM
        } WsAutonomousProgramIdType;
        
        typedef struct
        {
            int lumen_min_threshold;
            int lumen_max_threshold;
        } WsCameraProcessingImaqData;
        
        typedef struct
        {
            float elevator_motor_speed_off;
            float elevator_motor_speed_up;
            float elevator_motor_speed_down;
            float escalator_motor_speed_off;
            float escalator_motor_speed_up;
            float escalator_motor_speed_down;
            float accumulator_motor_speed_off;
            float accumulator_motor_speed_pullin;
            float accumulator_motor_speed_pushout;
        } WsBallTransportMotorValueType;
        
        typedef struct
        {
            INT32 ws_auto_tipshoot_coop_bridge_encoder_target; // Encoder Value
            float ws_auto_tipshoot_coop_bridge_throttle;
            float ws_auto_tipshoot_coop_bridge_heading;
            float ws_auto_tipshoot_coop_bridge_timeout;
            float ws_auto_tipshoot_wait_for_coast_timeout;
            float ws_auto_tipshoot_put_down_coop_bridge_timeout;
            INT32 ws_auto_tipshoot_drive_onto_coop_bridge_encoder_target; // Encoder Value
            float ws_auto_tipshoot_drive_onto_coop_bridge_throttle;
            float ws_auto_tipshoot_drive_onto_coop_bridge_heading;
            float ws_auto_tipshoot_drive_onto_coop_bridge_timeout;
            float ws_auto_tipshoot_accumulate_balls_timeout;
            INT32 ws_auto_tipshoot_shooting_position_encoder_target; // Encoder Value
            float ws_auto_tipshoot_shooting_position_throttle;
            float ws_auto_tipshoot_shooting_position_heading;
            float ws_auto_tipshoot_shooting_position_timeout;
            float ws_auto_tipshoot_wait_for_angle_timeout;
            float ws_auto_tipshoot_angle_between_facing_and_not_facing;
            float ws_auto_tipshoot_second_flywheel_setpoint;
            float ws_auto_tipshoot_wait_for_turret_timeout;
            float shoot_state_spin_fly_time_1;
            float shoot_state_shoot_1_time;
            float shoot_state_spin_fly_time_2;
            float shoot_state_shoot_2_time;
            float shoot_state_spin_fly_time_3;
            float shoot_state_shoot_3_time;
            float shoot_state_spin_fly_time_4;
            float shoot_state_shoot_4_time;
        } WsAutoTipShootProgramConfigType;
        
        typedef struct
        {
            INT32 ws_auto_drive_coop_bridge_encoder_target; // Encoder Value
            float ws_auto_drive_coop_bridge_throttle;
            float ws_auto_drive_coop_bridge_heading;
            float ws_auto_drive_coop_bridge_timeout;
            float ws_auto_drive_wait_for_coast_timeout;
            float ws_auto_drive_put_down_coop_bridge_timeout;
            INT32 ws_auto_drive_shooting_position_encoder_target; // Encoder Value
            float ws_auto_drive_shooting_position_throttle;
            float ws_auto_drive_shooting_position_heading;
            float ws_auto_drive_shooting_position_timeout;
            float ws_auto_drive_wait_for_angle_timeout;
            float ws_auto_drive_angle_between_facing_and_not_facing;
            float ws_auto_drive_second_flywheel_setpoint;
            float ws_auto_drive_wait_for_turret_timeout;
            float shoot_state_spin_fly_time_1;
            float shoot_state_shoot_1_time;
            float shoot_state_spin_fly_time_2;
            float shoot_state_shoot_2_time;
            float shoot_state_spin_fly_time_3;
            float shoot_state_shoot_3_time;
            
        } WsAutoDriveProgramConfigType;
        
        typedef struct
        {
            INT32 ws_auto_shoottip_coop_bridge_encoder_target; // Encoder Value
            float ws_auto_shoottip_coop_bridge_throttle;
            float ws_auto_shoottip_coop_bridge_heading;
            float ws_auto_shoottip_coop_bridge_timeout;
            float ws_auto_shoottip_wait_for_coast_timeout;
            float ws_auto_shoottip_put_down_coop_bridge_timeout;
            INT32 ws_auto_shoottip_drive_onto_coop_bridge_encoder_target; // Encoder Value
            float ws_auto_shoottip_drive_onto_coop_bridge_throttle;
            float ws_auto_shoottip_drive_onto_coop_bridge_heading;
            float ws_auto_shoottip_drive_onto_coop_bridge_timeout;
            float ws_auto_shoottip_accumulate_balls_timeout;
            INT32 ws_auto_shoottip_shooting_position_encoder_target; // Encoder Value
            float ws_auto_shoottip_shooting_position_throttle;
            float ws_auto_shoottip_shooting_position_heading;
            float ws_auto_shoottip_shooting_position_timeout;
            float shoot_state_spin_fly_time_1;
            float shoot_state_shoot_1_time;
            float shoot_state_spin_fly_time_2;
            float shoot_state_shoot_2_time;
            float shoot_state_spin_fly_time_3;
            float shoot_state_shoot_3_time;
            float shoot_state_spin_fly_time_4;
            float shoot_state_shoot_4_time;
            float ws_auto_shoottip_wait_for_angle_timeout;
            float ws_auto_shoottip_angle_between_facing_and_not_facing;
            float ws_auto_shoottip_second_flywheel_setpoint;
            float ws_auto_shoottip_wait_for_turret_timeout;
            
        } WsAutoShootTipProgramConfigType;
        
        typedef enum
        {
            WS_FLYWHEEL_CTRL_OFF = 0,
            WS_FLYWHEEL_CTRL_PRESET,
            WS_FLYWHEEL_CTRL_MANUAL
        } WsFlywheelControlType;
        
        typedef enum
        {
            WS_SHOOTER_PRESET_NONE = 0,
            WS_SHOOTER_PRESET_KEY_FORWARD,
            WS_SHOOTER_PRESET_KEY_BACK,
            WS_SHOOTER_PRESET_FENDER_LEFT,
            WS_SHOOTER_PRESET_FENDER_RIGHT,
            WS_SHOOTER_PRESET_AUTON
        } WsShooterPresetType;
        
        typedef struct
        {
            // Key Forward
            float turret_key_forward_voltage;
            float flywheel_key_forward_speed;
            float flywheel_key_forward_motor_val;
            
            // Key Backward
            float turret_key_reverse_voltage;
            float flywheel_key_reverse_speed;
            float flywheel_key_reverse_motor_val;
            
            // Fender Left
            float turret_fender_left_voltage;
            float flywheel_fender_left_speed;
            float flywheel_fender_left_motor_val;
            
            // Fender Right
            float turret_fender_right_voltage;
            float flywheel_fender_right_speed;
            float flywheel_fender_right_motor_val;
            
            // Generic Autonomous Use
            float turret_auton_voltage;
            float flywheel_auton_speed;
            float flywheel_auton_motor_val;
        } WsShooterPresetConfigT;
        
        typedef enum
        {
            WS_LG_RETRACTED,
            WS_LG_DEPLOYED
        } WsLandingGearStateType;
        
        typedef enum
        {
            WS_S_HIGHGEAR,
            WS_S_LOWGEAR
        } WsShifterStateType;
        
        typedef enum
        {
            WS_ROBOT_GRABBER_CYLINDER_RETRACTED,
            WS_ROBOT_GRABBER_CYLINDER_EXTENDED
        } WsRobotGrabberCylinderStateType;
        
        typedef enum
        {
            WS_ACCUMULATOR_EXTENDED,
            WS_ACCUMULATOR_RETRACTED
        } WsAccumulatorStateType;
        
        typedef struct
        {
            //WS_AUTO_FENDER_RIGHT_STATE_SPIN_FLY_AND_DRIVE,
            float ws_auto_fender_right_drive_fender_throttle;
            float ws_auto_fender_right_drive_fender_heading;
            INT32 ws_auto_fender_right_drive_fender_encoder_target;
            float ws_auto_fender_right_drive_fender_timeout;
            
            //WS_AUTO_FENDER_RIGHT_STATE_SQUARE_UP_ON_FENDER,
            float ws_auto_fender_right_square_up_throttle;
            float ws_auto_fender_right_square_up_timeout;
            
            //WS_AUTO_FENDER_RIGHT_STATE_WAIT_FOR_COAST,
            float ws_auto_fender_right_coast_timeout;
            
            //WS_AUTO_FENDER_RIGHT_STATE_SHOOT_FIRST_BALL,
            float ws_auto_fender_right_shoot_first_timeout;
            
            //WS_AUTO_FENDER_RIGHT_STATE_WAIT_FOR_FLYWHEEL,
            float ws_auto_fender_right_wait_for_flywheel_timeout;
            
            //WS_AUTO_FENDER_RIGHT_STATE_SHOOT_SECOND_BALL,
            float ws_auto_fender_right_shoot_second_timeout;
            
            //WS_AUTO_FENDER_RIGHT_STATE_BACK_UP,
            float ws_auto_fender_right_back_up_throttle;
            float ws_auto_fender_right_back_up_heading;
            INT32 ws_auto_fender_right_back_up_encoder_target;
            float ws_auto_fender_right_back_up_timeout;
            
            //WS_AUTO_FENDER_RIGHT_STATE_TURN_TO_ALLIANCE_BRIDGE,
            float ws_auto_fender_right_turn_throttle;
            float ws_auto_fender_right_turn_heading;
            INT32 ws_auto_fender_right_turn_encoder_target;
            float ws_auto_fender_right_turn_timeout;
            
            //WS_AUTO_FENDER_RIGHT_STATE_WAIT_FOR_TURN
            float ws_auto_fender_right_wait_for_turn_timeout;
            
            //WS_AUTO_FENDER_RIGHT_STATE_DRIVE_TO_ALLIANCE_BRIDGE,
            float ws_auto_fender_right_drive_alliance_throttle;
            float ws_auto_fender_right_drive_alliance_heading;
            INT32 ws_auto_fender_right_drive_alliance_encoder_target;
            float ws_auto_fender_right_drive_alliance_timeout;
            
            //WS_AUTO_FENDER_RIGHT_STATE_COAST_TO_ALLIANCE_BRIDGE,
            float ws_auto_fender_right_coast_alliance_timeout;
            
            //WS_AUTO_FENDER_RIGHT_STATE_DUMP_ALLIANCE_BRIDGE,
            float ws_auto_fender_right_dump_alliance_timeout;
        } WsAutoFenderRightProgramConfigType;
        
        typedef struct
        {
            //WS_AUTO_FENDER_LEFT_STATE_SPIN_FLY_AND_DRIVE,
            float ws_auto_fender_left_drive_fender_throttle;
            float ws_auto_fender_left_drive_fender_heading;
            INT32 ws_auto_fender_left_drive_fender_encoder_target;
            float ws_auto_fender_left_drive_fender_timeout;
            
            //WS_AUTO_FENDER_LEFT_STATE_SQUARE_UP_ON_FENDER,
            float ws_auto_fender_left_square_up_throttle;
            float ws_auto_fender_left_square_up_timeout;
            
            //WS_AUTO_FENDER_LEFT_STATE_WAIT_FOR_COAST,
            float ws_auto_fender_left_coast_timeout;
            
            //WS_AUTO_FENDER_LEFT_STATE_SHOOT_FIRST_BALL,
            float ws_auto_fender_left_shoot_first_timeout;
            
            //WS_AUTO_FENDER_LEFT_STATE_WAIT_FOR_FLYWHEEL,
            float ws_auto_fender_left_wait_for_flywheel_timeout;
            
            //WS_AUTO_FENDER_LEFT_STATE_SHOOT_SECOND_BALL,
            float ws_auto_fender_left_shoot_second_timeout;
            
            //WS_AUTO_FENDER_LEFT_STATE_BACK_UP,
            float ws_auto_fender_left_back_up_throttle;
            float ws_auto_fender_left_back_up_heading;
            INT32 ws_auto_fender_left_back_up_encoder_target;
            float ws_auto_fender_left_back_up_timeout;
            
            //WS_AUTO_FENDER_LEFT_STATE_TURN_TO_ALLIANCE_BRIDGE,
            float ws_auto_fender_left_turn_throttle;
            float ws_auto_fender_left_turn_heading;
            INT32 ws_auto_fender_left_turn_encoder_target;
            float ws_auto_fender_left_turn_timeout;
            
            //WS_AUTO_FENDER_LEFT_STATE_WAIT_FOR_TURN
            float ws_auto_fender_left_wait_for_turn_timeout;
            
            //WS_AUTO_FENDER_LEFT_STATE_DRIVE_TO_BARRIER,
            float ws_auto_fender_left_drive_barrier_throttle;
            float ws_auto_fender_left_drive_barrier_heading;
            INT32 ws_auto_fender_left_drive_barrier_encoder_target;
            float ws_auto_fender_left_drive_barrier_timeout;
            
            //WS_AUTO_FENDER_LEFT_STATE_COAST_TO_BARRIER,
            float ws_auto_fender_left_coast_barrier_timeout;
            
            
        } WsAutoFenderLeftProgramConfigType;
        
    private:
        virtual void classHasStaticAccessOnly(void) = 0;
};

#endif

//-------------------------------------------------------------------------
// END OF FILE
//-------------------------------------------------------------------------
