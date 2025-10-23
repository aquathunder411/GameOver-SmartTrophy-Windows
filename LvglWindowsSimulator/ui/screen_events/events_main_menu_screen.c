//#include "events_main_menu_screen.h"
#include "../actions.h"
#include "lvgl/lvgl.h"
#include "stdio.h"

bool is_connected()
{
    // Platform-specific implementation to check WiFi connection
    // This is a placeholder; actual implementation will vary

    #if defined(_WIN32) //Windows
        printf("Compiling for Windows.\n");
        // Assuming this is the Simulator, so return is different for testing
        return 0;
    #elif defined(__linux__)
        printf("Compiling for Linux.\n");
    #elif defined(__APPLE__)
        printf("Compiling for macOS.\n");
    #else
        printf("Compiling for an unknown or unsupported platform.\n");
    #endif
        return 0;
}

void action_check_wifi_status(lv_event_t * e)
{
    lv_obj_t* parent_obj = lv_screen_active();

    if (is_connected())
    {
       // Connect To Wifi
        lv_obj_t *obj = lv_label_create(parent_obj);
        lv_obj_t *connect_to_wifi = obj;
        lv_obj_set_pos(obj, 65 - 400, 9 - 240);
        lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
        lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
        lv_label_set_text(obj, "WiFi Connected");
    }
    else
    {
        lv_obj_t *obj = lv_btn_create(parent_obj);
        lv_obj_set_pos(obj, 19, 5);
        lv_obj_set_size(obj, 145, 66);
        {
            lv_obj_t *parent_obj = obj;
            {
                // Connect To Wifi
                lv_obj_t *obj = lv_label_create(parent_obj);
                lv_obj_t *connect_to_wifi = obj;
                lv_obj_set_pos(obj, 0, 0);
                lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                lv_label_set_text(obj, "Connect To Wifi");
            }
        }
    }
}
