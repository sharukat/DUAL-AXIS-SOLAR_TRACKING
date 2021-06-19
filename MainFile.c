/*
 * File:   MainFile.c
 * Author: Sharuka
 *
 * Created on February 18, 2018, 10:13 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "config.h"



#define _XTAL_FREQ 8000000

#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6
#define D7 RD7


typedef uint16_t adc_result_t;

adc_result_t LDR1_Value;
adc_result_t LDR2_Value;
adc_result_t LDR3_Value;
adc_result_t LDR4_Value;
adc_result_t Previous_LDR1_Value;
adc_result_t Previous_LDR2_Value;
adc_result_t Previous_LDR3_Value;
adc_result_t Previous_LDR4_Value;


typedef enum
{
    LDR1 =  0x0,
    LDR2 =  0x1,
    LDR4 =  0x5,
    LDR3 =  0x6,
    channel_FVR =  0xF
} adc_channel_t;



void ADC_Initilize(){
       
     ADCON0bits.ADCS0=0;
    ADCON0bits.ADCS1=1; // SETTING CLOCK TO DIVIDE BY 64
    ADCON1bits.ADCS2=1;
    ADCON1bits.ADFM=1; //RIGHT JUSTIFIED
    
    //CONFIGURATION OF PINS
    ADCON1bits.PCFG0=0;
    ADCON1bits.PCFG1=0;
    ADCON1bits.PCFG2=0;
    ADCON1bits.PCFG3=1;
    
    //SET CHANNEL NO:1
    ADCON0bits.CHS0=0;
    ADCON0bits.CHS1=0;
    ADCON0bits.CHS2=0;
    
    //SET CHANNEL NO:2
    ADCON0bits.CHS0=0;
    ADCON0bits.CHS1=0;
    ADCON0bits.CHS2=1;
    
    //SET CHANNEL NO:5
    ADCON0bits.CHS0=1;
    ADCON0bits.CHS1=0;
    ADCON0bits.CHS2=1;
    
    //SET CHANNEL NO:6
    ADCON0bits.CHS0=0;
    ADCON0bits.CHS1=1;
    ADCON0bits.CHS2=1;
    
    //SET STATUS BIT
    ADCON0bits.GO_DONE=1;
    
    //START ADC
    ADCON0bits.ADON=1;
}

adc_result_t ADC_GetConversion(adc_channel_t channel)
{
    // select the A/D channel
    ADCON0bits.CHS = channel;    
    _delay(1000);
    // Turn on the ADC module
    ADCON0bits.ADON = 1;

    // Start the conversion
    ADCON0bits.GO_nDONE = 1;

    // Wait for the conversion to finish
    while (ADCON0bits.GO_nDONE)
    {
    }

    // Conversion finished, return the result
    return ((adc_result_t)((ADRESH << 8) + ADRESL));
}


void main(void) {
    TRISB = 0x00;
    PORTB = 0x00;
    TRISA = 0xff;
    
   
    TRISD = 0x00;
    Lcd_Init();
    
    ADC_Initilize();
    
    
    while(1){
        
    
        LDR1_Value = ADC_GetConversion(LDR1);
        LDR2_Value = ADC_GetConversion(LDR2);
        LDR3_Value = ADC_GetConversion(LDR3);
        LDR4_Value = ADC_GetConversion(LDR4);
        
         
        if (LDR1_Value!=Previous_LDR1_Value && LDR2_Value !=Previous_LDR2_Value){
            if(LDR1_Value>LDR2_Value){
                RB0 = 1;
                RB1 = 0;
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("Tracking Motor 1");
                
                //Lcd_Clear();
               
            }
            else if (LDR1_Value<LDR2_Value){
                RB0 = 0;
                RB1 = 1;
                Lcd_Set_Cursor(1,1);
                Lcd_Write_String("Tracking Motor 1");
                
                //Lcd_Clear();
            }
            else if (LDR1_Value==LDR2_Value){
                 RB0 = 0;
                 RB1 = 0;
                 Lcd_Set_Cursor(1,1);
                 Lcd_Write_String("....Charging....");
                 
                //Lcd_Clear();
            }
        }
        
        if (LDR3_Value != Previous_LDR3_Value && LDR4_Value != Previous_LDR4_Value ){
            if (LDR3_Value>LDR4_Value){
                RB2 = 1;
                RB3 = 0;
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String("Tracking Motor 2");
                
                //Lcd_Clear();
            }
            else if (LDR3_Value<LDR4_Value){
                RB2 = 0;
                RB3 = 1;
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String("Tracking Motor 2");
                
                //Lcd_Clear();
            }
            else if (LDR3_Value==LDR4_Value){
                RB2 = 0;
                RB3 = 0;
                Lcd_Set_Cursor(2,1);
                Lcd_Write_String("....Charging....");
                
                //Lcd_Clear();
            }
        }
        
       
        
       
       }
    
        
    
    return;
}
