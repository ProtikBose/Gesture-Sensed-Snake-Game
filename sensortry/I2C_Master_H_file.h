/* Define bit rate */
//#include <util/twi.h>
#define SCL_CLK 100000UL
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))


/*
Programming ATmega16 I2C in Master mode

To program ATmega16 I2C in master mode first need to initialize I2C module with SCL frequency and then need to perform read and write operation using START, REPEATED START, STOP events. Let first initialize TWI module in ATmega16.

I2C Initialization

Here to initializeATmega16 TWI in master mode we need to set SCL clock frequency by setting values in TWBR register and TWPS bits in TWSR register.

TWBR value is defined by above SCL frequency formula. e.g.*/


void I2C_Init()			/* I2C initialize function */
{
    TWBR = BITRATE(TWSR=0x00);	/* Get bit rate register value by formula */
}

/*
START (S)

START condition issued by master.
It is generated by High to Low transition on SDA while SCL is High.
While bus is free master device issue START condition with slave device address (SLA) to initiate transmission.
I2C_Start function

This function initiate START condition

Input argument : - it has input argument of slave device write address (SLA+W).

Return                 : - it returns status of event.*/

uint8_t I2C_Start_Wait(char write_address)/* I2C start function */
{
    uint8_t status;		/* Declare variable */
    TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT); /* Enable TWI, generate START */
    while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
    status=TWSR&0xF8;		/* Read TWI status register */
    if(status!=0x08)		/* Check weather START transmitted or not? */
    return 0;			/* Return 0 to indicate start condition fail */
    TWDR=write_address;		/* Write SLA+W in TWI data register */
    TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI & clear interrupt flag */
    while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
    status=TWSR&0xF8;		/* Read TWI status register */	
    if(status==0x18)		/* Check for SLA+W transmitted &ack received */
    return 1;			/* Return 1 to indicate ack received */
    if(status==0x20)		/* Check for SLA+W transmitted &nack received */
    return 2;			/* Return 2 to indicate nack received */
    else
    return 3;			/* Else return 3 to indicate SLA+W failed */
}
/*
REPEATED START (Sr)

REPEATED START condition issued by master.
It is generated by issuing another one START condition to initiate new transmission.
REPEATED START condition with slave device address (SLA) is issued in between START and STOP condition
I2C_Repeated_Start function

This function generates REPEATED START condition for read operation.

Input argument : - it has input argument of slave device read address (SLA+R).

Return                 : - it returns status of event.*/

uint8_t I2C_Repeated_Start(char read_address) /* I2C repeated start function */
{
    uint8_t status;		/* Declare variable */
    TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT);/* Enable TWI, generate start */
    while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
    status=TWSR&0xF8;		/* Read TWI status register */
    if(status!=0x10)		/* Check for repeated start transmitted */
    return 0;			/* Return 0 for repeated start condition fail */
    TWDR=read_address;		/* Write SLA+R in TWI data register */
    TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
    while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
    status=TWSR&0xF8;		/* Read TWI status register */
    if(status==0x40)		/* Check for SLA+R transmitted &ack received */
    return 1;			/* Return 1 to indicate ack received */
    if(status==0x20)		/* Check for SLA+R transmitted &nack received */
    return 2;			/* Return 2 to indicate nack received */
    else
    return 3;			/* Else return 3 to indicate SLA+W failed */
}

/*
WRITE (W)

WRITE data/address event is issued by master after successful acknowledgment of START received from slave device.
I2C_Write function

This function write data/address on bus

Input argument : - it has input argument of data/address.

Return                 : - it returns status of event.*/
uint8_t I2C_Write(char data)	/* I2C write function */
{
    uint8_t status;		/* Declare variable */
    TWDR=data;			/* Copy data in TWI data register */
    TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
    while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
    status=TWSR&0xF8;		/* Read TWI status register */
    if(status==0x28)		/* Check for data transmitted &ack received */
    return 0;			/* Return 0 to indicate ack received */
    if(status==0x30)		/* Check for data transmitted &nack received */
    return 1;			/* Return 1 to indicate nack received */
    else
    return 2;			/* Else return 2 for data transmission failure */
}
/*
READ (R)

Read data event is issued by master after successful REPEATED START condition.
I2C_Read_Ack function

This function read data available on SDA line and returnsits acknowledgment to slave device about data read successful and also tells slave to transmit another data.

Input argument : - it has no input argument.

Return                 : - it returnsreceived data.*/

char I2C_Read_Ack()		/* I2C read ack function */
{
    TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA); /* Enable TWI, generation of ack */
    while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
    return TWDR;			/* Return received data */
}
/*
I2C_Read_Nack function

This function read last needed data byte available on SDA line but do not returns acknowledgment of it. It used to indicate slave that master don’t want next data and want to stop communication.

Input argument : - it has no input argument.

Return                 : - it returns received data.*/
char I2C_Read_Nack()		/* I2C read nack function */
{
    TWCR=(1<<TWEN)|(1<<TWINT);	/* Enable TWI and clear interrupt flag */
    while(!(TWCR&(1<<TWINT)));	/* Wait until TWI finish its current job */
    return TWDR;		/* Return received data */
}
/*
STOP (P)

STOP event issued by master to indicate it has to stop.
It is generated by Low to High transition on SDA while SCL is High.
I2C_Stop function

This function initiate STOP condition

Input argument : - it has no input argument.

Return                 : - it not returnsany data type.*/

void I2C_Stop()			/* I2C stop function */
{
    TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);/* Enable TWI, generate stop */
    while(TWCR&(1<<TWSTO));	/* Wait until stop condition execution */
}