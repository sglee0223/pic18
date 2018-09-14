#ifndef _ADC_H
#define	_ADC_H

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

void  init_AD_PORT(void);
unsigned long getADC(void);

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _ADC_H */
