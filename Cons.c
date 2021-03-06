//consumption - ������
#define m3_to_l  (0.27777777777777)
#define l_to_m3  (3.6)

int Ytray = 2; //��������� ������� > �� MMI

float consumption = 28.5; //m3/h ������ 
float level = -1;  //������� � �� ���������� �����������
int tray_num = 3; //����� ���������� �����

float real_level=0; //��������� �������
float zero_level=5000; //������� �� ���

float c0 = 0.1407;
float expN = 1.55;

void f_calc_cons (void)
{
    if (zero_level > level && level > 0) real_level = zero_level - level;
    else real_level = 0;
    consumption = c0 * pow(real_level,expN) * l_to_m3;
}

int num_tmp=0;
int max_count_tmp=0;

float Qmax = 30;  //������ �������� �������
float Qmin = 1;   //������ �������� �������

char *tray[]=
{                       //���� ��������� ������ 11 �������� ����������� � dos_win
"",                     
"��������",             //1     Parshall
"RBC",                  //2     RBC
"�������",              //3     Venturi
"��� ���������",        //4     Cutthoat
"� ����.�������",       //5     ThinWall
"�������-�������",      //6     Palmer
"�������",              //7     Montana
"HS-����",              //8     HS-type
"H-����",               //9     H-type
"�����������",          //10    Triangle
"��������������",       //11    Trapezoid
};

int max_count_tray=(sizeof(tray)/sizeof(tray[0])) -1;

/*--------------��������------------*/
char *Parshall_type[]=
{
"",
"�1-1",     //1
"�2-2",     //2
"�3-3",     //3
"�4-6",     //4
"�5-9",     //5
"�6-12",    //6
"�7-18",    //7
"�8-24",    //8
"�9-36",    //9
"�10-48",   //10
};
int max_count_Parshall=(sizeof(Parshall_type)/sizeof(Parshall_type[0])) -1;

float ParshallQmax[]=
{   //m3/h
0,
15.3,   //1
30.6,   //2
193.7,  //3
397.6,  //4
907.4,  //5
1641.4, //6
2508.0, //7
3374.6, //8
5138.4, //9
6922.5, //10
};

float ParshallQmin[]=
{   //m3/h
0,      
0.5,    //1
1.0,    //2
3.1,    //3
5.1,    //4
9.2,    //5
11.2,   //6
15.3,   //7
42.8,   //8
62.2,   //9
132.5,  //10
};

/*------------�������--------------------*/
char *Venturi_type[]=
{
"",
"�1",   //1
"�2",   //2
"�3",   //3
"�4",   //4
"�5",   //5
"�6",   //6
"�7",   //7
"�8",   //8
"�9",   //9
};
int max_count_Venturi=(sizeof(Venturi_type)/sizeof(Venturi_type[0])) -1;

float VenturiQmax[]=
{   //m3/h
0,
100,    //1
320,    //2
500,    //3
1250,   //4
4000,   //5
5000,   //6
8000,   //7
10000,  //8
20000,  //9
};

float VenturiQmin[]=
{   //m3/h
0,      
10,     //1
16,     //2
25,     //3
60,     //4
200,    //5
250,    //6
400,    //7
500,    //8
1000,   //9
};

/*------------RBC--------------------------*/
char *RBC_type[]=
{
"",
"�1",     //1
"�2",     //2
"�3",     //3
"�4",     //4
"�5",     //5
};
int max_count_RBC=(sizeof(RBC_type)/sizeof(RBC_type[0])) -1;

float RBCQmax[]=
{   //�/� 
0,
1.432 * l_to_m3,  //1
4.265 * l_to_m3,  //2
8.155 * l_to_m3,  //3
24.31 * l_to_m3,  //4
49.08 * l_to_m3,  //5
};

float RBCQmin[]=
{   //�/�
0,      
0.0367 * l_to_m3,    //1
0.1042 * l_to_m3,    //2
0.4255 * l_to_m3,    //3
0.6175 * l_to_m3,    //4
0.0231 * l_to_m3,    //5
};
/*-----------------------------------------*/
/*--------------��� ���������------------*/
char *Cutthoat_type[]=
{
"",
"�1-1",     //1
"�1-2",     //2
"�1-3",     //3
"�1-4",     //4
"�2-1",     //5
"�2-2",    //6
"�2-3",    //7
"�2-4",    //8
"�3-1",    //9
"�3-2",    //10
"�3-3",    //11
"�3-4",    //12
};
int max_count_Cutthoat=(sizeof(Cutthoat_type)/sizeof(Cutthoat_type[0])) -1;

float CutthoatQmax[]=
{   //�/�
0,
3.152 * l_to_m3,  //1
6.043 * l_to_m3,  //2
12.6 * l_to_m3,   //3
25.71 * l_to_m3,  //4
20.36 * l_to_m3,  //5
41.32 * l_to_m3,  //6
84.11 * l_to_m3,  //7
171.11 * l_to_m3, //8
54.61 * l_to_m3,  //9
111.5 * l_to_m3,  //10
226.4 * l_to_m3,  //11
455.6 * l_to_m3,  //12
};

float CutthoatQmin[]=
{   //�/�
0,      
0.0223 * l_to_m3,     //1
0.0428 * l_to_m3,     //2
0.396 * l_to_m3,      //3
0.808 * l_to_m3,      //4
0.0822 * l_to_m3,     //5
0.5972 * l_to_m3,     //6
1.216 * l_to_m3,      //7
2.472 * l_to_m3,      //8
0.518 * l_to_m3,      //9
1.058 * l_to_m3,      //10
21.48 * l_to_m3,      //11
4.322 * l_to_m3,      //12
};