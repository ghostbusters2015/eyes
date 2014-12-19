#define  LED      PORTA.0      // RUN LED 
#define  CLK      PORTD.1      // 24bit A/D 모듈 CK (PD_SCK)
#define  DATA    PIND.0         // 24Bit A/D 모듈 DO (DOUT)

unsigned long read_AD(void) 
{
	int i;
	unsigned long raw;   // 24Bit 로드셀 데이타 변수
	DATA=1;
	CLK=0;
	raw=0; 
	while(DATA);           // DO핀을 체크하면서 A/D 내부 변환완료시까지 기다림 
	for(i=0; i<24; i++) {   //  24Bit 데이타 읽기 
		CLK=1;
		raw <<= 1;
		CLK=0;
		if (DATA) raw++;
	}
	CLK=1;                  // CH_A Gain 128 (다음 A/D 변환시 게인 128로 설정, B 채널은 사용하지 않음)  
	//raw = raw ^ 0x800000;   // 2' 보수 (A/D 변환값이 2'보수로 출력되기 때문)
	CLK=0;                 // CLK 를 low 로 만듬 
	return raw;
}

void main(void)
{    
	unsigned long val;
	signed int rval;

	init_devices();
	printf("loadcell 24bit A/D module V1.0\r\n");

	while (1) { 
		val=read_AD();                                           // 센서 raw data 읽기  
		rval=(val>>8) & 0xffff;                                  // 24비트 데이타를 16 비트로 변경  
		LED ^= 1;                                                  // RUN LED 토글 
		printf("%lx %ld %d\r\n", val, val, rval);      // 24비트 raw hex, 24비트 raw dec, 16비트 raw dec 출력  
	}
}
