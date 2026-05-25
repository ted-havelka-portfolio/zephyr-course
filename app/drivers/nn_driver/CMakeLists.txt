# Copyright (c) 2026 Neela Nurseries
# SPDX-License-Identifier: Apache-2.0

if (CONFIG_NN_DRIVER)
	zephyr_library()
	zephyr_library_src(nn-driver.c)
endif()
