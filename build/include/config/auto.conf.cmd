deps_config := \
	/home/yuggo/esp/esp-idf/components/app_trace/Kconfig \
	/home/yuggo/esp/esp-idf/components/aws_iot/Kconfig \
	/home/yuggo/esp/esp-idf/components/bt/Kconfig \
	/home/yuggo/esp/esp-idf/components/esp32/Kconfig \
	/home/yuggo/esp/esp-idf/components/ethernet/Kconfig \
	/home/yuggo/esp/esp-idf/components/fatfs/Kconfig \
	/home/yuggo/esp/esp-idf/components/freertos/Kconfig \
	/home/yuggo/esp/esp-idf/components/log/Kconfig \
	/home/yuggo/esp/esp-idf/components/lwip/Kconfig \
	/home/yuggo/esp/esp-idf/components/mbedtls/Kconfig \
	/home/yuggo/esp/esp-idf/components/openssl/Kconfig \
	/home/yuggo/esp/esp-idf/components/spi_flash/Kconfig \
	/home/yuggo/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/home/yuggo/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/home/yuggo/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/home/yuggo/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
