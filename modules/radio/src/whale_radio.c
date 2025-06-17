#include "rp2x_rfm69_rudp.h"
#include "whale_radio.h"

rfm69_context_t rfm69_ctx = {0};
struct rudp_context rudp_ctx = {0};


static int W_RADIO_MODULE_ERROR = RUDP_UNINITIALIZED;

int w_radio_init(void) {
	// SPI init
    spi_init(W_RADIO_SPI, 1000*1000);
    gpio_set_function(W_RADIO_PIN_MISO, GPIO_FUNC_SPI);
    gpio_set_function(W_RADIO_PIN_SCK,  GPIO_FUNC_SPI);
    gpio_set_function(W_RADIO_PIN_MOSI, GPIO_FUNC_SPI);

	// Drive CS pin high
    gpio_init(W_RADIO_PIN_CS);
    gpio_set_dir(W_RADIO_PIN_CS, GPIO_OUT);
    gpio_put(W_RADIO_PIN_CS, 1);

	struct rfm69_config_s rfm_config = {
		.spi      = W_RADIO_SPI,
		.pin_cs   = W_RADIO_PIN_CS,
		.pin_rst  = W_RADIO_PIN_RST,
		.pin_dio0 = W_RADIO_PIN_DIO0,
		.pin_dio1 = W_RADIO_PIN_DIO1,
		.pin_dio2 = W_RADIO_PIN_DIO2,
		.pin_dio3 = W_RADIO_PIN_DIO3,
		.pin_dio4 = W_RADIO_PIN_DIO4,
		.pin_dio5 = W_RADIO_PIN_DIO5
	};

	struct rudp_config_s rudp_config = {
		.rfm = &rfm69_ctx,
		.rfm_config = &rfm_config,
		.tx_resend_timeout = 1000,
		.rx_wait_timeout = 10000 
	};

	if (!rudp_init(&rudp_ctx, &rudp_config)) {
		W_RADIO_MODULE_ERROR = RUDP_INIT_FAILURE;
		return W_RADIO_ERROR;
	}

	// TEST STUFF
	//rfm69_power_level_set(&rfm69_ctx, -2);
    //rfm69_bitrate_set(&rfm69_ctx, RFM69_MODEM_BITRATE_300);
	//rfm69_fdev_set(&rfm69_ctx, 300000);
	//rfm69_rxbw_set(&rfm69_ctx, RFM69_RXBW_MANTISSA_16, 0);

	uint8_t whale_sync_word[8] = {0x45, 0x01, 0xB7, 0x9A, 0xFE, 0x01, 0xAC, 0x86};
	rfm69_sync_value_set(&rfm69_ctx, whale_sync_word, 4);

	//uint8_t sync_config = 0;
	//rfm69_read(&rfm69_ctx, RFM69_REG_SYNC_CONFIG, &sync_config, 1);
	//sync_config |= 0x38;
	//rfm69_write(&rfm69_ctx, RFM69_REG_SYNC_CONFIG, &sync_config, 1);

	W_RADIO_MODULE_ERROR = RUDP_INIT_SUCCESS;
	return W_RADIO_OK;
}

// TODO: I want to imlement something better than this, but eh, it works for now.
// Its just some white trash errno.
int w_radio_error_get(void) {
	return W_RADIO_MODULE_ERROR;
}

int w_radio_node_address_set(int node_address) {
	if (!rfm69_node_address_set(&rfm69_ctx, (uint8_t) node_address)) {
		W_RADIO_MODULE_ERROR = RUDP_HARDWARE_ERROR;
		return W_RADIO_ERROR;
	}

	W_RADIO_MODULE_ERROR = RUDP_OK;
	return W_RADIO_OK;
}

int w_radio_node_address_get(int *node_address) {
	if (!rfm69_node_address_get(&rfm69_ctx, (uint8_t *)node_address)) {
		W_RADIO_MODULE_ERROR = RUDP_HARDWARE_ERROR;
		return W_RADIO_ERROR;
	}

	W_RADIO_MODULE_ERROR = RUDP_OK;
	return W_RADIO_OK;
}

int w_radio_subnet_address_set(int sub_address) {
	return -1;
}

int w_radio_subnet_address_get(int *sub_address) { 
	return -1;
}

int w_radio_rssi_get(int *rssi) {
	*rssi = rudp_ctx.trx_report.last_rssi;	

	return W_RADIO_OK;
}

int w_radio_rtr_count_get(int *rtr_count) {
	*rtr_count = rudp_ctx.trx_report.rtr_count;	

	return W_RADIO_OK;
}

int w_radio_dbm_set(int dbm) {
	if (!rfm69_power_level_set(&rfm69_ctx, dbm)) {
		W_RADIO_MODULE_ERROR = RUDP_HARDWARE_ERROR;
		return W_RADIO_ERROR;
	}

	return W_RADIO_OK;
}

int w_radio_tx(
	int rx_address,
	void *payload_buffer,
	ptrdiff_t buffer_size
) { 
	W_RADIO_MODULE_ERROR = rudp_tx(
		&rudp_ctx,
		rx_address,
		payload_buffer,
		buffer_size
	);

	if (W_RADIO_MODULE_ERROR != RUDP_TX_SUCCESS)
		return W_RADIO_ERROR;

	return W_RADIO_OK;
}

int w_radio_rx(
	void *payload_buffer,
	ptrdiff_t buffer_size,
	ptrdiff_t *payload_size,
	int *tx_address
) {
	W_RADIO_MODULE_ERROR = rudp_rx(
		&rudp_ctx,
		payload_buffer,
		buffer_size
	);

	if (W_RADIO_MODULE_ERROR != RUDP_RX_SUCCESS)
		return W_RADIO_ERROR;

	*payload_size = rudp_ctx.trx_report.payload_size;
	*tx_address = rudp_ctx.trx_report.tx_addr;

	return W_RADIO_OK;
}

int w_radio_tx_broadcast(
	void *payload_buffer,
	ptrdiff_t buffer_size
) {
	return -1;
}

int w_radio_rx_broadcast(
	void *payload_buffer,
	ptrdiff_t buffer_size,
	ptrdiff_t *payload_size,
	int *tx_address
) {
	return -1;
}
