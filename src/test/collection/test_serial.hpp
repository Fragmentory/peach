/**
 * \file test_serial.hpp
 * \author Koch, Roman (koch.roman@googlemail.com)
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "bytering.hpp"
#include "test_record.hpp"
#include "test_scheduler.hpp"
#include "test_serial_handler.hpp"
#include "uart_instance.hpp"
#include "unit_identifier.hpp"
#include "unity.h"

#include <cstdint>
#include <cstdio>
#include <pico/stdlib.h>

namespace test::collection
{
    namespace serial
    {
        namespace details
        {
            buffer_t buffer_a;
            buffer_t buffer_b;

            static void first_cross_handler(buffer_t *const _rx, buffer_t *const _tx)
            {
                uint8_t value = 0;
                bytering_peek(_rx, 0, &value);
                bytering_copy(_rx, &buffer_a);
                bytering_copy(&buffer_b, _tx);
            }

            static void second_cross_handler(buffer_t *const _rx, buffer_t *const _tx)
            {
                uint8_t value = 0;
                bytering_peek(_rx, 0, &value);
                bytering_copy(_rx, &buffer_b);
                bytering_copy(&buffer_a, _tx);
            }
        }

        record::Item<test::GROUP::SERIAL, test::serial::IDENTIFIER::ECHO> test_serial_echo(
            []()
            {
                until_timer scheduler(-5000, 10000);

                core::driver::uart::first::Variant &first_device = core::driver::uart::first::Variant::get_instance();
                first_device.set_handler(test::collection::details::handler::echo);

                core::driver::uart::second::Variant &second_device = core::driver::uart::second::Variant::get_instance();
                second_device.set_handler(test::collection::details::handler::echo);

                first_device.initialize();
                second_device.initialize();

                scheduler.perform(
                    [&first_device, &second_device]()
                    {
                        second_device.perform();
                        first_device.perform();
                    });

                second_device.shutdown();
                first_device.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::SERIAL, test::serial::IDENTIFIER::CROSS> test_serial_cross(
            []()
            {
                until_timer scheduler(-5000, 10000);

                uint8_t space_a[64];
                uint8_t space_b[64];

                details::buffer_a.chunk.space = space_a;
                details::buffer_a.chunk.size = 64;

                details::buffer_b.chunk.space = space_b;
                details::buffer_b.chunk.size = 64;

                core::driver::uart::first::Variant &first_device = core::driver::uart::first::Variant::get_instance();
                first_device.set_handler(details::first_cross_handler);

                core::driver::uart::second::Variant &second_device = core::driver::uart::second::Variant::get_instance();
                second_device.set_handler(details::second_cross_handler);

                first_device.initialize();
                second_device.initialize();

                scheduler.perform(
                    [&first_device, &second_device]()
                    {
                        second_device.perform();
                        first_device.perform();
                    });

                second_device.shutdown();
                first_device.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });
    }
}
