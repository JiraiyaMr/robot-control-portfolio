//
// Created by RM UI Designer
//

#include "ui_default_Ungroup_5.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 11
#define OBJ_NUM 2
#define FRAME_OBJ_NUM 2

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Ungroup_5;
ui_interface_round_t *ui_default_Ungroup_Round2 = (ui_interface_round_t *)&(ui_default_Ungroup_5.data[0]);
ui_interface_round_t *ui_default_Ungroup_Round3 = (ui_interface_round_t *)&(ui_default_Ungroup_5.data[1]);

void _ui_init_default_Ungroup_5() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_5.data[i].figure_name[0] = FRAME_ID;
        ui_default_Ungroup_5.data[i].figure_name[1] = GROUP_ID;
        ui_default_Ungroup_5.data[i].figure_name[2] = i + START_ID;
        ui_default_Ungroup_5.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Ungroup_5.data[i].operate_tpyel = 0;
    }

    ui_default_Ungroup_Round2->figure_tpye = 2;
    ui_default_Ungroup_Round2->layer = 0;
    ui_default_Ungroup_Round2->r = 15;
    ui_default_Ungroup_Round2->start_x = 390;
    ui_default_Ungroup_Round2->start_y = 603;
    ui_default_Ungroup_Round2->color = 3;
    ui_default_Ungroup_Round2->width = 10;

    ui_default_Ungroup_Round3->figure_tpye = 2;
    ui_default_Ungroup_Round3->layer = 0;
    ui_default_Ungroup_Round3->r = 15;
    ui_default_Ungroup_Round3->start_x = 390;
    ui_default_Ungroup_Round3->start_y = 545;
    ui_default_Ungroup_Round3->color = 5;
    ui_default_Ungroup_Round3->width = 10;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_5);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_5, sizeof(ui_default_Ungroup_5));
}

void _ui_update_default_Ungroup_5() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_5.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_5);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_5, sizeof(ui_default_Ungroup_5));
}

void _ui_remove_default_Ungroup_5() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_5.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_5);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_5, sizeof(ui_default_Ungroup_5));
}
