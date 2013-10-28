#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <pthread.h> /* Mutex stuff */

#include "crc8.h"
#include "serial_if.h"

// #define CRC_CHECK

#define TRANSACTION_SIZE	(sizeof(transaction_t)+1)
#define SOF					'a'

static pthread_mutex_t serial_mutex = PTHREAD_MUTEX_INITIALIZER;

int serial_init(void)
{
	struct termios tty;
	int USB_fd = -1;

	/* creates a mutex */
    if (pthread_mutex_init(&serial_mutex, NULL)) {
    	printf("Failed initializing mutex\n");
    	return E_SYS;
    }
    
    USB_fd = open( SERIAL_IF_PORT, O_RDWR| O_NOCTTY | O_NDELAY);
    if (USB_fd < 0) {
    	printf("Error opening port %s\n.", SERIAL_IF_PORT);
    	return USB_fd;
    } else {
    	fcntl(USB_fd, F_SETFL, 0);
    	printf("USB serial port %s is open with fd=%d\n", SERIAL_IF_PORT, USB_fd);
    }
    
    memset (&tty, 0, sizeof tty);
	/* Error Handling */
	if ( tcgetattr ( USB_fd, &tty ) != 0 )
	{
		printf ("error %d from tcgetattr", errno);
	}

	/* Setting other Port Stuff */
	tty.c_cflag     |=  CS8 | CREAD | CLOCAL;     // turn on READ & ignore ctrl lines
#if 1
	tty.c_cflag     &=  ~PARENB;        // Make 8n1
	tty.c_cflag     &=  ~CSTOPB;
	tty.c_cflag     &=  ~CSIZE;

	tty.c_cflag     &=  ~CRTSCTS;       // no flow control
	tty.c_cc[VMIN]      =   1;                  // read doesn't block
	tty.c_cc[VTIME]     =   5;                  // 0.5 seconds read timeout
#endif

	/* Make raw */
	cfmakeraw(&tty);

	/* Set Baud Rate */
	cfsetospeed (&tty, (speed_t)B9600);
	cfsetispeed (&tty, (speed_t)B9600);
	
	/* Flush Port, then applies attributes */
	printf("[%s] Mutex locked.\n", __FUNCTION__);
	pthread_mutex_lock(&serial_mutex);
	tcflush( USB_fd, TCIFLUSH );
	if ( tcsetattr ( USB_fd, TCSANOW, &tty ) != 0)
	{
		printf ("error %d from tcsetattr", errno);
	}
	pthread_mutex_unlock(&serial_mutex);
	printf("[%s] Mutex unlocked.\n", __FUNCTION__);
	
	sleep(2);
	
	return USB_fd;
}

void serial_close(int tty_fd)
{
	printf("[%s] Mutex locked.\n", __FUNCTION__);
	pthread_mutex_lock(&serial_mutex);
	close(tty_fd);
	pthread_mutex_unlock(&serial_mutex);
	printf("[%s] Mutex unlocked.\n", __FUNCTION__);
	
	/* destroys the mutex */
    pthread_mutex_destroy(&serial_mutex);
}

/* Return -1 if fails, 0 if success                */
int get_transaction(int tty_fd, transaction_t * transaction)
{
	uint8_t crc;
	int result;
	uint8_t serial_buffer[TRANSACTION_SIZE];
	
	if (transaction == NULL) return E_SYS;
	
	printf("[%s] Mutex locked.\n", __FUNCTION__);
	pthread_mutex_lock(&serial_mutex);
	memset(serial_buffer, 0, TRANSACTION_SIZE);
	result = read(tty_fd, serial_buffer, TRANSACTION_SIZE);
	pthread_mutex_unlock(&serial_mutex);
	printf("[%s] Mutex unlocked.\n", __FUNCTION__);
	
	transaction->cmd = serial_buffer[1];
	transaction->addr = serial_buffer[2];
	transaction->value = (int16_t)((serial_buffer[4] << 8) | serial_buffer[3]);
	transaction->crc = serial_buffer[5];
	
	printf("Receive %d-bytes transaction: cmd: %d, addr=%d, value=%d\n", result, transaction->cmd, transaction->addr, transaction->value);
	
	if (result < TRANSACTION_SIZE)
		return E_NOT_READY; 		// transaction not complete
		
#ifdef CRC_CHECK	
	crc = crc8(serial_buffer, TRANSACTION_SIZE-1);
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


int send_transaction(int tty_fd, transaction_t * transaction)
{   
	uint8_t serialized[TRANSACTION_SIZE];
	int i;
	int res;
	
	if (transaction == NULL) return E_SYS;
	
	printf("Sending transaction: cmd: %d, addr=%d, value=%d to fd=%d\n", transaction->cmd, transaction->addr, transaction->value, tty_fd);
	
	serialized[0] = SOF;
	memcpy(&serialized[1], transaction, sizeof(transaction_t));
	
	printf("[%s] Mutex locked.\n", __FUNCTION__);
	pthread_mutex_lock(&serial_mutex);
	res = write(tty_fd, serialized, TRANSACTION_SIZE);
	pthread_mutex_unlock(&serial_mutex);
	printf("[%s] Mutex unlocked.\n", __FUNCTION__);
	
	if (res < TRANSACTION_SIZE)
		return E_SYS;
	
	return E_OK;
}

