/**
 * \file test_engine.hpp
 * \author Koch, Roman (koch.roman@googlemail.com)
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "test_application.hpp"
#include "test_reboot.hpp"
#include "test_record.hpp"
#include "test_serial_handler.hpp"
#include "unit_identifier.hpp"
#include "unity.h"

#include <hardware/gpio.h>

namespace test::collection
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

    record::Item<test::GROUP::ENGINE, test::engine::IDENTIFIER::IDLE_STATE> test_engine_idle_state(
        []()
        {
            bool result = true;
            // init_gpio();

            details::application::Instance application_instance;
            details::application::Trigger<pulp::event::Stop> application_stopper(application_instance.event_bus, application_instance.core_instance, 5000);

            application_instance.initialize();

            application_instance.perform();

            application_instance.shutdown();

            TEST_ASSERT_MESSAGE(result, "engine instance done");
        });

    record::Item<test::GROUP::ENGINE, test::engine::IDENTIFIER::IDLE_STATE> test_engine_ready_state(
        []()
        {
            bool result = false;

            TEST_ASSERT_MESSAGE(result, "tbd ready state");
        });

    record::Item<test::GROUP::ENGINE, test::engine::IDENTIFIER::IDLE_STATE> test_engine_update_state(
        []()
        {
            bool result = false;

            TEST_ASSERT_MESSAGE(result, "tbd update state");
        });

    record::Item<test::GROUP::ENGINE, test::engine::IDENTIFIER::IDLE_STATE> test_engine_connect_event(
        []()
        {
            bool result = false;

            TEST_ASSERT_MESSAGE(result, "tbd connect event");
        });

    record::Item<test::GROUP::ENGINE, test::engine::IDENTIFIER::IDLE_STATE> test_engine_disconnect_event(
        []()
        {
            bool result = false;

            init_gpio();

            details::application::Instance application_instance;
            details::application::Trigger<pulp::event::Stop> application_stop(application_instance.event_bus, application_instance.core_instance, 5000);
            details::application::Trigger<pulp::event::Disconnect> application_disconnect(
                application_instance.event_bus, application_instance.core_instance, 10);

            application_instance.initialize();
            application_instance.perform();
            application_instance.shutdown();

            TEST_ASSERT_MESSAGE(result, "error event");
        });

    record::Item<test::GROUP::ENGINE, test::engine::IDENTIFIER::IDLE_STATE> test_engine_update_event(
        []()
        {
            bool result = false;

            TEST_ASSERT_MESSAGE(result, "tbd update event");
        });

    record::Item<test::GROUP::ENGINE, test::engine::IDENTIFIER::IDLE_STATE> test_engine_error_event(
        []()
        {
            bool result = false;
            printf("send error event\n");

            TEST_ASSERT_MESSAGE(result, "tbd error event");
        });
}
