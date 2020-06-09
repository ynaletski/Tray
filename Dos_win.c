int n_pp=0;

#define d_F2 (4)
#define n_mmi_str 16 // количество строк на дисплее MMI

#define n_mmi_str_1 (n_mmi_str-1)
#define n_mmi_str_2 (n_mmi_str-2)
#define n_mmi_str_3 (n_mmi_str-3)
#define n_mmi_str_4 (n_mmi_str-4)
float ftmp_DP=1;
int mode_hst = 0;

char str_prod[22]=" -------- ";
char str_dsp1[31]="";
char str_dsp2[31]="";
char str_dsp3[31]="";
char str_dsp4[31]="";
char str_dsp5[31]="";
char str_dsp6[31]="";
char str_dsp7[31]="";

int type_dlv=0;
int sect_num=0;
int sect_mass=0;
int sect_vol=0;
int sect_rslt=0;

float MassStamp=-1000.;
float VolStamp=-1000.;

float vBIG_P=BIG_P;
long int tm_md=0;

char str_empt[]="                               ";

long int fl_zeroing=0;
#define Time_zeroing (50000)
long int zero_time;

char s_empty[]="                              ";
char str_ESC[]="ESC  Возврат";

void f_d_ESC()
{
  MmiGotoxy(0,6); MmiPuts(str_ESC);
}

int PageD=0;
int flg_ch=0;
int Flag_f_corr=0,flag_mass=0,pass_ret=0;
float k_v_flow=1,k_m_flow=1;
float k_dens=1.;

float k_mv_tmp;
long int pass1=0,pass2=0;

long int n_id_a=0;

int flag_esc=1;
//----------------------------------
void f_prn_error()
{
   f_clr_scr_MMI();
    MmiGotoxy(0,1);  MmiPrintf("   Система учета расхода   ");
    MmiGotoxy(0,2);  MmiPrintf(" воды в безнапорных потоках");
//  MmiGotoxy(0,1);   MmiPrintf("                        ");
    MmiGotoxy(0,5);   MmiPrintf("!Ошибка функционирования ");
    MmiGotoxy(0,7);   MmiPrintf("Sht-F1 отображение ошибок");
    MmiGotoxy(0,9);   MmiPrintf("Sht-ESC очистка ошибок   ");
}
//-------------------------------------
void  f_prn_begin()
{
    if(flag_esc == 0) goto m1;
    else if(FL_err) f_prn_error();
    else
    {
        m1:
        f_clr_scr_MMI();
        //  MmiPrintf("                              ");
        #if defined(MMI_NEW)
        MmiGotoxy(0,1);  MmiPrintf("   Система учета расхода   ");
        MmiGotoxy(0,2);  MmiPrintf(" воды в безнапорных потоках ");
        MmiGotoxy(17,14);  MmiPrintf(" ESC - меню ");
        #endif
    }
}
//-------------------------------------
//-------------------------------
#define mmi_page   page_MMI
char mmi_point=0;
int mmi_prm=0;
int mmi_flag=0;

int num_page=0.;
int sw_mmi=0;
int mmi_str=0;
int mmi_com=1;
int flag_chg=0;
int nn_chg=0;
unsigned int pass_cnt;
int max_par=1;
#define NumPlace 22

int FlagWinSum=0;

//struct arch_str_t adate11;

struct dis_set_MMI *ds_list=NULL;

long int Password=5555;
long int PassW=0;
long int Password_m=0;
long int PassWm=0;
int k_ext=0;
int k_ext1=0;
float chg_nn_par=0,chg_old=0,chg_new=0;
float dose_dlvr=0;
int n_adc_mm=0;

int f_menu_MMI()
{
    time_t ltime1;
    time_t ltime2;
    int year,month,day,hour,min,sec;
    int key;
    char str[31];
    int i,j,k;
    int itmp;
    struct s_icp_dev *ICPl_DEV;
    //struct arch_str *ar_str_ptr;
    long int ltmp;
    struct tm *tdat;

    if((i=fstin())==fst_OK)
    {   // введено числовое значение
        key=DATA;  // признак окончания ввода строки
        sw_fst=0;
    }
    else if(i==fst_ESC)
    {
        key=DATA_ESC;  // признак окончания ввода строки
        sw_fst=0;
    }
    else if( MmiKbhit())
    {  // ввод поступившего символа из клавиатуры
        key=MmiGetch();
    }
    else if(sw_fst==0)
    {
        key=PASS; // признак холостого прохода (без введенного символа)
              // используется для индикации изменяющихся значений
    }
    else return 0;

    switch(sw_mmi)
    {
        case 0: /*заголовок*/
        //---------------------------------
        if(key==F2)
        {
            flag_esc=0;
            PageD=0;
            f_prn_begin();
        }
        else if(key=='1')
        {
            flag_esc=0;
            f_prn_begin();
        }
        else if(key==DATA_ESC)
        {
            flag_esc=1;
            goto mm00;
        }
        else
        //---------------------------------
        if (key==Sht_ESC)
        {  // очистка ошибок
            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);
            sw_mmi=26;
            //f_cl_error();
            MmiGotoxy(5,2);    MmiPuts("   Ошибки очищены   ");
            MmiGotoxy(0,4);    MmiPuts(" Enter - продолжить ");
            flag_esc=1;
        }
        else if (key==Sht_F1)
        {  // распечатка ошибок

            f_clr_scr_MMI();
            SetDisplayPage(EmptPage);
            sw_mmi=26;
            //f_disp_error();
            flag_esc=1;
        }
        else if(key==ESC)
        {
            PassWm=0;
            flag_esc=1;
            if(mode_hst == 0 )
            {
                m0_f1:          
                main_menu:
                m_120p:
                if(Password_m != 0)
                if(PassWm != Password_m)
                {
                    pass_ret=20;
                    goto m_pass;
                }
                PageD=0;
                f_clr_scr_MMI();
                SetDisplayPage(EmptPage);
                //f_prepare_t (&adate00);
                sw_mmi=120;
                PassW=0;
                MmiGotoxy(0,0);     MmiPrintf("           Меню");
                MmiGotoxy(0,2);     MmiPuts("1  Выбор лотка");
                //MmiGotoxy(0,4);    MmiPuts("2  Показания расходомера");
                MmiGotoxy(0,4);     MmiPuts("2  Версия ПО ");
                MmiGotoxy(0,6);     MmiPuts("3  Просмотр журнала событий");
                MmiGotoxy(0,8);     MmiPuts("4  Сервисные функции");
                MmiGotoxy(0,15);    MmiPuts("                ESC  Возврат");
            }
            else
            {
            f_clr_scr_MMI();
            f_prn_begin();
            }
            break;
        }
        else if(key==PASS)
        {  // отображение даты и времени
            if( ((FL_err) == 0) || ( flag_esc == 0 ))
            {
                if(PageD==0)
                {
                    MmiGotoxy(0,6);  MmiPrintf("  Расход воды:    28.5 м3/ч");
                }
            }
            GetDate(&year,&month,&day);GetTime(&hour,&min,&sec);
            MoveToXY(4,11);
            MmiPrintf("%02d.%02d.%02d   %02d:%02d:%02d       " ,day,month,year-2000 ,hour,min,sec);
        }
        break;
        /*========================================*/
        case 14:
        m_pass:
        MmiGotoxy(1,1);
        f_clr_scr_MMI();
        #if defined(MMI_NEW)
        SetDisplayPage(2); // пустая страница
        #endif
        MmiPuts("Введите пароль:" );
        sprintf( fst_str,"******");
        sw_mmi=121;
        fst_n=7;
        sw_fst=1;
        MmiGotoxy(11,3);
        break;
        /*========================================*/
        case 120:  // Основное меню
        if((key==ESC)||(key==ENTER))    /* переход в меню */
        {
            mm00:      
            sw_mmi=0;
            PassWm=0;
            PageD=0;
            SetDisplayPage(ZeroPage);
            f_clr_scr_MMI();
            f_prn_begin();
            break;
        }
        /*========================================*/
        case 26:/* ожидание нажатия ESC,Enter */
        if((key==ESC)||(key==ENTER))
        {
            goto m_main;
        }
        break;
        /*========================================*/
        default:
        if((key==ESC)||(key==ENTER))  /* переход на начальную страницу */
        {
            m_main:
            PassWm=0;
            SetDisplayPage(ZeroPage);
            sw_mmi=0;
            f_clr_scr_MMI();
            f_prn_begin();
            break;
        }
        break;
    }
    return 0;
}