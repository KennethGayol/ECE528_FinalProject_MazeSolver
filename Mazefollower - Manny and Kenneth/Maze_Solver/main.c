//@author Manny Vartanian and Kenneth Gayol

#include "msp.h"
#include "inc/Clock.h"
#include "inc/GPIO.h"
#include "inc/EUSCI_A0_UART.h"
#include "inc/Motor.h"
#include "inc/Timer_A1_Interrupt.h"
#include "inc/SysTick_Interrupt.h"
#include "inc/Reflectance_Sensor.h"

#define PWM_NOMINAL 2500

static uint32_t Timer_A1_ms_elapsed = 0;

static int turning = 0;
static uint32_t turn_start_time = 0;

static uint32_t white_start_time = 0;
static uint8_t all_white_flag = 0;

static uint32_t intersection_time = 0;
static uint8_t at_intersection = 0;

#define CENTER_MASK 0b00011000
#define LEFT_MASK   0b11100000
#define RIGHT_MASK  0b00000111
#define ALL_WHITE   0b00000000

#define LED_OFF      0x00
#define LED_RED      0x01
#define LED_GREEN    0x02
#define LED_YELLOW   0x03
#define LED_BLUE     0x04
#define LED_MAGENTA  0x05
#define LED_CYAN     0x06

static void Set_LED(uint8_t color)
{
    LED2_Output(color);
}

void Detect_Line_Position(uint8_t data)
{
    Set_LED(LED_OFF);

    if(turning != 0)
    {
        uint32_t t = Timer_A1_ms_elapsed - turn_start_time;

        if(turning == -1 && t < 1050)
        {
            Set_LED(LED_BLUE);
            Motor_Left(PWM_NOMINAL, PWM_NOMINAL);
            return;
        }
        else if(turning == 1 && t < 1050)
        {
            Set_LED(LED_RED);
            Motor_Right(PWM_NOMINAL, PWM_NOMINAL);
            return;
        }
        else if(turning == 2 && t < 2500)
        {
            Set_LED(LED_MAGENTA);
            Motor_Left(PWM_NOMINAL, PWM_NOMINAL);
            return;
        }
        else
        {
            turning = 0;
        }
    }

    if(data == ALL_WHITE)
    {
        if(!all_white_flag)
        {
            all_white_flag = 1;
            white_start_time = Timer_A1_ms_elapsed;
        }

        if((Timer_A1_ms_elapsed - white_start_time) > 300)
        {
            Set_LED(LED_CYAN);
            turning = 2;
            turn_start_time = Timer_A1_ms_elapsed;
            return;
        }
    }
    else
    {
        all_white_flag = 0;
    }

    uint8_t left = (data & LEFT_MASK);
    uint8_t right = (data & RIGHT_MASK);
    uint8_t center = (data & CENTER_MASK);

    if(center == CENTER_MASK)
    {
        if(left == LEFT_MASK)
        {
            if(!at_intersection)
            {
                at_intersection = 1;
                intersection_time = Timer_A1_ms_elapsed;
                Set_LED(LED_YELLOW);
            }

            if((Timer_A1_ms_elapsed - intersection_time) > 50)
            {
                turning = -1;
                turn_start_time = Timer_A1_ms_elapsed;
                at_intersection = 0;
                return;
            }
        }
        else if(right >= 0b00000111)
        {
            if(!at_intersection)
            {
                at_intersection = 1;
                intersection_time = Timer_A1_ms_elapsed;
                Set_LED(LED_YELLOW);
            }

            if((Timer_A1_ms_elapsed - intersection_time) > 50)
            {
                turning = 1;
                turn_start_time = Timer_A1_ms_elapsed;
                at_intersection = 0;
                return;
            }
        }
        else
        {
            at_intersection = 0;
        }
    }
    else
    {
        at_intersection = 0;
    }

    if(center == CENTER_MASK)
    {
        Set_LED(LED_GREEN);
        Motor_Forward(PWM_NOMINAL, PWM_NOMINAL);
    }
    else if(data & 0b00001000)
    {
        Set_LED(LED_GREEN);
        Motor_Forward(PWM_NOMINAL, PWM_NOMINAL/3);
    }
    else if(data & 0b00010000)
    {
        Set_LED(LED_GREEN);
        Motor_Forward(PWM_NOMINAL/3, PWM_NOMINAL);
    }
    else
    {
        Set_LED(LED_GREEN);
        Motor_Forward(PWM_NOMINAL/2, PWM_NOMINAL/2);
    }
}

void Timer_A1_Periodic_Task(void)
{
    Timer_A1_ms_elapsed++;

    if ((Timer_A1_ms_elapsed % 10) == 0)
    {
        Reflectance_Sensor_Start();
    }

    if ((Timer_A1_ms_elapsed % 10) == 1)
    {
        uint8_t data = Reflectance_Sensor_End();
        Detect_Line_Position(data);
    }
}

int main(void)
{
    DisableInterrupts();

    Clock_Init48MHz();

    LED1_Init();
    LED2_Init();

    Motor_Init();

    EUSCI_A0_UART_Init_Printf();

    Reflectance_Sensor_Init();

    SysTick_Interrupt_Init(SYSTICK_INT_NUM_CLK_CYCLES, SYSTICK_INT_PRIORITY);

    Timer_A1_Interrupt_Init(&Timer_A1_Periodic_Task, TIMER_A1_INT_CCR0_VALUE);

    EnableInterrupts();

    while(1){}
}
