//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//  Class Template - Assignment 2.2
//  Artificial Intelligence for Genetics Programming I - 5SD809
//  Uppsala University
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//---------------------------------------------------------------------
#include "Genetics.h"
//---------------------------------------------------------------------
//---------------------------------------------------------------------
Genetics::Genetics(void){
    //--------------------------------------------
	mMtxFont = new char[128][7][5];
    InitMtxFont();
    //--------------------------------------------
    mChromIdx2GoalFunc = new double[MAX_CHROMS_M];
    mCityCoordinates = new double[MAX_CITIES_N][2];
    mGP = new int[MAX_CHROMS_M][MAX_CITIES_N];
    //--------------------------------------------
    mW = 1024;//1280;
    mH = int(double(mW)*10./16.);
    sRandSeed = 0;
    //--------------------------------------------
    mMouseX = mMouseY = 0;
    mMouseButton = mMouseState = 0;
    //--------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------



//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                                 GFX
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::Init(){
    InitGFX();
    InitFirstGen();
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::ChangeSize(int w, int h){
	mW = w, mH = h;
	glViewport(0,0,mW,mH);
	glMatrixMode(GL_PROJECTION); glLoadIdentity();
	glOrtho(0.0f,mW,mH,0.0f,-1.0f,1.0f);
	glMatrixMode(GL_MODELVIEW); glLoadIdentity();
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::Draw(void){
    //--------------------------------------------
    For (i,GFX_GENS_PER_FRAME) EvalNextGen();
    //--------------------------------------------
    glClear(GL_COLOR_BUFFER_BIT);
    //--------------------------------------------
    glColor3ub(0,127,255);
    //--------------------------------------------
    glBegin(GL_LINE_LOOP);
    For (n,mN) glVertex2dv(mCityCoordinates[mGP[0][n]]);
    glEnd();
    //--------------------------------------------
    glColor3ub(0,255,255);
    //--------------------------------------------
    glPointSize(12);
    glBegin(GL_POINTS);
    For (i,mN) glVertex2dv(mCityCoordinates[i]);
    glEnd();
    //--------------------------------------------
    glColor3ub(255,191,0);
    GFX_Text(mW-400,mH-50,"GENERATION: %d",mGenIdx);
    double x = GoalFunc(0);
    GFX_Text(mW-400,mH-30,"BEST PATH:  %3.1lf (%3.1lf%%)",
             x,100.*x/mStartValueBestChrom);
    //--------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------



//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                      Input (for Optional Use)
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::NormalKeys(unsigned char key, int state){
	if (key >= SDLK_0 && key <= SDLK_9){}
	if (key == SDLK_RETURN){}//Return
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::SpecialKeys(int key, int state){
    if (key == SDLK_LEFT){}
    if (key == SDLK_RIGHT){}
    if (key == SDLK_UP){}
    if (key == SDLK_DOWN){}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::Mouse(int button, int state, int x, int y){
	mMouseButton = button;//SDL_BUTTON_LEFT/SDL_BUTTON_MIDDLE/SDL_BUTTON_RIGHT
	mMouseState = state;//SDL_PRESSED/SDL_RELEASED
	mMouseX = x; mMouseY = y;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------



//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                                 Aux
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::InitGFX(){
    glClearColor(0.f,0.f,.2f,1.f);
    glLineWidth(3);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::InitMtxFont(){
	
	For (i,128) For (j,7) For (k,5) mMtxFont[i][j][k] = 1;
	
    const char F0[] =
    "11000" "11001" "00010" "00100" "01000" "10011" "00011"//%
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "00001" "00010" "00100" "00100" "00100" "00010" "00001"//(
    "10000" "01000" "00100" "00100" "00100" "01000" "10000"//)
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "00000" "00000" "00000" "11111" "00000" "00000" "00000"//-
    "00000" "00000" "00000" "00000" "00000" "01100" "01100"//.
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "01110" "10001" "10011" "10101" "11001" "10001" "01110"//0
    "00100" "01100" "00100" "00100" "00100" "00100" "01110"//1
    "01110" "10001" "00001" "00010" "00100" "01000" "11111"//2
    "01110" "10001" "00001" "00110" "00001" "10001" "01110"//3
    "00010" "00110" "01010" "10010" "11111" "00010" "00111"//4
    "11111" "10000" "11110" "00001" "00001" "10001" "01110"//5
    "01110" "10001" "10000" "11110" "10001" "10001" "01110"//6
    "11111" "10001" "00010" "00010" "00100" "00100" "00100"//7
    "01110" "10001" "10001" "01110" "10001" "10001" "01110"//8
    "01110" "10001" "10001" "01111" "00001" "00001" "01110"//9
    "00000" "01100" "01100" "00000" "01100" "01100" "00000"//:
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "00000" "00000" "11111" "00000" "11111" "00000" "00000"//=
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "01110" "10001" "10001" "00010" "00100" "00000" "00100"//?
    "11111" "11111" "11111" "11111" "11111" "11111" "11111"//
    "01110" "10001" "10001" "11111" "10001" "10001" "10001"//A
    "11110" "10001" "10001" "11110" "10001" "10001" "11110"//B
    "01110" "10001" "10000" "10000" "10000" "10001" "01110"//C
    "11110" "10001" "10001" "10001" "10001" "10001" "11110"//D
    "11111" "10000" "10000" "11110" "10000" "10000" "11111"//E
    "11111" "10000" "10000" "11110" "10000" "10000" "10000"//F
    "01110" "10001" "10000" "10111" "10001" "10001" "01110"//G
    "10001" "10001" "10001" "11111" "10001" "10001" "10001"//H
    "01110" "00100" "00100" "00100" "00100" "00100" "01110"//I
    "00001" "00001" "00001" "00001" "10001" "10001" "01110"//J
    "10001" "10010" "10100" "11000" "10100" "10010" "10001"//K
    "10000" "10000" "10000" "10000" "10000" "10000" "11111"//L
    "10001" "11011" "10101" "10101" "10001" "10001" "10001"//M
    "10001" "10001" "11001" "10101" "10011" "10001" "10001"//N
    "01110" "10001" "10001" "10001" "10001" "10001" "01110"//O
    "11110" "10001" "10001" "11110" "10000" "10000" "10000"//P
    "01110" "10001" "10001" "10001" "10101" "10010" "01101"//Q
    "11110" "10001" "10001" "11110" "10100" "10010" "10001"//R
    "01111" "10000" "10000" "01110" "00001" "00001" "11110"//S
    "11111" "00100" "00100" "00100" "00100" "00100" "00100"//T
    "10001" "10001" "10001" "10001" "10001" "10001" "01110"//U
    "10001" "10001" "10001" "10001" "10001" "01010" "00100"//V
    "10001" "10001" "10001" "10101" "10101" "10101" "01010"//W
    "10001" "10001" "01010" "00100" "01010" "10001" "10001"//X
    "10001" "10001" "10001" "01010" "00100" "00100" "00100"//Y
    "11111" "00001" "00010" "00100" "01000" "10000" "11111";//Z
    
	For (i,54) For (j,7) For (k,5) mMtxFont[i+37][j][k] = F0[35*i+5*j+k];
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::GFX_Text(double x, double y, const char *fmt,...){
    //--------------------------------------------
    //A simple C-string based print function
    //--------------------------------------------
	char text[256];
    va_list	ap;
	if (fmt == NULL) return;
	va_start(ap, fmt);
#ifdef _WIN32 //32-bit and 64-bit Windows
    vsprintf_s(text, fmt, ap);
#else //Typically Unix such as Mac OSX or Linux
    vsprintf(text, fmt, ap);
#endif
    va_end(ap);
    //--------------------------------------------
    int n = 0;
    //--------------------------------------------
    glPushMatrix();
    glLoadIdentity();
	glPointSize(2.f);
    glBegin(GL_POINTS);
	while (text[n] != 0){
		Draw_MtxFont(x+14.*double(n), y, text[n]);
		n++;
	}
    glEnd();
    //--------------------------------------------
    glPopMatrix();
    //--------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void inline Genetics::Draw_MtxFont(double x, double y, Uint8 c){
	if (c == ' ') return;	
    For (m,7) For (n,5){
        if (mMtxFont[c][m][n] == '0') continue;
		glVertex2d(x+2*n,y+2*m);
	}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------



//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                                Rand
//---------------------------------------------------------------------
//---------------------------------------------------------------------
int Genetics::Rand(int a){
    if (a < 1) a = 1; else if (a > 32768) a = 32768;
    sRandSeed = sRandSeed * 1103515245 + 12345;
    return (unsigned int)(sRandSeed/65536) % a;//return {0,1,...,a-1}
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
double Genetics::RandX(){
    return double(Rand(32768))/32768.;//return [0,1[
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------



//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                            Goal Function
//---------------------------------------------------------------------
//---------------------------------------------------------------------
double Genetics::GoalFunc(int chromIdx){
    double dist = 0., x0, x1, dx, y0, y1, dy;
    For (i,mN){
        int from = i;
        int to = i+1;
        if (to == mN) to = 0;
        int cityFromIdx = mGP[chromIdx][from];
        int cityToIdx = mGP[chromIdx][to];
        x0 = mCityCoordinates[cityFromIdx][0];
        x1 = mCityCoordinates[cityToIdx][0];
        dx = x1 - x0;
        y0 = mCityCoordinates[cityFromIdx][1];
        y1 = mCityCoordinates[cityToIdx][1];
        dy = y1 - y0;
        dist += sqrt(dx*dx + dy*dy);
    }
    return dist;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
inline void Genetics::EvalGoalFuncs(){
    For (m,mM) mChromIdx2GoalFunc[m] = GoalFunc(m);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------



//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                            Fundamentals
//---------------------------------------------------------------------
//---------------------------------------------------------------------
inline void Genetics::CopyChroms(int fromChromIdx, int toChromIdx){
    For (i,mN) mGP[toChromIdx][i] = mGP[fromChromIdx][i];
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::SwapChroms(int cIdx1, int cIdx2){
    double tmp = mChromIdx2GoalFunc[cIdx1];
    mChromIdx2GoalFunc[cIdx1] = mChromIdx2GoalFunc[cIdx2];
    mChromIdx2GoalFunc[cIdx2] = tmp;
    For (j,mN-1){
        int tmp = mGP[cIdx1][j];
        mGP[cIdx1][j] = mGP[cIdx2][j];
        mGP[cIdx2][j] = tmp;
    }
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
inline bool Genetics::IsEqual(int cIdx1, int cIdx2){
    For (n,mN) if (mGP[cIdx1][n] != mGP[cIdx2][n]) return false;
    return true;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::PartialSort(){
    //--------------------------------------------
    //--------------------------------------------
    //   Mark all Children Identical to Parents
    //--------------------------------------------
    //--------------------------------------------
    bool skipChrom[MAX_CHROMS_M];
    For (i,mM) skipChrom[i] = false;
    For (i,PARENTS_N){
        for (int ii = PARENTS_N; ii < mM; ii++){
            if ((mChromIdx2GoalFunc[i] == mChromIdx2GoalFunc[ii]) &&
                IsEqual(i,ii))
                skipChrom[ii] = true;
        }
    }
    //--------------------------------------------
    //--------------------------------------------
    //        Mark Duplicates of Children
    //--------------------------------------------
    //--------------------------------------------
    for (int i = PARENTS_N; i < mM; i++){
        for (int ii = i+1; ii < mM; ii++){
            if ((mChromIdx2GoalFunc[i] == mChromIdx2GoalFunc[ii]) &&
                IsEqual(i,ii))
                skipChrom[ii] = true;
        }
    }
    //--------------------------------------------
    //--------------------------------------------
    //              Select Survivals
    //--------------------------------------------
    //--------------------------------------------
    double min;
    int    minIdx;
    For (i,PARENTS_N){
        //----------------------------------------
        //----------------------------------------
        min = mChromIdx2GoalFunc[i];
        minIdx = i;
        for (int ii = i+1; ii < mM; ii++){
            if (skipChrom[ii]) continue;
            if (mChromIdx2GoalFunc[ii] < min){
                min = mChromIdx2GoalFunc[ii];
                minIdx = ii;
            }
        }
        if (minIdx != i) SwapChroms(i,minIdx);
        //----------------------------------------
        //----------------------------------------
    }
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::CloneChroms(){
    for (int toIdx = PARENTS_N; toIdx < mM; toIdx++){
        int fromIdx = toIdx % PARENTS_N;
        CopyChroms(fromIdx,toIdx);
    }  
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------



//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                              Basic Ops
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::Mutate(int chromIdx){//Cities {1,2,...,mN-2}
    int a = Rand(mN-1), b = Rand(mN-1);
    while (a == b) b = Rand(mN-1);
    Mutate(chromIdx,a,b);
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
inline void Genetics::Mutate(int chromIdx, int a, int b){
    int tmp = mGP[chromIdx][a];
    mGP[chromIdx][a] = mGP[chromIdx][b];
    mGP[chromIdx][b] = tmp;
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::TransformGP(){
    //--------------------------------------------
    //--------------------------------------------
    for (int i = PARENTS_N; i < mM; i++){
        //----------------------------------------
        //----------------------------------------
        if (i >= mM-2){
            if (i == mM-2){MPC(i,i+1); i++; break;}
            else {Mutate(i); break;}
        }
        //----------------------------------------
        //----------------------------------------
        int rnd = Rand(MPCS_PER_MUTATION+1);//0-3
        if (rnd == 0) Mutate(i);
        else {MPC(i,i+1); i++;}
        //----------------------------------------
        //----------------------------------------
    }
    //--------------------------------------------
    //--------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------



//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                             City Coords
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::GenCityCoords(){
    //--------------------------------------------
    //--------------------------------------------
    double min[2], delta[2];
    min[0] = .1 * double(mW); delta[0] = .8 * double(mW);
    min[1] = .1 * double(mH); delta[1] = .8 * double(mH);
    For (i,mN) For (j,2){
        mCityCoordinates[i][j] = min[j] + delta[j] * RandX();}
    //--------------------------------------------
    //--------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                      Generate First Generation
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::InitFirstGen(){
    //--------------------------------------------
    //--------------------------------------------
    sRandSeed = (unsigned int)time(NULL);//Random seed for each run
    GenCityCoords();
    For (m,mM){
        For (n,mN) mGP[m][n] = n;
        For (i,1000) Mutate(m);
    }
    EvalGoalFuncs();
    mGenIdx = 0;
    //--------------------------------------------
    //--------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                      Generate Next Generation
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::EvalNextGen(){
    //--------------------------------------------
    PartialSort();
    if (mGenIdx == 0) mStartValueBestChrom = mChromIdx2GoalFunc[0];
    //--------------------------------------------
    if (mGenIdx >= MAX_NEW_GENS) return;
    //--------------------------------------------
    CloneChroms();
    TransformGP();
    EvalGoalFuncs();
    mGenIdx++;
    //--------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------



//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                                 MPC
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::MPC(int chromIdx1, int chromIdx2){
    //--------------------------------------------
    //----------------------------------------------
    //------------------------------------------------
    //--------------------------------------------------
    return;//Comment out this line for the completion of MPC_T_RepairChrom!
    //--------------------------------------------------
    //------------------------------------------------
    //----------------------------------------------
    //--------------------------------------------
    // Evaluate the MPC interval for this MPC as
    // a function of random numbers 
    //--------------------------------------------
    //--------------------------------------------
    int maxMPCWidth = mN/2;
    mWidth = 3 + Rand(maxMPCWidth-3);
    mFromIdx = Rand(mN - mWidth);
    mToIdx = mFromIdx + mWidth - 1;
    //--------------------------------------------
    //--------------------------------------------
    // Swap the data within the interval
    // [mFromIdx,mToIdx] between the chromosomes
    // chromIdx1 and chromIdx2
    //--------------------------------------------
    //--------------------------------------------
    for (int i = mFromIdx; i <= mToIdx; i++){
        int tmp = mGP[chromIdx2][i];
        mGP[chromIdx2][i] = mGP[chromIdx1][i];
        mGP[chromIdx1][i] = tmp;
    }
    //--------------------------------------------
    //--------------------------------------------
    //           Repair the chromosomes
    //--------------------------------------------
    //--------------------------------------------
    MPC_T_RepairChrom(chromIdx1);
    MPC_T_RepairChrom(chromIdx2);
    //--------------------------------------------
    //--------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------



//---------------------------------------------------------------------
//---------------------------------------------------------------------
//                           Assignment 2.2
//---------------------------------------------------------------------
//---------------------------------------------------------------------
void Genetics::MPC_T_RepairChrom(int chromIdx){
    //--------------------------------------------
    //--------------------------------------------
    // Assignment 2.2: Complete this method and
    // upload Genetics.cpp to
    // "A2.2 - Genetic Algorithms"
    //--------------------------------------------
    //--------------------------------------------
    // Step 1: Outside of the crossover interval
    // [mFromIdx,mToIdx], replace each duplicate
    // number with -1 and add each position to the
    // list duplicatesList, followed by
    // numberOfDuplicatesN++ after each addition
    // to keep a track the number of found
    // duplicates
    //--------------------------------------------
    //--------------------------------------------
    int duplicatesList[MAX_CITIES_N];
    int numberOfDuplicatesN = 0;
    //... implement code (A)
    //--------------------------------------------
    //--------------------------------------------
    // Step 2: Find out which numbers that are
    // present in the chrom except for -1
    //--------------------------------------------
    //--------------------------------------------
    bool numbersIncluded[MAX_CITIES_N];
    For (i,MAX_CITIES_N) numbersIncluded[i] = false;
    //... implement code (B)
    //--------------------------------------------
    //--------------------------------------------
    // Step 3: Find out which numbers that are
    // missing in the chrom based on
    // numbersIncluded, and add each number to the
    // list missingNumbersList, followed by
    // numberOfMissingNumbersN++ after each
    // addition to keep track
    //--------------------------------------------
    //--------------------------------------------
    int missingNumbersList[MAX_CITIES_N];
    int numberOfMissingNumbersN = 0;
    //... implement code (C)
    //--------------------------------------------
    //--------------------------------------------
    // Note: numberOfDuplicatesN !=
    // numberOfMissingNumbersN => Error!
    //--------------------------------------------
    //--------------------------------------------
    // Step 4: Fill the holes with the missing
    // numbers by ascending order (important for
    // the test to pass)
    //--------------------------------------------
    //--------------------------------------------
    //... implement code (D)
    //--------------------------------------------
    //--------------------------------------------
}
//---------------------------------------------------------------------
//---------------------------------------------------------------------






