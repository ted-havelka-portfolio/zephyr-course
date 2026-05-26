// #include <nn-driver.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

#define NN_DRIVER_NODE DT_NODELABEL(nn_driver0)

#if DT_NODE_HAS_STATUS(NN_DRIVER_NODE, okay)
const struct device *driver = DEVICE_DT_GET(NN_DRIVER_NODE);
#warning "- DEV 0525 - nn_driver node found enabled, declaring a const struct device pointer variable . . ."
#else
#warning "- DEV 0525 - No enabled nn_driver node found!"
#endif



/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }
    return 0;
}
