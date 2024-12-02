/**
 * \file test_ticker.hpp
 * \author Koch, Roman (koch.roman@googlemail.com)
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "test_record.hpp"
#include "test_scheduler.hpp"
#include "unit_identifier.hpp"
#include "unity.h"

#include <hardware/timer.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

namespace test::collection
{
    namespace ticker
    {
        static void init_ticker_test()
        {
            /*
                ATTENTION: do not initialize, perform or shutdown the peach core here.
            */

            stdio_init_all();

            gpio_init(12);
            gpio_init(13);
            gpio_init(14);
            gpio_init(15);
            gpio_set_dir(12, GPIO_OUT);
            gpio_set_dir(13, GPIO_OUT);
            gpio_set_dir(14, GPIO_OUT);
            gpio_set_dir(15, GPIO_OUT);

            printf("*NOTE*: USE SALEAE TOOL TO SNIFF TICKER DATA ON GPIO12, GPIO3, GPIO14 and GPIO15\n");
        }

        record::Item<test::GROUP::TICKER, test::ticker::IDENTIFIER::LOOP> test_loop(
            []()
            {
                init_ticker_test();
                until_timer scheduler(-5000, 100);

                scheduler.perform(
                    []()
                    {
                        static int counter = 0;
                        static int duration = 0;
                        if (!(duration++ < 5))
                        {
                            duration = 0;
                            if (counter++ < 10)
                            {
                                counter = 10;
                                gpio_put(12, !gpio_get(12));
                            }
                        }
                        gpio_put(13, !gpio_get(13));
                    });

                TEST_ASSERT_MESSAGE(1, "done");
            });
    }
}