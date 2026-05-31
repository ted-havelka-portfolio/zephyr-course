/*
 * SPDX-License-Identifier: MIT
 *
 * file main.cpp for course work of 2026 Iomico Zephyr Course
 */

#include "api-wrappers.h"

#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/shell/shell.h>

#include <stdlib.h>

LOG_MODULE_REGISTER(shell_support, LOG_LEVEL_INF);

#define BASE_10 10

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

        // . . .
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
