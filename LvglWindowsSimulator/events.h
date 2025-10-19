#pragma once
#include "ui/ui.h"

//extern void loadScreen(enum ScreensEnum screenId);

void action_open_videos_screen(lv_event_t* e);

void action_load_video_data(lv_event_t* e);

typedef struct{
    char* name;
    char* filePath;
    char* thumbnailPath;
    lv_obj_t* presetImage;
}sdCard_Video;
