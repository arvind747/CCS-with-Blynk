#include <stdint.h>
#include "BlynkHandlers.h"

#ifndef _WIZNET_BLYNK_H_
#define _WIZNET_BLYNK_H_

#define	ARDUINO
#define WIZNET_W5500_EVB
//#define WIZNET_WIZ550WEB

#if defined(WIZNET_W5500_EVB)
#define WIZNET_DEVICE	WIZNET_W5500_EVB
#elif defined (WIZNET_WIZ550WEB)
#define WIZNET_DEVICE	WIZNET_WIZ550WEB
#else
#define WIZNET_DEVICE	ARDUINO
#endif


// Change these settings to match your need
#define BLYNK_DEFAULT_DOMAIN "cloud.blynk.cc"
#define BLYNK_DEFAULT_PORT   8442
#define BLYNK_MAX_READBYTES  255

// Professional settings
#define BLYNK_VERSION        "0.2.1"
#define BLYNK_HEARTBEAT      10
#define BLYNK_TIMEOUT_MS     1500
//#define BLYNK_MSG_LIMIT      20
#define BLYNK_DEBUG

#ifndef BLYNK_INFO_DEVICE
	#define BLYNK_INFO_DEVICE  "TI CC3200-LaunchXL"
	//#define BLYNK_INFO_DEVICE  "WIZWiki"
#endif

#ifndef BLYNK_INFO_CPU
	#define BLYNK_INFO_CPU  "CC3200"
	//#define BLYNK_INFO_CPU  "ST103FRB"
#endif

#ifndef BLYNK_INFO_CONNECTION
	#define BLYNK_INFO_CONNECTION "CC3200"
#endif

#define BLYNK_PARAM_KV(k, v) k "\0" v "\0"

// General defines
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

// Custom defines
#define BLYNK_DEFAULT_CLIENT_PORT		1025
#define BLYNK_CONNECTION_TIMEOUT_MS     5000
#define BLINK_HEADER_SIZE				5

//#ifndef BlynkProtocolDefs_h
//#define BlynkProtocolDefs_h

enum BlynkCmd
{
    BLYNK_CMD_RESPONSE			= 0,
    BLYNK_CMD_REGISTER			= 1,
    BLYNK_CMD_LOGIN				= 2,
    BLYNK_CMD_SAVE_PROF			= 3,
    BLYNK_CMD_LOAD_PROF			= 4,
    BLYNK_CMD_GET_TOKEN			= 5,
    BLYNK_CMD_PING				= 6,
    BLYNK_CMD_TWEET				= 12,
    BLYNK_CMD_EMAIL				= 13,
    BLYNK_CMD_PUSH_NOTIFICATION	= 14,
    BLYNK_CMD_BRIDGE			= 15,
    BLYNK_CMD_HARDWARE			= 20
};

enum BlynkStatus
{
    BLYNK_SUCCESS				= 200,
    BLYNK_TIMEOUT				= 1,
    BLYNK_BAD_FORMAT			= 2,
    BLYNK_NOT_REGISTERED		= 3,
    BLYNK_ALREADY_REGISTERED	= 4,
    BLYNK_NO_LOGIN				= 5,
    BLYNK_NOT_ALLOWED			= 6,
    BLYNK_NO_CONNECTION			= 7,
    BLYNK_NOT_SUPPORTED			= 8,
    BLYNK_INVALID_TOKEN			= 9,
    BLYNK_SERVER_ERROR			= 10,
    BLYNK_ALREADY_LOGGED_IN		= 11
};

typedef struct _BlynkHeader
{
    uint8_t  type;
    uint16_t msg_id;
    uint16_t length;
}
BlynkHeader;

typedef struct _BlynkParam
{
	uint8_t * buff;
	uint16_t len;
}
BlynkParam;

typedef struct _BlynkReq
{
    uint8_t pin;
}
BlynkReq;

typedef void (*WidgetReadHandler)(BlynkReq request);
typedef void (*WidgetWriteHandler)(BlynkReq request, const BlynkParam param);
WidgetReadHandler GetReadHandler(uint8_t pin);
WidgetWriteHandler GetWriteHandler(uint8_t pin);

#define UART_PRINT Report

void blynk_begin(uint8_t * auth, uint8_t * dest_ip, uint16_t dest_port, uint8_t * buf, uint8_t socket);
void blynk_run(void);
void disconnect(int s);
void blynk_time_handler(void);

#endif
