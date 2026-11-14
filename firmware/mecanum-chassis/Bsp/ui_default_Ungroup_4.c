//
// Created by RM UI Designer
//

#include "ui_default_Ungroup_4.h"

#define FRAME_ID 0
#define GROUP_ID 0
#define START_ID 4
#define OBJ_NUM 7
#define FRAME_OBJ_NUM 7

CAT(ui_, CAT(FRAME_OBJ_NUM, _frame_t)) ui_default_Ungroup_4;
ui_interface_line_t *ui_default_Ungroup_Line0 = (ui_interface_line_t *)&(ui_default_Ungroup_4.data[0]);
ui_interface_line_t *ui_default_Ungroup_Line1 = (ui_interface_line_t *)&(ui_default_Ungroup_4.data[1]);
ui_interface_line_t *ui_default_Ungroup_Line2 = (ui_interface_line_t *)&(ui_default_Ungroup_4.data[2]);
ui_interface_line_t *ui_default_Ungroup_Line3 = (ui_interface_line_t *)&(ui_default_Ungroup_4.data[3]);
ui_interface_round_t *ui_default_Ungroup_Round0 = (ui_interface_round_t *)&(ui_default_Ungroup_4.data[4]);
ui_interface_round_t *ui_default_Ungroup_Round1 = (ui_interface_round_t *)&(ui_default_Ungroup_4.data[5]);
ui_interface_round_t *ui_default_Ungroup_Round4 = (ui_interface_round_t *)&(ui_default_Ungroup_4.data[6]);

void _ui_init_default_Ungroup_4() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_4.data[i].figure_name[0] = FRAME_ID;
        ui_default_Ungroup_4.data[i].figure_name[1] = GROUP_ID;
        ui_default_Ungroup_4.data[i].figure_name[2] = i + START_ID;
        ui_default_Ungroup_4.data[i].operate_tpyel = 1;
    }
    for (int i = OBJ_NUM; i < FRAME_OBJ_NUM; i++) {
        ui_default_Ungroup_4.data[i].operate_tpyel = 0;
    }

    ui_default_Ungroup_Line0->figure_tpye = 0;
    ui_default_Ungroup_Line0->layer = 0;
    ui_default_Ungroup_Line0->start_x = 940;
    ui_default_Ungroup_Line0->start_y = 400;
    ui_default_Ungroup_Line0->end_x = 955;
    ui_default_Ungroup_Line0->end_y = 400;
    ui_default_Ungroup_Line0->color = 1;
    ui_default_Ungroup_Line0->width = 1;

    ui_default_Ungroup_Line1->figure_tpye = 0;
    ui_default_Ungroup_Line1->layer = 0;
    ui_default_Ungroup_Line1->start_x = 961;
    ui_default_Ungroup_Line1->start_y = 400;
    ui_default_Ungroup_Line1->end_x = 976;
    ui_default_Ungroup_Line1->end_y = 400;
    ui_default_Ungroup_Line1->color = 1;
    ui_default_Ungroup_Line1->width = 1;

    ui_default_Ungroup_Line2->figure_tpye = 0;
    ui_default_Ungroup_Line2->layer = 0;
    ui_default_Ungroup_Line2->start_x = 958;
    ui_default_Ungroup_Line2->start_y = 382;
    ui_default_Ungroup_Line2->end_x = 958;
    ui_default_Ungroup_Line2->end_y = 397;
    ui_default_Ungroup_Line2->color = 1;
    ui_default_Ungroup_Line2->width = 1;

    ui_default_Ungroup_Line3->figure_tpye = 0;
    ui_default_Ungroup_Line3->layer = 0;
    ui_default_Ungroup_Line3->start_x = 958;
    ui_default_Ungroup_Line3->start_y = 405;
    ui_default_Ungroup_Line3->end_x = 958;
    ui_default_Ungroup_Line3->end_y = 420;
    ui_default_Ungroup_Line3->color = 1;
    ui_default_Ungroup_Line3->width = 1;

    ui_default_Ungroup_Round0->figure_tpye = 2;
    ui_default_Ungroup_Round0->layer = 0;
    ui_default_Ungroup_Round0->r = 1;
    ui_default_Ungroup_Round0->start_x = 958;
    ui_default_Ungroup_Round0->start_y = 400;
    ui_default_Ungroup_Round0->color = 1;
    ui_default_Ungroup_Round0->width = 1;

    ui_default_Ungroup_Round1->figure_tpye = 2;
    ui_default_Ungroup_Round1->layer = 0;
    ui_default_Ungroup_Round1->r = 15;
    ui_default_Ungroup_Round1->start_x = 390;
    ui_default_Ungroup_Round1->start_y = 714;
    ui_default_Ungroup_Round1->color = 1;
    ui_default_Ungroup_Round1->width = 10;

    ui_default_Ungroup_Round4->figure_tpye = 2;
    ui_default_Ungroup_Round4->layer = 1;
    ui_default_Ungroup_Round4->r = 15;
    ui_default_Ungroup_Round4->start_x = 390;
    ui_default_Ungroup_Round4->start_y = 660;
    ui_default_Ungroup_Round4->color = 7;
    ui_default_Ungroup_Round4->width = 10;


    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_4);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_4, sizeof(ui_default_Ungroup_4));
}

void _ui_update_default_Ungroup_4() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_4.data[i].operate_tpyel = 2;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_4);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_4, sizeof(ui_default_Ungroup_4));
}

void _ui_remove_default_Ungroup_4() {
    for (int i = 0; i < OBJ_NUM; i++) {
        ui_default_Ungroup_4.data[i].operate_tpyel = 3;
    }

    CAT(ui_proc_, CAT(FRAME_OBJ_NUM, _frame))(&ui_default_Ungroup_4);
    SEND_MESSAGE((uint8_t *) &ui_default_Ungroup_4, sizeof(ui_default_Ungroup_4));
}
