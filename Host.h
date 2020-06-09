extern int ComPortHost;

extern int (*ff_putch[5])(int ch);

extern int flag_echo_host;
extern int flag_Deb;

#define ToutAnsHost 100
#define ToutAnsHost_rtu 100

// типы данных для dis_set,dis_set_MMI
#define T_FLOAT 1
#define T_INT   2
#define T_INT_L  3
#define T_HEX  4
#define T_HX2  5
#define T_HX4  6
#define T_STR  7
#define T_DBL  8
#define T_HEX_L 9

#define RD_INT    1
#define RD_LONG   2
#define RD_FLOAT  3
#define RD_STRING 4
#define RD_COIL   5

#define WR_INT    6
#define WR_LONG   7
#define WR_FLOAT  8
#define WR_STRING 9
#define WR_COIL   10



#define wrd_w 33
#define HEX_MAX       0xffffff
#define BIG_P (10e30)
#define BIGL_P (0x7fffffff)
#define BIG_I (32767)

#define icp_mod_max 20