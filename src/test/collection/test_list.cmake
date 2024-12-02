# Copyright (c) 2023, Roman Koch, koch.roman@gmail.com
# SPDX-License-Identifier: MIT

# ---------------------------------------------------------
# ctest call script
# ---------------------------------------------------------
set(GLOBAL_TEST_LIST "")
include(${CMAKE_CURRENT_LIST_DIR}/agency/do_test.cmake)

do_test(checksum_passed)
do_test(checksum_failed)

do_test(checksum_sum "${PROJECT_SOURCE_DIR}/bin/${EXECUTABLE_NAME}.uf2")
do_test(checksum_crc)
do_test(checksum_crc_byte)
do_test(checksum_crc_post)
do_test(checksum_crc_reflected)
do_test(checksum_crc_reflected_post)
do_test(checksum_hash)

do_test(random_sequence_11)
do_test(random_sequence_32)
do_test(random_sequence_8)
do_test(random_sequence_many)

do_test(serial_echo)
do_test(serial_cross)

do_test(audio_pwm)
do_test(audio_beep)
do_test(audio_boop)
do_test(audio_toot)
do_test(audio_taat)
do_test(audio_alert)
do_test(audio_done)
do_test(audio_init)
do_test(audio_relax)
do_test(audio_service)
do_test(audio_start)
do_test(audio_work)
do_test(audio_multi)
do_test(audio_theme)

do_test(visual_leds)
do_test(visual_smooth)
do_test(visual_program_active)
do_test(visual_program_alert)
do_test(visual_program_const)
do_test(visual_program_init)
do_test(visual_program_off)
do_test(visual_program_ready)
do_test(visual_program_update)
do_test(visual_program_cold_warm_hot)
do_test(visual_temperature)

do_test(base_i2c@initialize)
do_test(base_i2c@write)
do_test(base_i2c@read)
do_test(base_i2c@text)
do_test(base_i2c@unknown)
do_test(base_i2c@overflow)

do_test(registry_instance)

do_test(registry_check)
do_test(registry_backup)
do_test(registry_format)

do_test(registry_audio_feedback)
do_test(registry_charger_calibration)
do_test(registry_device_name)
do_test(registry_features)
do_test(registry_load_calibration)
do_test(registry_maintainer)
do_test(registry_position)
do_test(registry_serial_number)
do_test(registry_unique_identifier)
do_test(registry_user)
do_test(registry_visual_feedback)

do_test(ticker_loop)

do_test(start_platform)
do_test(start_application)

do_test(engine_idle_state)
do_test(engine_ready_state)
do_test(engine_update_event)
do_test(engine_update_state)
do_test(engine_connect_event)
do_test(engine_disconnect_event)
do_test(engine_error_event)

# ---------------------------------------------------------
# Generate the header file
# do not move this section
# ---------------------------------------------------------
include(${CMAKE_CURRENT_LIST_DIR}/agency/test_identifier.cmake)
generate_test_identifier_header(${EXECUTABLE_NAME} "${GLOBAL_TEST_LIST}")
