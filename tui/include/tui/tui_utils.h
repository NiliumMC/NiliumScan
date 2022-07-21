/*****************************************************************************
 *
 *    tui/include/tui/tui_utils.h
 *    This file is part of TMCScan distribution released under GPL 3.0
 *    Copyright (C) 2022 _Magenta_
 *    https://github.com/0Magenta0/TMCScan
 *
 *****************************************************************************/

/* Enum Direction In Which Is Located Next TUI Element */
enum tui_direct {
    direct_up,
    direct_down,
    direct_left,
    direct_right
};

/* One TUI Button Structure */
struct tui_button {
    int y_pos, x_pos, attrs;
    const char *name;
    const int len,
              current_id,
              up_id,
              down_id,
              left_id,
              right_id;
};

/* One TUI TextField Structure */
struct tui_textfield {
    int y_pos, x_pos, attrs;
    const char *name;
    const int len_name,
              len_buf,
              max_buf_len;
    char *buf;
    int buf_size;
    const int current_id,
              up_id,
              down_id,
              left_id,
              right_id;
};

void print_min_size (const int, const int, const int, const int); /* Draw The Minimum Window Size If It Lower Of Defined Size */
void print_clear_win_at (const int, const int, const int, const int, const char *); /* Draw Empty Window On Certain Position */
void print_tui_buttons (const int, const struct tui_button *, const int); /* Draw Window's Buttons */
void print_tui_textfields (const int, const struct tui_textfield *, const int); /* Draw Window's TextFields */
int find_next_item (const int, const MEVENT *, int *, const struct tui_button *, const int, const struct tui_textfield *, const int); /* Get Next Window's Element */
void enter_to_textfield (const int, const int, struct tui_textfield *, const int); /* Handle Key When Some TextField Is Active */
void cursor_to_textfield (const int, const struct tui_textfield *, const int); /* Set Cursor Positon On Active TextField */
int check_entering_field_key (const int); /* Check If Pressed Key Valid For TextField */
int check_move_key (const int); /* Check If Pressed Key Is Move Bind */
int check_enter_key (const int); /* Check If Pressed Key Is Select Bind */
int check_mouse_click (const MEVENT *); /* Check If Mouse Click Is LMB/RMB */
int check_mouse_double_click (const MEVENT *); /* Check If Mouse Click Is Double LMB/RMB */
int check_mouse_pos_serv_list (const int, const int, const int, const MEVENT *); /* Check If Pressed Mouse Key At Servers List Position */
void sort_servers (const int); /* Sort Servers List */
int compare_serv_ip (const void *, const void *); /* Compare Two Servers IP Values*/
int compare_serv_ports (const void *, const void *); /* Compare Two Servers Port Values*/
int compare_serv_online (const void *, const void *); /* Compare Two Servers Online Values*/
int compare_serv_version (const void *, const void *); /* Compare Two Servers Version Values*/
int compare_serv_motd (const void *, const void *); /* Compare Two Servers MOTD Values*/

