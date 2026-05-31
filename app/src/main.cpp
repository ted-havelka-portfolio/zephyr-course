/*
 * SPDX-License-Identifier: MIT
 *
 * file main.cpp for course work of 2026 Iomico Zephyr Course
 */

#include <../../../deps/modules/lib/nn-custom-driver/include/nn-custom-driver.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#define NN_DRIVER_NODE DT_NODELABEL(nn_driver0)

#define BASE_10 10

#if DT_NODE_HAS_STATUS(NN_DRIVER_NODE, okay)
static const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(nn_driver0));
#else
#warning "- DEV 0525 - No enabled nn_driver node found"
#endif

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

namespace {
	void test_channel_get(void) {
		int32_t rc = 0;
		struct sensor_value val;
		// Note following call is to custom driver, not DEVICE_API() based one:
		rc = channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val, &led);
	};

	void test_sample_fetch(void) {
		int32_t rc = 0;
		// Note following call is to custom driver, not DEVICE_API() based one:
		rc = sample_fetch(driver, SENSOR_CHAN_AMBIENT_TEMP, &led);
	};
};

int32_t wrapper_show_settings(void)
{
    return (int32_t)show_settings(driver);
}

int32_t wrapper_set_id(const uint32_t id)
{
    return (uint32_t)set_id(driver, id);
}

static int cmd_driver_show_settings(const struct shell *shell, size_t argc, char *argv[])
{
	return (uint32_t)wrapper_show_settings();
}

static int cmd_driver_set_id(const struct shell *shell, size_t argc, char *argv[])
{
        uint32_t value = 0;
        char *endptr, *str;
        int32_t rc = 0;

	str = argv[1];
	value = strtol(str, &endptr, BASE_10);

	// TOOD [ ] Sanity check outcome of `strtol()`.
        shell_fprintf(shell, SHELL_NORMAL, "setting driver id to %u\n", value);

        rc = (uint32_t)wrapper_set_id(value);
	if (rc != 0)
	{
		LOG_ERR("Failed to show custom driver settings, error %d", rc);
	}

	return rc;
}

SHELL_STATIC_SUBCMD_SET_CREATE(
        cmds_custom_driver,
        SHELL_CMD_ARG(show_settings, NULL, "show nn custom driver run time data",
                cmd_driver_show_settings, 1, 0),
        SHELL_CMD_ARG(set_id, NULL, "set custom driver numeric identifier with unsigned int",
                cmd_driver_set_id, 2, 0),
        SHELL_SUBCMD_SET_END
);

SHELL_CMD_REGISTER(driver, &cmds_custom_driver, "- driver - show and set commands", NULL);

int main(void)
{
    bool led_state = true;
    static uint32_t call_count = 0;
    uint32_t rc = 0;

    // Note Zephyr 3.7.0 (possibly a few earlier versions) require application
    // main() to return zero.  We do so here even on a device input/output
    // error:
    if (!gpio_is_ready_dt(&led)) goto end;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) goto end;

    if (!device_is_ready(driver)) {
        LOG_ERR("LED device not ready!  Exiting early . . .");
        goto end;
    }

    rc = (uint32_t)show_settings(driver);

    LOG_INF("- DEV 0530 - setting driver id to non-zero value . . .");
    rc = (uint32_t)set_id(driver, 555);

    LOG_INF("- DEV 0530 - showing settings again:");
    rc = (uint32_t)show_settings(driver);

    while (1) {
	call_count++;
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
#ifdef LED_STATUS_TO_UART
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
#endif
	if ((call_count % 3) == 0) {
		test_channel_get();
	} else {
		test_sample_fetch();
	}

        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }

end:
    return 0;
}
