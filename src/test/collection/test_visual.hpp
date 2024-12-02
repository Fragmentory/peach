/**
 * \file test_visual.hpp
 * \author Koch, Roman (koch.roman@googlemail.com)
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "board_assembly.hpp"
#include "chunk.h"
#include "core.hpp"
#include "core_temperature.hpp"
#include "neopixel_variant.hpp"
#include "soc_variant.hpp"
#include "test_gpio.hpp"
#include "test_record.hpp"
#include "test_scheduler.hpp"
#include "test_temperatur_interface.hpp"
#include "unit_identifier.hpp"
#include "unity.h"
#include "visual.hpp"
#include "visual_color_defines.hpp"
#include "visual_variant.hpp"

#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

namespace test::collection
{
    namespace visual
    {
        record::Item<test::GROUP::VISUAL, test::visual::IDENTIFIER::LEDS> test_visual_leds(
            []()
            {
                until_timer scheduler(core::soc::Variant::DEFAULT_TICKER_DELAY_US, 1000); // x200 steps

                core::driver::neopixel::Variant neopixel_instance;
                neopixel_instance.initialize();

                scheduler.perform(
                    [&neopixel_instance]()
                    {
                        static int counter = 0;

                        if (counter == 0)
                        {
                            neopixel_instance.put_pixel(core::driver::visual::COLOR_RED);
                            neopixel_instance.put_pixel(core::driver::visual::COLOR_GREEN);
                            neopixel_instance.put_pixel(core::driver::visual::COLOR_BLUE);
                            neopixel_instance.put_pixel(core::driver::visual::COLOR_MAGENTA);
                            neopixel_instance.put_pixel(core::driver::visual::COLOR_YELLOW);
                        }
                        else if (counter == 100)
                        {
                            neopixel_instance.put_pixel(core::driver::visual::COLOR_RED_DARK);
                            neopixel_instance.put_pixel(core::driver::visual::COLOR_GREEN_DARK);
                            neopixel_instance.put_pixel(core::driver::visual::COLOR_BLUE_DARK);
                            neopixel_instance.put_pixel(core::driver::visual::COLOR_MAGENTA_DARK);
                            neopixel_instance.put_pixel(core::driver::visual::COLOR_YELLOW_DARK);
                        }
                        counter++;
                    });

                neopixel_instance.shutdown();
                sleep_ms(10);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::VISUAL, test::visual::IDENTIFIER::SMOOTH> test_visual_smooth(
            []()
            {
                details::test_init_gpio();

                until_timer scheduler(core::soc::Variant::DEFAULT_TICKER_DELAY_US, 5000);
                gpio_put(13, 1);

                details::TickerCallback ticker_callback;
                core::driver::neopixel::Variant neopixel_instance;
                core::driver::visual::Variant visual_driver(neopixel_instance);

                neopixel_instance.initialize();
                visual_driver.initialize();

                core::driver::visual::Color left = {
                    .value = {0xff, 0x00, 0x00, 0xff}
                };
                core::driver::visual::Color right = {
                    .value = {0x00, 0x00, 0xff, 0xff}
                };

                visual_driver.color(right, left, scheduler.steps / 2);
                visual_driver.set_program(core::driver::visual::PROGRAM::FREEZE);

                gpio_put(12, 0);
                scheduler.perform(
                    [&visual_driver]()
                    {
                        visual_driver.perform();
                        gpio_put(12, !gpio_get(12));
                    });
                gpio_put(12, 0);

                visual_driver.clean();

                visual_driver.shutdown();
                neopixel_instance.shutdown();

                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::VISUAL, test::visual::IDENTIFIER::PROGRAM_ACTIVE> test_visual_active(
            []()
            {
                details::test_init_gpio();

                until_timer scheduler(core::soc::Variant::DEFAULT_TICKER_DELAY_US, 20000);

                details::TickerCallback ticker_callback;
                core::driver::neopixel::Variant neopixel_instance;
                core::driver::visual::Variant visual_driver(neopixel_instance);

                gpio_put(13, 1);

                neopixel_instance.initialize();
                visual_driver.initialize();

                // details::TemperaturInterface ti;
                // core::Visual visual(visual_driver, ti);

                printf("active phase\n");
                visual_driver.set_program(core::driver::visual::PROGRAM::ACTIVE);

                gpio_put(12, 0);
                scheduler.perform(
                    [&visual_driver]()
                    {
                        visual_driver.perform();
                        gpio_put(12, !gpio_get(12));
                    });
                gpio_put(12, 0);

                printf("clean phase\n");
                visual_driver.clean();

                visual_driver.shutdown();
                neopixel_instance.shutdown();

                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::VISUAL, test::visual::IDENTIFIER::PROGRAM_ALERT> test_visual_alert(
            []()
            {
                until_timer scheduler(core::soc::Variant::DEFAULT_TICKER_DELAY_US, 10000);

                details::TickerCallback ticker_callback;
                core::driver::neopixel::Variant neopixel_instance;
                core::driver::visual::Variant visual_driver(neopixel_instance);

                neopixel_instance.initialize();
                visual_driver.initialize();

                // details::TemperaturInterface ti;
                // core::Visual visual(visual_driver, ti);

                printf("init phase\n");
                scheduler.perform(
                    [&scheduler, &visual_driver]()
                    {
                        static uint64_t counter = 0;
                        if (counter == scheduler.steps / 3)
                        {
                            printf("alert phase\n");
                            visual_driver.set_program(core::driver::visual::PROGRAM::ALERT);
                        }
                        counter++;
                        visual_driver.perform();
                    });

                visual_driver.clean();

                visual_driver.shutdown();
                neopixel_instance.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::VISUAL, test::visual::IDENTIFIER::PROGRAM_CONST> test_visual_program_const(
            []()
            {
                until_timer scheduler(core::soc::Variant::DEFAULT_TICKER_DELAY_US, 3000);

                details::TickerCallback ticker_callback;
                core::driver::neopixel::Variant neopixel_instance;
                core::driver::visual::Variant visual_driver(neopixel_instance);

                neopixel_instance.initialize();
                visual_driver.initialize();

                // details::TemperaturInterface ti;
                // core::Visual visual(visual_driver, ti);

                printf("init phase\n");
                scheduler.perform(
                    [&visual_driver, &scheduler]()
                    {
                        static uint64_t counter = 0;
                        if (counter == scheduler.steps / 4)
                        {
                            printf("freeze phase\n");
                            visual_driver.set_program(core::driver::visual::PROGRAM::FREEZE);
                        }
                        counter++;
                        visual_driver.perform();
                    });

                printf("clean phase\n");
                visual_driver.clean();

                visual_driver.shutdown();
                neopixel_instance.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::VISUAL, test::visual::IDENTIFIER::PROGRAM_INIT> test_visual_program_init(
            []()
            {
                until_timer scheduler(core::soc::Variant::DEFAULT_TICKER_DELAY_US, 10000);

                details::TickerCallback ticker_callback;
                core::driver::neopixel::Variant neopixel_instance;
                core::driver::visual::Variant visual_driver(neopixel_instance);

                neopixel_instance.initialize();
                visual_driver.initialize();

                // details::TemperaturInterface ti;
                // core::Visual visual(visual_driver, ti);

                // visual.idle();

                scheduler.perform([&visual_driver]() { visual_driver.perform(); });

                printf("clean phase\n");
                visual_driver.clean();

                visual_driver.shutdown();
                neopixel_instance.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::VISUAL, test::visual::IDENTIFIER::PROGRAM_OFF> test_visual_off(
            []()
            {
                until_timer scheduler(core::soc::Variant::DEFAULT_TICKER_DELAY_US, 2000); // x400 steps

                details::TickerCallback ticker_callback;
                core::driver::neopixel::Variant neopixel_instance;
                core::driver::visual::Variant visual_driver(neopixel_instance);

                neopixel_instance.initialize();
                visual_driver.initialize();

                // details::TemperaturInterface ti;
                // core::Visual visual(visual_driver, ti);

                printf("init phase\n");
                scheduler.perform(
                    [&visual_driver, &scheduler]()
                    {
                        static uint64_t counter = 0;

                        if (counter == scheduler.steps / 4)
                        {
                            printf("const phase\n");
                            visual_driver.set_program(core::driver::visual::PROGRAM::FREEZE);
                        }
                        else if (counter == scheduler.steps / 2)
                        {
                            printf("off phase\n");
                            visual_driver.set_program(core::driver::visual::PROGRAM::OFF);
                        }

                        counter++;
                        visual_driver.perform();
                    });

                printf("clean phase\n");
                visual_driver.clean();

                visual_driver.shutdown();
                neopixel_instance.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::VISUAL, test::visual::IDENTIFIER::PROGRAM_READY> test_visual_ready(
            []()
            {
                until_timer scheduler(core::soc::Variant::DEFAULT_TICKER_DELAY_US, 10000);

                details::TickerCallback ticker_callback;
                core::driver::neopixel::Variant neopixel_instance;
                core::driver::visual::Variant visual_driver(neopixel_instance);

                neopixel_instance.initialize();
                visual_driver.initialize();

                // details::TemperaturInterface ti;
                // core::Visual visual(visual_driver, ti);

                printf("init phase\n");
                scheduler.perform(
                    [&scheduler, &visual_driver]()
                    {
                        static uint64_t counter = 0;

                        if (counter == scheduler.steps / 8)
                        {
                            printf("ready phase\n");
                            visual_driver.set_program(core::driver::visual::PROGRAM::READY);
                        }

                        counter++;
                        visual_driver.perform();
                    });

                printf("clean phase\n");
                visual_driver.clean();

                visual_driver.shutdown();
                neopixel_instance.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::VISUAL, test::visual::IDENTIFIER::PROGRAM_UPDATE> test_visual_update(
            []()
            {
                until_timer scheduler(core::soc::Variant::DEFAULT_TICKER_DELAY_US, 10000);

                details::TickerCallback ticker_callback;
                core::driver::neopixel::Variant neopixel_instance;
                core::driver::visual::Variant visual_driver(neopixel_instance);

                neopixel_instance.initialize();
                visual_driver.initialize();

                // details::TemperaturInterface ti;
                // core::Visual visual(visual_driver, ti);

                printf("init phase\n");
                scheduler.perform(
                    [&scheduler, &visual_driver]()
                    {
                        static uint64_t counter = 0;

                        if (counter == scheduler.steps / 4)
                        {
                            printf("update phase\n");
                            visual_driver.set_program(core::driver::visual::PROGRAM::SERVICE);
                        }

                        counter++;
                        visual_driver.perform();
                    });

                printf("clean phase\n");
                visual_driver.clean();

                visual_driver.shutdown();
                neopixel_instance.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::VISUAL, test::visual::IDENTIFIER::PROGRAM_COLD_WARM_HOT> test_visual_cold_warm_hot(
            []()
            {
                until_timer scheduler(core::soc::Variant::DEFAULT_TICKER_DELAY_US, 10000);

                details::TickerCallback ticker_callback;
                core::driver::neopixel::Variant neopixel_instance;
                core::driver::visual::Variant visual_driver(neopixel_instance);

                neopixel_instance.initialize();
                visual_driver.initialize();

                details::TemperaturInterface ti;
                core::Visual visual(visual_driver);

                printf("init phase\n");
                visual.idle();

                scheduler.perform(
                    [&scheduler, &visual, &visual_driver]()
                    {
                        static uint64_t counter = 0;

                        if (counter == static_cast<uint64_t>(static_cast<double>(scheduler.steps) * 0.25))
                        {
                            printf("cold phase\n");
                            visual.cold();
                        }
                        else if (counter == static_cast<uint64_t>(static_cast<double>(scheduler.steps) * 0.5))
                        {
                            printf("warm phase\n");
                            visual.warm();
                        }
                        else if (counter == static_cast<uint64_t>(static_cast<double>(scheduler.steps) * 0.75))
                        {
                            printf("hot phase\n");
                            visual.hot();
                        }

                        counter++;
                        visual_driver.perform();
                    });

                printf("clean phase\n");
                visual_driver.clean();

                visual_driver.shutdown();
                neopixel_instance.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::VISUAL, test::visual::IDENTIFIER::TEMPERATURE> test_visual_temperature(
            []()
            {
                until_timer scheduler(core::soc::Variant::DEFAULT_TICKER_DELAY_US, 10000);

                details::TickerCallback ticker_callback;
                core::driver::neopixel::Variant neopixel_instance;
                core::driver::visual::Variant visual_driver(neopixel_instance);

                neopixel_instance.initialize();
                visual_driver.initialize();

                details::TemperaturInterface ti;
                core::Visual visual(visual_driver);

                printf("init phase\n");
                scheduler.perform(
                    [&scheduler, &visual, &visual_driver, &ti]()
                    {
                        static uint64_t counter = 0;

                        if (counter == static_cast<uint64_t>(static_cast<double>(scheduler.steps) * 0.25))
                        {
                            printf("temperature phase\n");
                            visual.scale(ti);
                        }

                        counter++;
                        visual_driver.perform();
                    });

                printf("clean phase\n");
                visual_driver.clean();

                visual_driver.shutdown();
                neopixel_instance.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });
    }
}
