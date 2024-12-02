/**
 * \file test_checksum.hpp
 * \author Koch, Roman (koch.roman@googlemail.com)
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "checksum.hpp"
#include "chunk.h"
#include "test_record.hpp"
#include "test_scheduler.hpp"
#include "unit_identifier.hpp"
#include "unity.h"

#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

namespace test::collection
{
    namespace checksum
    {
        static uint8_t a[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a};
        static chunk_t b = {a, 10};

        static const uint16_t INITAL_VALUE = 0;

        static const uint16_t RESULT_SUM = 55;
        static const uint16_t RESULT_CRC = 0xcd4b;
        static const uint16_t RESULT_CRC_REFLECTED = 0x94c5;
        static const uint16_t RESULT_CRC_POST = 0xcd4b;
        static const uint16_t RESULT_CRC_REFLECTED_POST = 0x94c5;
        static const uint32_t RESULT_HASH = 0xb384c08b;

        // void test_checksum()
        // {
        record::Item<test::GROUP::CHECKSUM, test::checksum::IDENTIFIER::PASSED> test_passed([]() { TEST_ASSERT_MESSAGE(true, "tbd"); });

        record::Item<test::GROUP::CHECKSUM, test::checksum::IDENTIFIER::PASSED> test_failed([]() { TEST_ASSERT_MESSAGE(true, "tbd"); });

        record::Item<test::GROUP::CHECKSUM, test::checksum::IDENTIFIER::SUM> test_sum(
            []()
            {
                until_timer scheduler(-5000, 5000);

                const uint16_t result_value = checksum_sum(&b, INITAL_VALUE);
                printf("checksum sum: %04x %d\n", result_value, result_value);

                scheduler.perform([]() {});

                TEST_ASSERT_MESSAGE(result_value == RESULT_SUM, "wrong checksum");
            });

        record::Item<test::GROUP::CHECKSUM, test::checksum::IDENTIFIER::CRC> test_crc(
            []()
            {
                const uint16_t result_value = checksum_crc(&b, INITAL_VALUE);
                printf("checksum crc: %04x\n", result_value);
                TEST_ASSERT_MESSAGE(result_value == RESULT_CRC, "wrong crc");
            });

        record::Item<test::GROUP::CHECKSUM, test::checksum::IDENTIFIER::CRC_REFLECTED> test_reflected(
            []()
            {
                const uint16_t result_value = checksum_crc_reflected(&b, INITAL_VALUE);
                printf("checksum crc reflected: %04x\n", result_value);
                TEST_ASSERT_MESSAGE(result_value == RESULT_CRC_REFLECTED, "wrong reflected crc");
            });

        record::Item<test::GROUP::CHECKSUM, test::checksum::IDENTIFIER::CRC_BYTE> test_crc_byte(
            []()
            {
                uint16_t result_value = INITAL_VALUE;
                const size_t ARRAY_SIZE = sizeof(a) / sizeof(a[0]);
                for (size_t i = 0; i < ARRAY_SIZE; ++i)
                {
                    result_value = checksum_crc_byte(a[i], result_value);
                }

                printf("checksum crc byte: %04x\n", result_value);
                TEST_ASSERT_MESSAGE(result_value == RESULT_CRC, "wrong crc byte by byte");
            });

        record::Item<test::GROUP::CHECKSUM, test::checksum::IDENTIFIER::CRC_POST> test_crc_post(
            []()
            {
                const uint16_t crc_result_value = checksum_crc(&b, INITAL_VALUE);
                printf("checksum crc: %04x\n", crc_result_value);
                TEST_ASSERT_MESSAGE(crc_result_value == RESULT_CRC, "wrong crc");

                const uint16_t post_result_value = checksum_crc_postprocess(crc_result_value);
                printf("checksum post: %04x\n", post_result_value);
                TEST_ASSERT_MESSAGE(post_result_value == RESULT_CRC_POST, "wrong post value");
            });

        record::Item<test::GROUP::CHECKSUM, test::checksum::IDENTIFIER::CRC_REFLECTED_POST> test_crc_reflected_post(
            []()
            {
                const uint16_t crc_result_value = checksum_crc_reflected(&b, INITAL_VALUE);
                printf("checksum crc: %04x\n", crc_result_value);
                TEST_ASSERT_MESSAGE(crc_result_value == RESULT_CRC_REFLECTED, "wrong crc value");

                const uint16_t post_result_value = checksum_crc_postprocess(crc_result_value);
                printf("checksum post: %04x\n", post_result_value);
                TEST_ASSERT_MESSAGE(post_result_value == RESULT_CRC_REFLECTED_POST, "bla blub test assert");
            });

        record::Item<test::GROUP::CHECKSUM, test::checksum::IDENTIFIER::HASH> test_hash(
            []()
            {
                const uint32_t result_value = checksum_hash(&b);
                printf("checksum hash: %08x\n", result_value);
                TEST_ASSERT_MESSAGE(result_value == RESULT_HASH, "wrong hash value");
            });
        // }
    }
}
