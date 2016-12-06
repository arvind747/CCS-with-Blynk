//*****************************************************************************
// Copyright (C) 2014 Texas Instruments Incorporated
//
// All rights reserved. Property of Texas Instruments Incorporated.
// Restricted rights to use, duplicate or disclose this code are
// granted through contract.
// The program may not be used without the written permission of
// Texas Instruments Incorporated or against the terms and conditions
// stipulated in the agreement under which this program has been supplied,
// and under no circumstances can it be used with non-TI connectivity device.
//
//*****************************************************************************


//*****************************************************************************
//
// Application Name     - HTTP Client Demo
// Application Overview - This sample application demonstrates how to use
//                          HTTP Client (In Minimum mode) API for HTTP based
//                          application development.
//                          This application explain user to how to:
//                          1. Connect to an access point
//                          2. Connect to a HTTP Server with and without proxy
//                          3. Do POST, GET, PUT and DELETE
//                          4. Parse JSON data using “Jasmine JSON Parser”
// Note: To use HTTP Client in minimum mode, user need to compile library (webclient)
// 			with HTTPCli_LIBTYPE_MIN option.
//
// 			HTTP Client (minimal) library supports synchronous mode, redirection
// 			handling, chunked transfer encoding support, proxy support and TLS
// 			support (for SimpleLink Only. TLS on other platforms are disabled)
//
// 			HTTP Client (Full) library supports all the features of the minimal
// 			library + asynchronous mode and content handling support +
// 			TLS support (all platforms). To use HTTP Client in full mode user need
//			to compile library (webclient) with HTTPCli_LIBTYPE_MIN option. For full
//			mode RTOS is needed.
//
// Application Details  -
// http://processors.wiki.ti.com/index.php/
// or
// docs\examples\
//
//*****************************************************************************


#include <string.h>

// SimpleLink includes
#include "simplelink.h"

// driverlib includes
#include "hw_ints.h"
#include "hw_types.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "utils.h"
#include "interrupt.h"

// common interface includes
#include "uart_if.h"
#include "common.h"
#include "pinmux.h"

//DHT
#include "hw_timer.h"
#include "systick.h"
#include "timer.h"
#include "hw_timer.h"
#include "hw_nvic.h"
#include "gpio_if.h"
#include "hw_memmap.h"
#include "hw_gpio.h"
#include "pin.h"
#include "gpio.h"
#include "timer_if.h"
#include "blynk.h"
#include "blynkDependency.h"

// HTTP Client lib
#include <http/client/httpcli.h>
#include <http/client/common.h>

// JSON Parser
#include "jsmn.h"

#define APPLICATION_VERSION "1.1.1"
#define APP_NAME            "HTTP Client"

#define POST_REQUEST_URI 	"/post"
#define POST_DATA           "{\n\"name\":\"xyz\",\n\"address\":\n{\n\"plot#\":12,\n\"street\":\"abc\",\n\"city\":\"ijk\"\n},\n\"age\":30\n}"

#define DELETE_REQUEST_URI 	"/delete"


#define PUT_REQUEST_URI 	"/put"
#define PUT_DATA            "PUT request."

#define GET_REQUEST_URI 	"/get"


#define HOST_NAME       	"cloud.blynk.cc" //"<host name>"
#define HOST_PORT           8442
#define SERVER_NAME         "cloud.blynk.cc"

#define PROXY_IP       	    <proxy_ip>
#define PROXY_PORT          <proxy_port>

#define READ_SIZE           1450
#define MAX_BUFF_SIZE       1460

//DHT
#define SYSTICKMS           (1000 / SYSTICKHZ)
#define SYSTICKHZ           1000
#define F_CPU               80000000L
#define clockCyclesPerMicrosecond() ( 80000000L / 1000000L )
#define clockCyclesToMicroseconds(a) ( (a) / clockCyclesPerMicrosecond() )
#define microsecondsToClockCycles(a) ( (a) * clockCyclesPerMicrosecond() )
#define bitSet(value, bit) ((value) |= (1UL << (bit)))


// Application specific status/error codes
typedef enum{
 /* Choosing this number to avoid overlap with host-driver's error codes */
    DEVICE_NOT_IN_STATION_MODE = -0x7D0,       
    DEVICE_START_FAILED = DEVICE_NOT_IN_STATION_MODE - 1,
    INVALID_HEX_STRING = DEVICE_START_FAILED - 1,
    TCP_RECV_ERROR = INVALID_HEX_STRING - 1,
    TCP_SEND_ERROR = TCP_RECV_ERROR - 1,
    FILE_NOT_FOUND_ERROR = TCP_SEND_ERROR - 1,
    INVALID_SERVER_RESPONSE = FILE_NOT_FOUND_ERROR - 1,
    FORMAT_NOT_SUPPORTED = INVALID_SERVER_RESPONSE - 1,
    FILE_OPEN_FAILED = FORMAT_NOT_SUPPORTED - 1,
    FILE_WRITE_ERROR = FILE_OPEN_FAILED - 1,
    INVALID_FILE = FILE_WRITE_ERROR - 1,
    SERVER_CONNECTION_FAILED = INVALID_FILE - 1,
    GET_HOST_IP_FAILED = SERVER_CONNECTION_FAILED  - 1,
    
    STATUS_CODE_MAX = -0xBB8
}e_AppStatusCodes;

//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
//*****************************************************************************
volatile unsigned long  g_ulStatus = 0;//SimpleLink Status
unsigned long  g_ulDestinationIP; // IP address of destination server
unsigned long  g_ulGatewayIP = 0; //Network Gateway IP address
unsigned char  g_ucConnectionSSID[SSID_LEN_MAX+1]; //Connection SSID
unsigned char  g_ucConnectionBSSID[BSSID_LEN_MAX]; //Connection BSSID
unsigned char g_buff[MAX_BUFF_SIZE+1];
long bytesReceived = 0; // variable to store the file size

#if defined(ccs) || defined(gcc)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
//*****************************************************************************
//                 GLOBAL VARIABLES -- End
//*****************************************************************************

//for DHT11
#define DATA_BUF_SIZE	2048
static unsigned int dht_port = 0;
static unsigned char dht_pin = 0;
static unsigned long milliseconds = 0;
void delayMicroseconds(unsigned int us);
uint8_t auth[] = "ab76cedebc894b87b811443b95bcbb99";	// You should get Auth Token in the Blynk App
uint8_t blynk_server_ip[4] = {45, 55, 195, 102};		// Blynk cloud server IP (cloud.blynk.cc, 8422)
uint8_t TX_BUF[DATA_BUF_SIZE];

//*****************************************************************************
//
// Globals used by the timer interrupt handler.
//
//*****************************************************************************
static volatile unsigned long g_ulSysTickValue;
static volatile unsigned long g_ulBase;
static volatile unsigned long g_ulRefBase;
static volatile unsigned long g_ulRefTimerInts = 0;
static volatile unsigned long g_ulIntClearVector;
static unsigned char g_timerFlag = 0;
unsigned long g_ulTimerInts;

//*****************************************************************************
// SimpleLink Asynchronous Event Handlers -- Start
//*****************************************************************************


//*****************************************************************************
//
//! \brief The Function Handles WLAN Events
//!
//! \param[in]  pWlanEvent - Pointer to WLAN Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkWlanEventHandler(SlWlanEvent_t *pWlanEvent)
{
    switch(pWlanEvent->Event)
    {
        case SL_WLAN_CONNECT_EVENT:
        {
            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);

            //
            // Information about the connected AP (like name, MAC etc) will be
            // available in 'slWlanConnectAsyncResponse_t'-Applications
            // can use it if required
            //
            //  slWlanConnectAsyncResponse_t *pEventData = NULL;
            // pEventData = &pWlanEvent->EventData.STAandP2PModeWlanConnected;
            //

            // Copy new connection SSID and BSSID to global parameters
            memcpy(g_ucConnectionSSID,pWlanEvent->EventData.
                   STAandP2PModeWlanConnected.ssid_name,
                   pWlanEvent->EventData.STAandP2PModeWlanConnected.ssid_len);
            memcpy(g_ucConnectionBSSID,
                   pWlanEvent->EventData.STAandP2PModeWlanConnected.bssid,
                   SL_BSSID_LENGTH);

            UART_PRINT("[WLAN EVENT] STA Connected to the AP: %s ,"
                            " BSSID: %x:%x:%x:%x:%x:%x\n\r",
                      g_ucConnectionSSID,g_ucConnectionBSSID[0],
                      g_ucConnectionBSSID[1],g_ucConnectionBSSID[2],
                      g_ucConnectionBSSID[3],g_ucConnectionBSSID[4],
                      g_ucConnectionBSSID[5]);
        }
        break;

        case SL_WLAN_DISCONNECT_EVENT:
        {
            slWlanConnectAsyncResponse_t*  pEventData = NULL;

            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_CONNECTION);
            CLR_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

            pEventData = &pWlanEvent->EventData.STAandP2PModeDisconnected;

            // If the user has initiated 'Disconnect' request,
            //'reason_code' is SL_USER_INITIATED_DISCONNECTION
            if(SL_USER_INITIATED_DISCONNECTION == pEventData->reason_code)
            {
                UART_PRINT("[WLAN EVENT]Device disconnected from the AP: %s,"
                "BSSID: %x:%x:%x:%x:%x:%x on application's request \n\r",
                           g_ucConnectionSSID,g_ucConnectionBSSID[0],
                           g_ucConnectionBSSID[1],g_ucConnectionBSSID[2],
                           g_ucConnectionBSSID[3],g_ucConnectionBSSID[4],
                           g_ucConnectionBSSID[5]);
            }
            else
            {
                UART_PRINT("[WLAN ERROR]Device disconnected from the AP AP: %s,"
                "BSSID: %x:%x:%x:%x:%x:%x on an ERROR..!! \n\r",
                           g_ucConnectionSSID,g_ucConnectionBSSID[0],
                           g_ucConnectionBSSID[1],g_ucConnectionBSSID[2],
                           g_ucConnectionBSSID[3],g_ucConnectionBSSID[4],
                           g_ucConnectionBSSID[5]);
            }
            memset(g_ucConnectionSSID,0,sizeof(g_ucConnectionSSID));
            memset(g_ucConnectionBSSID,0,sizeof(g_ucConnectionBSSID));
        }
        break;

        default:
        {
            UART_PRINT("[WLAN EVENT] Unexpected event [0x%x]\n\r",
                       pWlanEvent->Event);
        }
        break;
    }
}

//*****************************************************************************
//
//! \brief This function handles network events such as IP acquisition, IP
//!           leased, IP released etc.
//!
//! \param[in]  pNetAppEvent - Pointer to NetApp Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *pNetAppEvent)
{
    switch(pNetAppEvent->Event)
    {
        case SL_NETAPP_IPV4_IPACQUIRED_EVENT:
        {
            SlIpV4AcquiredAsync_t *pEventData = NULL;

            SET_STATUS_BIT(g_ulStatus, STATUS_BIT_IP_AQUIRED);

            //Ip Acquired Event Data
            pEventData = &pNetAppEvent->EventData.ipAcquiredV4;

            //Gateway IP address
            g_ulGatewayIP = pEventData->gateway;

            UART_PRINT("[NETAPP EVENT] IP Acquired: IP=%d.%d.%d.%d , "
            "Gateway=%d.%d.%d.%d\n\r",
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip,3),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip,2),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip,1),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.ip,0),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway,3),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway,2),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway,1),
            SL_IPV4_BYTE(pNetAppEvent->EventData.ipAcquiredV4.gateway,0));
        }
        break;

        default:
        {
            UART_PRINT("[NETAPP EVENT] Unexpected event [0x%x] \n\r",
                       pNetAppEvent->Event);
        }
        break;
    }
}


//*****************************************************************************
//
//! \brief This function handles HTTP server events
//!
//! \param[in]  pServerEvent - Contains the relevant event information
//! \param[in]    pServerResponse - Should be filled by the user with the
//!                                      relevant response information
//!
//! \return None
//!
//****************************************************************************
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *pHttpEvent,
                                  SlHttpServerResponse_t *pHttpResponse)
{
    // Unused in this application
}

//*****************************************************************************
//
//! \brief This function handles General Events
//!
//! \param[in]     pDevEvent - Pointer to General Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *pDevEvent)
{
    //
    // Most of the general errors are not FATAL are are to be handled
    // appropriately by the application
    //
    UART_PRINT("[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n",
               pDevEvent->EventData.deviceEvent.status,
               pDevEvent->EventData.deviceEvent.sender);
}


//*****************************************************************************
//
//! This function handles socket events indication
//!
//! \param[in]      pSock - Pointer to Socket Event Info
//!
//! \return None
//!
//*****************************************************************************
void SimpleLinkSockEventHandler(SlSockEvent_t *pSock)
{
    //
    // This application doesn't work w/ socket - Events are not expected
    //
       switch( pSock->Event )
    {
        case SL_SOCKET_TX_FAILED_EVENT:
        	switch( pSock->socketAsyncEvent.SockTxFailData.status )
            {
                case SL_ECLOSE:
                    UART_PRINT("[SOCK ERROR] - close socket (%d) operation "
                    "failed to transmit all queued packets\n\n",
                           pSock->socketAsyncEvent.SockAsyncData.sd);
                    break;
                default:
                    UART_PRINT("[SOCK ERROR] - TX FAILED : socket %d , reason"
                        "(%d) \n\n",
                        pSock->socketAsyncEvent.SockAsyncData.sd,
                        pSock->socketAsyncEvent.SockTxFailData.status);
            }
            break;

        default:
            UART_PRINT("[SOCK EVENT] - Unexpected Event [%x0x]\n\n",pSock->Event);
    }
}


//*****************************************************************************
// SimpleLink Asynchronous Event Handlers -- End
//*****************************************************************************



//*****************************************************************************
//
//! \brief This function initializes the application variables
//!
//! \param    None
//!
//! \return None
//!
//*****************************************************************************
static void InitializeAppVariables()
{
    g_ulStatus = 0;
    g_ulGatewayIP = 0;
    memset(g_ucConnectionSSID,0,sizeof(g_ucConnectionSSID));
    memset(g_ucConnectionBSSID,0,sizeof(g_ucConnectionBSSID));
}


//*****************************************************************************
//! \brief This function puts the device in its default state. It:
//!           - Set the mode to STATION
//!           - Configures connection policy to Auto and AutoSmartConfig
//!           - Deletes all the stored profiles
//!           - Enables DHCP
//!           - Disables Scan policy
//!           - Sets Tx power to maximum
//!           - Sets power policy to normal
//!           - Unregister mDNS services
//!           - Remove all filters
//!
//! \param   none
//! \return  On success, zero is returned. On error, negative is returned
//*****************************************************************************
static long ConfigureSimpleLinkToDefaultState()
{
    SlVersionFull   ver = {0};
    _WlanRxFilterOperationCommandBuff_t  RxFilterIdMask = {0};

    unsigned char ucVal = 1;
    unsigned char ucConfigOpt = 0;
    unsigned char ucConfigLen = 0;
    unsigned char ucPower = 0;

    long lRetVal = -1;
    long lMode = -1;

    lMode = sl_Start(0, 0, 0);
    ASSERT_ON_ERROR(lMode);

    // If the device is not in station-mode, try configuring it in station-mode 
    if (ROLE_STA != lMode)
    {
        if (ROLE_AP == lMode)
        {
            // If the device is in AP mode, we need to wait for this event 
            // before doing anything 
            while(!IS_IP_ACQUIRED(g_ulStatus))
            {
#ifndef SL_PLATFORM_MULTI_THREADED
              _SlNonOsMainLoopTask(); 
#endif
            }
        }

        // Switch to STA role and restart 
        lRetVal = sl_WlanSetMode(ROLE_STA);
        ASSERT_ON_ERROR(lRetVal);

        lRetVal = sl_Stop(0xFF);
        ASSERT_ON_ERROR(lRetVal);

        lRetVal = sl_Start(0, 0, 0);
        ASSERT_ON_ERROR(lRetVal);

        // Check if the device is in station again 
        if (ROLE_STA != lRetVal)
        {
            // We don't want to proceed if the device is not coming up in STA-mode 
            return DEVICE_NOT_IN_STATION_MODE;
        }
    }
    
    // Get the device's version-information
    ucConfigOpt = SL_DEVICE_GENERAL_VERSION;
    ucConfigLen = sizeof(ver);
    lRetVal = sl_DevGet(SL_DEVICE_GENERAL_CONFIGURATION, &ucConfigOpt, 
                                &ucConfigLen, (unsigned char *)(&ver));
    ASSERT_ON_ERROR(lRetVal);
    
    UART_PRINT("Host Driver Version: %s\n\r",SL_DRIVER_VERSION);
    UART_PRINT("Build Version %d.%d.%d.%d.31.%d.%d.%d.%d.%d.%d.%d.%d\n\r",
    ver.NwpVersion[0],ver.NwpVersion[1],ver.NwpVersion[2],ver.NwpVersion[3],
    ver.ChipFwAndPhyVersion.FwVersion[0],ver.ChipFwAndPhyVersion.FwVersion[1],
    ver.ChipFwAndPhyVersion.FwVersion[2],ver.ChipFwAndPhyVersion.FwVersion[3],
    ver.ChipFwAndPhyVersion.PhyVersion[0],ver.ChipFwAndPhyVersion.PhyVersion[1],
    ver.ChipFwAndPhyVersion.PhyVersion[2],ver.ChipFwAndPhyVersion.PhyVersion[3]);

    // Set connection policy to Auto + SmartConfig 
    //      (Device's default connection policy)
    lRetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION, 
                                SL_CONNECTION_POLICY(1, 0, 0, 0, 1), NULL, 0);
    ASSERT_ON_ERROR(lRetVal);

    // Remove all profiles
    lRetVal = sl_WlanProfileDel(0xFF);
    ASSERT_ON_ERROR(lRetVal);

    //
    // Device in station-mode. Disconnect previous connection if any
    // The function returns 0 if 'Disconnected done', negative number if already
    // disconnected Wait for 'disconnection' event if 0 is returned, Ignore 
    // other return-codes
    //
    lRetVal = sl_WlanDisconnect();
    if(0 == lRetVal)
    {
        // Wait
        while(IS_CONNECTED(g_ulStatus))
        {
#ifndef SL_PLATFORM_MULTI_THREADED
              _SlNonOsMainLoopTask(); 
#endif
        }
    }

    // Enable DHCP client
    lRetVal = sl_NetCfgSet(SL_IPV4_STA_P2P_CL_DHCP_ENABLE,1,1,&ucVal);
    ASSERT_ON_ERROR(lRetVal);

    // Disable scan
    ucConfigOpt = SL_SCAN_POLICY(0);
    lRetVal = sl_WlanPolicySet(SL_POLICY_SCAN , ucConfigOpt, NULL, 0);
    ASSERT_ON_ERROR(lRetVal);

    // Set Tx power level for station mode
    // Number between 0-15, as dB offset from max power - 0 will set max power
    ucPower = 0;
    lRetVal = sl_WlanSet(SL_WLAN_CFG_GENERAL_PARAM_ID, 
            WLAN_GENERAL_PARAM_OPT_STA_TX_POWER, 1, (unsigned char *)&ucPower);
    ASSERT_ON_ERROR(lRetVal);

    // Set PM policy to normal
    lRetVal = sl_WlanPolicySet(SL_POLICY_PM , SL_NORMAL_POLICY, NULL, 0);
    ASSERT_ON_ERROR(lRetVal);

    // Unregister mDNS services
    lRetVal = sl_NetAppMDNSUnRegisterService(0, 0);
    ASSERT_ON_ERROR(lRetVal);

    // Remove  all 64 filters (8*8)
    memset(RxFilterIdMask.FilterIdMask, 0xFF, 8);
    lRetVal = sl_WlanRxFilterSet(SL_REMOVE_RX_FILTER, (_u8 *)&RxFilterIdMask,
                       sizeof(_WlanRxFilterOperationCommandBuff_t));
    ASSERT_ON_ERROR(lRetVal);


    lRetVal = sl_Stop(SL_STOP_TIMEOUT);
    ASSERT_ON_ERROR(lRetVal);

    InitializeAppVariables();

    return SUCCESS;
}



//****************************************************************************
//
//! \brief Connecting to a WLAN Accesspoint
//!
//!  This function connects to the required AP (SSID_NAME) with Security
//!  parameters specified in te form of macros at the top of this file
//!
//! \param  Status value
//!
//! \return  None
//!
//! \warning    If the WLAN connection fails or we don't aquire an IP
//!            address, It will be stuck in this function forever.
//
//****************************************************************************
static long WlanConnect()
{
    SlSecParams_t secParams = {0};
    long lRetVal = 0;

    secParams.Key = (signed char *)SECURITY_KEY;
    secParams.KeyLen = strlen(SECURITY_KEY);
    secParams.Type = SECURITY_TYPE;

    lRetVal = sl_WlanConnect((signed char *)SSID_NAME,
                           strlen((const char *)SSID_NAME), 0, &secParams, 0);
    ASSERT_ON_ERROR(lRetVal);

    // Wait for WLAN Event
    while((!IS_CONNECTED(g_ulStatus)) || (!IS_IP_ACQUIRED(g_ulStatus)))
    {
        // wait till connects to an AP
        _SlNonOsMainLoopTask();
    }

    return SUCCESS;

}

//*****************************************************************************
//
//! \brief Handler for parsing JSON data
//!
//! \param[in]  ptr - Pointer to http response body data
//!
//! \return 0 on success else error code on failure
//!
//*****************************************************************************
int ParseJSONData(char *ptr)
{
	long lRetVal = 0;
    int noOfToken;
    jsmn_parser parser;
    jsmntok_t   *tokenList;


    /* Initialize JSON PArser */
    jsmn_init(&parser);

    /* Get number of JSON token in stream as we we dont know how many tokens need to pass */
    noOfToken = jsmn_parse(&parser, (const char *)ptr, strlen((const char *)ptr), NULL, 10);
    if(noOfToken <= 0)
    {
    	UART_PRINT("Failed to initialize JSON parser\n\r");
    	return -1;

    }

    /* Allocate memory to store token */
    tokenList = (jsmntok_t *) malloc(noOfToken*sizeof(jsmntok_t));
    if(tokenList == NULL)
    {
        UART_PRINT("Failed to allocate memory\n\r");
        return -1;
    }

    /* Initialize JSON Parser again */
    jsmn_init(&parser);
    noOfToken = jsmn_parse(&parser, (const char *)ptr, strlen((const char *)ptr), tokenList, noOfToken);
    if(noOfToken < 0)
    {
    	UART_PRINT("Failed to parse JSON tokens\n\r");
    	lRetVal = noOfToken;
    }
    else
    {
    	UART_PRINT("Successfully parsed %ld JSON tokens\n\r", noOfToken);
    }

    free(tokenList);

    return lRetVal;
}

void output_mode()
{
    GPIODirModeSet(GPIOA1_BASE, 0x8, GPIO_DIR_MODE_OUT);
    PinConfigSet(PIN_02, PIN_STRENGTH_2MA|PIN_STRENGTH_6MA, PIN_TYPE_STD);
}
void input_mode()
{
    GPIODirModeSet(GPIOA1_BASE, 0x8, GPIO_DIR_MODE_IN);
    PinConfigSet(PIN_02, PIN_STRENGTH_2MA|PIN_STRENGTH_6MA, PIN_TYPE_OD_PU);
}

/*************************************************************************************
 *
 * Convert Humidity
 ************************************************************************************/
float convertHumidity(int16_t rawhumidity)
{
  return (float)rawhumidity/10.0;
}

/*************************************************************************************
 *
 * Convert into temprature
 ************************************************************************************/
//dht22 only, since dht11 does not have decimals
float convertTemperature(int16_t rawtemperature)
{
  if(rawtemperature & 0x8000) {
    return (float)((rawtemperature & 0x7FFF) / 10.0) * -1.0;
  } else {
    return (float)rawtemperature/10.0;
  }
}

//*****************************************************************************
//
//! The interrupt handler for the first timer interrupt.
//!
//! \param  None
//!
//! \return none
//
//*****************************************************************************
void
TimerBaseIntHandler(void)
{
    //
    // Clear the timer interrupt.
    //
    Timer_IF_InterruptClear(g_ulBase);

    g_timerFlag = 1;
}


/************************************************************************************
 *micoseconds delay
 ***********************************************************************************/
unsigned long micros(void)
{
	return (milliseconds * 1000) + ( ((F_CPU / SYSTICKHZ) - MAP_SysTickValueGet()) / (F_CPU/1000000));
}
/************************************************************************************
 * Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse.
 * **********************************************************************************/

unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
    uint8_t stateMask;
    uint32_t start, end, result;
//    unsigned long numloops = 0;

    stateMask = (state ? 1 : 0);

    // convert the timeout from microseconds to a number of times through
    // the initial loop; it takes 11 clock cycles per iteration.
    unsigned long numloops = 0;
    unsigned long maxloops = microsecondsToClockCycles(timeout) / 11;

    // wait for any previous pulse to end
    while (GPIO_IF_Get(11, dht_port, dht_pin)== stateMask)
        if (numloops++ == maxloops) {
            return (0);
        }

    // wait for the pulse to start
    while (GPIO_IF_Get(11, dht_port, dht_pin)!= stateMask)                                   //GPIO_IF_Get(dht_pin, dht_port, 11);
        if (numloops++ == maxloops) {
            return (0);
        }

    // wait for the pulse to stop
    start = micros();

    while (GPIO_IF_Get(11, dht_port, dht_pin) == stateMask) {                               //GPIO_IF_Get(dht_pin, dht_port, 11);
        if (numloops++ == maxloops) {
            return (0);
        }
    }
    end = micros();
    result = end - start;

    return (result);
    // convert the reading to microseconds. The loop has been determined
    // to be 13 clock cycles long and have about 11 clocks between the edge
    // and the start of the loop. There will be some error introduced by
    // the interrupt handlers.
    //return clockCyclesToMicroseconds(width * 13 + 11);
/*

    delayMicroseconds(timeout);

    while(GPIO_IF_Get(11, dht_port, dht_pin) == stateMask)
    {
    	numloops++;
    }

    result = numloops-MILLISECONDS_TO_TICKS(timeout);
*/
  //  return result;
}

/***************************************************************************
 * Delay function
 **************************************************************************/
void delay(uint32_t millis)
{
	unsigned long i;
	for(i=0; i<millis*2; i++){
		delayMicroseconds(500);
	}
}

/****************************************************************************
 *Create delay in Microseconds
 ***************************************************************************/

void delayMicroseconds(unsigned int us)
{

/*	int i=0;
	// Systick timer rolls over every 1000000/SYSTICKHZ microseconds
	if (us > (1000000UL / SYSTICKHZ - 1)) {
		delay(us / 1000);  // delay milliseconds
		us = us % 1000;     // handle remainder of delay
	};

	//
	// Set the period of the SysTick counter.
	//
	HWREG(NVIC_ST_RELOAD) = 0;

	//
	// Enable SysTick.
	//
	HWREG(NVIC_ST_CTRL) |= NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_ENABLE;


	// 24 bit timer - mask off undefined bits
	unsigned long startTime = HWREG(NVIC_ST_CURRENT) & NVIC_ST_CURRENT_M;

	unsigned long ticks = (unsigned long)us * (F_CPU/1000000L);
	volatile unsigned long elapsedTime;

	if (ticks > startTime) {
		ticks = (ticks + (NVIC_ST_CURRENT_M - (unsigned long)F_CPU / SYSTICKHZ)) & NVIC_ST_CURRENT_M;
	}

	do {
		elapsedTime = (startTime-(HWREG(NVIC_ST_CURRENT) & NVIC_ST_CURRENT_M )) & NVIC_ST_CURRENT_M;
	} while(elapsedTime <= ticks);
*/
	//
	// Turn on the timers feeding values in uSec
	//
	g_timerFlag = 0;
	Timer_IF_Start(g_ulBase, TIMER_A, us);
	while(!g_timerFlag);
	Timer_IF_Stop(g_ulBase, TIMER_A);

}


/****************************************************************************
* In this application DHT11 is connected to pin02 of CC3200-Launchpad
* DHT11 raw sensor Data read
* readRawData API read sensor data in raw
* param:/ pin represent pin on which DHT is connected
*         temperature variable is for store temperature value
*         humidity variable is for store temperature value
*         dht22 = true menas DHT22 or dht22 = false means DHT11
*
* return:/ negative on error
*          0 on success
*
***************************************************************************/
int8_t readRawData(unsigned char pin, int16_t *temperature, int16_t *humidity, bool dht22)
{
	uint8_t bits[5];
	int8_t i,j = 0;

	memset(bits, 0, sizeof(bits));

	//get the GPIO pin and port number for write and read
	GPIO_IF_GetPortNPin(11,&dht_port,&dht_pin);


	output_mode();                                                                //pinMode(pin, OUTPUT);
	GPIO_IF_Set(11, dht_port, dht_pin, 0);                                       //digitalWrite(pin, LOW);
	if (dht22)
	  delayMicroseconds(500);
		else
	  delay(18);

	GPIO_IF_Set(11, dht_port, dht_pin, 1);                                      //digitalWrite(pin, HIGH);
	delayMicroseconds(40);

	input_mode();                                                               //pinMode(pin, OUTPUT);
	//check start condition 1
	if(GPIO_IF_Get(11, dht_port, dht_pin)==1) {                              //GPIO_IF_Get(dht_pin, dht_port, 11); //digitalRead(pin)
		return -1;
	}
	delayMicroseconds(80);
	//check start condition 2
	if(GPIO_IF_Get(11, dht_port, dht_pin)==0) {                              //GPIO_IF_Get(dht_pin, dht_port, 11);
		return -2;
	}
	delayMicroseconds(80);

		//Sensor init ok, now read 5 bytes ...
		for (j=0; j<5; j++)
		{
		  for ( i=7; i>=0; i--)
		  {
			if (pulseIn(pin, 1, 1000) > 30)
			  bitSet(bits[j], i);
		  }
		}

	//reset port
    MAP_GPIODirModeSet(GPIOA1_BASE, 0x8, GPIO_DIR_MODE_IN);                                      //pinMode(pin, INPUT);

	if ((uint8_t)(bits[0] + bits[1] + bits[2] + bits[3]) == bits[4]) {
		//return temperature and humidity
				if (dht22) {
		  *humidity = bits[0]<<8 | bits[1];
				  *temperature = bits[2]<<8 | bits[3];
				} else {
		  *humidity = bits[0];
				  *temperature = bits[2];
				}
		return 0;
	}

	return -5;
}

/******************************************************************************
 * DHT11 sensor Data read
 * readFloatData API read sensor data in float
 * param:/ pin represent pin on which DHT is connected
 *         temperature variable is for store temperature value
 *         humidity variable is for store temperature value
 *         dht22 = true menas DHT22 or dht22 = false means DHT11
 *
 * return:/ negative on error
 *          0 on success
 *****************************************************************************/
int readFloatData(unsigned char pin, float *temperature, float *humidity, bool dht22)
{
	int16_t rawtemperature, rawhumidity;
	int8_t res = readRawData(pin, &rawtemperature, &rawhumidity, 0);
	  if (res != 0)
	  {
	    return res;
	  }
	  if (dht22) {
	    *temperature = convertTemperature(rawtemperature);
	    *humidity = convertHumidity(rawhumidity);
	  } else {
	    *temperature = (float)rawtemperature;
	    *humidity = (float)rawhumidity;
	  }
	  return 0;
}

/******************************************************************************
 *
 *  This API is for dweet.io cloud
 *  param/      temerature value and humidity value
 *  return/     nothing
 *****************************************************************************/
void POST2dweet_io(float fTemp, float fHum)
{
  signed char g_Host[] = SERVER_NAME;
  char acRecvBuff[1024];
  unsigned int uiIP;
  long lRetVal = sl_NetAppDnsGetHostByName(g_Host, strlen((const char *)g_Host), (unsigned long*)&uiIP, SL_AF_INET);

  int iSockID = 0;
  iSockID = sl_Socket(SL_AF_INET,SL_SOCK_STREAM, 0);

  SlSockAddrIn_t    Addr;
  int    iAddrSize;
  Addr.sin_family = SL_AF_INET;
  Addr.sin_port = sl_Htons(8442); // default port for http
  Addr.sin_addr.s_addr = sl_Htonl(uiIP);
  iAddrSize = sizeof(SlSockAddrIn_t);

  lRetVal = sl_Connect(iSockID, ( SlSockAddr_t *)&Addr, iAddrSize);

  const char *pPOSTHeaderFormat =
    "PUT http://blynk-qa.cloudapp.net/ab76cedebc894b87b811443b95bcbb99 HTTP/1.1\r\n"
    "Content-Type: application/json\r\n"
    "Content-Length: 30\r\n"
    "\r\n";
  const char *pPOSTBodyFormat =
    "TI CC3200-LaunchXL vw 1 27";

  char acSendBuff[512] = { 0 };
  long lBodyLen = sprintf(acSendBuff, pPOSTBodyFormat, fHum, fTemp);
  lBodyLen = sprintf(acSendBuff, pPOSTHeaderFormat, lBodyLen);
  lBodyLen += sprintf(acSendBuff + lBodyLen, pPOSTBodyFormat, fHum, fTemp);
  acSendBuff[lBodyLen] = 0;
  int iTXStatus;
  iTXStatus = sl_Send(iSockID, acSendBuff, lBodyLen, 0);
  if(iTXStatus<0)
  {
	 UART_PRINT("unable to send data to dweet.io");
  }

  iTXStatus = sl_Recv(iSockID,acRecvBuff,sizeof(acRecvBuff),0);
  if(iTXStatus<0)
  {
     UART_PRINT("unable to send data to dweet.io");
  }
  else
  {
	  UART_PRINT("Receive data = %s\r\n",acRecvBuff);
  }


}


//*****************************************************************************
//
//! Function to connect to AP
//!
//! \param  none
//!
//! \return Error-code or SUCCESS
//!
//*****************************************************************************
static long ConnectToAP()
{
    long lRetVal = -1;
    
    //
    // Following function configure the device to default state by cleaning
    // the persistent settings stored in NVMEM (viz. connection profiles &
    // policies, power policy etc)
    //
    // Applications may choose to skip this step if the developer is sure
    // that the device is in its desired state at start of applicaton
    //
    // Note that all profiles and persistent settings that were done on the
    // device will be lost
    //
    lRetVal = ConfigureSimpleLinkToDefaultState();
    if(lRetVal < 0)
    {
        if (DEVICE_NOT_IN_STATION_MODE == lRetVal)
        {
            UART_PRINT("Failed to configure the device in its default state, "
                            "Error-code: %d\n\r", DEVICE_NOT_IN_STATION_MODE);
        }

        return -1;
    }

    UART_PRINT("Device is configured in default state \n\r");

    //
    // Assumption is that the device is configured in station mode already
    // and it is in its default state
    //
    lRetVal = sl_Start(0, 0, 0);
    if (lRetVal < 0 || ROLE_STA != lRetVal)
    {
        ASSERT_ON_ERROR(DEVICE_START_FAILED);
    }

    UART_PRINT("Device started as STATION \n\r");

    // Connecting to WLAN AP - Set with static parameters defined at the top
    // After this call we will be connected and have IP address
    lRetVal = WlanConnect();

    UART_PRINT("Connected to the AP: %s\r\n", SSID_NAME);
    return 0;
}

//*****************************************************************************
//
//! Application startup display on UART
//!
//! \param  none
//!
//! \return none
//!
//*****************************************************************************
static void
DisplayBanner(char * AppName)
{
    UART_PRINT("\n\n\n\r");
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\t\t      CC3200 %s Application       \n\r", AppName);
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\n\n\n\r");
}


//*****************************************************************************
//
//! Board Initialization & Configuration
//!
//! \param  None
//!
//! \return None
//
//*****************************************************************************
static void
BoardInit(void)
{
/* In case of TI-RTOS vector table is initialize by OS itself */
#ifndef USE_TIRTOS
  //
  // Set vector table base
  //
#if defined(ccs) || defined(gcc)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}


int main()
{
    long lRetVal = -1;
    float temperature,humidity;
 //   HTTPCli_Struct httpClient;


    //
    // Board Initialization
    //
    BoardInit();

    //
    // Configure the pinmux settings for the peripherals exercised
    //
    PinMuxConfig();

    //
    // Configuring UART
    //
    InitTerm();

    //
    // Display banner
    //
    DisplayBanner(APP_NAME);

    InitializeAppVariables();
/*
    //
	// Base address for first timer
	//
	g_ulBase = TIMERA0_BASE;

	//
	// Configuring the timers
	//
	Timer_IF_Init(PRCM_TIMERA0, g_ulBase, TIMER_CFG_PERIODIC, TIMER_A, 0);

	//
	// Setup the interrupts for the timer timeouts.
	//
	Timer_IF_IntSetup(g_ulBase, TIMER_A, blynk_time_handler);

	Timer_IF_Start(g_ulBase, TIMER_A, 1);
*/
    lRetVal = ConnectToAP();
    if(lRetVal < 0)
    {
        LOOP_FOREVER();
    }

/*
    lRetVal = ConnectToHTTPServer(&httpClient);
    if(lRetVal < 0)
    {
        LOOP_FOREVER();
    }

    UART_PRINT("\n\r");
    UART_PRINT("HTTP Get Begin:\n\r");
    lRetVal = HTTPGetMethod(&httpClient);
    if(lRetVal < 0)
    {
    	UART_PRINT("HTTP Post Get failed.\n\r");
    }
    UART_PRINT("HTTP Get End:\n\r");
    UART_PRINT("\n\r");
*/
//    readFloatData(11, &temperature, &humidity, false);
//    UART_PRINT("temperature = %f humidity =%f\r\n",temperature,humidity);
//    POST2dweet_io(1.2, 1.9);

    blynk_begin(auth, 0, 0, TX_BUF, 0);
    while(1)
    {
    blynk_run();
    }
    // Stop the CC3200 device

    LOOP_FOREVER();
}

