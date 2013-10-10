#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <pthread.h> /* Mutex stuff */

#include "crc8.h"
#include "serial_if.h"

#define CRC_CHECK

#define TRANSACTION_SIZE	4

static uint8_t serial_buffer[TRANSACTION_SIZE];
static pthread_mutex_t serial_mutex = PTHREAD_MUTEX_INITIALIZER;

int serial_init(void)
{
	struct termios tio;
    int tty_fd = 0;
    
    /* creates a mutex */
    if (!pthread_mutex_init(&serial_mutex, NULL)) return -1;
        
	memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=0;		/* MIN > 0; TIME == 0: read(2) blocks until the lesser of MIN bytes or the number of bytes requested are available, and returns the lesser of these two values. */

	pthread_mutex_lock(&serial_mutex);
    tty_fd=open(SERIAL_IF_PORT, O_RDWR | O_NONBLOCK);
    if (tty_fd != -1) {      
		cfsetospeed(&tio,SERIAL_IF_BAUD_RATE);            // 9600 baud
		cfsetispeed(&tio,SERIAL_IF_BAUD_RATE);            // 9600 baud
    }
    pthread_mutex_unlock(&serial_mutex);
    
    return tty_fd;
}

void serial_close(int tty_fd)
{
	pthread_mutex_lock(&serial_mutex);
	close(tty_fd);
	pthread_mutex_unlock(&serial_mutex);
	
	/* destroys the mutex */
    pthread_mutex_destroy(&serial_mutex);
}

/* Return -1 if fails, 0 if success                */
int receive_transaction(int tty_fd, transaction_t * transaction)
{
	uint8_t crc;
	int result;
	
	if (transaction == NULL) return E_SYS;
	
	pthread_mutex_lock(&serial_mutex);
	result = read(tty_fd, serial_buffer, TRANSACTION_SIZE);
	pthread_mutex_unlock(&serial_mutex);
	
	if (result < TRANSACTION_SIZE)
		return E_NOT_READY; 		// transaction not complete
	
	transaction->cmd = (serial_buffer[0] & CMD_MASK) >> 4;
	transaction->addr = serial_buffer[0] & ADDR_MASK;
	transaction->value = (serial_buffer[1] << 8) | serial_buffer[2];
	transaction->crc = serial_buffer[3];

#ifdef CRC_CHECK	
	crc = crc8(serial_buffer, 3);
	if (crc == transaction->crc) {		/* CRC OK ? */
		result = E_OK;
	} else {
		result = E_CRC;
	}	
#else
	result = E_OK;
#endif	
	
	return result;
}


int send_response(int tty_fd, transaction_t * transaction)
{   
	uint8_t serialized[TRANSACTION_SIZE];
	int i;
	int res;
	
	if (transaction == NULL) return E_SYS;
	
	serialized[0] = (transaction->cmd << 4) | (transaction->addr & ADDR_MASK);
	serialized[1] = (transaction->value >> 8) & 0xff;
	serialized[2] = transaction->value & 0xff;
	serialized[3] = crc8(serialized, 3);
	
	pthread_mutex_lock(&serial_mutex);
	res = write(tty_fd, serialized, TRANSACTION_SIZE);
	pthread_mutex_unlock(&serial_mutex);
	
	if (res < TRANSACTION_SIZE)
		return E_SYS;
	
	return E_OK;
}

