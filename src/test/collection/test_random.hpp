/**
 * \file test_random.hpp
 * \author Koch, Roman (koch.roman@googlemail.com)
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "checksum.hpp"
#include "chunk.h"
#include "core.hpp"
#include "random_sequence.hpp"
#include "temperature_interface.hpp"
#include "test_record.hpp"
#include "test_temperatur_interface.hpp"
#include "unit_identifier.hpp"
#include "unity.h"

#include <pico/stdlib.h>
#include <stdint.h>
#include <stdio.h>

namespace test::collection
{
    namespace random
    {
        static uint8_t a8[8];
        static chunk_t c8 = {a8, 8};
        static uint8_t a11[11];
        static chunk_t c11 = {a11, 11};
        static uint8_t a32[32];
        static chunk_t c32 = {a32, 32};

        static const int NUMBER_OF_SEQUENCES = 100;
        uint16_t pool[NUMBER_OF_SEQUENCES];

        record::Item<test::GROUP::RANDOM, test::random::IDENTIFIER::SEQUENCE_8> test_random_sequence_8(
            []()
            {
                details::TemperaturInterface ti;
                core::service::RandomSequence random_sequence(ti);

                random_sequence.init();
                random_sequence.create(c8);
                printf("sequence 8: [");
                for (size_t i = 0; i < 8; ++i)
                {
                    printf("%02x ", c8.space[i]);
                }
                printf("]\n");

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::RANDOM, test::random::IDENTIFIER::SEQUENCE_11> test_random_sequence_11(
            []()
            {
                details::TemperaturInterface ti;
                core::service::RandomSequence random_sequence(ti);

                random_sequence.init();
                random_sequence.create(c11);
                printf("sequence 11: [");
                for (size_t i = 0; i < 11; ++i)
                {
                    printf("%02x ", c11.space[i]);
                }
                printf("]\n");

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::RANDOM, test::random::IDENTIFIER::SEQUENCE_32> test_random_sequence_32(
            []()
            {
                details::TemperaturInterface ti;
                core::service::RandomSequence random_sequence(ti);

                random_sequence.init();
                random_sequence.create(c32);
                printf("sequence 32: [");
                for (size_t i = 0; i < 32; ++i)
                {
                    printf("%02x ", c32.space[i]);
                }
                printf("]\n");

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::RANDOM, test::random::IDENTIFIER::SEQUENCE_MANY> test_random_many_sequences(
            []()
            {
                details::TemperaturInterface ti;
                core::service::RandomSequence random_sequence(ti);

                int equal = 0;
                random_sequence.init();

                for (int i = 0; i < NUMBER_OF_SEQUENCES; ++i)
                {
                    random_sequence.create(c8);
                    pool[i] = checksum_crc(&c8, 0);
                }
                for (int i = 0; i < NUMBER_OF_SEQUENCES; ++i)
                {
                    for (int j = i + 1; j < NUMBER_OF_SEQUENCES; ++j)
                    {
                        if (pool[i] == pool[j])
                        {
                            equal++;
                        }
                    }
                }
                printf("equal sequences: %d\n", equal);

                TEST_ASSERT_MESSAGE(equal == 0, "equal sequences created");
            });
    }
}