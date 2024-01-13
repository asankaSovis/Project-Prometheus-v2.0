/*
        PROJECT PROMETHEUS v2.0
        Prometheus Fire Alarm system developed on nRF7002 platform.
        © 2024, Asanka Sovis
        v2.0.0
        Started: 01/11/2023
        Last Edited: 11/01/2024
*/

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/spi.h>
#include <zephyr/drivers/uart.h>
#include <stdio.h>
#include <string.h>
#include <zephyr/data/json.h>
#include <cJSON.h>

// Device Nodes
#define ENGAGE_NODE DT_NODELABEL(engage_device)         // Enable for level shifter
#define INDICATOR_LED_NODE DT_NODELABEL(indicator_led)  // Indicator LED
#define SOLENOID_NODE DT_NODELABEL(solenoid)            // Solenoid control
#define BUZZER_NODE DT_NODELABEL(buzzer)                // Buzzer
#define LED0_NODE DT_ALIAS(led0)                        // Built-in LED
#define SPI0_NODE DT_NODELABEL(mq135)                   // MQ-135 sensor
#define UART_DEVICE_NODE DT_CHOSEN(zephyr_shell_uart)   // Internal UART

#define CMD_SIZE 20                                     // UART _command size
#define MSG_SIZE 32                                     // UART message size
K_MSGQ_DEFINE(uart_msgq, MSG_SIZE+CMD_SIZE+2, 10, 4);   // UART message size definition

#define INITIALIZE_TIMEOUT 1                            // Timeout for initialization in sec

#define DEVICE_INFO "{\"name\":\"Fire Alarm model\",\"version\":\"v1.2\",\"id\":\"ZO5RtFTPNTBvs6QhrtnXeFCldG3vZgJu\"}"
                                                        // Device info of the device

// GPIO pin definition
static const struct gpio_dt_spec pin_engage = GPIO_DT_SPEC_GET(ENGAGE_NODE, gpios);     // Enable for level shifter
static const struct gpio_dt_spec pin_led = GPIO_DT_SPEC_GET(INDICATOR_LED_NODE, gpios); // Indicator LED
static const struct gpio_dt_spec pin_solenoid = GPIO_DT_SPEC_GET(SOLENOID_NODE, gpios); // Solenoid control
static const struct gpio_dt_spec pin_buzzer = GPIO_DT_SPEC_GET(BUZZER_NODE, gpios);     // Buzzer
static const struct gpio_dt_spec pin_builtin_led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);  // Built-in LED

// SPI configuration
const struct spi_config spi_cfg = {
    .frequency = DT_PROP(DT_NODELABEL(spi3), clock_frequency),
    .operation = SPI_OP_MODE_MASTER | SPI_TRANSFER_MSB | SPI_WORD_SET(8),
    .cs = {
        .gpio = {
            .port = DEVICE_DT_GET(DT_SPI_DEV_CS_GPIOS_CTLR(DT_NODELABEL(mq135))),
            .pin = DT_SPI_DEV_CS_GPIOS_PIN(DT_NODELABEL(mq135)),
            .dt_flags = DT_SPI_DEV_CS_GPIOS_FLAGS(DT_NODELABEL(mq135)),
        },
        .delay = 400,
    },
};

// Device configuration
const struct device *mq135_spi = DEVICE_DT_GET(DT_NODELABEL(spi3));                     // MQ-135
const struct device *dht11 = DEVICE_DT_GET_ONE(aosong_dht);                             // DHT-11
const struct device *uart_dev = DEVICE_DT_GET(UART_DEVICE_NODE);                        // Internal UART

// Variables ----------------------------------------------------------------------
// CO2, Temperature and Humidity levels recorded
double temperature = 0; double humidity = 0; int co2 = 0;

// Caps of CO2, Temperature and Humidity to turn on the alarm
int co2_cap = 1000; double temp_cap = 85; double humid_cap = 30;

bool warning = false;           // Indicate whether to turn on the warning
bool open_valve = false;         // Indicate whether to open the valve
bool override = false;          // Variable to override the warning
bool user_buzz = false;          // Variable to override and turn on the warning
bool user_valve = false;         // Variable to override and turn on the valve

// Function definitions -----------------------------------------------------------
// Utility
static const int now_str(void); // Prints the current time since execution

// DHT-11
int initialize_dht11();         // Initializes the DHT-11
int read_dht11() ;              // Reads the DHT-11
int print_temp_humid();         // Prints the temperature and humidity

// MQ-135
int initialize_mq135();         // Initializes the DHT-11
int read_mq135();               // Reads the DHT-11
int print_co2();                // Prints the CO2 level

// GPIO
int initialize_device(const struct gpio_dt_spec _device, gpio_flags_t _flags);
                                // Initialize GPIO

// UART
int initialize_uart();          // Initialize UART
void serial_cb(const struct device *_dev, void *_user_data);
                                // Serial read

// Device Specific
void initialize();              // Initialize device
void send_update(float _co2, float _temp, float _humid);
                                // Send status through UART
void compare(float _co2, float _temp, float _humid);
                                // Compare for anomalies
void buzz();                    // Buzz the buzzer
void control_valve();           // Open the valve
void indicator();               // Indicator run
void override_alarm(bool _value);// Override alarm
void custom_buzz(bool _value);  // Buzz by user
void custom_valve(bool _value); // Valve open by user
void get_caps();                // Sends the cap levels
void set_caps(char *_input);    // Set the cap levels
void command_recieved(char * _command, char * _data);
                                // Recieved a command

// Start of functions --------------------------------------------------------
int main(void)
{
        printk("\nPROMETHEUS FIRE ALARM SYSTEM v2.0\n");
        now_str();
        printk("Initializing hardware...\n");
        initialize();

        now_str();
        printk("Arming the device...\n");
        gpio_pin_set_dt(&pin_engage, 1);
        printk("Device armed. Sensors actively monitoring...\n");

        printk("initialize:\n");

	while (true) {
                if (false) {
                        if (read_dht11() == 0) {
                                print_temp_humid();
                        }

                        if (read_mq135() == 0) {
                                print_co2();
                        }
                }

                compare(co2, temperature, humidity);

                buzz();
                indicator();
                control_valve();
		
                if (false) {
                        printk("\n");
                }
                k_sleep(K_SECONDS(0.5));
	}

	return 0;
}


/* 
GENERAL FUNCTIONS -------------------------------------------------------------
 */

static const int now_str(void)
{
	uint32_t now = k_uptime_get_32();
	unsigned int ms = now % MSEC_PER_SEC;
	unsigned int s;
	unsigned int min;
	unsigned int h;

	now /= MSEC_PER_SEC;
	s = now % 60U;
	now /= 60U;
	min = now % 60U;
	now /= 60U;
	h = now;

	printk("[%u:%02u:%02u.%03u]: ",
		 h, min, s, ms);
	return 0;
}

// ---------------------------------------------------------------------------

/*
DHT 11 Functions -------------------------------------------------------------
*/

int print_temp_humid() {
        now_str();
        printk("%.1f Cel ; %.1f %%RH\n",
                temperature,
                humidity
                );
        return 0;
}

int read_dht11() {
        now_str();
        printk("Reading DHT11 data...\n");

        struct sensor_value sense_temperature;
        struct sensor_value sense_humidity;

        int error = sensor_sample_fetch(dht11);

        error += sensor_channel_get(dht11, SENSOR_CHAN_AMBIENT_TEMP,
                                &sense_temperature);
        error += sensor_channel_get(dht11, SENSOR_CHAN_HUMIDITY,
                                &sense_humidity);
        if (error != 0) {
                now_str();
                printk("Failed to read DHT11. Error ID: %d\n", error);

                return error;
        } else {
                temperature = sensor_value_to_double(&sense_temperature);
                humidity = sensor_value_to_double(&sense_humidity);

                now_str();
                printk("DHT11 successfully read.\n");
        }

        return 0;
}

int initialize_dht11() {
	if (!device_is_ready(dht11)) {
                now_str();
		printk("DHT11 is not ready.\n");

		return 1;
	}

        return 0;
}

// ---------------------------------------------------------------------------

/*
MQ 135 Functions -------------------------------------------------------------
*/

int print_co2() {
        now_str();
        printk("CO2 Percentage: %d", co2);
        return 0;
}

int read_mq135() {
        struct spi_buf rx_bufs = { &co2, 8 };
        struct spi_buf_set rx = { &rx_bufs, 1 };

        //gpio_pin_set_dt(&spi_cfg.cs.gpio, 0);

        int error = spi_read(mq135_spi, &spi_cfg, &rx);

        if (error != 0) {
                now_str();
                printk("Failed to read DHT11. Error ID: %d\n", error);

                return error;
        } else {
                now_str();
                printk("DHT11 successfully read.\n");
        }

        return 0;
}

int initialize_mq135() {
        if (!device_is_ready(mq135_spi)) {
                now_str();
                printk("MQ135 is not ready.\n");

                return 1;
        }

        return 0;
}

// ---------------------------------------------------------------------------

/*
IO DEVICE FUNCTIONS ---------------------------------------------
*/

int initialize_device(const struct gpio_dt_spec _device, gpio_flags_t _flags) {
	if(!device_is_ready(_device.port)) {
                now_str();
                printk("ERROR: %s could not be initialized. Device is not ready.\n", _device.port->name);

		return -1;
	}

        int ret = gpio_pin_configure_dt(&_device, _flags);
	if (ret) {
                now_str();
                printk("ERROR: %s could not be initialized. Pin configuration failed.\n", _device.port->name);

		return -2;
	}
        gpio_pin_set_dt(&_device, 0);

        return 0;
}

// ---------------------------------------------------------------------------

int initialize_uart() {
        if (!device_is_ready(uart_dev)) {
                now_str();
                printk("UART device is not ready.\n");

                return 1;
        }

        /* configure interrupt and callback to receive data */
        int ret = uart_irq_callback_user_data_set(uart_dev, serial_cb, NULL);

        if (ret < 0) {
                if (ret == -ENOTSUP) {
                        now_str();
                        printk("Interrupt-driven UART API support not enabled\n");
                } else if (ret == -ENOSYS) {
                        now_str();
                        printk("UART device does not support interrupt-driven API\n");
                } else {
                        now_str();
                        printk("Error setting UART callback: %d\n", ret);
                }

                return ret;
        } else {
                uart_irq_rx_enable(uart_dev);
        }

        return 0;
}



void initialize() {
        int error_flag = 0;

        do {
                error_flag += initialize_device(pin_engage, GPIO_OUTPUT);
                error_flag += initialize_device(pin_led, GPIO_OUTPUT);
                error_flag += initialize_device(pin_solenoid, GPIO_OUTPUT);
                error_flag += initialize_device(pin_buzzer, GPIO_OUTPUT);
                error_flag += initialize_device(pin_builtin_led, GPIO_OUTPUT);
                error_flag += initialize_dht11();
                error_flag += initialize_mq135();
                error_flag += initialize_uart();

                if (error_flag != 0) {
                        now_str();
                        printk("One or more devices could not be initialized. Will retry in %d seconds...\n", INITIALIZE_TIMEOUT);
                        k_sleep(K_SECONDS(INITIALIZE_TIMEOUT ));
                        error_flag = 0;
                } else {
                        now_str();
                        printk("Hardware initialized.\n");
                }
        } while (error_flag != 0);
}

/*
AAAA
*/

void send_update(float _co2, float _temp, float _humid) {
        // Serialized the sensor data as JSON and sends it to the serial port
        // Accepts _co2, _temp, _humid as float and return null

        now_str();
        printk("Sending current update. Encoding...\n");

        cJSON *update = cJSON_CreateObject();

        cJSON_AddItemToObject(update, "co2", cJSON_CreateNumber(_co2));
        cJSON_AddItemToObject(update, "temp", cJSON_CreateNumber(_temp));
        cJSON_AddItemToObject(update, "humid", cJSON_CreateNumber(_humid));

        if(override)
                cJSON_AddItemToObject(update, "override", cJSON_CreateTrue());
        else
                cJSON_AddItemToObject(update, "override", cJSON_CreateFalse());

        if(open_valve)
                cJSON_AddItemToObject(update, "open", cJSON_CreateTrue());
        else
                cJSON_AddItemToObject(update, "open", cJSON_CreateFalse());

        if(warning)
                cJSON_AddItemToObject(update, "warning", cJSON_CreateTrue());
        else
                cJSON_AddItemToObject(update, "warning", cJSON_CreateFalse());

        printk("update:%s\n", cJSON_PrintUnformatted(update));
}

void compare(float _co2, float _temp, float _humid) {
        // Function to check if the levels are high
        // Accepts _co2, _temp, _humid as float, return null
        // NEEDS TUNING

        warning = ((_co2 > co2_cap) || (_humid < humid_cap));
        open_valve = _temp > temp_cap;
}

void buzz() {
        // Function that controls the buzzer
        // Accepts null, returns null

        if (user_valve || (open_valve && !(override))) {
                gpio_pin_set_dt(&pin_buzzer, 1);
                k_sleep(K_SECONDS(0.5));
        } else if (user_buzz || (warning && !(override))) {
                gpio_pin_set_dt(&pin_buzzer, 1);
                k_sleep(K_SECONDS(0.5));
                gpio_pin_set_dt(&pin_buzzer, 0);
        } else {
                gpio_pin_set_dt(&pin_buzzer, 0);
                k_sleep(K_SECONDS(0.5));
        }
}

void control_valve() {
        // Function that controls the valve
        // Accepts null, returns null

        if (user_valve || (open_valve && !(override))) {
                gpio_pin_set_dt(&pin_solenoid, 0);
        } else {
                gpio_pin_set_dt(&pin_solenoid, 1);
        }
}

void indicator() {
        // Function that controls the indicator LED
        // Accepts null, returns null

        if (user_buzz || (warning && !(override))) {
                gpio_pin_set_dt(&pin_led, 1);
        } else if (user_valve || (open_valve && !(override))) {
                gpio_pin_set_dt(&pin_led, 1);
        } else {
                gpio_pin_set_dt(&pin_led, 1);
                k_sleep(K_SECONDS(0.1));
                gpio_pin_set_dt(&pin_led, 0);
        }

        k_sleep(K_SECONDS(0.5));
}

void override_alarm(bool _value) {
        override = _value;
}

void custom_buzz(bool _value) {
        user_buzz = _value;
}

void custom_valve(bool _value) {
        user_valve = _value;
}

void get_caps() {
        // Sends the cap values set for Temp, CO2 and Humidity
        // Accepts none and return null

        now_str();
        printk("Sending current cap values. Encoding...\n");

        cJSON *cap_results = cJSON_CreateObject();
        cJSON_AddItemToObject(cap_results, "co2", cJSON_CreateNumber((double)co2_cap));
        cJSON_AddItemToObject(cap_results, "humid", cJSON_CreateNumber(humid_cap));
        cJSON_AddItemToObject(cap_results, "temp", cJSON_CreateNumber(temp_cap));

        printk("get_caps:%s\n", cJSON_PrintUnformatted(cap_results));
}

void set_caps(char *_input) {
        // Sets the cap values set for Temp, CO2 and Humidity
        //Accepts input as String and return null

        now_str();
        printk("Recieved updated cap values. Decoding...\n");

        cJSON *cap_results = cJSON_Parse(_input);

        const cJSON *co2Cap_json = cJSON_GetObjectItemCaseSensitive(cap_results, "co2");
        const cJSON *humidCap_json = cJSON_GetObjectItemCaseSensitive(cap_results, "humid");
        const cJSON *tempCap_json = cJSON_GetObjectItemCaseSensitive(cap_results, "temp");

        co2_cap = cJSON_GetNumberValue(co2Cap_json);
        humid_cap = cJSON_GetNumberValue(humidCap_json);
        temp_cap = cJSON_GetNumberValue(tempCap_json);

        printk("Caps Updated: CO2 %dppm | Temperature %.1f Cel | Humidity %.1f %%RH\n", co2_cap, temp_cap, humid_cap);
}

void command_recieved(char * _command, char * _data) {
        // Function to parse recieved commands
        // Accepts _data as String and return null
        //
        
        if (strcmp(_command, "override") == 0) {
                override_alarm(strcmp(_data, "true") == 0);
        } else if (strcmp(_command, "userBuzz") == 0) {
                custom_buzz(strcmp(_data, "true") == 0);
        } else if (strcmp(_command, "userValve") == 0) {
                custom_valve(strcmp(_data, "true") == 0);
        } else if (strcmp(_command, "deviceInfo") == 0) {
                printk("deviceInfo:%s\n", DEVICE_INFO);
        } else if (strcmp(_command, "get_caps") == 0) {
                get_caps();
        } else if (strcmp(_command, "set_caps") == 0) {
                set_caps(_data);
        } else if (strcmp(_command, "update") == 0) {
                send_update(co2, temperature, humidity);
        } else {
                printk("%s:unknown _command\n", _command);
                return;
        }    

        printk("ack:%s\n", _command);
}

void serial_cb(const struct device *_dev, void *_user_data)
{
        /*
        Read characters from UART until line end is detected. Afterwards push the
        data to the message queue.
        */

        static char cmd_buf[CMD_SIZE];
        static char msg_buf[MSG_SIZE];

        static int rx_buf_pos;
        bool cmd_read = true;
        uint8_t c;

        if (!uart_irq_update(uart_dev)) {
                return;
        }

        if (!uart_irq_rx_ready(uart_dev)) {
                return;
        }

        while (uart_fifo_read(uart_dev, &c, 1) == 1) {
                if ((c == '\n' || c == '\r') && rx_buf_pos > 0) {
                        /* terminate string */
                        msg_buf[rx_buf_pos] = '\0';

                        /* if queue is full, message is silently dropped */
                        k_msgq_put(&uart_msgq, &msg_buf, K_NO_WAIT);

                        /* reset the buffer (it was copied to the msgq) */
                        rx_buf_pos = 0;
                }else if (c == ':') {
                        cmd_read = false;
                        cmd_buf[rx_buf_pos] = '\0';
                        rx_buf_pos = 0;
                } else if (rx_buf_pos < (sizeof(msg_buf) - 1)) {
                        if (cmd_read)
                                cmd_buf[rx_buf_pos++] = c;
                        else
                                msg_buf[rx_buf_pos++] = c;
                }
                /* else: characters beyond buffer size are dropped */
                printk(" \b");
        }

        printk("Command Recieved: %s\n", &cmd_buf[0]);

        command_recieved(&cmd_buf[0], &msg_buf[0]);

        memset(&cmd_buf, 0, sizeof cmd_buf - 1);
        memset(&msg_buf, 0, sizeof msg_buf - 1);
        rx_buf_pos = 0;
        cmd_read = true;
}
