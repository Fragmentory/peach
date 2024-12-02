/**
 * \file test_registry.hpp
 * \author Koch, Roman (koch.roman@googlemail.com)
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "chunk.h"
#include "param_audio.hpp"
#include "param_charger.hpp"
#include "param_features.hpp"
#include "param_load.hpp"
#include "param_maintainer.hpp"
#include "param_name.hpp"
#include "param_position.hpp"
#include "param_serial_number.hpp"
#include "param_unique_identifier.hpp"
#include "param_user.hpp"
#include "param_visual.hpp"
#include "parameter.hpp"
#include "registry.hpp"
#include "test_record.hpp"
#include "test_registry_notification.hpp"
#include "test_temperatur_interface.hpp"
#include "unit_identifier.hpp"
#include "unity.h"

#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

namespace test::collection
{
    namespace registry
    {
        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::INSTANCE> test_registry_instance(
            []()
            {
                details::registry::NotificationHandler notification_handler;
                details::TemperaturInterface ti;
                core::service::RandomSequence random_sequence(ti);

                pulp::registry::Instance registry_instance(notification_handler, random_sequence);
                printf("registry instantiated\n");

                registry_instance.initialize();
                printf("registry initialized\n");

                const pulp::registry::result_t result = registry_instance.is_ready();
                TEST_ASSERT_MESSAGE(result == pulp::registry::SUCCESS, "registry was not initialized");
                printf("registry is ready\n");

                registry_instance.shutdown();
                printf("registry is down\n");
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::CHECK> test_registry_check(
            []()
            {
                pulp::registry::result_t load_result = pulp::registry::parameter::param_load();
                TEST_ASSERT_MESSAGE(load_result == pulp::registry::SUCCESS, "param load failed");
                printf("parameter load done\n");

                pulp::registry::result_t check_result = pulp::registry::parameter::param_check();
                TEST_ASSERT_MESSAGE(check_result == pulp::registry::SUCCESS, "check failed, registry broken");
                printf("parameter check done\n");
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::BACKUP> test_registry_backup(
            []()
            {
                {
                    pulp::registry::result_t result = pulp::registry::parameter::param_backup_check();
                    TEST_ASSERT_MESSAGE(result == pulp::registry::SUCCESS, "backup check failed");
                    printf("backup check done\n");
                }
                {
                    pulp::registry::result_t result = pulp::registry::parameter::param_backup_create();
                    TEST_ASSERT_MESSAGE(result == pulp::registry::SUCCESS, "backup create failed");
                    printf("backup create done\n");
                }
                {
                    pulp::registry::result_t result = pulp::registry::parameter::param_backup_restore();
                    TEST_ASSERT_MESSAGE(result == pulp::registry::SUCCESS, "backup restore failed");
                    printf("backup restore done\n");
                }
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::FORMAT> test_registry_format(
            []()
            {
                pulp::registry::result_t format_result = pulp::registry::parameter::param_format();
                TEST_ASSERT_MESSAGE(format_result == pulp::registry::SUCCESS, "param format failed");
                printf("parameter format done\n");

                pulp::registry::result_t check_result = pulp::registry::parameter::param_check();
                TEST_ASSERT_MESSAGE(check_result == pulp::registry::SUCCESS, "check failed, registry broken");
                printf("parameter check done\n");
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::AUDIO_FEEDBACK> test_registry_audio_feedback(
            []()
            {
                pulp::registry::parameter::audio::register_t audio;

                audio.initialize();

                audio.value.loudness = 200;
                audio.value.verbosity = 0xff;

                uint8_t space[sizeof(pulp::registry::parameter::audio::register_t)];
                uint8_t *ptr = space;

                audio.serialize(&ptr);

                const uint8_t *x = space;

                audio.deserialize(x);

                TEST_ASSERT_MESSAGE(audio.value.loudness <= 100, "wrong loudness");
                TEST_ASSERT_MESSAGE(audio.value.verbosity <= 0x07, "wrong verbosity");
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::CHARGER_CALIBRATION> test_registry_charger_calibration(
            []()
            {
                using param_t = pulp::registry::parameter::charger::register_t;
                param_t parameter_under_test;

                parameter_under_test.initialize();

                uint8_t space[sizeof(param_t)];

                uint8_t *ptr = space;
                parameter_under_test.serialize(&ptr);

                const uint8_t *x = space;
                parameter_under_test.deserialize(x);

                TEST_ASSERT_MESSAGE(true, "charger calibration failed");
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::DEVICE_NAME> test_registry_device_name(
            []()
            {
                using param_t = pulp::registry::parameter::name::register_t;
                param_t parameter_under_test;

                parameter_under_test.initialize();

                uint8_t space[sizeof(param_t)];

                uint8_t *ptr = space;
                parameter_under_test.serialize(&ptr);

                const uint8_t *x = space;
                parameter_under_test.deserialize(x);

                TEST_ASSERT_MESSAGE(true, "device name failed");
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::FEATURES> test_registry_features(
            []()
            {
                using param_t = pulp::registry::parameter::features::register_t;
                param_t parameter_under_test;

                parameter_under_test.initialize();

                uint8_t space[sizeof(param_t)];

                uint8_t *ptr = space;
                parameter_under_test.serialize(&ptr);

                const uint8_t *x = space;
                parameter_under_test.deserialize(x);

                TEST_ASSERT_MESSAGE(true, "features failed");
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::LOAD_CALIBRATION> test_registry_load_calibration(
            []()
            {
                using param_t = pulp::registry::parameter::load::register_t;
                param_t parameter_under_test;

                parameter_under_test.initialize();

                uint8_t space[sizeof(param_t)];

                uint8_t *ptr = space;
                parameter_under_test.serialize(&ptr);

                const uint8_t *x = space;
                parameter_under_test.deserialize(x);

                TEST_ASSERT_MESSAGE(true, "load calibration failed");
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::MAINTAINER> test_registry_maintainer(
            []()
            {
                using param_t = pulp::registry::parameter::maintainer::register_t;
                param_t parameter_under_test;

                parameter_under_test.initialize();

                uint8_t space[sizeof(param_t)];

                uint8_t *ptr = space;
                parameter_under_test.serialize(&ptr);

                const uint8_t *x = space;
                parameter_under_test.deserialize(x);

                TEST_ASSERT_MESSAGE(true, "maintainer failed");
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::POSITION> test_registry_position(
            []()
            {
                using param_t = pulp::registry::parameter::position::register_t;
                param_t parameter_under_test;

                parameter_under_test.initialize();

                uint8_t space[sizeof(param_t)];

                uint8_t *ptr = space;
                parameter_under_test.serialize(&ptr);

                const uint8_t *x = space;
                parameter_under_test.deserialize(x);

                TEST_ASSERT_MESSAGE(true, "position failed");
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::SERIAL_NUMBER> test_registry_serial_number(
            []()
            {
                using param_t = pulp::registry::parameter::serial_number::register_t;
                param_t parameter_under_test;

                parameter_under_test.initialize();

                uint8_t space[sizeof(param_t)];

                uint8_t *ptr = space;
                parameter_under_test.serialize(&ptr);

                const uint8_t *x = space;
                parameter_under_test.deserialize(x);

                TEST_ASSERT_MESSAGE(true, "serial number failed");
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::UNIQUE_IDENTIFIER> test_registry_unique_number(
            []()
            {
                using param_t = pulp::registry::parameter::unique_identifier::register_t;
                param_t parameter_under_test;

                parameter_under_test.initialize();

                uint8_t space[sizeof(param_t)];

                uint8_t *ptr = space;
                parameter_under_test.serialize(&ptr);

                const uint8_t *x = space;
                parameter_under_test.deserialize(x);

                TEST_ASSERT_MESSAGE(true, "unique identifier failed");
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::USER> test_registry_user(
            []()
            {
                using param_t = pulp::registry::parameter::user::register_t;
                param_t parameter_under_test;

                parameter_under_test.initialize();

                uint8_t space[sizeof(param_t)];

                uint8_t *ptr = space;
                parameter_under_test.serialize(&ptr);

                const uint8_t *x = space;
                parameter_under_test.deserialize(x);

                TEST_ASSERT_MESSAGE(true, "user failed");
            });

        record::Item<test::GROUP::REGISTRY, test::registry::IDENTIFIER::VISUAL_FEEDBACK> test_registry_visual_feedback(
            []()
            {
                using param_t = pulp::registry::parameter::visual::register_t;
                param_t parameter_under_test;

                parameter_under_test.initialize();

                uint8_t space[sizeof(param_t)];

                uint8_t *ptr = space;
                parameter_under_test.serialize(&ptr);

                const uint8_t *x = space;
                parameter_under_test.deserialize(x);

                TEST_ASSERT_MESSAGE(true, "visual feedback failed");
            });
    }
}
