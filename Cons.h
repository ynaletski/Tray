extern float consumption;
extern float level;

extern float real_level;
extern float zero_level;

extern float c0;
extern float expN;
void f_calc_cons(void);

extern int tray_num;

extern float Qmax;
extern float Qmin;

extern int num_tmp;
extern int max_count_tmp;

extern char *tray[];
extern int max_count_tray;

extern char *Parshall_type[];
extern int max_count_Parshall;
extern float ParshallQmax[];
extern float ParshallQmin[];

extern char *Venturi_type[];
extern int max_count_Venturi;
extern float VenturiQmax[];
extern float VenturiQmin[];

extern char *RBC_type[];
extern int max_count_RBC;
extern float RBCQmax[];
extern float RBCQmin[];

extern char *Cutthoat_type[];
extern int max_count_Cutthoat;
extern float CutthoatQmax[];
extern float CutthoatQmin[];

extern int Ytray;   //положение курсора > на MMI

#define Parshall 1
#define Venturi 2
#define RBC 3
#define Cutthoat    4
#define ThinWall    5
#define Palmera 6
#define Montana 7
#define HS-type 8
#define H-type  9
#define Triangle    10
#define Trapezoid   11