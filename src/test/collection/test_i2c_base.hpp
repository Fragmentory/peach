/**
 * \file test_i2c_base.hpp
 * \author Koch, Roman (koch.roman@googlemail.com)
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "core.hpp"
#include "i2c_master.hpp"
#include "i2c_master_variant.hpp"
#include "i2c_slave.hpp"
#include "i2c_slave_variant.hpp"
#include "test_record.hpp"
#include "test_scheduler.hpp"
#include "unit_identifier.hpp"
#include "unity.h"

#include <cstring>
#include <hardware/timer.h>
#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

namespace test::collection
{
    namespace i2c_base
    {
        static void init_test()
        {
            stdio_init_all();
            printf("*NOTE*: USE SALEAE TOOL TO SNIFF I2C DATA ON SDA:GPIO%02d(%02d) and SCL:GPIO%02d(%02d) *\n",
                   core::driver::i2c::slave::Variant::SDA_PIN,
                   core::driver::i2c::master::Variant::SDA_PIN,
                   core::driver::i2c::slave::Variant::SCL_PIN,
                   core::driver::i2c::master::Variant::SCL_PIN);
        }

        record::Item<test::GROUP::BASE_I2C, test::base_i2c::IDENTIFIER::INITIALIZE> test_initialize(
            []()
            {
                init_test();
                until_timer scheduler(-5000);

                core::driver::i2c::memory::DefaultBank default_bank;
                default_bank.initialize();

                core::driver::i2c::Master i2c_master;
                i2c_master.initialize();

                core::driver::i2c::Slave i2c_slave(default_bank);
                i2c_slave.initialize();

                scheduler.perform([]() {});

                i2c_slave.shutdown();
                i2c_master.shutdown();
                default_bank.shutdown();

                TEST_ASSERT_MESSAGE(true, "done");
            });

        record::Item<test::GROUP::BASE_I2C, test::base_i2c::IDENTIFIER::WRITE> test_write(
            []()
            {
                init_test();
                until_timer scheduler(-5000);

                core::driver::i2c::memory::DefaultBank default_bank;
                default_bank.initialize();

                core::driver::i2c::Master i2c_master;
                i2c_master.initialize();

                core::driver::i2c::Slave i2c_slave(default_bank);
                i2c_slave.initialize();

                /* master side */
                printf("initialize master data\n");
                core::driver::i2c::memory::bla::register_t bla;
                bla.value.data = 0xbeef;
                core::driver::i2c::memory::blub::register_t blub;
                blub.value.x = 0xcafe;
                blub.value.a = 0x0f;
                blub.value.b = 0xf0;

                printf("master write\n");
                i2c_master.start_from(static_cast<uint8_t>(bla.ADDRESS));
                i2c_master.write_done(chunk_t{bla.byte, bla.SIZE});

                i2c_master.start_from(static_cast<uint8_t>(blub.ADDRESS));
                i2c_master.write_done(chunk_t{blub.byte, blub.SIZE});

                scheduler.perform([]() {});

                printf("slave data\n");
                printf("bla: 0x%04x\n", default_bank.bla_register.value.data);
                printf("blub a: 0x%02x b:0x%02x x:0x%04x\n",
                       default_bank.blub_register.value.a,
                       default_bank.blub_register.value.b,
                       default_bank.blub_register.value.x);

                sleep_ms(200);

                i2c_slave.shutdown();
                i2c_master.shutdown();
                default_bank.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::BASE_I2C, test::base_i2c::IDENTIFIER::READ> test_read(
            []()
            {
                init_test();
                until_timer scheduler(-5000);

                core::driver::i2c::memory::DefaultBank default_bank;
                default_bank.initialize();

                printf("initialize slave data\n");
                default_bank.bla_register.value.data = 0xbeef;
                default_bank.blub_register.value.x = 0xcafe;
                default_bank.blub_register.value.a = 0x0f;
                default_bank.blub_register.value.b = 0xf0;

                core::driver::i2c::Master i2c_master;
                i2c_master.initialize();

                core::driver::i2c::Slave i2c_slave(default_bank);
                i2c_slave.initialize();

                /* master side */
                printf("master read\n");
                core::driver::i2c::memory::bla::register_t bla;

                i2c_master.start_from(static_cast<uint8_t>(bla.ADDRESS));
                i2c_master.read_done(chunk_t{bla.byte, bla.SIZE});

                core::driver::i2c::memory::blub::register_t blub;

                i2c_master.start_from(static_cast<uint8_t>(blub.ADDRESS));
                i2c_master.read_done(chunk_t{blub.byte, blub.SIZE});

                scheduler.perform([]() {});

                printf("master data\n");
                printf("bla: 0x%04x\n", bla.value.data);
                printf("blub a: 0x%02x b:0x%02x x:0x%04x\n", blub.value.a, blub.value.b, blub.value.x);

                sleep_ms(200);

                i2c_slave.shutdown();
                i2c_master.shutdown();
                default_bank.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::BASE_I2C, test::base_i2c::IDENTIFIER::TEXT> test_text(
            []()
            {
                init_test();
                until_timer scheduler(-5000);

                core::driver::i2c::memory::DefaultBank default_bank;
                default_bank.initialize();

                core::driver::i2c::Master i2c_master;
                i2c_master.set_address(0x55);
                i2c_master.initialize();

                core::driver::i2c::Slave i2c_slave(default_bank);
                i2c_slave.set_address(0x55);
                i2c_slave.initialize();

                /* master side */
                printf("initialize master data\n");
                core::driver::i2c::memory::serial::register_t serial;
                {
                    uint8_t tmp[12] = {
                        0x01,
                        0x23,
                        0x45,
                        0x67,
                        0x89,
                        0xab,
                        0xcd,
                        0xef,
                        0xf1,
                        0xe2,
                        0xd3,
                        0xc4,
                    };
                    memcpy(serial.byte, tmp, 12);
                }

                printf("master write\n");
                i2c_master.start_from(static_cast<uint8_t>(serial.ADDRESS));
                i2c_master.write_next(chunk_t{&serial.byte[0], 4});
                i2c_master.write_next(chunk_t{&serial.byte[4], 4});
                i2c_master.write_done(chunk_t{&serial.byte[8], 4});

                printf("print slave data\n");
                for (uint8_t i = 0; i < 12; ++i)
                {
                    printf("%02x ", default_bank.serial_register.byte[i]);
                }
                printf("\n");

                // TEST_ASSERT_MESSAGE(default_bank.is_overflow(), "unexpected overflow");

                default_bank.reset();
                printf("change slave data\n");
                for (uint8_t i = 0; i < 12; ++i)
                {
                    default_bank.serial_register.byte[i] = i;
                    printf("%02x ", default_bank.serial_register.byte[i]);
                }
                printf("\n");

                printf("master read\n");
                i2c_master.start_from(static_cast<uint8_t>(serial.ADDRESS));
                i2c_master.read_next(chunk_t{&serial.byte[0], 6});
                i2c_master.read_done(chunk_t{&serial.byte[6], 6});

                printf("master data\n");
                for (uint8_t i = 0; i < 12; ++i)
                {
                    printf("%02x ", serial.byte[i]);
                }
                printf("\n");

                // TEST_ASSERT_MESSAGE(default_bank.is_overflow(), "unexpected overflow");

                scheduler.perform([]() {});

                i2c_slave.shutdown();
                i2c_master.shutdown();
                default_bank.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::BASE_I2C, test::base_i2c::IDENTIFIER::UNKNOWN> test_unknown(
            []()
            {
                init_test();
                until_timer scheduler(-5000);

                core::driver::i2c::memory::DefaultBank default_bank;
                default_bank.initialize();

                core::driver::i2c::Master i2c_master;
                i2c_master.initialize();

                core::driver::i2c::Slave i2c_slave(default_bank);
                i2c_slave.initialize();

                /* master side */
                printf("initialize master data\n");
                core::driver::i2c::memory::serial::register_t serial;
                {
                    uint8_t tmp[12] = {
                        0x01,
                        0x23,
                        0x45,
                        0x67,
                        0x89,
                        0xab,
                        0xcd,
                        0xef,
                        0xf1,
                        0xe2,
                        0xd3,
                        0xc4,
                    };
                    memcpy(serial.byte, tmp, 12);
                }

                printf("master write\n");
                i2c_master.start_from(0xFE);
                i2c_master.write_done(chunk_t{serial.byte, serial.SIZE});

                printf("chage slave data\n");
                for (uint8_t i = 0; i < 12; ++i)
                {
                    default_bank.serial_register.byte[i] = i;
                }

                printf("master read\n");
                i2c_master.start_from(0xFE);
                i2c_master.read_done(chunk_t{serial.byte, serial.SIZE});

                scheduler.perform([]() {});

                i2c_slave.shutdown();
                i2c_master.shutdown();
                default_bank.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::BASE_I2C, test::base_i2c::IDENTIFIER::OVERFLOW> test_overflow(
            []()
            {
                init_test();
                until_timer scheduler(-5000);

                core::driver::i2c::memory::DefaultBank default_bank;
                default_bank.initialize();

                core::driver::i2c::Master i2c_master;
                i2c_master.initialize();
                sleep_ms(100);

                core::driver::i2c::Slave i2c_slave(default_bank);
                i2c_slave.initialize();
                sleep_ms(100);

                /* master side */
                printf("initialize master data\n");
                core::driver::i2c::memory::serial::register_t serial;
                {
                    uint8_t tmp[12] = {
                        0x01,
                        0x23,
                        0x45,
                        0x67,
                        0x89,
                        0xab,
                        0xcd,
                        0xef,
                        0xf1,
                        0xe2,
                        0xd3,
                        0xc4,
                    };
                    memcpy(serial.byte, tmp, 12);
                }

                printf("master write\n");
                i2c_master.start_from(0xFE);
                for (uint8_t i = 0; i < 14; ++i)
                {
                    uint8_t byte[1] = {i};
                    i2c_master.write_done(chunk_t{byte, i});
                }

                printf("master read\n");
                for (uint8_t i = 0; i < 12; ++i)
                {
                    default_bank.serial_register.byte[i] = i;
                }

                printf("master read\n");
                i2c_master.start_from(0xFE);
                i2c_master.read_done(chunk_t{serial.byte, serial.SIZE});

                scheduler.perform([]() {});

                i2c_slave.shutdown();
                i2c_master.shutdown();
                default_bank.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });
    }
}
