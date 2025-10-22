
#include "ui/ui.h"
#include "events.h"
#include <stdio.h>
#include <string.h>

sdCard_Video sd_cardVideos[4] = { "","","","" };

// Static Functions
void loadSdCard();

void action_open_videos_screen(lv_event_t* e)
{
    loadScreen(SCREEN_ID_VIDEOS_PLAYER_SCREEN);

}


void action_load_video_data(lv_event_t* e)
{
    lv_obj_t* parent_obj = lv_event_get_target(e);

    loadSdCard();

        // Video Container_0
    {
        lv_obj_t *obj = lv_obj_create(parent_obj);
        lv_obj_set_pos(obj, 0, 0);
        lv_obj_set_size(obj, 800, 480);
        lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        for (int i = 0; i < sizeof(sd_cardVideos) / sizeof(sdCard_Video); i++)
        {
            {
                lv_obj_t* parent_obj = obj;
                {
                    lv_obj_t* obj = lv_btn_create(parent_obj);
                    lv_obj_set_pos(obj, -1, i * 120);
                    lv_obj_set_size(obj, 800, 120);
                    lv_obj_add_event_cb(obj, action_open_videos_screen, LV_EVENT_PRESSED, (void*)0);
                    lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                    {
                        lv_obj_t* parent_obj = obj;
                        {
                            // Video_Thumbail
                            lv_obj_t* obj = lv_img_create(parent_obj);
                            lv_obj_set_pos(obj, 5, 4);
                            lv_obj_set_size(obj, 90, 90);
                            lv_image_set_src(obj, sd_cardVideos[i].thumbnailPath);
                        }
                            // Video_Name Label
                        {
                            lv_obj_t* obj = lv_label_create(parent_obj);
                            lv_obj_set_pos(obj, 0, 0);
                            lv_obj_set_size(obj, 504, LV_SIZE_CONTENT);
                            lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                            lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                            lv_obj_add_state(obj, LV_STATE_PRESSED);
                            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                            lv_obj_set_style_text_font(obj, &lv_font_montserrat_42, LV_PART_MAIN | LV_STATE_PRESSED);
                            lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_PRESSED);
                            lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_PRESSED);
                            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_PRESSED);
                            lv_label_set_text(obj, sd_cardVideos[i].name);
                        }
                    }
                }
            }
        }
    }
}
void loadSdCard()
{
    sd_cardVideos[0].name = "Video 1";

    lv_obj_t* img1 = lv_image_create(lv_screen_active());
    lv_image_set_src(img1, "C:/Users/rober/Downloads/Video.bin");
    //lv_img_set_src(img_obj, &new_image_asset);
    /*
    for (int i = 0; i < sd_cardVideo; i++)
    {
        // Video Container_0
        lv_obj_t* obj = lv_obj_create(parent_obj);
        objects.video_container_0 = obj;
        lv_obj_set_pos(obj, -2, -2);
        lv_obj_set_size(obj, 800, 120);
        lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE | LV_OBJ_FLAG_SCROLL_CHAIN_HOR | LV_OBJ_FLAG_SCROLL_CHAIN_VER | LV_OBJ_FLAG_SCROLL_ELASTIC);
        lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
        lv_obj_set_style_arc_rounded(obj, false, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_color(obj, lv_color_hex(0xffe0e0e0), LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_obj_set_style_border_width(obj, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
        {
            lv_obj_t* parent_obj = obj;
            {
                lv_obj_t* obj = lv_btn_create(parent_obj);
                lv_obj_set_pos(obj, -1, -1);
                lv_obj_set_size(obj, 800, 120);
                lv_obj_add_event_cb(obj, action_open_videos_screen, LV_EVENT_PRESSED, (void*)0);
                lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                {
                    lv_obj_t* parent_obj = obj;
                    {
                        // Video_Thumbail
                        lv_obj_t* obj = lv_img_create(parent_obj);
                        objects.video_thumbail = obj;
                        lv_obj_set_pos(obj, 5, 4);
                        lv_obj_set_size(obj, 90, 90);
                    }
                    {
                        lv_obj_t* obj = lv_label_create(parent_obj);
                        objects.obj1 = obj;
                        lv_obj_set_pos(obj, 0, 0);
                        lv_obj_set_size(obj, 504, LV_SIZE_CONTENT);
                        lv_label_set_long_mode(obj, LV_LABEL_LONG_DOT);
                        lv_obj_clear_flag(obj, LV_OBJ_FLAG_SCROLLABLE);
                        lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
                        lv_obj_add_state(obj, LV_STATE_PRESSED);
                        lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                        lv_obj_set_style_text_font(obj, &lv_font_montserrat_42, LV_PART_MAIN | LV_STATE_PRESSED);
                        lv_obj_set_style_text_color(obj, lv_color_hex(0xffffffff), LV_PART_MAIN | LV_STATE_PRESSED);
                        lv_obj_set_style_text_opa(obj, 255, LV_PART_MAIN | LV_STATE_PRESSED);
                        lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_PRESSED);
                        lv_label_set_text(obj, "");
                    }
                }
            }
        }
    }*/
}

//First Variable Getting Called, call SD 
const char* get_var_video_0_name()
{
   
}

void action_play_demo(lv_event_t* e)
{
    loadScreen(SCREEN_ID_VIDEOS_PLAYER_SCREEN);
}

bool is_video_finished = false;
char filePathBuffer[128];
static int current_video_index = 0;

const char* playlist[] = {
"video1.mp4",
"video2.mp4",
//"intro.mp4",
NULL // Sentinel value to indicate the end of the playlist
};


void action_play_video(lv_event_t* e)
{
    
    static lv_obj_t* player = lv_ffmpeg_player_create(lv_screen_active());
    sprintf(filePathBuffer, "%s%s", ".\\ui\\video\\", playlist[current_video_index]);
        
    lv_ffmpeg_player_set_src(player, filePathBuffer);
    lv_obj_add_event_cb(player, action_play_video, LV_EVENT_READY, (void*)0);

    // Play the current video
    lv_ffmpeg_player_set_cmd(player, LV_FFMPEG_PLAYER_CMD_START);
    lv_obj_center(player);

    // Move to the next video in the playlist
    current_video_index++;
    if (playlist[current_video_index] == NULL)
    {
        current_video_index = 0;
    }

}
void initializeSDCard()
{
   
}
