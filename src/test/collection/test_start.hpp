/**
 * \file test_start.hpp
 * \author Koch, Roman (koch.roman@googlemail.com)
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "core.hpp"
#include "event_bus.hpp"
#include "pulp_event.hpp"
#include "test_application.hpp"
#include "test_record.hpp"
#include "test_scheduler.hpp"
#include "test_serial_handler.hpp"
#include "unit_identifier.hpp"
#include "unity.h"

#include <hardware/gpio.h>

namespace test::collection
{
    namespace start
    {
        static void init_gpio()
        {
            gpio_init(12);
            gpio_init(13);
            gpio_init(14);
            gpio_init(15);

            gpio_set_dir(12, GPIO_OUT);
            gpio_set_dir(13, GPIO_OUT);
            gpio_set_dir(14, GPIO_OUT);
            gpio_set_dir(15, GPIO_OUT);
        }

        record::Item<test::GROUP::START, test::start::IDENTIFIER::PLATFORM> test_platform(
            []()
            {
                init_gpio();

                pulp::event::Bus event_bus;
                core::Instance core_instance(event_bus);

                until_timer scheduler(-5000, 100);

                core_instance.set_first_handler(details::handler::black_hole);
                core_instance.set_second_handler(details::handler::black_hole);

                core_instance.initialize();

                scheduler.perform([]() {});

                core_instance.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::START, test::start::IDENTIFIER::APPLICATION> test_application(
            []()
            {
                init_gpio();

                details::application::Instance application_instance;
                application_instance.initialize();

                details::application::Trigger<pulp::event::Stop> application_stopper(application_instance.event_bus, application_instance.core_instance, 10000);

                application_instance.perform();

                application_instance.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });
    }
}
