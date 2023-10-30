#ifndef CRYPTO_H /* prevent circular inclusions */
#define CRYPTO_H /* by using protection macros */

#include "dma.h"

#define MAX_PAYLOAD_LENGTH 65535
#define NUMBER_BYTES_PER_BLOCK 16
#define KEY_LENGTH 32
#define AAD_LENGTH 16
#define TAG_LENGTH 16
#define BLOCK_LENGTH 16
#define TX_BUFFER_INITIAL_LENGTH (KEY_LENGTH + AAD_LENGTH)

#define GROUND_STATION_TO_SAT_1 0x11
#define GROUND_STATION_TO_SAT_2 0x12
#define GROUND_STATION_TO_SAT_3 0x13
#define SAT_1_TO_GROUND_STATION 0x11
#define SAT_2_TO_GROUND_STATION 0x21
#define SAT_3_TO_GROUND_STATION 0x31

typedef struct CryptoHeader {
	u8 source_dest_ID;
	u8 key_index;
	u16 payload_length;
	u16 reset_replay_counter;
	u8 replay_counter[6];
} CryptoHeader;

typedef struct CryptoPacket {
	CryptoHeader header;
	u8 data[MAX_PAYLOAD_LENGTH + TAG_LENGTH]; /* Reserve enough space for payload and tag */
} CryptoPacket;

typedef struct TxBuffer {
	u8 key[KEY_LENGTH];
	/* The reserved field and the header field
	 * makes up the AAD to be sent to the Dummy Crypto IP */
	u32 reserved;
	CryptoHeader header;
	u8 payload[MAX_PAYLOAD_LENGTH]; /* Reserve enough space for payload */
} TxBuffer;

typedef struct RxBuffer {
	u8 data[MAX_PAYLOAD_LENGTH + TAG_LENGTH]; /* Reserve enough space for payload and tag */
} RxBuffer;

int encrypt(TxBuffer* tx_buffer_ptr, RxBuffer* rx_buffer_ptr);
int decrypt(TxBuffer* tx_buffer_ptr, RxBuffer* rx_buffer_ptr);

/************ Function to verify correctness of encrypt function ************/
int checkEncryption(TxBuffer* tx_buffer_ptr, RxBuffer* rx_buffer_ptr);

/************ Function for filling with dummy data and dummy key ************/
void fillTxBuffer(TxBuffer* tx_buffer_ptr, int payload_length);
void fillDummyKey(TxBuffer* tx_buffer_ptr);

/************ Utility Functions ************/
int alignToBlock(int payload_length);

/************ Functions for printing ************/
void printTxBuffer(TxBuffer* tx_buffer_ptr);
void printRxBuffer(RxBuffer* rx_buffer_ptr, int payload_length);

#endif
