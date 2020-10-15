extern struct arch_str_t ahour00;
extern struct arch_str_t tim_000;
extern struct arch_str_t adate00;

extern struct tm  mdat0;

void f_prepare_t(struct arch_str_t *s_arch);

extern unsigned char flg_sec,flg_min,flg_arc_h,flg_arc_d,flg_month;
extern int avg_old,dlt_tm;
extern int contrh;

extern unsigned char Size_str;
extern unsigned char mmi_str_[30];

struct consump //расходы 9 шт.
{
  float avg;
  float accum;
  float hour;
  float day;
  float month;
  float year;
  float last_hour;
  float last_day;
  float last_month;
};

void f_arch();
extern struct counter cnt;
extern struct consump cons;

void ConvToBynare (float val,unsigned char * bytes);
void WriteArchive (unsigned char typ_arc);
void SaveParameters ();
void InitializeMain (void);
void RestoreBasicParameters (void);
void FormateArchive (unsigned char buf[]);
unsigned char GetArcPoint (int *pointer,unsigned *segment);
void ClearFlashSeg (unsigned char page,unsigned pointer);
unsigned char ConvToFloatVerify (float *val,unsigned char a,unsigned char b,unsigned char c,unsigned char d);
void ClearArchive ();
void ReadFromArchive (unsigned char bufer[]);
unsigned char GetArcPoint (int *pointer,unsigned *segment);
void GetArcReadPoint(int *pointer,unsigned *segment,unsigned char *page);
void ByteToString (unsigned char val,unsigned char index,unsigned char typ);
void FloatToString (float val,unsigned char buf_str[],unsigned char offset);
void ClearBuffer (void);