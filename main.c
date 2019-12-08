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
// Application Name     -   MQTT Client
// Application Overview -   This application acts as a MQTT client and connects
//                          to the IBM MQTT broker, simultaneously we can
//                          connect a web client from a web browser. Both
//                          clients can inter-communicate using appropriate
//                          topic names.
//
// Application Details  -
// http://processors.wiki.ti.com/index.php/CC32xx_MQTT_Client
// or
// docs\examples\CC32xx_MQTT_Client.pdf
//
//*****************************************************************************

//*****************************************************************************
//
//! \addtogroup mqtt_client
//! @{
//
//*****************************************************************************

// Standard includes
#include <stdlib.h>
#include <stdbool.h>
//#include <string.h>
#include <stdint.h>
#include <stdio.h>

// simplelink includes
#include "simplelink.h"

// driverlib includes
#include "hw_types.h"
#include "hw_ints.h"
#include "hw_memmap.h"
#include "interrupt.h"
#include "rom_map.h"
#include "prcm.h"
#include "uart.h"
#include "timer.h"
#include "hw_common_reg.h"
#include "spi.h"
#include "rom.h"
#include "utils.h"
#include "timer.h"

// common interface includes
#include "network_if.h"
#ifndef NOTERM
#include "uart_if.h"
#endif

#include "button_if.h"
#include "gpio_if.h"
#include "timer_if.h"
#include "common.h"
#include "utils.h"

//#include "systick_if.h"
#include "timer_if.h"


#include "sl_mqtt_client.h"

// application specific includes
#include "pinmux.h"

#define APPLICATION_VERSION     "1.1.1"

/*Operate Lib in MQTT 3.1 mode.*/
#define MQTT_3_1_1              false /*MQTT 3.1.1 */
#define MQTT_3_1                true /*MQTT 3.1*/

#define WILL_TOPIC              "Client"
#define WILL_MSG                "Client Stopped"
#define WILL_QOS                QOS1
#define WILL_RETAIN             false

/*Defining Broker IP address and port Number*/
#define SERVER_ADDRESS          "a3njzpcieyczfg-ats.iot.us-west-2.amazonaws.com"
#define PORT_NUMBER             8883
//#define PORT_NUMBER             1883

#define MAX_BROKER_CONN         1

#define SERVER_MODE             MQTT_3_1_1
/*Specifying Receive time out for the Receive task*/
#define RCV_TIMEOUT             30

/*Background receive task priority*/
#define TASK_PRIORITY           3

/* Keep Alive Timer value*/
#define KEEP_ALIVE_TIMER        100

/*Clean session flag*/
#define CLEAN_SESSION           true

/*Retain Flag. Used in publish message. */
#define RETAIN                  0//1

/*Defining Publish Topic*/
#define PUB_TOPIC_FOR_HBR       "/cc3200/HBR_Sending"

/*Defining Number of topics*/
#define TOPIC_COUNT             1

/*Defining Subscription Topic Values*/
#define TOPIC1                  "/cc3200/HBR_Requesting"

/*Defining QOS levels*/
#define QOS0                    0
#define QOS1                    1
#define QOS2                    2

/*Spawn task priority and OSI Stack Size*/
#define OSI_STACK_SIZE          2048
#define UART_PRINT              Report

//NEED TO UPDATE THIS FOR IT TO WORK!
#define DATE                6   /* Current Date */
#define MONTH               12     /* Month 1-12 */
#define YEAR                2019  /* Current year */
#define HOUR                14   /* Time - hours */
#define MINUTE              36    /* Time - minutes */
#define SECOND              6    /* Time - seconds */

//*****************************************************************************
//
// Application Master/Slave mode selector macro
//
// MASTER_MODE = 1 : Application in master mode
// MASTER_MODE = 0 : Application in slave mode
//
//*****************************************************************************
#define MASTER_MODE      1

#define SYS_CLK                 80000000
#define SPI_IF_BIT_RATE  10000 //10000
#define TR_BUFF_SIZE     100
#define SAMPLES_PER_SECOND SPI_IF_BIT_RATE/16

#define MASTER_MSG       "This is CC3200 SPI Master Application\n\r"
#define SLAVE_MSG        "This is CC3200 SPI Slave Application\n\r"

#define NUMSAMPLE       2000

#define SLOW_TIMER_DELAY_uS 2000

#define FILTER_LEN  5
double coeffs[ FILTER_LEN ] =
{
  -0.0110, 0.0939, 0.4895, 0.4895, 0.0939//, -0.0110
};

typedef struct
{
   /* time */
   unsigned long tm_sec;
   unsigned long tm_min;
   unsigned long tm_hour;
   /* date */
   unsigned long tm_day;
   unsigned long tm_mon;
   unsigned long tm_year;
   unsigned long tm_week_day; //not required
   unsigned long tm_year_day; //not required
   unsigned long reserved[3];
}SlDateTime;

typedef struct connection_config{
    SlMqttClientCtxCfg_t broker_config;
    void *clt_ctx;
    unsigned char *client_id;
    unsigned char *usr_name;
    unsigned char *usr_pwd;
    bool is_clean;
    unsigned int keep_alive_time;
    SlMqttClientCbs_t CallBAcks;
    int num_topics;
    char *topic[TOPIC_COUNT];
    unsigned char qos[TOPIC_COUNT];
    SlMqttWill_t will_params;
    bool is_connected;
}connect_config;

typedef enum
{
    PUSH_BUTTON_SW2_PRESSED,
    HBR_REQUESTED,
    BROKER_DISCONNECTION
}events;

typedef struct
{
    void * hndl;
    events event;
}event_msg;

//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
//*****************************************************************************
static unsigned short g_ucTxBuff[TR_BUFF_SIZE];
static unsigned short g_ucRxBuff[TR_BUFF_SIZE];
static unsigned char ucTxBuffNdx;
static unsigned char ucRxBuffNdx;
unsigned long TimerInts;

static unsigned long g_ulA2IntCount;

// array to hold input samples
//double insamp[2000];

//float floatInput[NUMSAMPLE];


#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
//*****************************************************************************
//                 GLOBAL VARIABLES -- End
//*****************************************************************************


//*****************************************************************************
//                      LOCAL FUNCTION PROTOTYPES
//*****************************************************************************
static void
Mqtt_Recv(void *app_hndl, const char  *topstr, long top_len, const void *payload,
          long pay_len, bool dup,unsigned char qos, bool retain);
static void sl_MqttEvt(void *app_hndl,long evt, const void *buf,
                       unsigned long len);
static void sl_MqttDisconnect(void *app_hndl);
void pushButtonInterruptHandler3();
void TimerPeriodicIntHandler(void);
void LedTimerConfigNStart();
void LedTimerDeinitStop();
void BoardInit(void);
static void DisplayBanner(char * AppName);
void MqttClient(void *pvParameters);

static void TimerA1IntHandler(void);
void MasterMain();
void firFloatInit(void);
void firFloat( double *coeffs, double *input, double *output, int length, int filterLength );
float findMax(float* InArray, int size, int* index, float* temp);
float findMin(float* InArray, int size, int* index, float* temp);
int HBR_Detection(void);


static int set_time();
//*****************************************************************************
//                 GLOBAL VARIABLES -- Start
//*****************************************************************************
#ifdef USE_FREERTOS
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif
#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#endif

char *security_file_list[] = {"/cert/private.der", "/cert/client.der", "/cert/StarfieldClass2CA.der"};  //Order: Private Key, Certificate File, CA File, DH Key (N/A)
SlDateTime g_time;

unsigned short g_usTimerInts;
/* AP Security Parameters */
SlSecParams_t SecurityParams = {0};

/*Message Queue*/
OsiMsgQ_t g_PBQueue;

/* connection configuration */
connect_config usr_connect_config[] =
{
    {
        {
            {
                (SL_MQTT_NETCONN_URL|SL_MQTT_NETCONN_SEC),
                SERVER_ADDRESS,
                PORT_NUMBER,
                SL_SO_SEC_METHOD_TLSV1_2,   //Method (TLS1.2)
                //SL_SEC_MASK_TLS_ECDHE_RSA_WITH_AES_128_CBC_SHA256,
                SL_SEC_MASK_TLS_RSA_WITH_AES_256_CBC_SHA,   //Cipher
                3,                          //number of files
                security_file_list          //name of files

            },
            SERVER_MODE,
            true,
        },
        NULL,
        "CC3200_Patient",
        NULL,
        NULL,
        true,
        KEEP_ALIVE_TIMER,
        {Mqtt_Recv, sl_MqttEvt, sl_MqttDisconnect},
        TOPIC_COUNT,
        {TOPIC1},       //, TOPIC2, TOPIC3
        {QOS1},         //, QOS1, QOS1
        //{WILL_TOPIC,WILL_MSG,WILL_QOS,WILL_RETAIN},
        NULL,
        false
    }
};

/* library configuration */
SlMqttClientLibCfg_t Mqtt_Client={
    1882,
    TASK_PRIORITY,
    30,
    true,
    (long(*)(const char *, ...))UART_PRINT
};

/*Publishing topics and messages*/
//const char *pub_topic_sw2 = PUB_TOPIC_FOR_SW2;
const char *pub_topic_hbr = PUB_TOPIC_FOR_HBR;
//unsigned char *data_sw2={"Push button sw2 is pressed on CC32XX device"};


//Request message from nurse board
const char *HBRRequest = {"HBR is requested by the nurse."};

void *app_hndl = (void*)usr_connect_config;
//*****************************************************************************
//                 GLOBAL VARIABLES -- End
//*****************************************************************************


//*****************************************************************************
//
//! \TimerA0IntHandler
//!
//! Handles interrupts for Timer A2.
//! This interrupt handler clears the source of the interrupt and
//! increments a counter and returns.
//!
//! \param None.
//!
//! \return None.
//
//*****************************************************************************
static void
TimerA1IntHandler(void)
{
    unsigned long ulStatus;

    //
    // Clear all interrupts for Timer.
    //
    ulStatus = TimerIntStatus(TIMERA1_BASE, true);
    TimerIntClear(TIMERA1_BASE, ulStatus);

    //
    // Increment our global interrupt counter.
    //
    g_ulA2IntCount++;

}

//*****************************************************************************
//
//! SPI Master mode main loop
//!
//! This function configures SPI module as master and enables the channel for
//! communication
//!
//! \return None.
//
//*****************************************************************************
void MasterMain()
{

    unsigned long ulUserData;
    unsigned long ulDummy;

    //
    // Initialize the message
    //
    memcpy(g_ucTxBuff,MASTER_MSG,sizeof(MASTER_MSG));

    //
    // Set Tx buffer index
    //
    ucTxBuffNdx = 0;
    ucRxBuffNdx = 0;

    //
    // Reset SPI
    //
    SPIReset(GSPI_BASE);

    //
    // Configure SPI interface
    //
    SPIConfigSetExpClk(GSPI_BASE,MAP_PRCMPeripheralClockGet(PRCM_GSPI),
                     SPI_IF_BIT_RATE,SPI_MODE_MASTER,SPI_SUB_MODE_0,
                     (SPI_SW_CTRL_CS |
                     SPI_4PIN_MODE |
                     SPI_TURBO_OFF |
                     SPI_CS_ACTIVELOW |
                     SPI_WL_16));

    //
    // Enable SPI for communication
    //
    SPIEnable(GSPI_BASE);

    //
    // Print mode on uart
    //
    Message("Enabled SPI Interface in Master Mode\n\r");


    //
    // Send the string to slave. Chip Select(CS) needs to be
    // asserted at start of transfer and deasserted at the end.
    //
    SPITransfer(GSPI_BASE,g_ucTxBuff,g_ucRxBuff,50,
            SPI_CS_ENABLE|SPI_CS_DISABLE);

    //
    // Initialize variable
    //
    ulUserData = 0;


}


/***************************************************************************************
*    Title: Filter Code Definitions
*    Author: Shawn
*    Date: 10/29/2019
*    Code version: 1.0
*    Availability: https://sestevenson.wordpress.com/implementation-of-fir-filtering-in-c-part-1/
*
***************************************************************************************/
/*
// FIR init
void firFloatInit( void )
{
    memset( insamp, 0, sizeof( insamp ) );
}

// the FIR filter function
void firFloat( double *coeffs, double *input, double *output,
       int length, int filterLength )
{
    double acc;     // accumulator for MACs
    double *coeffp; // pointer to coefficients
    double *inputp; // pointer to input samples
    int n;
    int k;

    // put the new samples at the high end of the buffer
    memcpy( &insamp[filterLength - 1], input,
            length * sizeof(double) );

    // apply the filter to each input sample
    for ( n = 0; n < length; n++ ) {
        // calculate output n
        coeffp = coeffs;
        inputp = &insamp[filterLength - 1 + n];
        acc = 0;
        for ( k = 0; k < filterLength; k++ ) {
            acc += (*coeffp++) * (*inputp--);
        }
        output[n] = acc;
    }
    // shift input samples back in time for next time
    memmove( &insamp[0], &insamp[length],
            (filterLength - 1) * sizeof(double) );

}
*/

//Helper function to find local max
float findMax(float* InArray, int size, int* index, float* temp){
    int i = 0;
    temp = InArray;
    float max = -1;
    for(i = 0; i < size; i++){
        if(temp[i] > max){
            max = temp[i];
            *index = i;
        }
    }
    return max;

}


//Helper function to find local min
float findMin(float* InArray, int size, int* index, float* temp){
    int i = 0;

    temp = InArray;
    float min = 10000;
    for(i = 0; i < size; i++){
        if(temp[i] < min){
            min = temp[i];
            *index = i;
        }
    }
    return min;

}

//****************************************************************************
//! Defines Mqtt_Pub_Message_Receive event handler.
//! Client App needs to register this event handler with sl_ExtLib_mqtt_Init 
//! API. Background receive task invokes this handler whenever MQTT Client 
//! receives a Publish Message from the broker.
//!
//!\param[out]     topstr => pointer to topic of the message
//!\param[out]     top_len => topic length
//!\param[out]     payload => pointer to payload
//!\param[out]     pay_len => payload length
//!\param[out]     retain => Tells whether its a Retained message or not
//!\param[out]     dup => Tells whether its a duplicate message or not
//!\param[out]     qos => Tells the Qos level
//!
//!\return none
//****************************************************************************
static void
Mqtt_Recv(void *app_hndl, const char  *topstr, long top_len, const void *payload,
                       long pay_len, bool dup,unsigned char qos, bool retain)
{
    
    char *output_str=(char*)malloc(top_len+1);
    memset(output_str,'\0',top_len+1);
    strncpy(output_str, (char*)topstr, top_len);
    output_str[top_len]='\0';

    UART_PRINT("\n\rPublish Message Received");
    UART_PRINT("\n\r Test Test");                 ////////////////////////////////////////////////////////
    UART_PRINT("\n\rTopic: ");
    UART_PRINT("%s",output_str);
    free(output_str);
    UART_PRINT(" [Qos: %d] ",qos);
    if(retain)
      UART_PRINT(" [Retained]");
    if(dup)
      UART_PRINT(" [Duplicate]");
    
    output_str=(char*)malloc(pay_len+1);
    memset(output_str,'\0',pay_len+1);
    strncpy(output_str, (char*)payload, pay_len);
    output_str[pay_len]='\0';
    UART_PRINT("\n\rData is: ");
    UART_PRINT("\n\r Test2 Test2");
    UART_PRINT("%s",(char*)output_str);
    if(output_str != NULL){
        event_msg msg;
        msg.event = HBR_REQUESTED;
        msg.hndl = NULL;
        //
        // write message indicating exit from sending loop
        //
        osi_MsgQWrite(&g_PBQueue,&msg,OSI_NO_WAIT);
    }
    UART_PRINT("\n\r");
    free(output_str);
    
    return;
}

//****************************************************************************
//! Defines sl_MqttEvt event handler.
//! Client App needs to register this event handler with sl_ExtLib_mqtt_Init 
//! API. Background receive task invokes this handler whenever MQTT Client 
//! receives an ack(whenever user is in non-blocking mode) or encounters an error.
//!
//! param[out]      evt => Event that invokes the handler. Event can be of the
//!                        following types:
//!                        MQTT_ACK - Ack Received 
//!                        MQTT_ERROR - unknown error
//!                        
//!  
//! \param[out]     buf => points to buffer
//! \param[out]     len => buffer length
//!       
//! \return none
//****************************************************************************
static void
sl_MqttEvt(void *app_hndl, long evt, const void *buf,unsigned long len)
{
    int i;
    switch(evt)
    {
      case SL_MQTT_CL_EVT_PUBACK:
        UART_PRINT("PubAck:\n\r");
        UART_PRINT("%s\n\r",buf);
        break;
    
      case SL_MQTT_CL_EVT_SUBACK:
        UART_PRINT("\n\rGranted QoS Levels are:\n\r");
        
        for(i=0;i<len;i++)
        {
          UART_PRINT("QoS %d\n\r",((unsigned char*)buf)[i]);
        }
        break;
        
      case SL_MQTT_CL_EVT_UNSUBACK:
        UART_PRINT("UnSub Ack \n\r");
        UART_PRINT("%s\n\r",buf);
        break;
    
      default:
        break;
  
    }
}

//****************************************************************************
//
//! callback event in case of MQTT disconnection
//!
//! \param app_hndl is the handle for the disconnected connection
//!
//! return none
//
//****************************************************************************
static void
sl_MqttDisconnect(void *app_hndl)
{
    connect_config *local_con_conf;
    event_msg msg;
    local_con_conf = app_hndl;
    msg.hndl = app_hndl;
    msg.event = BROKER_DISCONNECTION;

    UART_PRINT("disconnect from broker %s\r\n",
           (local_con_conf->broker_config).server_info.server_addr);
    local_con_conf->is_connected = false;
    //
    // write message indicating publish message
    //
    osi_MsgQWrite(&g_PBQueue,&msg,OSI_NO_WAIT);

}

//****************************************************************************
//
//! Push Button Handler3(GPIOS3). Press push button3 (GPIOSW3) Whenever user
//! wants to publish a message. Write message into message queue signaling the
//!    event publish messages
//!
//! \param none
//!
//! return none
//
//****************************************************************************
void pushButtonInterruptHandler3()
{
    event_msg msg;
    msg.event = HBR_REQUESTED;
    msg.hndl = NULL;
    //
    // write message indicating exit from sending loop
    //
    osi_MsgQWrite(&g_PBQueue,&msg,OSI_NO_WAIT);

}


//*****************************************************************************
//
//! Periodic Timer Interrupt Handler
//!
//! \param None
//!
//! \return None
//
//*****************************************************************************
void
TimerPeriodicIntHandler(void)
{
    unsigned long ulInts;

    //
    // Clear all pending interrupts from the timer we are
    // currently using.
    //
    ulInts = MAP_TimerIntStatus(TIMERA0_BASE, true);
    MAP_TimerIntClear(TIMERA0_BASE, ulInts);

    //
    // Increment our interrupt counter.
    //
    g_usTimerInts++;
    if(!(g_usTimerInts & 0x1))
    {
        //
        // Off Led
        //
        GPIO_IF_LedOff(MCU_RED_LED_GPIO);
    }
    else
    {
        //
        // On Led
        //
        GPIO_IF_LedOn(MCU_RED_LED_GPIO);
    }
}

//****************************************************************************
//
//! Function to configure and start timer to blink the LED while device is
//! trying to connect to an AP
//!
//! \param none
//!
//! return none
//
//****************************************************************************
void LedTimerConfigNStart()
{
    //
    // Configure Timer for blinking the LED for IP acquisition
    //
    Timer_IF_Init(PRCM_TIMERA0,TIMERA0_BASE,TIMER_CFG_PERIODIC,TIMER_A,0);
    Timer_IF_IntSetup(TIMERA0_BASE,TIMER_A,TimerPeriodicIntHandler);
    Timer_IF_Start(TIMERA0_BASE,TIMER_A,100);
}

//****************************************************************************
//
//! Disable the LED blinking Timer as Device is connected to AP
//!
//! \param none
//!
//! return none
//
//****************************************************************************
void LedTimerDeinitStop()
{
    //
    // Disable the LED blinking Timer as Device is connected to AP
    //
    Timer_IF_Stop(TIMERA0_BASE,TIMER_A);
    Timer_IF_DeInit(TIMERA0_BASE,TIMER_A);

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
void BoardInit(void)
{
    /* In case of TI-RTOS vector table is initialize by OS itself */
    #ifndef USE_TIRTOS
    //
    // Set vector table base
    //
    #if defined(ccs)
        IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
    #endif
    #if defined(ewarm)
        IntVTableBaseSet((unsigned long)&__vector_table);
    #endif
    #endif
    //
    // Enable Processor
    //
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);

    PRCMCC3200MCUInit();
}


int HBR_Detection(void){
    unsigned long ulRecvData;
    int sampSize = 0;
    int j = 0;
    int k = 0;
    int p = 0;
    float realV = 0;
    unsigned long prevValue = 0;
    float localMax = 0;
    float localMin = 0;

    float *floatInput;
    floatInput = (float *)malloc(NUMSAMPLE * sizeof(float));          //2000
    //float floatInput[NUMSAMPLE];
    float *floatInputHalf;
    floatInputHalf = (float *)malloc(NUMSAMPLE/5 * sizeof(float));

    int tempMaxIdx;
    int tempMinIdx;
    int localMaxIdx;
    int localMinIdx;
    int numDrops;
    int HBR;
    int traverseSize = 10;
    float maxDiff;
    float localDiff;
    unsigned long startTime = 0;
    unsigned long endTime = 0;
    float durationTime = 0;
    int sucTime = 0;
    int lastHBR = 0;
    int numFails = 0;


    float *tempArray;
    tempArray = (float *)malloc(traverseSize * sizeof(float));       //malloc an array with size traverseSize
    float *tempMax;
    tempMax = (float *)malloc(traverseSize * sizeof(float));
    float *tempMin;
    tempMin = (float *)malloc(traverseSize * sizeof(float));

    g_ulA2IntCount = 0;


    // Configuring the timers
    //
    Timer_IF_Init(PRCM_TIMERA1, TIMERA1_BASE, TIMER_CFG_PERIODIC, TIMER_A, 0);

    //
    // Setup the interrupts for the timer timeouts.
    //
    Timer_IF_IntSetup(TIMERA1_BASE, TIMER_A, TimerA1IntHandler);

    Timer_IF_Start(TIMERA1_BASE, TIMER_A, 1); //10ms

    while(1){
        sampSize = 0;
        j=0;
        k = 1;
        Message("Detecting....Don't move.\n\r");
        startTime = g_ulA2IntCount;
        while(j<NUMSAMPLE)
        {
            //
            // Enable Chip select
            //
            SPICSEnable(GSPI_BASE);

            SPIDataPut(GSPI_BASE,g_ucTxBuff[ucTxBuffNdx%TR_BUFF_SIZE]);
            ucTxBuffNdx++;

            SPIDataGet(GSPI_BASE,&ulRecvData);
            g_ucTxBuff[ucRxBuffNdx%TR_BUFF_SIZE] = ulRecvData;
            ulRecvData = ulRecvData >> 3;
            ulRecvData = (ulRecvData & 0x7FF);

            realV = (float)(ulRecvData * 3.3 / 1024);
            //Report("%f\n\r",realV);

            //Take every 3 samples
            if(k == 3){
                //prevValue = g_ulA2IntCount;
                floatInput[j] = realV;
                //Report("%f\n\r",floatInput[j]);
                ucRxBuffNdx++;
                j++;
                k = 0;
            }

            k++;


           //
           // Disable chip select
           //
           SPICSDisable(GSPI_BASE);
        }

        endTime = g_ulA2IntCount;
        durationTime = (int)(endTime - startTime) * 0.001;
        Report("duration time is %f \n\r", durationTime);

        int downRate = 5;

        //Down sampling
        k = 1;
        p=0;
        for(j = 0; j < NUMSAMPLE; j++){
            //Sampling for every 5 samples
            if(k == downRate){
                floatInputHalf[p] = floatInput[j];
                //Report("    %f\n\r",floatInputHalf[p]);
                p++;
                k = 0;
            }
            k++;
        }

        sampSize = NUMSAMPLE/(downRate);
        tempMaxIdx = -1;
        tempMinIdx = -1;
        localMaxIdx = -1;
        localMinIdx = -1;
        numDrops = 0;
        HBR = 0;
        traverseSize = 10;
        maxDiff = 0;
        localDiff = 0;

        for(k = 0; k < sampSize - traverseSize; k++){
            for(j = 0; j < traverseSize; j++){
                tempArray[j] = floatInputHalf[k+j];
                //Report("realV is %f \n\r", tempArray[j]);
            }
            //tempArray = &(floatInputHalf[k]);
            localMax = findMax(tempArray, traverseSize, &tempMaxIdx, tempMax);
            localMin = findMin(tempArray, traverseSize, &tempMinIdx, tempMin);
            localDiff = localMax - localMin;
            if((localDiff > maxDiff) && (tempMaxIdx < tempMinIdx) && (localDiff < 0.1)){
                maxDiff = localDiff;
            }
        }

        //Report("        maxDiff is %f \n\r", maxDiff);

        k = 0;
        while(k < (sampSize - traverseSize)){
            for(j = 0; j < traverseSize; j++){
                tempArray[j] = floatInputHalf[k+j];
                //Report("realV is %f \n\r", tempArray[j]);
            }
            //tempArray = &(floatInputHalf[k]);
            localMax = findMax(tempArray, traverseSize, &tempMaxIdx, tempMax);
            localMin = findMin(tempArray, traverseSize, &tempMinIdx, tempMin);
            if((localMax-localMin) > (maxDiff * 0.6) && (tempMaxIdx < tempMinIdx)){//Drop found
                localMaxIdx = tempMaxIdx + k;
                localMinIdx = tempMinIdx + k;
                numDrops++;
                k = localMinIdx - 1;        //Increment by 1 outside
            }
            k++;
        }

        Report("Num Drop is %d \n\r", numDrops);
        HBR = (int)(numDrops / durationTime * 60);

        //Report("Your Heart Beat Rate is %d bpm\n\r", HBR);



        if(HBR > 50 && HBR < 200 && sucTime == 0){
            Report("Your Heart Beat Rate is %d bpm\n\r", HBR);
            lastHBR = HBR;
            sucTime++;
        }else if(HBR > 50 && HBR < 200 && sucTime == 1){
            if((HBR < lastHBR + 10) && (HBR > lastHBR - 10)){
                HBR = (lastHBR + HBR) / 2;
                Report("Measurement Completed. Your Heart Beat Rate is %d bpm\n\r", HBR);
                break;
            }
        }else{
            Message("Measurement Failed: Keep your finger on!\n\r");
            numFails++;
            lastHBR = 0;
            sucTime = 0;
            if(numFails >= 5){
                HBR = 0;
                break;
            }
        }


    }


    free(floatInput);
    TimerDisable(TIMERA1_BASE, TIMER_A);
    TimerIntDisable(TIMERA1_BASE, TIMER_TIMA_TIMEOUT);


    return HBR;
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
    UART_PRINT("\t\t    CC3200 %s Application       \n\r", AppName);
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\n\n\n\r");
}
  
extern volatile unsigned long g_ulStatus;

//*****************************************************************************
//
//! Task implementing MQTT client communication to other web client through
//!    a broker
//!
//! \param  none
//!
//! This function
//!    1. Initializes network driver and connects to the default AP
//!    2. Initializes the mqtt library and set up MQTT connection configurations
//!    3. set up the button events and their callbacks(for publishing)
//!    4. handles the callback signals
//!
//! \return None
//!
//*****************************************************************************
void MqttClient(void *pvParameters)
{
    
    long lRetVal = -1;
    int iCount = 0;
    int iNumBroker = 0;
    int iConnBroker = 0;
    event_msg RecvQue;
    unsigned char policyVal;
    


    connect_config *local_con_conf = (connect_config *)app_hndl;

    //
    // Configure LED
    //
    GPIO_IF_LedConfigure(LED1|LED2|LED3);

    GPIO_IF_LedOff(MCU_RED_LED_GPIO);
    GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);

    //
    // Reset The state of the machine
    //
    Network_IF_ResetMCUStateMachine();

    //
    // Start the driver
    //
    lRetVal = Network_IF_InitDriver(ROLE_STA);
    if(lRetVal < 0)
    {
       UART_PRINT("Failed to start SimpleLink Device\n\r",lRetVal);
       LOOP_FOREVER();
    }

    // switch on Green LED to indicate Simplelink is properly up
    GPIO_IF_LedOn(MCU_ON_IND);

    // Start Timer to blink Red LED till AP connection
    LedTimerConfigNStart();

    // Initialize AP security params
    SecurityParams.Key = (signed char *)SECURITY_KEY;
    SecurityParams.KeyLen = strlen(SECURITY_KEY);
    SecurityParams.Type = SECURITY_TYPE;

    //
    // Connect to the Access Point
    //
    lRetVal = Network_IF_ConnectAP(SSID_NAME, SecurityParams);
    if(lRetVal < 0)
    {
       UART_PRINT("Connection to an AP failed\n\r");
       LOOP_FOREVER();
    }

    set_time();
    if(lRetVal < 0){
      UART_PRINT("Unable to set time in the device");
    }

    lRetVal = sl_WlanProfileAdd(SSID_NAME,strlen(SSID_NAME),0,&SecurityParams,0,1,0);

    //set AUTO policy
    lRetVal = sl_WlanPolicySet(SL_POLICY_CONNECTION,
                      SL_CONNECTION_POLICY(1,0,0,0,0),
                      &policyVal, 1 );//PolicyValLen);

    //
    // Disable the LED blinking Timer as Device is connected to AP
    //
    LedTimerDeinitStop();

    //
    // Switch ON RED LED to indicate that Device acquired an IP
    //
    GPIO_IF_LedOn(MCU_IP_ALLOC_IND);

    UtilsDelay(20000000);

    GPIO_IF_LedOff(MCU_RED_LED_GPIO);
    GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
    GPIO_IF_LedOff(MCU_GREEN_LED_GPIO);

    //
    // Register Push Button Handlers
    //
    //Button_IF_Init(pushButtonInterruptHandler2,pushButtonInterruptHandler3);

    //
    // Initialze MQTT client lib
    //
    lRetVal = sl_ExtLib_MqttClientInit(&Mqtt_Client);
    if(lRetVal != 0)
    {
        // lib initialization failed
        UART_PRINT("MQTT Client lib initialization failed\n\r");
        LOOP_FOREVER();
    }
    
    /******************* connection to the broker ***************************/
    iNumBroker = sizeof(usr_connect_config)/sizeof(connect_config);
    if(iNumBroker > MAX_BROKER_CONN)
    {
        UART_PRINT("Num of brokers are more then max num of brokers\n\r");
        LOOP_FOREVER();
    }

connect_to_broker:
    while(iCount < iNumBroker)
    {
        //create client context
        local_con_conf[iCount].clt_ctx =
        sl_ExtLib_MqttClientCtxCreate(&local_con_conf[iCount].broker_config,
                                      &local_con_conf[iCount].CallBAcks,
                                      &(local_con_conf[iCount]));

        //
        // Set Client ID
        //
        sl_ExtLib_MqttClientSet((void*)local_con_conf[iCount].clt_ctx,
                            SL_MQTT_PARAM_CLIENT_ID,
                            local_con_conf[iCount].client_id,
                            strlen((char*)(local_con_conf[iCount].client_id)));

        //
        // Set will Params
        //
        if(local_con_conf[iCount].will_params.will_topic != NULL)
        {
            sl_ExtLib_MqttClientSet((void*)local_con_conf[iCount].clt_ctx,
                                    SL_MQTT_PARAM_WILL_PARAM,
                                    &(local_con_conf[iCount].will_params),
                                    sizeof(SlMqttWill_t));
        }

        //
        // setting username and password
        //
        if(local_con_conf[iCount].usr_name != NULL)
        {
            sl_ExtLib_MqttClientSet((void*)local_con_conf[iCount].clt_ctx,
                                SL_MQTT_PARAM_USER_NAME,
                                local_con_conf[iCount].usr_name,
                                strlen((char*)local_con_conf[iCount].usr_name));

            if(local_con_conf[iCount].usr_pwd != NULL)
            {
                sl_ExtLib_MqttClientSet((void*)local_con_conf[iCount].clt_ctx,
                                SL_MQTT_PARAM_PASS_WORD,
                                local_con_conf[iCount].usr_pwd,
                                strlen((char*)local_con_conf[iCount].usr_pwd));
            }
        }

        //
        // connectin to the broker
        //
        if((sl_ExtLib_MqttClientConnect((void*)local_con_conf[iCount].clt_ctx,
                            local_con_conf[iCount].is_clean,
                            local_con_conf[iCount].keep_alive_time) & 0xFF) != 0)
        {
            UART_PRINT("\n\rBroker connect fail for conn no. %d \n\r",iCount+1);
            
            //delete the context for this connection
            sl_ExtLib_MqttClientCtxDelete(local_con_conf[iCount].clt_ctx);
            
            break;
        }
        else
        {
            UART_PRINT("\n\rSuccess: conn to Broker no. %d\n\r ", iCount+1);
            local_con_conf[iCount].is_connected = true;
            iConnBroker++;
        }

        //
        // Subscribe to topics
        //

        if(sl_ExtLib_MqttClientSub((void*)local_con_conf[iCount].clt_ctx,
                                   local_con_conf[iCount].topic,
                                   local_con_conf[iCount].qos, TOPIC_COUNT) < 0)
        {
            UART_PRINT("\n\r Subscription Error for conn no. %d\n\r", iCount+1);
            UART_PRINT("Disconnecting from the broker\r\n");
            sl_ExtLib_MqttClientDisconnect(local_con_conf[iCount].clt_ctx);
            local_con_conf[iCount].is_connected = false;
            
            //delete the context for this connection
            sl_ExtLib_MqttClientCtxDelete(local_con_conf[iCount].clt_ctx);
            iConnBroker--;
            break;
        }
        else
        {
            int iSub;
            UART_PRINT("Client subscribed on following topics:\n\r");
            for(iSub = 0; iSub < local_con_conf[iCount].num_topics; iSub++)
            {
                UART_PRINT("%s\n\r", local_con_conf[iCount].topic[iSub]);
            }
        }
        iCount++;
    }

    if(iConnBroker < 1)
    {
        //
        // no succesful connection to broker
        //
        goto end;
    }

    iCount = 0;

    for(;;)
    {
        osi_MsgQRead( &g_PBQueue, &RecvQue, OSI_WAIT_FOREVER);
        if(HBR_REQUESTED == RecvQue.event)
        {
            UART_PRINT("Heart Rate Detecting...\n\r");

            int HBR = HBR_Detection();

            char data_send [10];

            //char *data_send = "80";

            UART_PRINT("HBR: %d\n\r",HBR);

            itoa((short)HBR, data_send);

            //strcat(data_send, "\0");

            //UART_PRINT("HBR_String: %s\n\r",data_send);
            //Button_IF_EnableInterrupt(SW3);
            //
            // send publish message
            //
            sl_ExtLib_MqttClientSend((void*)local_con_conf[iCount].clt_ctx,
                    pub_topic_hbr,(const void*)data_send,strlen((char*)data_send),QOS1,RETAIN);
            UART_PRINT("\n\r CC3200 Publishes the following message \n\r");
            UART_PRINT("Topic: %s\n\r",pub_topic_hbr);
            UART_PRINT("Data: %s\n\r",data_send);
        }
        else if(BROKER_DISCONNECTION == RecvQue.event)
        {
            iConnBroker--;
            /* Derive the value of the local_con_conf or clt_ctx from the message */
            sl_ExtLib_MqttClientCtxDelete(((connect_config*)(RecvQue.hndl))->clt_ctx);

            if(!IS_CONNECTED(g_ulStatus))
            {
                UART_PRINT("device has disconnected from AP \n\r");

                UART_PRINT("retry connection to the AP\n\r");

                while(!(IS_CONNECTED(g_ulStatus)) || !(IS_IP_ACQUIRED(g_ulStatus)))
                {
                    osi_Sleep(10);
                }
                goto connect_to_broker;

            }
            if(iConnBroker < 1)
            {
                //
                // device not connected to any broker
                //
                goto end;
            }
        }
    }
end:
    //
    // Deinitializating the client library
    //
    sl_ExtLib_MqttClientExit();
    UART_PRINT("\n\r Exiting the Application\n\r");
    
    LOOP_FOREVER();
}

//*****************************************************************************
//
//! Main 
//!
//! \param  none
//!
//! This function
//!    1. Invokes the SLHost task
//!    2. Invokes the MqttClient
//!
//! \return None
//!
//*****************************************************************************
void main()
{ 
    long lRetVal = -1;
    //
    // Initialize the board configurations
    //
    BoardInit();

    //
    // Pinmux for UART
    //
    PinMuxConfig();

    //
    // Configuring UART
    //
    InitTerm();

    //
    // Clearing the Terminal.
    //
    ClearTerm();

    //
    // Display Application Banner
    //
    DisplayBanner("Heart Beat Monitoring - Patient");

    //
    // Reset the peripheral
    //
    PRCMPeripheralReset(PRCM_GSPI);

    MasterMain();

    //
    // Start the SimpleLink Host
    //
    lRetVal = VStartSimpleLinkSpawnTask(SPAWN_TASK_PRIORITY);
    if(lRetVal < 0)
    {
        ERR_PRINT(lRetVal);
        LOOP_FOREVER();
    }

    //
    // Start the MQTT Client task
    //
    osi_MsgQCreate(&g_PBQueue,"PBQueue",sizeof(event_msg),10);
    lRetVal = osi_TaskCreate(MqttClient,
                            (const signed char *)"Mqtt Client App",
                            OSI_STACK_SIZE, NULL, 2, NULL );

    if(lRetVal < 0)
    {
        ERR_PRINT(lRetVal);
        LOOP_FOREVER();
    }
    //
    // Start the task scheduler
    //
    osi_start();
}

//*****************************************************************************
//
//! This function updates the date and time of CC3200.
//!
//! \param None
//!
//! \return
//!     0 for success, negative otherwise
//!
//*****************************************************************************

static int set_time()
{
    long retVal;

    g_time.tm_day = DATE;
    g_time.tm_mon = MONTH;
    g_time.tm_year = YEAR;
    g_time.tm_sec = HOUR;
    g_time.tm_hour = MINUTE;
    g_time.tm_min = SECOND;

    retVal = sl_DevSet(SL_DEVICE_GENERAL_CONFIGURATION,
                          SL_DEVICE_GENERAL_CONFIGURATION_DATE_TIME,
                          sizeof(SlDateTime),(unsigned char *)(&g_time));

    ASSERT_ON_ERROR(retVal);
    return SUCCESS;
}
