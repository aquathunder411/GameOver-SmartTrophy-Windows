#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <lvgl/lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_open_videos_screen(lv_event_t * e);
extern void action_load_video_data(lv_event_t * e);
extern void action_open_osd(lv_event_t * e);
extern void action_play_demo(lv_event_t * e);
extern void action_play_video(lv_event_t * e);

extern void action_osd_play(lv_event_t * e);
extern void action_osd_pause(lv_event_t * e);
extern void action_osd_stop(lv_event_t * e);

extern void action_check_wifi_status(lv_event_t * e);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/
