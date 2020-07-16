
struct arch_str_t tim_000 = {
0,34,1,
30,7,9
};
//-------------------
struct arch_str_t ahour00=
{
0,       //   sec
0,       //   min
8,       //   hour
};
//-------------------
struct arch_str_t adate00=
{
0,       //   sec
0,       //   min
0,       //   hour
1,       //   day
1,       //   month
9,       //   year
};
//-------------------
//                   sec min hour
struct tm  mhour00 = { 0, 0,  8 };
//                sec min hour day   month-1, year-1900
struct tm  mdat0= { 0, 34, 01,   30,     6,   109 };
struct tm  mdat = { 0, 0, 12,   25,     11,    88 };
time_t ltime0,ltime;
//----------------------------
//-------------------------------
void f_prepare_t(struct arch_str_t *s_arch)
{
  TIME_DATE tim_dat;
 GetTimeDate(&tim_dat);
 s_arch->year  = tim_dat.year -2000 ;
 s_arch->month = tim_dat.month ;
 s_arch->day   = tim_dat.day   ;
 s_arch->hour  = tim_dat.hour  ;
 s_arch->min   = tim_dat.minute;
 s_arch->sec   = tim_dat.sec   ;
}

//-------------------------------

unsigned char flg_sec,flg_min,flg_arc_h,flg_arc_d,flg_month,avg_old,dlt_tm;
int year,month,day,hour,min,sec;
unsigned char contrh=8;

unsigned char Size_str_min = 11; //размер структуры для записи 1-тип 6-дата,время 4-точка
unsigned char Size_str=27; //1-тип 6-дата,время 20 - 4 точки по 4 байта

#define Max_arch_record   20  /*макс длина архив записи в байтах*/

struct runtime
{
  unsigned char old_sec;
  unsigned char old_min;
  unsigned char old_hour;
  unsigned char old_month;
};
struct runtime Prt;

struct counter //счетчики для усреднений
{
  unsigned sec;
  unsigned min;
  unsigned hour;
  unsigned day;
  unsigned month;
};
struct counter cnt={0,0,0,0,0,};

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
struct consump cons={0,0,0,0,0,0,0,0,0,};

void f_arch()
{
  unsigned char cr[4];
  if (flg_sec == 1) //раз в секунду
  {
    GetDate(&year,&month,&day); GetTime(&hour,&min,&sec);
    //sec
    if (sec >= avg_old) dlt_tm=sec-avg_old;
    else dlt_tm=sec-avg_old+60;
    if (dlt_tm >= 3)
    {
      //усреднение секундных значений за 3 сек.
      avg_old=sec;
      cons.avg=(cons.avg*cnt.sec+consumption)/cnt.sec+1; //формула усреднения расхода
      cnt.sec++;
    }
    //min
    if (min != Prt.old_min) 
    {
      Prt.old_min=min;
      flg_min=1;
    }
    if (hour != Prt.old_hour)
    {
      Prt.old_hour=hour;
      flg_arc_h=1;
	    if (hour==contrh)
	    {
	      flg_arc_d=1;
        if (Prt.old_month != month)
	      { 
          Prt.old_month=month;
          WriteNVRAM(30,month);
          flg_month=1;
        }
	    }
    }
    flg_sec=0;
    //накопления расхода раз в сек. и запись в энергонезависимую память
    if(consumption>0) cons.accum+=consumption/3600;
    ConvToBynare(cons.accum,cr);
    X607_WriteFn(0x9000,4,cr);
  }
  if (flg_min == 1) //раз в минуту
  {
    flg_min=0;
    WriteArchive(2);  /*запись в минутный архив*/
    cons.hour += cons.accum;
    cons.day += cons.accum;
    cons.month += cons.accum;
    cons.year += cons.accum;
    cons.accum=0.0;
    cnt.sec=0;
    cons.avg=0.0;
    SaveParameters();
  }
  if (flg_arc_h == 1) //раз в час
  {
    flg_arc_h=0;
    WriteArchive(0);
    cons.last_hour += cons.hour;
    cons.hour=0.0;
  }
  if (flg_arc_d == 1) //раз в сутки
  {
	  flg_arc_d=0;
    WriteArchive(1);
    cons.last_day += cons.day;
    cons.day=0.0;
  }
	if (flg_month == 1) //раз в месяц
	{
	  flg_month=0;
    cons.last_month += cons.month;
    cons.month=0.0;
  }
}

/********* cохранение параметров точки учёта в ЭНП ***************/
void SaveParameters ()
{
  unsigned char j,k,ind,cr[4],buf[40];ind=0;
  for (j=0;j<8;j++) //8 колличество переменных
  {
    switch (j)
    {
      case 0:
        ConvToBynare(cons.accum,cr);for (k=0;k< 4;k++) buf[ind*4+k]=cr[k];ind++;
      break;
      case 1:
        ConvToBynare(cons.day,cr);for (k=0;k< 4;k++) buf[ind*4+k]=cr[k];ind++;
      break;
      case 2:
        ConvToBynare(cons.hour,cr);for (k=0;k< 4;k++) buf[ind*4+k]=cr[k];ind++;
      break;
      case 3:
        ConvToBynare(cons.last_day,cr);for (k=0;k< 4;k++) buf[ind*4+k]=cr[k];ind++;
      break;
      case 4:
        ConvToBynare(cons.last_hour,cr);for (k=0;k< 4;k++) buf[ind*4+k]=cr[k];ind++;
      break;
      case 5:
        ConvToBynare(cons.last_month,cr);for (k=0;k< 4;k++) buf[ind*4+k]=cr[k];ind++;
      break;
      case 6:
        ConvToBynare(cons.month,cr);for (k=0;k< 4;k++) buf[ind*4+k]=cr[k];ind++;
      break;
      case 7:
        ConvToBynare(cons.year,cr);for (k=0;k< 4;k++) buf[ind*4+k]=cr[k];ind++;
      break;
    }
  } 
  X607_WriteFn(0x9000,32,buf); //32- 8 переменных по 4 байта
}

/* инициализация, запись:события откл.пит,час.архив,восст.счётчиков*/
void InitializeMain (void)
{
  int year,month,day,hour,min,sec;
  unsigned char tp_arc;

  Prt.old_month=ReadNVRAM(30);

  flg_month=flg_arc_d=0;
  flg_min=flg_arc_h=0;

  GetDate(&year,&month,&day);GetTime(&hour,&min,&sec);
  if ((ReadNVRAM(2)==day && ReadNVRAM(3)>=contrh)||
    (ReadNVRAM(2)==day && ReadNVRAM(3)<contrh && hour<contrh)||
    (ReadNVRAM(2)!=day && ReadNVRAM(3)>=contrh && hour<contrh))
  { /*выбор типа архива:часовой или суточный*/
      tp_arc=0;
  }
  else tp_arc=1;

  WriteArchive(tp_arc);
  Prt.old_min=min;
  Prt.old_hour=hour;

  cons.last_hour=cons.hour;
  cons.hour=0.0;
  if (tp_arc==1)
  {
    cons.last_day=cons.day;
    cons.day=0.0;
  }

}

/***** восстановление базовых параметров и счётчиков *********/
void RestoreBasicParameters (void)
{
  unsigned char i,j,ind,buf[40];
  ind=0; 
  X607_ReadFn(0x9000,32,buf); //32 - 8 переменных по 4 байта
  for (j=0;j<8;j++) //8 колличество переменных
  {
    switch (j)
    {
      case 0:
        ConvToFloatVerify(&cons.accum,buf[ind*4],buf[1+ind*4],buf[2+ind*4],buf[3+ind*4]);
      break;
      case 1:
        ConvToFloatVerify(&cons.day,buf[ind*4],buf[1+ind*4],buf[2+ind*4],buf[3+ind*4]);              
      break;
      case 2:
        ConvToFloatVerify(&cons.hour,buf[ind*4],buf[1+ind*4],buf[2+ind*4],buf[3+ind*4]);        
      break;
      case 3:
        ConvToFloatVerify(&cons.last_day,buf[ind*4],buf[1+ind*4],buf[2+ind*4],buf[3+ind*4]);
      break;
      case 4:
        ConvToFloatVerify(&cons.last_hour,buf[ind*4],buf[1+ind*4],buf[2+ind*4],buf[3+ind*4]);    
      break;
      case 5:
        ConvToFloatVerify(&cons.last_month,buf[ind*4],buf[1+ind*4],buf[2+ind*4],buf[3+ind*4]);      
      break;
      case 6:
        ConvToFloatVerify(&cons.month,buf[ind*4],buf[1+ind*4],buf[2+ind*4],buf[3+ind*4]);        
      break;
      case 7:
        ConvToFloatVerify(&cons.year,buf[ind*4],buf[1+ind*4],buf[2+ind*4],buf[3+ind*4]);        
      break;
    }
    ind++;
  }
  cons.hour += cons.accum;
  cons.day += cons.accum;
  cons.month += cons.accum;
  cons.year += cons.accum;
  cons.accum=0.0;
}
/******* преобразование действит. числа в 4 байта ****************/
void ConvToBynare (float val,unsigned char * bytes)
{ /*прямое чередование параметров при записи память ЕЕР или флэш*/
  unsigned char buf_char[4];
  memcpy(&buf_char,&val,4);*(bytes)=buf_char[0];*(bytes+1)=buf_char[1];
  *(bytes+2)=buf_char[2];*(bytes+3)=buf_char[3];
}
/****** преобразование 4-х байт в float число ********/
unsigned char ConvToFloatVerify (float *val,unsigned char a,
		    unsigned char b,unsigned char c,unsigned char d)
{
  float value; unsigned char buf_char[4],flag;
  if ((d==255 || d==127) && c>127) {flag=1; goto M;}
  buf_char[0]=a; buf_char[1]=b; buf_char[2]=c; buf_char[3]=d;
  memcpy(&value, &buf_char, 4); *val=value; flag=0;
  M: return flag;
}

/**  запись строки данных в архив для газа:0-час,1-сут,2-мин ***/
void WriteArchive (unsigned char typ_arc)
{
  unsigned char i,j,pnt_min,num_page,buf_arc[Max_arch_record];
  int pnt_arc;unsigned segf,adrf;
  unsigned char cr[4];
  
  FormateArchive(buf_arc);

  if (typ_arc != 2)
  {
    buf_arc[0]=typ_arc;for (i=0;i<6;i++) buf_arc[i+1]=ReadNVRAM(i);
    num_page=GetArcPoint(&pnt_arc,&segf);
    adrf=pnt_arc*Size_str;
    for (i=0;i<Size_str;i++) FlashWrite(segf,adrf+i,buf_arc[i]);
    pnt_arc++;
    ClearFlashSeg(num_page,pnt_arc);
  } 
  else
  {
    pnt_min=ReadNVRAM(21);
    if (pnt_min < 60)
    {
      buf_arc[0]=typ_arc;
	    for (i=0;i<6;i++) buf_arc[i+1]=ReadNVRAM(i);
       ConvToBynare(cons.avg,cr);
      for (j=0;j<4;j++) buf_arc[7+j]=cr[j];
	    X607_WriteFn(0x6000+(pnt_min*Size_str_min),Size_str_min,buf_arc);
    } 
    pnt_min++;
    if (pnt_min >= 60) pnt_min=0;
    WriteNVRAM(21,pnt_min);
  }
}

/******** формирование архивной записи по типу архива и точкам ***/
void FormateArchive (unsigned char buf[])
{
  unsigned char i,j,ind_pnt,cr[4];float value;ind_pnt=0;
  for (i=0;i<5;i++) //коллич.точек(средний,час,сут,мес,год)
  {
    switch (i)
    {
      case 0: value=cons.avg;   break;
      case 1: value=cons.hour;  break;
      case 2: value=cons.day;   break;
      case 3: value=cons.month; break;
      case 4: value=cons.year;  break;
    }
    ConvToBynare(value,cr);
    for (j=0;j<4;j++) buf[7+ind_pnt*4+j]=cr[j];
    ind_pnt++;
  }
}
/******************************************************************/

/****** формирование указателя архива и сегмента *******************/
unsigned char GetArcPoint (int *pointer,unsigned *segment)
{ /*применение: инициализация, запись в архив, работа с MMI*/
  unsigned char page;page=ReadNVRAM(12);
  switch (page)
  {
    case 0:*segment=0xd000;*pointer=ReadNVRAM(13)*256+ReadNVRAM(14);
	   break;
    case 1:*segment=0xe000;*pointer=ReadNVRAM(15)*256+ReadNVRAM(16);
	   break;
    case 2:*segment=0xc000;*pointer=ReadNVRAM(17)*256+ReadNVRAM(18);
	   break;
  } return page;
}

/******* сохранение указателя и очистка сегмента флэш-памяти *********/
void ClearFlashSeg (unsigned char page,unsigned pointer)
{ /*применение: инициализация, запись в архив*/
  unsigned char i,j,num_page;unsigned max;
  max=65535/Size_str;
  if ((pointer+1) > max)
  {
    switch (page)
    {
      case 0:num_page=1;FlashErase(0xe000);WriteNVRAM(15,0);
	     WriteNVRAM(16,0);break;
      case 1:num_page=2;FlashErase(0xc000);WriteNVRAM(17,0);
	     WriteNVRAM(18,0);break;
      case 2:num_page=0;FlashErase(0xd000);WriteNVRAM(13,0);
	     WriteNVRAM(14,0);break;
    } WriteNVRAM(12,num_page);/*очищаем страницу и сохраняем номер страницы*/
  } else
  {
    i=pointer/256;j=pointer-i*256;
    switch (page)
    {
      case 0:WriteNVRAM(13,i);WriteNVRAM(14,j);break;
      case 1:WriteNVRAM(15,i);WriteNVRAM(16,j);break;
      case 2:WriteNVRAM(17,i);WriteNVRAM(18,j);break;
    } /*сохраняем указатель*/
  }
}