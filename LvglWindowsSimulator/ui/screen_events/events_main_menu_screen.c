//#include "events_main_menu_screen.h"
#include "../actions.h"
#include "lvgl/lvgl.h"
#include "stdio.h"
#include "register_device.h"
#include <stdlib.h>    // For perror
#include "synchronize.h"

/* #include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // For close()*/

int is_connected() {
   /* int sock;
    struct sockaddr_in server_addr;

    // Create a socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80); // Port 80 for HTTP
    if (inet_pton(AF_INET, "8.8.8.8", &server_addr.sin_addr) <= 0) { // Google DNS IP
        perror("Invalid address/ Address not supported");
        close(sock);
        return 1;
    }

    // Attempt to connect
    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        printf("No internet connection detected.\n");
        close(sock);
        return 0; // Return 0 to indicate no connection
    }

    printf("Internet connection detected.\n");
    close(sock);*/  
    return 1; // Return 1 to indicate connection
}

void action_check_wifi_status(lv_event_t * e)
{
    lv_obj_t* parent_obj = lv_screen_active();

    if (is_connected())
    {
        {
            // Connect To Wifi
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 65 - 400, 9 - 240);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text(obj, "WiFi Connected");
        }
        {
            // Synchronize_Button
            lv_obj_t *obj = lv_btn_create(parent_obj);
            lv_obj_set_pos(obj, 565, 180);
            lv_obj_set_size(obj, 193, 96);
            lv_obj_add_event_cb(obj, run_sync_process, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "Synchronize");
                }
            }
        }
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
