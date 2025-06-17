cmake_minimum_required(VERSION 3.13)

set(target "whale_radio_module")
project(${target} C CXX ASM)

add_subdirectory(./libs/rp2x_rfm69_rudp)

add_library(${target} INTERFACE)

target_sources(${target} INTERFACE
	src/whale_radio.c
)

target_include_directories(${target} INTERFACE
	..
)

target_link_libraries(${target} INTERFACE
	rp2x_rfm69_rudp_lib
)

target_compile_definitions(${target} INTERFACE
	# Enables high power routines for RFM69H series
	RFM69_HIGH_POWER

	# RFM69 pin definitions 
	W_RADIO_SPI=spi0
	W_RADIO_PIN_MISO=16	
	W_RADIO_PIN_MOSI=19
	W_RADIO_PIN_CS=17
	W_RADIO_PIN_SCK=18
	W_RADIO_PIN_RST=20
	W_RADIO_PIN_DIO0=10
	W_RADIO_PIN_DIO1=11
	W_RADIO_PIN_DIO2=22
	W_RADIO_PIN_DIO3=23
	W_RADIO_PIN_DIO4=24
	W_RADIO_PIN_DIO5=25
)
