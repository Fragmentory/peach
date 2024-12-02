/**
 * \file test_audio.hpp
 * \author Koch, Roman (koch.roman@googlemail.com)
 *
 * Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "audio.hpp"
#include "audio_defines.hpp"
#include "audio_pwm.hpp"
#include "board_assembly.hpp"
#include "chunk.h"
#include "core.hpp"
#include "soc_variant.hpp"
#include "test_gpio.hpp"
#include "test_record.hpp"
#include "test_scheduler.hpp"
#include "test_ticker_callback.hpp"
#include "unit_identifier.hpp"
#include "unity.h"

#include <cstdint>
#include <cstdio>
#include <pico/stdlib.h>

namespace test::collection
{
    namespace audio
    {
        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::PWM> test_audio_pwm(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 1000); // x200 ticks

                core::soc::RP2040Pwm pwm;

                pwm.initialize();

                pwm.set_pwm_frequency(2000);
                pwm.set_pwm_duty_cycle(0x7fff);

                gpio_put(12, 1);
                pwm.enable();
                printf("test audio: pwm enabled\n");

                scheduler.perform(
                    [&pwm]()
                    {
                        static int counter = 0;
                        if (counter == 100)
                        {
                            pwm.disable();
                            TEST_ASSERT_MESSAGE(pwm.is_enabled() != true, "pwm not disabled");
                            printf("test audio: pwm disabled\n");
                        }
                        else if (counter == 101)
                        {
                            pwm.set_pwm_frequency(1000);
                            pwm.enable();
                        }

                        counter++;
                    });

                pwm.disable();
                gpio_put(12, 0);

                pwm.shutdown();

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::BEEP> test_audio_beep(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 1000); // x200 ticks

                gpio_put(13, 1);
                core::soc::Variant soc;
                details::TickerCallback ticker_callback;
                soc.ticker_set_callback(ticker_callback);
                core::driver::audio::Variant audio_driver(soc);
                core::Audio audio(audio_driver);

                soc.initialize();
                audio_driver.initialize();

                gpio_put(12, 1);
                audio.beep();
                scheduler.perform([&audio_driver]() { audio_driver.perform(); });
                gpio_put(12, 0);

                audio_driver.shutdown();
                soc.shutdown();
                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::BOOP> test_audio_boop(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 1000); // x200 ticks

                gpio_put(13, 1);
                core::soc::Variant soc;
                details::TickerCallback ticker_callback;
                soc.ticker_set_callback(ticker_callback);
                core::driver::audio::Variant audio_driver(soc);
                core::Audio audio(audio_driver);

                soc.initialize();
                audio_driver.initialize();

                gpio_put(12, 1);
                audio.boop();
                scheduler.perform([&audio_driver]() { audio_driver.perform(); });
                gpio_put(12, 0);

                audio_driver.shutdown();
                soc.shutdown();
                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::TOOT> test_audio_toot(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 1000); // x200 ticks

                gpio_put(13, 1);
                core::soc::Variant soc;
                details::TickerCallback ticker_callback;
                soc.ticker_set_callback(ticker_callback);
                core::driver::audio::Variant audio_driver(soc);
                core::Audio audio(audio_driver);

                soc.initialize();
                audio_driver.initialize();

                gpio_put(12, 1);
                audio.toot();
                scheduler.perform([&audio_driver]() { audio_driver.perform(); });
                gpio_put(12, 0);

                audio_driver.shutdown();
                soc.shutdown();
                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::TAAT> test_audio_taat(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 1000); // x200 ticks

                gpio_put(13, 1);
                core::soc::Variant soc;
                details::TickerCallback ticker_callback;
                soc.ticker_set_callback(ticker_callback);
                core::driver::audio::Variant audio_driver(soc);
                core::Audio audio(audio_driver);

                soc.initialize();
                audio_driver.initialize();

                gpio_put(12, 1);
                audio.taat();
                scheduler.perform([&audio_driver]() { audio_driver.perform(); });
                gpio_put(12, 0);

                audio_driver.shutdown();
                soc.shutdown();
                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::ALERT> test_audio_alert(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 1000); // x200 ticks

                gpio_put(13, 1);
                core::soc::Variant soc;
                details::TickerCallback ticker_callback;
                soc.ticker_set_callback(ticker_callback);
                core::driver::audio::Variant audio_driver(soc);
                core::Audio audio(audio_driver);

                soc.initialize();
                audio_driver.initialize();

                gpio_put(12, 1);
                audio.play_alert();
                scheduler.perform([&audio_driver]() { audio_driver.perform(); });
                gpio_put(12, 0);

                audio_driver.shutdown();
                soc.shutdown();
                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::DONE> test_audio_done(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 1000); // x200 ticks

                gpio_put(13, 1);
                core::soc::Variant soc;
                details::TickerCallback ticker_callback;
                soc.ticker_set_callback(ticker_callback);
                core::driver::audio::Variant audio_driver(soc);
                core::Audio audio(audio_driver);

                soc.initialize();
                audio_driver.initialize();

                gpio_put(12, 1);
                audio.play_done();
                scheduler.perform([&audio_driver]() { audio_driver.perform(); });
                gpio_put(12, 0);

                audio_driver.shutdown();
                soc.shutdown();
                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::INIT> test_audio_init(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 1000); // x200 ticks

                gpio_put(13, 1);
                core::soc::Variant soc;
                details::TickerCallback ticker_callback;
                soc.ticker_set_callback(ticker_callback);
                core::driver::audio::Variant audio_driver(soc);
                core::Audio audio(audio_driver);

                soc.initialize();
                audio_driver.initialize();

                gpio_put(12, 1);
                audio.play_init();
                scheduler.perform([&audio_driver]() { audio_driver.perform(); });
                gpio_put(12, 0);

                audio_driver.shutdown();
                soc.shutdown();
                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::RELAX> test_audio_relax(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 1000); // x200 ticks

                gpio_put(13, 1);
                core::soc::Variant soc;
                details::TickerCallback ticker_callback;
                soc.ticker_set_callback(ticker_callback);
                core::driver::audio::Variant audio_driver(soc);
                core::Audio audio(audio_driver);

                soc.initialize();
                audio_driver.initialize();

                gpio_put(12, 1);
                audio.play_relax();
                scheduler.perform([&audio_driver]() { audio_driver.perform(); });
                gpio_put(12, 0);

                audio_driver.shutdown();
                soc.shutdown();
                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::SERVICE> test_audio_service(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 1000); // x200 ticks

                gpio_put(13, 1);
                core::soc::Variant soc;
                details::TickerCallback ticker_callback;
                soc.ticker_set_callback(ticker_callback);
                core::driver::audio::Variant audio_driver(soc);
                core::Audio audio(audio_driver);

                soc.initialize();
                audio_driver.initialize();

                gpio_put(12, 1);
                audio.play_service();
                scheduler.perform([&audio_driver]() { audio_driver.perform(); });
                gpio_put(12, 0);

                audio_driver.shutdown();
                soc.shutdown();
                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::START> test_audio_start(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 1000); // x200 ticks

                gpio_put(13, 1);
                core::soc::Variant soc;
                details::TickerCallback ticker_callback;
                soc.ticker_set_callback(ticker_callback);
                core::driver::audio::Variant audio_driver(soc);
                core::Audio audio(audio_driver);

                soc.initialize();
                audio_driver.initialize();

                gpio_put(12, 1);
                audio.play_start();
                scheduler.perform([&audio_driver]() { audio_driver.perform(); });
                gpio_put(12, 0);

                audio_driver.shutdown();
                soc.shutdown();
                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::WORK> test_audio_work(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 1000); // x200 ticks

                gpio_put(13, 1);
                core::soc::Variant soc;
                details::TickerCallback ticker_callback;
                soc.ticker_set_callback(ticker_callback);
                core::driver::audio::Variant audio_driver(soc);
                core::Audio audio(audio_driver);

                soc.initialize();
                audio_driver.initialize();

                gpio_put(12, 1);
                audio.play_work();
                scheduler.perform([&audio_driver]() { audio_driver.perform(); });
                gpio_put(12, 0);

                audio_driver.shutdown();
                soc.shutdown();
                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::MULTI> test_audio_multi(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 1000); // x200 ticks

                gpio_put(13, 1);
                core::soc::Variant soc;
                details::TickerCallback ticker_callback;
                soc.ticker_set_callback(ticker_callback);
                core::driver::audio::Variant audio_driver(soc);
                core::Audio audio(audio_driver);

                soc.initialize();
                audio_driver.initialize();

                gpio_put(12, 1);

                audio.play_service();
                audio.play_done();

                audio.play_work();
                audio.play_relax();

                scheduler.perform([&audio_driver]() { audio_driver.perform(); });
                gpio_put(12, 0);

                audio_driver.shutdown();
                soc.shutdown();
                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });

        record::Item<test::GROUP::AUDIO, test::audio::IDENTIFIER::THEME> test_audio_theme(
            []()
            {
                details::test_init_gpio();
                until_timer scheduler(-5000, 10000);

                gpio_put(13, 1);
                core::soc::Variant soc;
                details::TickerCallback ticker_callback;
                soc.ticker_set_callback(ticker_callback);
                core::driver::audio::Variant audio_driver(soc);
                core::Audio audio(audio_driver);

                soc.initialize();
                audio_driver.initialize();

                gpio_put(12, 1);

                audio.play_theme(core::audio::IMPERIAL_MARCH);

                scheduler.perform([&audio_driver]() { audio_driver.perform(); });
                gpio_put(12, 0);

                audio_driver.shutdown();
                soc.shutdown();
                gpio_put(13, 0);

                TEST_ASSERT_MESSAGE(1, "done");
            });
    }
}
