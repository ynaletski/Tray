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
void f_prn_menu(int count, char *menu[])
{
    int iii;
    for(iii=1;iii<=count;iii++)
    {
        MmiGotoxy(0,iii+1);  MmiPrintf(" %d. %s",iii,menu[iii]);
    }
}
//----------------------------------
void f_prn_error()
{
   f_clr_scr_MMI();
    MmiGotoxy(0,1);     MmiPrintf("     Система учета расхода    ");
    MmiGotoxy(0,2);     MmiPrintf("  воды в безнапорных потоках  ");
//  MmiGotoxy(0,1);     MmiPrintf("                              ");
    MmiGotoxy(0,5);     MmiPrintf("  !Ошибка функционирования    ");
    MmiGotoxy(0,7);     MmiPrintf("  Sht-F1 отображение ошибок   ");
    MmiGotoxy(0,9);     MmiPrintf("  Sht-ESC очистка ошибок      ");
}
//-------------------------------------
//-------------------------------------
void f_drv_list_MMI(int *nn)
{ // показать список драйверов устройств
int i,i1,i2;
char str[31];
  struct s_icp_dev *ICPl_DEV;

   f_clr_scr_MMI();

  //  if( (*nn+n_mmi_str_3) > icp_lst_max) i=icp_lst_max-n_mmi_str_3;
  //  else
    i=*nn;
    if(i<0) i=0;
    if((i+n_mmi_str_3) < icp_lst_max) i1=i+n_mmi_str_3;
    else i1=icp_lst_max;

    *nn=i;
    MmiGotoxy(0,0); MmiPuts("        Список драйверов      ");
    MmiGotoxy(0,1); MmiPuts(" N     Имя   Сост. Порт Адр.");
   for(i2=0;i<i1;i++,i2++)
    {
      ICPl_DEV=ICP_dd[i];
      if(ICPl_DEV==NULL) break;
      MmiGotoxy(0,3+i2);
//         MmiPuts(" N      Имя   Сост. Порт Адр.");
//                 "xx.....xxxxx  .x    .x   xx"
      MmiPrintf("%2d%10s  %2d    %2d   %02X",i+1,ICPl_DEV->name,ICPl_DEV->status,ICPl_DEV->port,ICPl_DEV->addr);
    }
}
//-------------------------------
void f_drv_stat_MMI(int *nn)
{ // показать статистику драйверов устройств
int i,i1,i2;
char str[31];
  struct s_icp_dev *ICPl_DEV;

   f_clr_scr_MMI();

//  if( (*nn+n_mmi_str_3) > icp_lst_max) i=icp_lst_max-n_mmi_str_3;
//  else
    i=*nn;
    if(i<0) i=0;
    if((i+n_mmi_str_3) < icp_lst_max) i1=i+n_mmi_str_3;
    else i1=icp_lst_max;

    *nn=i;
    MmiGotoxy(0,0);  MmiPuts(" Статистика работы драйверов");
    MmiGotoxy(0,1);  MmiPuts(" N     Имя    Послано  Ошибок");
    for(i2=0;i<i1;i++,i2++)
    {
      ICPl_DEV=ICP_dd[i];
      if(ICPl_DEV==NULL) break;
      MmiGotoxy(0,3+i2);
//         MmiPuts(" N     Имя    Послано  Ошибок");
//                 "xx.....xxxxx  xxxxxxx xxxxxxx"
         MmiPrintf("%2d%10s  %7ld %7ld",i+1,ICPl_DEV->name,ICPl_DEV->n_transaction,ICPl_DEV->n_timeout_error+ICPl_DEV->n_CRC_error);
    }
}
//-------------------------------
void  f_prn_begin()
{
    if(flag_esc == 0) goto m1;
    else if(CRC_err) f_prn_CRC_error();
    else if(FL_err) f_prn_error();
    else
    {
        m1:
        f_clr_scr_MMI();
        //  MmiPrintf("                              ");
        #if defined(MMI_NEW)
        MmiGotoxy(0,1);  MmiPrintf("     Система учета расхода    ");
        MmiGotoxy(0,2);  MmiPrintf("  воды в безнапорных потоках  ");
        MmiGotoxy(17,15);  MmiPrintf(" ESC - меню ");
        #endif
    }
}
//-------------------------------------
int CRC_err=0;
//-------------------------------------
void f_prn_CRC_error()
{
 MmiGotoxy(0,2);   MmiPrintf("!Ошибка контрольной суммы   ");
 MmiGotoxy(0,3);   MmiPrintf("!Параметры потеряны         ");
 MmiGotoxy(0,4);   MmiPrintf("!Необходима перенастройка   ");
 MmiGotoxy(0,5);   MmiPuts(s_empty);
 CRC_err=1;
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

struct arch_str_t adate11;

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
                //MmiGotoxy(0,6);     MmiPuts("3  Просмотр журнала событий");
                MmiGotoxy(0,6);     MmiPuts("3  Сервисные функции");
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
                    if (consumption<Qmin || consumption>Qmax)
                    {
                        MmiGotoxy(0,3); MmiPuts("           Внимание!          ");
                        MmiGotoxy(0,4); MmiPuts("  Выход за диапазон измерения ");
                    }
                    else 
                    {
                        MmiGotoxy(0,3); MmiPuts("                              ");
                        MmiGotoxy(0,4); MmiPuts("                              ");
                    }
                    MmiGotoxy(0,5);  MmiPrintf("Лоток       : %s",tray[tray_num]);
                    MmiGotoxy(0,7);  MmiPrintf("Диап.измер. : %5.2f-%5.2f",Qmin,Qmax);
                    MmiGotoxy(0,9);  MmiPrintf("Уровень воды: %8.3f см", real_level);
                    MmiGotoxy(0,11); MmiPrintf("Расход воды : %8.3f м3/ч", consumption);
                }
            }
            GetDate(&year,&month,&day);GetTime(&hour,&min,&sec);
            MoveToXY(4,14);
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
            else if(key=='1')
            {   /* переход в Меню 1. Выбор лотка. */
                m_m1:
                SetDisplayPage(EmptPage);
                f_clr_scr_MMI();
                MmiGotoxy(0,0);     MmiPuts(" Меню 1. Выбор лотка.");
                f_prn_menu(max_count_tray, tray);
                MmiGotoxy(0,2); MmiPuts(">");
                MmiGotoxy(0,15); MmiPuts("0-вниз/3-вверх/Enter/ESCотмена");
                sw_mmi=130;
                PassW=0;
                Ytray=2; //положение курсора > на MMI
            }
            else if(key== '3') // 4  Сервисные функции
            {
                m_m3:
                SetDisplayPage(EmptPage);
                f_clr_scr_MMI();
                MmiGotoxy(0,0);    MmiPuts(" Меню 3. Сервисные функции.");
                MmiGotoxy(0,2);    MmiPuts("1  Список драйверов");
                MmiGotoxy(0,4);    MmiPuts("2  Статистика  драйверов");
                MmiGotoxy(0,6);    MmiPuts("3  Просмотр/ввод параметров");
                MmiGotoxy(0,8);    MmiPuts("4  Другие функции");
                MmiGotoxy(0,10);    MmiPuts("5  Время, дата");
                MmiGotoxy(0,15);    MmiPuts("ESC  Возврат");
                sw_mmi=150;
                PassW=0;
            }
        break;
        /*========================================*/
        case 121:   // // ввод пароля на изменение данных
            if((key==ESC)||(key==ENTER))    /* переход в меню */
            {
                f_clr_scr_MMI();
                SetDisplayPage(EmptPage);    // Чистая страница
                m121:
                switch(pass_ret)
                { // пароль не введен
                    case 0:  //goto par_out14;
                    case 1:  //goto m_m9;
                    case 2:  //goto m_m9;
                    case 3:  //goto m_m3;
                    case 5:  //goto m_m9s;
                    case 6:  //goto m_m9s;
                    case 7:  //goto m_m9;
                    case 8:  goto m_m1s_2;
                    case 9:  goto m_m1s_2;
                    case 10: goto m_m1;
                    case 11: goto m_m1s_1;
                    case 20: goto m_main;
                    default: goto main_menu;
                }
            }
            else if(key==DATA)
            {
                sscanf(fst_str,"%ld",&ltmp);
                if(pass_ret < 20)
                {
                    if(ltmp == Password) PassW=Password;
                    else goto m_pass;
                }
                else
                {
                    if(ltmp == Password_m) PassWm=Password_m;
                    else goto m_pass;
                }
                f_clr_scr_MMI();
                SetDisplayPage(EmptPage);    // Чистая страница
                switch(pass_ret)
                { // пароль введен корректно
                    case 0:  //goto par_out14;
                    case 1:
                        //sw_mmi=125;
                        //goto m_m9_1;
                    case 2:
                        //sw_mmi=125;
                        //goto m_m9_2;
                    case 7:
                        //sw_mmi=125;
                        //goto m_m9_3;
                    case 3:
                        //sw_mmi=162;
                        //goto m_m3_6_1;
                    case 5:
                        //sw_mmi=127;
                        //goto m_m9s_1;
                    case 6:
                        //sw_mmi=127;
                        //goto m_m9s_2;
                    case 8:
                        sw_mmi=132;
                        goto m_m1s_2_qmin;
                    case 9:
                        sw_mmi=132;
                        goto m_m1s_2_qmax;
                    case 10:
                        goto m_m1s_2;
                    case 11:
                        goto m_m1s_1_ok;
                    case 20:
                        //sw_mmi=0;
                        //goto m_120p;
                    default: goto main_menu;
                }
            }
            else if(key==DATA_ESC)
            {
                f_clr_scr_MMI();
                SetDisplayPage(EmptPage);    // Чистая страница
                goto m121;
            }
        /*========================================*/
        break;
        /*========================================*/
        case 130:   //  Меню 1. Выбор лотка.
        if(key==ESC)    /* переход в меню */
        {
            goto main_menu;
        }
        if(key=='0')    //вниз по меню
        {   
            max_count_tmp=max_count_tray;
            m1_f2:
            if(Ytray<=max_count_tmp)
            {
                MmiGotoxy(0,Ytray); MmiPuts(" ");
                Ytray+=1;
                MmiGotoxy(0,Ytray); MmiPuts(">");
            }
            break;
        }
        if(key=='3')    //вверх по меню
        {
            m1_f1:
            if(Ytray>2)
            {
                MmiGotoxy(0,Ytray); MmiPuts(" ");
                Ytray-=1;
                MmiGotoxy(0,Ytray); MmiPuts(">");
            }
            break;
        }
        if(key==ENTER)
        {
            SetDisplayPage(EmptPage);
            f_clr_scr_MMI();
            num_tmp=Ytray-1;
            if(num_tmp==Parshall || num_tmp==Venturi  || num_tmp==RBC || num_tmp==Cutthoat)
            {
                m_m1s_1:
                MmiGotoxy(0,0);     MmiPuts("       Выбор типа лотка");
                switch (num_tmp)
                {
                    case Parshall:
                    max_count_tmp=max_count_Parshall;
                    f_prn_menu(max_count_Parshall, Parshall_type);
                    break;
                    case Venturi:
                    max_count_tmp=max_count_Venturi;
                    f_prn_menu(max_count_Venturi, Venturi_type);
                    break;
                    case RBC:
                    max_count_tmp=max_count_RBC;
                    f_prn_menu(max_count_RBC, RBC_type);
                    break;
                    case Cutthoat:
                    max_count_tmp=max_count_Cutthoat;
                    f_prn_menu(max_count_Cutthoat, Cutthoat_type);
                    break;
                    default:
                    break;
                }
                MmiGotoxy(0,2); MmiPuts(">");
                MmiGotoxy(0,15); MmiPuts("0-вниз/3-вверх/Enter/ESCотмена");
                sw_mmi=131;
                PassW=0;
                Ytray=2; //положение курсора > на MMI
            }
            else //все остальный, у которых нет таблиц, ввод параметров вручную
            {   
                if(PassW != Password)
                {
                    pass_ret=10;
                    goto m_pass;
                }
                m_m1s_2:
                tray_num = num_tmp; //номер выбранного лотка 
                MmiGotoxy(0,0);     MmiPuts("       Ввод параметров");

                MmiGotoxy(0,2);     MmiPuts("1. Минимальный расход");
                MmiGotoxy(0,3);     MmiPuts("2. Максимальный расход");

                MmiGotoxy(0,5);  MmiPrintf(" мин.расход   : %8.3f м3/ч",Qmin);
                MmiGotoxy(0,6);  MmiPrintf(" макс.расход  : %8.3f м3/ч",Qmax);

                MmiGotoxy(0,15);    MmiPuts("                ESC  Меню");
                sw_mmi=132;
                PassW=0;
                Ytray=2; //положение курсора > на MMI
            }
            break;
        }
        break;
        /*========================================*/
        case 131:
            if(key==ESC)    /* переход в Меню 1. Выбор лотка. */
            {
                goto m_m1;
            }
            if(key=='0')    //вниз по меню
            {
                goto m1_f2;
            }
            if(key=='3')    //вверх по меню
            {
                goto m1_f1;
            }
            if(key==ENTER)
            {
                if(PassW != Password)
                {
                    pass_ret=11;
                    goto m_pass;
                }
                m_m1s_1_ok:
                tray_num = num_tmp;
                num_tmp = Ytray-1;
                switch (tray_num)
                {
                    case Parshall:
                        Qmax = ParshallQmax[num_tmp];
                        Qmin = ParshallQmin[num_tmp];
                    break;
                    case Venturi:
                        Qmax = VenturiQmax[num_tmp];
                        Qmin = VenturiQmin[num_tmp];                 
                    break;
                    case RBC:
                        Qmax = RBCQmax[num_tmp];
                        Qmin = RBCQmin[num_tmp];                  
                    break;
                    case Cutthoat:
                        Qmax = CutthoatQmax[num_tmp];
                        Qmin = CutthoatQmin[num_tmp];                   
                    break;
                }
                goto main_menu;
            }
        break;
        /*========================================*/
        case 132:
            if(key==ESC)    /* переход в Меню 1. Выбор лотка. */
            {
                 goto m_m1;
            }
            if(key=='1')
            {
                if(PassW != Password)
                {
                    pass_ret=8;
                    goto m_pass;
                }
                m_m1s_2_qmin:
                MmiGotoxy(0,1);
                SetDisplayPage(EmptPage); // ввод значения
                f_clr_scr_MMI();
                MmiPuts("Введите мин.расход м3/ч: " );
                k_mv_tmp=Qmin;
                flag_mass=1;
                m_m1s_2_1:
                MmiGotoxy(0,n_mmi_str_2);  MmiPuts("\"1...9,SHT-7\"-Ввод,\"SHT-4\"-Уд.");
                MmiGotoxy(0,n_mmi_str_1);  MmiPuts("\"ESC - возврат");
                sprintf( fst_str,"%8.3f",k_mv_tmp);
                fst_n=9;
                sw_fst=1;
                MmiGotoxy(10,3);
                break;
            }
            if(key=='2')
            {
                if(PassW != Password)
                {
                    pass_ret=9;
                    goto m_pass;
                }
                m_m1s_2_qmax:
                MmiGotoxy(0,1);
                SetDisplayPage(EmptPage); // ввод значения
                f_clr_scr_MMI();
                MmiPuts("Введите макс.расход м3/ч: " );
                k_mv_tmp=Qmax;
                flag_mass=0;
                goto m_m1s_2_1;
            }
            else if(key==DATA)
            {
                sscanf(fst_str,"%f",&k_mv_tmp);
                if(flag_mass==1)
                {
                    if((k_mv_tmp<0)||(k_mv_tmp>900.) )
                    goto m_m1s_2_qmin;
                }
                else if (flag_mass==0)
                {
                    if((k_mv_tmp<0)||(k_mv_tmp>100000.) )
                    goto m_m1s_2_qmax;
                }
            
                // введенное значение в k_mv_tmp
                if(flag_mass==1)
                {
                    Qmin = k_mv_tmp;
                }
                else if (flag_mass==0)
                {
                    Qmax = k_mv_tmp;
                }
                goto m_m1s_2;
            }
            else if(key==DATA_ESC)
            {
                goto m_m1s_2;
            }
        break;
        /*========================================*/
        case 150: // 3  Сервисные функции
            if((key==ESC)||(key==ENTER))    /* переход в меню */
            {
                goto main_menu;
            }
            else if(key== '1')  // 1  Список драйверов
            {
                mmi_str=0;
                max_par=icp_lst_max;
                m20_2:
                SetDisplayPage(EmptPage);
                f_clr_scr_MMI();
                f_drv_list_MMI(&mmi_str);
                sw_mmi=22;
            }
            else if(key== '2')  // Статистика  драйверов
            {
                mmi_str=0;
                max_par=icp_lst_max;
                m20_3:
                SetDisplayPage(EmptPage);  // чистый экран
                f_clr_scr_MMI();
                f_drv_stat_MMI(&mmi_str);
                pass_cnt=0;
                sw_mmi=23;
            }
            else if(key== '3')   //  3  Просмотр/ввод параметров
            {
                m_m3_3:
                // переход в меню выбора точки для просмотра параметров
                sw_mmi=11;
                SetDisplayPage(EmptPage);
                f_clr_scr_MMI();
                MmiGotoxy(0,0);    MmiPuts(" Меню выбора списка просмотра");
                MmiGotoxy(0,7);    MmiPuts("1...6  номер страницы ");
                MmiGotoxy(0,15);    MmiPuts("ESC  Возврат");
                break;
            }
            else if(key== '4')   //  4  Другие функции
            {
                m_m3_4:
                SetDisplayPage(EmptPage);  // чистый экран
                f_clr_scr_MMI();
                MmiGotoxy(0,0);    MmiPuts(" Меню 4.4.Другие функции. ");
                MmiGotoxy(0,4);    MmiPuts("1  Поиск ICP/RTU модулей");
                MmiGotoxy(0,5);    MmiPuts("2  Список найденных модулей");
                MmiGotoxy(0,6);    MmiPuts("ESC  Возврат");
                sw_mmi=160;
                break;
            }
            else if(key== '5')   //  5  Время, дата
            {
                m_m3_5:
                //f_prepare_t ( &adate00);
                //adate11=adate00;
                SetDisplayPage(EmptPage);  // чистый экран
                f_clr_scr_MMI();
                MmiGotoxy(0,0);    MmiPuts("      Текущая дата ");
                MmiGotoxy(0,1);    MmiPuts("      День.Мес.Год");
                /*MmiGotoxy(0,3);
                if(FlagWinSum==0)   MmiPuts("Переход лет/зим.время выключен");
                else    MmiPuts("Переход лет/зим.время включен");*/
                MmiGotoxy(0,4);   MmiPuts(" 0...9,'.' - изменить");
                MmiGotoxy(0,5);   MmiPuts(" Enter - ввод");
                MmiGotoxy(0,6);   MmiPuts(" ESC   - отменить");
                MmiGotoxy(8,2);
                //sprintf(fst_str,"%02d.%02d.%04d",(int)adate00.day,(int)adate00.month,(int)(adate00.year+2000) );
                fst_n=11;
                sw_fst=1;
                sw_mmi=165;
                 break;
            }
            else if(key== '6')   //  6  Изменить пароль
            {
                m_m3_6:
                SetDisplayPage(EmptPage);  // чистый экран
                f_clr_scr_MMI();
                MmiGotoxy(0,0);    MmiPuts(" Меню 4.6.Изменение пароля ");
                MmiGotoxy(0,2);    MmiPuts("1  Изменить пароль");
                f_d_ESC();
                sw_mmi=162;
                break;
            }
        break;
        /*========================================*/
        case 165:   //  Ввод даты для изменения
            if((key==ESC)||(key==ENTER)||(key==DATA_ESC))    /* переход в меню */
            {
                goto m_m3;
            }
            else if(key==DATA)
            {
                year=(int)(adate00.year+2000);
                month=(int)adate00.month;
                day=(int)adate00.day;
                sscanf(fst_str,"%d.%d.%d",&day,&month,&year);
                if((year<2000)||(year > 2100)) goto m_m3_5;
                if((month<1)|| (month > 12))  goto m_m3_5;
                if((day<1)|| (day > 31))  goto m_m3_5;
                adate00.year = year-2000 ;
                adate00.month = month;
                adate00.day = day;
                // дата введена
                if((adate00.year  != adate11.year ) ||
                    (adate00.month != adate11.month) ||
                    (adate00.day   != adate11.day  ) )
                {
                    // printf("\n\r %02d.%02d.%02d" ,day,month,year-2000);
                    //f_wr_evt(evt_none);
                    SetDate( year, month, day);
                    WriteNVRAM((int)nr_year  ,(int)(year-2000));
                    WriteNVRAM((int)nr_month ,(int)month  );
                    WriteNVRAM((int)nr_day   ,(int)day    );
                    //init_win_sum(0);
                    //f_wr_evt(evt_ch_date);
                    adate11=adate00;
                    //printf("\n\r Date have changed ");
                }
                m_m3_5_1:
                SetDisplayPage(EmptPage);
                f_clr_scr_MMI();
                MmiGotoxy(0,0);    MmiPuts("      Текущее время ");
                MmiGotoxy(0,1);    MmiPuts("       Час Мин Сек");
                // MmiGotoxy(0,2);    MmiPuts("         08.01.00");
                MmiGotoxy(0,3);
                /*if(FlagWinSum==0)             MmiPuts("Переход лет/зим.время выключен");
                else                          MmiPuts("Переход лет/зим.время включен");*/
                /*
                if(ReadNVRAM(nr_win_sum))
                // 1 - летнее
                                              MmiPuts("      Летнее время");
                     else                     MmiPuts("      Зимнее время");
                */
                MmiGotoxy(0,4);    MmiPuts(" 0...9,'.' - изменить");
                MmiGotoxy(0,5);    MmiPuts(" Enter - ввод");
                MmiGotoxy(0,6);    MmiPuts(" ESC   - отменить");
                MmiGotoxy(8,2);
                sprintf(fst_str,"%02d.%02d.%02d",(int)adate00.hour,(int)adate00.min,(int)adate00.sec);
                fst_n=9;
                sw_fst=1;
                sw_mmi=166;
            }
        break;

 /*========================================*/
        case 166:   //  Ввод времени
            if((key==ESC)||(key==ENTER)||(key==DATA_ESC))    /* переход в меню */
            {
                goto m_m3_5;
            }
            else if(key==DATA)
            {
                hour=(int)adate00.hour;
                min=(int)adate00.min;
                sec=(int)adate00.sec;
                sscanf(fst_str,"%d.%d.%d",&hour,&min,&sec);
                if((min<0)|| (min > 59))  goto m_m3_5_1;
                if((sec<0)|| (sec > 59))  goto m_m3_5_1;
                if((hour<0)|| (hour > 23))  goto m_m3_5_1;
                adate00.min = min;
                adate00.hour = hour;
                adate00.sec = sec;
                // время введено
                if((adate00.hour  != adate11.hour ) ||
                    (adate00.min   != adate11.min) ||
                    (adate00.sec   != adate11.sec  ) )
                {
                    //printf("\n\r Time have changed ");
                    //f_wr_evt(evt_none);
                    SetTime( adate00.hour, adate00.min,adate00.sec);
                    WriteNVRAM((int)nr_hour   ,(int)hour   );
                    WriteNVRAM((int)nr_min    ,(int)min    );
                    WriteNVRAM((int)nr_sec    ,(int)sec    );
                    //init_win_sum(0);
                    //f_wr_evt(evt_ch_time);
                }
                SetDisplayPage(EmptPage);
                f_clr_scr_MMI();
                goto m_m3;
            }
        break;
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