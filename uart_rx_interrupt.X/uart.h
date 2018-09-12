#ifndef _UART_H
#define	_UART_H

#include <xc.h> 

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

void init_uart(void);
void putch(unsigned char byte);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _UART_H */

