#ifndef APP_MAIN_H_  
#define APP_MAIN_H_

#include <stdio.h>
#include <string.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <esp_system.h>
#include <esp_event.h>
#include <esp_log.h>

#include <hap.h>
#include <hap_apple_servs.h>
#include <hap_apple_chars.h>
 
#include <hap_fw_upgrade.h>
#include <iot_button.h>

#include <app_wifi.h>
#include <app_hap_setup_payload.h>

#include <driver/gpio.h>

#include "dht22.h"
#include "light.h"

/* Reset network credentials if button is pressed for more than 3 seconds and then released */
#define RESET_NETWORK_BUTTON_TIMEOUT        3

#define RESET_PAIRING_BUTTON_TIMEOUT        6

/* Reset to factory if button is pressed and held for more than 10 seconds */
#define RESET_TO_FACTORY_BUTTON_TIMEOUT     10

/* The button "Boot" will be used as the Reset button for the example */
#define RESET_GPIO  GPIO_NUM_0

#define FAN_TASK_PRIORITY  1
#define FAN_TASK_STACKSIZE 4 * 1024
#define FAN_TASK_NAME      "hap_temp_hum"

char server_cert[] = {};

static const char   *TAG = "DHT22";
int level = 0;

// == function prototypes =======================================

void        Task(void *p);
void        Task2(void *p);

void init_Aaccessory(hap_acc_t *accessory, char accessory_name[], hap_cid_t cid);


static int  bridge_identify(hap_acc_t *ha);
static int  dht_identify(hap_acc_t *ha);
static int  createHum(hap_acc_t *accessory, hap_serv_t *service, char *accessory_name, hap_cid_t cid);
static int  hum_read(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv);
static int  temp_read(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv);
void        getTepmInMonitor(void);
static int  writeLight(hap_write_data_t write_data[], int count, void *serv_priv, void *write_priv);
static void reset_key_init(uint32_t key_gpio_pin);
static void reset_network_handler(void* arg);
static void reset_pairing_handler(void* arg);
static void reset_to_factory_handler(void* arg);
static int accessory_identify(hap_acc_t *ha);
static int led(hap_write_data_t write_data[], int count, void *serv_priv, void *write_priv);
void add_accessory_in_bridge(hap_acc_t *accessory, hap_cid_t cid, char accessory_name[], hap_acc_t *hap_serv_create, int (*write_cb)(hap_write_data_t, int));
#endif