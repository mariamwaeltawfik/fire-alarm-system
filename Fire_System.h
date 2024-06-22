
#ifndef FIRE_SYSTEM_H_
#define FIRE_SYSTEM_H_



#define YELLOW_LED  PINB4
#define RED_LED     PINB5
#define BUZZ        PINB6
#define DAGER_TEMP  500
#define TEMP_CH     CH0

void Fire_Init(void);
void Normal(void);
void Heat(void);
void Fire(void);
void Fire_System_Runnable(void);



#endif /* FIRE_SYSTEM_H_ */