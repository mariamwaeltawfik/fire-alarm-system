#include "StdTypes.h"
#include "LCD_Int.h"
#include "KeyPad_Int.h"
#include "MOTOR_Int.h"
#include "ADC_Int.h"
#include "SENSORS_Int.h"
#include "Fire_System.h"
typedef enum {
	NORMAL_STATE=0,
	HEAT_STATE,
	FIRE_STATE
	
}Fire_moods_t;

static u8 mood=NORMAL_STATE;
u8 *str[10];
u8 *pass="12345";

void Fire_Init(void){
	
}
void Normal(void){
	
	if(mood==NORMAL_STATE){
    u16 temp=TEMP_Read(); 
	LCD_SetCursor(1,1);
	LCD_WriteString("Normal Mode");
	LCD_SetCursor(2,1);
	LCD_WriteString("T: ");
	LCD_WriteNumber(temp/10);
	LCD_WriteChar('.');
	LCD_WriteNumber(temp%10);
	LCD_WriteString("   ");
	MOTOR_Stop(M1);
	MOTOR_Stop(M2);
	DIO_WritePin(YELLOW_LED,LOW);
	DIO_WritePin(RED_LED,LOW);
	DIO_WritePin(BUZZ,LOW);
	if(temp>=DAGER_TEMP){
		mood=HEAT_STATE;
		LCD_Clear();
	}
	}
	

}
void Heat(void){
	  
		if(mood==HEAT_STATE){
			  u16 temp=TEMP_Read();
			  u16 smoke=MPX4115_Pressure();
			  u8 p=(((u32)smoke*100)/1300);
	    LCD_SetCursor(1,1);
	    LCD_WriteString("Heat Mode");
		LCD_WriteString("T:");
		LCD_WriteNumber(temp/10);
		LCD_WriteChar('.');
		LCD_WriteNumber(temp%10);
	    LCD_SetCursor(2,1);
	LCD_WriteString("Smoke: ");
	LCD_WriteNumber(p);
	LCD_WriteChar(37);
	LCD_WriteString("   ");
	
		
	    MOTOR_Stop(M1);
	    MOTOR_Stop(M2);
	    DIO_WritePin(YELLOW_LED,HIGH);
	    DIO_WritePin(RED_LED,LOW);
		DIO_WritePin(BUZZ,LOW);
		if(temp<=DAGER_TEMP-50&&p<48){
			mood=NORMAL_STATE;
			LCD_Clear();
		}
		else if(p>=50){
			mood=FIRE_STATE;
			LCD_Clear();
		}
		}
	    

}
void Fire(void){
	
	
	if(mood==FIRE_STATE){
		u16 temp=TEMP_Read();
		u16 smoke=MPX4115_Pressure();
		u8 k=KEYPAD_GetKey(),p=(((u32)smoke*100)/1300);
		static u8 i=0,flag=0,j=6,m,f=0;
	LCD_SetCursor(1,1);
	LCD_WriteString("T:");
	LCD_WriteNumber(temp/10);
	LCD_WriteChar('.');
	LCD_WriteNumber(temp%10);

	LCD_WriteString(" Smk: ");
	LCD_WriteNumber(p);
	LCD_WriteChar(37);
	LCD_WriteString("   ");
	
	LCD_SetCursor(2,1);
	LCD_WriteString("Pass ");
	if(k>='0'&& k<='9' && k!=NO_KEY){
		if (flag==0){
			
			LCD_SetCursor(2,6);
			LCD_WriteString("          ");
			flag=1;
		}
		LCD_SetCursor(2,j);
		j++;
		LCD_WriteChar('*');
		str[i]=k;
		i++;
	}
	else if(k== '='){
		str[i]=0;
		i++;
		for(m=0;m<i;m++){
			if(str[m]!=pass[m]){
				f=1;
			}
		}
		if(f==0&& temp<=DAGER_TEMP-50&&p<=48){
			mood=NORMAL_STATE;
			LCD_Clear();
		}
		else if(f==1&& temp<=DAGER_TEMP-50&&p<=48){
			LCD_SetCursor(2,6);
			LCD_WriteString("Wrong!");
			_delay_ms(500);
			LCD_SetCursor(2,6);
			LCD_WriteString("         ");
		}
		else if(f==1&& temp>=DAGER_TEMP&&p>=50){
			LCD_SetCursor(2,6);
			LCD_WriteString("Wrong!");
			_delay_ms(500);
			LCD_SetCursor(2,6);
			LCD_WriteString("         ");
		}
		else if(f==0&& temp>=DAGER_TEMP&&p>=50){
			LCD_SetCursor(2,6);
			LCD_WriteString("Danger!!");
			_delay_ms(500);
			LCD_SetCursor(2,6);
			LCD_WriteString("         ");
		}
		flag=0;
		i=0;
		j=6;
		f=0;
	}
	MOTOR_CW(M1);
	MOTOR_CW(M2);
	DIO_WritePin(YELLOW_LED,LOW);
	DIO_WritePin(RED_LED,HIGH);
	DIO_WritePin(BUZZ,HIGH);
	}
}
void Fire_System_Runnable(void){
	Normal();
	Heat();
	Fire();
}

