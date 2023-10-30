#include "crypto.h"

int encrypt(TxBuffer* tx_buffer_ptr, RxBuffer* rx_buffer_ptr) {
	int Status;

	int payload_length = tx_buffer_ptr->header.payload_length;

	flushCache((UINTPTR) tx_buffer_ptr, TX_BUFFER_INITIAL_LENGTH + alignToBlock(payload_length));
	flushCache((UINTPTR) rx_buffer_ptr, alignToBlock(payload_length) + TAG_LENGTH);

	Status = DMA_Xfer((UINTPTR) rx_buffer_ptr, alignToBlock(payload_length) + TAG_LENGTH, XAXIDMA_DEVICE_TO_DMA);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = DMA_Xfer((UINTPTR) tx_buffer_ptr, TX_BUFFER_INITIAL_LENGTH + alignToBlock(payload_length), XAXIDMA_DMA_TO_DEVICE);

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	return XST_SUCCESS;
}

int decrypt(TxBuffer* tx_buffer_ptr, RxBuffer* rx_buffer_ptr) {
	//TODO
	return XST_SUCCESS;
}

void fillDummyKey(TxBuffer* tx_buffer_ptr) {
	u8 dummy_key[] = {72,139,79,112,133,147,28,185,115,73,198,190,210,70,127,215,9,180,47,197,123,59,183,180,75,15,233,159,107,13,220,32};
	for (int i = 0; i < KEY_LENGTH; i++)
		tx_buffer_ptr->key[i] = dummy_key[i];
}

void fillTxBuffer(TxBuffer* tx_buffer_ptr, int payload_length) {
	// Set Key
	fillDummyKey(tx_buffer_ptr);

	// Set Crypto Header
	tx_buffer_ptr->header.source_dest_ID = SAT_1_TO_GROUND_STATION;
	tx_buffer_ptr->header.key_index = 0x02;
	tx_buffer_ptr->header.payload_length = payload_length;
	tx_buffer_ptr->header.reset_replay_counter = 0x04;
	for (int i = 0; i < 6; i++)
		tx_buffer_ptr->header.replay_counter[i] = 0x05 + i;

	// Set payload
	for (int i = 0; i < payload_length; i++)
		tx_buffer_ptr->payload[i] = i;
}

int checkEncryption(TxBuffer* tx_buffer_ptr, RxBuffer* rx_buffer_ptr)
{
	int payload_length = alignToBlock(tx_buffer_ptr->header.payload_length);
	invalidateCache((UINTPTR) rx_buffer_ptr, payload_length + TAG_LENGTH);

	// Check Encrypted Payload
	for (int i = 0; i < tx_buffer_ptr->header.payload_length; i++) {
		if (rx_buffer_ptr->data[i] != (tx_buffer_ptr->payload[i] ^ tx_buffer_ptr->key[i % 16])) {
			xil_printf("Payload incorrect \r\n");
			return XST_FAILURE;
		}
	}

	// Check Tag
	u8* AAD = (u8*) &(tx_buffer_ptr->reserved);
	for (int i = 0; i < TAG_LENGTH; i++) {
		if (rx_buffer_ptr->data[payload_length + i] != (AAD[i] ^ tx_buffer_ptr->key[16 + i])) {
			xil_printf("Tag incorrect \r\n");
			return XST_FAILURE;
		}
	}

	return XST_SUCCESS;
}

void printTxBuffer(TxBuffer* tx_buffer_ptr) {
	int payload_length = alignToBlock(tx_buffer_ptr->header.payload_length);

	xil_printf("----------- TxBuffer Start -----------\r\n");
	xil_printf("\r\n");

	u8* buffer = (u8*) tx_buffer_ptr;
	for (int i = 0; i < TX_BUFFER_INITIAL_LENGTH + payload_length; i++) {
		xil_printf("%02x ", buffer[i]);
		if (i % 4 == 3) {
			xil_printf("\r\n");
		}
		if (i % NUMBER_BYTES_PER_BLOCK == 15) {
			xil_printf("\r\n");
		}
	}
	xil_printf("------------ TxBuffer End ------------\r\n");
}

void printRxBuffer(RxBuffer* rx_buffer_ptr, int payload_length) {
	payload_length = alignToBlock(payload_length);
	invalidateCache((UINTPTR) rx_buffer_ptr, payload_length + TAG_LENGTH);
	xil_printf("----------- RxBuffer Start -----------\r\n");
	xil_printf("\r\n");
	for (int i = 0; i < payload_length + TAG_LENGTH; i++) {
		xil_printf("%02x ", rx_buffer_ptr->data[i]);
		if (i % 4 == 3) {
			xil_printf("\r\n");
		}
		if (i % NUMBER_BYTES_PER_BLOCK == 15) {
			xil_printf("\r\n");
		}
	}
	xil_printf("------------ RxBuffer End ------------\r\n");
}

int alignToBlock(int payload_length) {
	int remainder = payload_length % NUMBER_BYTES_PER_BLOCK;
	if (remainder > 0) {
		return payload_length + (NUMBER_BYTES_PER_BLOCK - remainder);
	} else {
		return payload_length;
	}
}
