#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _objects_t {
    lv_obj_t *main_menu_screen;
    lv_obj_t *videos_select_screen;
    lv_obj_t* videos_player_screen;
    lv_obj_t *osd_menu;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *player;
} objects_t;

extern objects_t objects;

enum ScreensEnum {
    SCREEN_ID_MAIN_MENU_SCREEN = 1,
    SCREEN_ID_VIDEOS_SELECT_SCREEN = 2,
    SCREEN_ID_VIDEOS_PLAYER_SCREEN = 3,
};

void create_screen_main_menu_screen();
void tick_screen_main_menu_screen();

void create_screen_videos_select_screen();
void tick_screen_videos_select_screen();

void create_screen_videos_player_screen();
void tick_screen_videos_player_screen();

void create_user_widget_video_buttons(lv_obj_t *parent_obj, int startWidgetIndex);
void tick_user_widget_video_buttons(int startWidgetIndex);

void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();


#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/
