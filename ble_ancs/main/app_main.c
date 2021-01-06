#include "app_main.h"

int enable = 0;

void app_main()
{
    gpio_set_direction(GPIO_NUM_2, GPIO_MODE_OUTPUT);
    setDHTgpio(23);
    printf("Starting DHT Task\n\n");
    xTaskCreate(Task, FAN_TASK_NAME, FAN_TASK_STACKSIZE, NULL, FAN_TASK_PRIORITY, NULL);
    xTaskCreate(Task2, FAN_TASK_NAME, FAN_TASK_STACKSIZE, NULL, 2, NULL);

}

void Task2(void *p)
{
    while (1)
    {
        printf("TASK2 -> \n");
        getTepmInMonitor();
        if (enable){
            gpio_set_level(GPIO_NUM_2, level);
        } else {
            gpio_set_level(GPIO_NUM_2, 0);
        } 
        level = !level;
        sys_delay_ms(2000);
    }
    vTaskDelete(NULL);
}

void Task(void *p)
{
    hap_acc_t *accessory;
    hap_serv_t *service;

    /* Initialize the HAP core */
    hap_init(HAP_TRANSPORT_WIFI);

    /* Initialise the mandatory parameters for Accessory which will be added as
     * the mandatory services internally
     */
    hap_acc_cfg_t cfg = {
        .name = "Esp-Bridge",
        .manufacturer = "Espressif",
        .model = "EspBridge01",
        .serial_num = "001122334455",
        .fw_rev = "0.9.0",
        .hw_rev = NULL,
        .pv = "1.1.0",
        .identify_routine = bridge_identify,
        .cid = HAP_CID_BRIDGE,
    };
    /* Create accessory object */
    accessory = hap_acc_create(&cfg);

    /* Add a dummy Product Data */
    uint8_t product_data[] = {'E','S','P','3','2','H','A','P'};
    hap_acc_add_product_data(accessory, product_data, sizeof(product_data));

    /* Add the Accessory to the HomeKit Database */
    hap_add_accessory(accessory);


    // char accessory_name[12] = "Accessory1";
    //     hap_acc_cfg_t bridge_cfg;// = {
    //         bridge_cfg.name = accessory_name,
    //         bridge_cfg.manufacturer = "Espressif",
    //         bridge_cfg.model = "EspFan01",
    //         bridge_cfg.serial_num = "abcdefg",
    //         bridge_cfg.fw_rev = "0.9.0",
    //         bridge_cfg.hw_rev = NULL,
    //         bridge_cfg.pv = "1.1.0",
    //         bridge_cfg.identify_routine = accessory_identify,
    //         bridge_cfg.cid = HAP_CID_SENSOR,
    //     // };
    //     accessory = hap_acc_create(&bridge_cfg);
    //     service = hap_serv_humidity_sensor_create(30.);
    //     hap_serv_add_char(service, hap_char_name_create(accessory_name));
    //     hap_serv_set_priv(service, strdup(accessory_name));
    //     hap_serv_set_read_cb(service, hum_read);
    //     hap_acc_add_serv(accessory, service);
    //     // init_Aaccessory(accessory, accessory_name, HAP_CID_SENSOR);

    //     /* Add the Accessory to the HomeKit Database */
    //     hap_add_bridged_accessory(accessory, hap_get_unique_aid(accessory_name));

    //     // accessory_name[12] = "Accessory2";
    //     memset(accessory_name, 0, 12);
    //     // strncpy(accessory_name, "Accessory2", 10);
    //     sprintf(accessory_name, "Accessory2");
    //     // bridge_cfg = {
    //         bridge_cfg.name = accessory_name,
    //         bridge_cfg.manufacturer = "Espressif",
    //         bridge_cfg.model = "EspFan01",
    //         bridge_cfg.serial_num = "abcdefg",
    //         bridge_cfg.fw_rev = "0.9.0",
    //         bridge_cfg.hw_rev = NULL,
    //         bridge_cfg.pv = "1.1.0",
    //         bridge_cfg.identify_routine = accessory_identify,
    //         bridge_cfg.cid = HAP_CID_SENSOR,
    //     // };
    //     accessory = hap_acc_create(&bridge_cfg);
    //     service = hap_serv_temperature_sensor_create(24);
    //     hap_serv_add_char(service, hap_char_name_create(accessory_name));
    //     hap_serv_set_priv(service, strdup(accessory_name));
    //     hap_serv_set_read_cb(service, temp_read);
    //     hap_acc_add_serv(accessory, service);

    //     hap_add_bridged_accessory(accessory, hap_get_unique_aid(accessory_name));


    //     memset(accessory_name, 0, 12);
    //     // strncpy(accessory_name, "Accessory2", 10);
    //     sprintf(accessory_name, "Accessory3");
    //     // bridge_cfg = {
    //         bridge_cfg.name = accessory_name,
    //         bridge_cfg.manufacturer = "Espressif",
    //         bridge_cfg.model = "EspFan01",
    //         bridge_cfg.serial_num = "abcdefg",
    //         bridge_cfg.fw_rev = "0.9.0",
    //         bridge_cfg.hw_rev = NULL,
    //         bridge_cfg.pv = "1.1.0",
    //         bridge_cfg.identify_routine = accessory_identify,
    //         bridge_cfg.cid = HAP_CID_SWITCH,
    //     // };
    //     accessory = hap_acc_create(&bridge_cfg);
    //     service = hap_serv_switch_create(false);
    //     hap_serv_add_char(service, hap_char_name_create(accessory_name));
    //     hap_serv_set_priv(service, strdup(accessory_name));
    //     // hap_serv_set_read_cb(service, temp_read);
    //     hap_serv_set_write_cb(service, led);
    //     hap_acc_add_serv(accessory, service);

    //     hap_add_bridged_accessory(accessory, hap_get_unique_aid(accessory_name));

    // add_accessory_in_bridge(accessory, HAP_CID_SENSOR, "Hum Sensor", hap_serv_humidity_sensor_create(20));
    // add_accessory_in_bridge(accessory, HAP_CID_SENSOR, "Term Sensor", hap_serv_temperature_sensor_create(20));
    add_accessory_in_bridge(accessory, HAP_CID_SWITCH, "Switch", hap_serv_switch_create(false), led);
    // add_accessory_in_bridge(accessory, HAP_CID_THERMOSTAT, "Termostat", hap_serv_thermostat_create(0, 0, 20.0, 25.0, 0), 0, 0);

    reset_key_init(RESET_GPIO);

#ifdef CONFIG_EXAMPLE_USE_HARDCODED_SETUP_CODE
    /* Unique Setup code of the format xxx-xx-xxx. Default: 111-22-333 */
    hap_set_setup_code(CONFIG_EXAMPLE_SETUP_CODE);
    /* Unique four character Setup Id. Default: ES32 */
    hap_set_setup_id(CONFIG_EXAMPLE_SETUP_ID);
#ifdef CONFIG_APP_WIFI_USE_WAC_PROVISIONING
    app_hap_setup_payload(CONFIG_EXAMPLE_SETUP_CODE, CONFIG_EXAMPLE_SETUP_ID, true, cfg.cid);
#else
    app_hap_setup_payload(CONFIG_EXAMPLE_SETUP_CODE, CONFIG_EXAMPLE_SETUP_ID, false, cfg.cid);
#endif
#endif

    hap_enable_mfi_auth(HAP_MFI_AUTH_HW);

    app_wifi_init();

    hap_start();

    app_wifi_start(portMAX_DELAY);
    vTaskDelete(NULL);
}

static int led(hap_write_data_t write_data[], int count, void *serv_priv, void *write_priv)
{
    if (hap_req_get_ctrl_id(write_priv)) {
        ESP_LOGI(TAG, "Received write from %s", hap_req_get_ctrl_id(write_priv));
    }
    ESP_LOGI(TAG, "Fan Write called with %d chars", count);
    int i, ret = HAP_SUCCESS;
    hap_write_data_t *write;
    for (i = 0; i < count; i++) {
        write = &write_data[i];
        if (!strcmp(hap_char_get_type_uuid(write->hc), HAP_CHAR_UUID_ON)) {
            ESP_LOGI(TAG, "Received Write. Fan %s", write->val.b ? "On" : "Off");
            if (write->val.b){
                enable = 1;
            } else {
                enable = 0;
            }
            
            hap_char_update_val(write->hc, &(write->val));
            *(write->status) = HAP_STATUS_SUCCESS;
        } else {
            *(write->status) = HAP_STATUS_RES_ABSENT;
        }
    }
    return ret;
}

static int bridge_identify(hap_acc_t *ha)
{
    ESP_LOGI(TAG, "Bridge identified");
    size_t status = 0;
    for (size_t i = 0; i < 10; i++){
        gpio_set_level(GPIO_NUM_2, status);
        status = !status;
        sys_delay_ms(200);
    }
    return HAP_SUCCESS;
}

static int dht_identify(hap_acc_t *ha)
{
    ESP_LOGI(TAG, "Accessory identified");
    return HAP_SUCCESS;
}

static int hum_read(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv)
{
    printf("hun.1!!!!\n");
    if (!strcmp(hap_char_get_type_uuid(hc), HAP_CHAR_UUID_CURRENT_RELATIVE_HUMIDITY))
    {
        ESP_LOGI(TAG, "Received Read for Current Hum");
        const hap_val_t *cur_val = hap_char_get_val(hc);
        ESP_LOGI(TAG, "Current Hum: %f", cur_val->f);
        printf("hum.2!!!!\n");
        hap_val_t new_val;
        new_val.f = getHumidity();
        ESP_LOGI(TAG, "Updated Hum: %f", new_val.f);
        hap_char_update_val(hc, &new_val);
        *status_code = HAP_STATUS_SUCCESS;
        return HAP_SUCCESS;
    }
    else
    {
        *status_code = HAP_STATUS_RES_ABSENT;
    }
    return HAP_FAIL;
}

static int temp_read(hap_char_t *hc, hap_status_t *status_code, void *serv_priv, void *read_priv)
{
    printf("temp.1!!!!\n");
    if (!strcmp(hap_char_get_type_uuid(hc), HAP_CHAR_UUID_CURRENT_TEMPERATURE))
    {
        ESP_LOGI(TAG, "Received Read for Current Hum");
        const hap_val_t *cur_val = hap_char_get_val(hc);
        ESP_LOGI(TAG, "Current Hum: %f", cur_val->f);
        printf("temp.2!!!!\n");
        hap_val_t new_val;
        new_val.f = getTemperature();
        ESP_LOGI(TAG, "Updated Hum: %f", new_val.f);
        hap_char_update_val(hc, &new_val);
        *status_code = HAP_STATUS_SUCCESS;
        return HAP_SUCCESS;
    }
    else
    {
        *status_code = HAP_STATUS_RES_ABSENT;
    }
    return HAP_FAIL;
}

void getTepmInMonitor(void)
{
    printf("=== Reading DHT ===\n");
    int ret = readDHT();
    errorHandler(ret);
    printf("Hum %.1f\n", getHumidity());
    printf("Tmp %.1f\n", getTemperature());
}

static void reset_key_init(uint32_t key_gpio_pin)
{
    button_handle_t handle = iot_button_create(key_gpio_pin, BUTTON_ACTIVE_LOW);
    iot_button_add_on_release_cb(handle, RESET_NETWORK_BUTTON_TIMEOUT, reset_network_handler, NULL);
    iot_button_add_on_release_cb(handle, RESET_PAIRING_BUTTON_TIMEOUT, reset_pairing_handler, NULL);
    iot_button_add_on_press_cb(handle, RESET_TO_FACTORY_BUTTON_TIMEOUT, reset_to_factory_handler, NULL);
}

static void reset_network_handler(void* arg)
{
    hap_reset_network();
}

static void reset_pairing_handler(void* arg)
{
    hap_reset_pairings();
}

static void reset_to_factory_handler(void* arg)
{
    hap_reset_to_factory();
}


static int accessory_identify(hap_acc_t *ha)
{
    hap_serv_t *hs = hap_acc_get_serv_by_uuid(ha, HAP_SERV_UUID_ACCESSORY_INFORMATION);
    hap_char_t *hc = hap_serv_get_char_by_uuid(hs, HAP_CHAR_UUID_NAME);
    const hap_val_t *val = hap_char_get_val(hc);
    char *name = val->s;

    ESP_LOGI(TAG, "Bridged Accessory %s identified", name);
    return HAP_SUCCESS;
}


void add_accessory_in_bridge(hap_acc_t *accessory, hap_cid_t cid, char accessory_name[], hap_acc_t *hap_serv_create, int (*write_cb)(hap_write_data_t, int)){
    hap_serv_t *service;
    hap_acc_cfg_t bridge_cfg = {
        .name = accessory_name,
        .manufacturer = "Espressif",
        .model = "EspFan01",
        .serial_num = "abcdefg",
        .fw_rev = "0.9.0",
        .hw_rev = NULL,
        .pv = "1.1.0",
        .identify_routine = accessory_identify,
        .cid = cid,
    };
    accessory = hap_acc_create(&bridge_cfg);
    service = hap_serv_create;
    hap_serv_add_char(service, hap_char_name_create(accessory_name));
    hap_serv_set_priv(service, strdup(accessory_name));
    hap_serv_set_write_cb(service, write_cb);
    // hap_serv_set_bulk_read_cb
    hap_acc_add_serv(accessory, service);
    hap_add_bridged_accessory(accessory, hap_get_unique_aid(accessory_name));
}