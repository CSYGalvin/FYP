// Include custom header files
#include "dma.h"
#include "crypto.h"

__attribute__((aligned(16))) TxBuffer tx_buffer;
__attribute__((aligned(16))) RxBuffer rx_buffer;

extern volatile u32 TxDone;
extern volatile u32 RxDone;
extern volatile u32 Error;

int main(void)
{
	xil_printf("\r\n--- Entering main() --- \r\n");

	int Status;

	Status = InitDMA();

	if (Status != XST_SUCCESS) {
		xil_printf("DMA Init Failed\r\n");
		return XST_FAILURE;
	}

	char input;
	int payload_length = 0;

	xil_printf("Do you want to specify your own crypto packet? [y/n]\r\n");
	input = inbyte();
	// ignore new line
	inbyte();
	if (input == 'y') {
		xil_printf(
				"0: Ground Station to SAT 1\r\n"
				"1: Ground Station to SAT 2\r\n"
				"2: Ground Station to SAT 3\r\n"
				"3: SAT 1 to Ground Station\r\n"
				"4: SAT 2 to Ground Station\r\n"
				"5: SAT 3 to Ground Station\r\n"
				"Please enter direction: ");
		input = inbyte();
		xil_printf("%c\r\n", input);
		u8 dirs[] = {0x11, 0x12, 0x13, 0x11, 0x21, 0x31};
		tx_buffer.header.source_dest_ID = dirs[input - 48];
		// ignore new line
		inbyte();

		xil_printf("Please enter key index [0 - 9]: ");
		input = inbyte();
		xil_printf("%c\r\n", input);
		tx_buffer.header.key_index = input - 48;
		// ignore new line
		inbyte();

		xil_printf("Please enter payload: ");
		while (1) {
			input = inbyte();
			if (input == '\r') {
				break;
			} else {
				xil_printf("%c", input);
				tx_buffer.payload[payload_length] = (u8) input;
				payload_length++;
			}
		}
		xil_printf("\r\n");

		fillDummyKey(&tx_buffer);
		tx_buffer.header.payload_length = payload_length;
	} else {
		payload_length = 40;
		fillTxBuffer(&tx_buffer, payload_length);
	}

	printTxBuffer(&tx_buffer);
	Status = encrypt(&tx_buffer, &rx_buffer);

	if (Status != XST_SUCCESS) {
		xil_printf("Encrypt Failed\r\n");
		return XST_FAILURE;
	}

	while (!RxDone || Error) {}

	if (Error) {
		xil_printf("Error Flag Set\r\n");
		return XST_FAILURE;
	}

	printRxBuffer(&rx_buffer, payload_length);

	Status = checkEncryption(&tx_buffer, &rx_buffer);

	if (Status != XST_SUCCESS) {
		xil_printf("Data check failed\r\n");
	} else {
		xil_printf("Encrypt Test Pass\r\n");
	}

	xil_printf("--- Exiting main() --- \r\n");

	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}
