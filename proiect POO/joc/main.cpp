#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <ctime>
#include <fstream>
#include <iomanip>

using namespace std;

ifstream inp;  //operatorii de citire și scriere a fișierelor de date
ofstream outp;

char c[30][21]; //variabilă pentru stocarea particulelor de ecran (pixeli)
int n[30][21];  //variabilă pentru verificare
int highscore;
int contr,tuk=0,score=0,t=0,bt=0,birdx=0,birdy=0; //variabile pentru anumite operații
bool err; //pentru detectarea erorilor

void game();  //diverse funcții

void screen();
void pipes();
void bird();
bool gameover();
void checkscore();
void help();
void menu();
void endgame();
void credits();

int main()
{
srand(time(0));  //însămânțarea numărului aleatoriu gen, vom avea nevoie de el mai târziu;
inp.open("/Program Files/FlappyBird/options.txt");  //deschiderea fișierului în care este stocat scorul maxim
if(inp.is_open()) //dacă fișierul se deschide cu succes, citește scorul maxim
{
    inp>>highscore;
    inp.close();
    err=false;  //eroarea va fi falsă, deoarece fișierul a fost deschis cu succes
}
else
{
    highscore=0; //dacă fișierul nu există, scorul maxim va fi 0 și err va fi adevărat
    err=true;
    err=true;
}

int a=0,b;
char sl; //variabila de selectie

while(1) //buclă pentru repetarea acțiunilor după fiecare pornire
{
    if(a==0) goto play;
    if(a>0)               //dacă nu joci prima dată, te va întreba dacă vrei să joci
    {
        score=0;
        cout<<"Vrei sa joci din nou? [y/n] ";
        cin>>sl;
        if(sl=='n') goto quit;
        else goto play;
    }
    play:
    menu(); //Apelarea funcției de meniu
    cin>>sl;
    switch(sl) //selecții de meniu

    {
        case '1':
        {
            game(); //dacă alegeți jocul, acesta va rula
            break;
        }
        case '2': //alte selecții-alte funcții
        {
            help();
            goto play;
            break;
        }
        case '3':
        {
            credits();
            goto play;
            break;
        }
        case '4':
        {
            goto quit; //iese din joc
            break;
        }
        default:
        {
            goto play;
            break;
        }
    }
    a++; //variabilă pentru a verifica de câte ori ai jucat
}
quit:
{
   cout<<"I quit."; //oprește jocul, aplicația se închide.
}

return 0;
}

void game()  //funcție pentru joc
{
    int x,y;
    char s;
    for(y=0;y<21;y++)  //ecran de setare

    {
        for(x=0;x<30;x++)
        {
            if(y<20)
            {
            c[x][y]=' ';
            n[x][y]=0;
            }
            if(y==20)
            {
                c[x][y]='-';
                n[x][y]=2;
            }
        }
    }
    c[10][10]='*';  //în aceste coordonate va fi pasărea noastră, marcată cu *
    screen();      //apelează funcția pentru afișarea ecranului
    while(1)       //începe bucla, începe jocul propriu-zis
    {
        s='~';  //variabilă de control implicită
        Sleep(0.2*1000);  //asta stabilește cât de repede se mișcă totul
        t++;              //aceasta este o variabilă pentru stocarea „timp”, sau de câte ori a trecut o buclă
        if(kbhit()) //dacă tasta este apăsată, se fac anumite operații pentru ca pasărea să se miște în sus.
        {
            s=getch();        //primește ce tastă este apăsată
            if(s!='~') tuk=1; //dacă nu este implicit, atunci „tuk” va fi egal cu 1, ceea ce înseamnă că pasărea va zbura în sus
        }
        for(x=0;x<30;x++) //setam terenul
        {
            c[x][20]='-';
            n[x][20]=2;
        }
        bird();                       //funcția de mutare a păsări
        checkscore();                 ////verifică scorul
        if(gameover()==true) goto gameEnd;   //verifică dacă pasărea lovește peretii, dacă da, jocul se termină
        pipes();                             //generează și mută peretii
        if(score>highscore) highscore=score;  //scorul cel mai mare
        screen();                            //în final, apelează funcția ecran pentru a afișa totul.

        if(tuk>0) tuk++;           // tasta f este apăsată, pasărea va zbura în sus de 2 ori.
        if(tuk==3) tuk=0;          //in final, pasarea cade
    }
    gameEnd:   //ends game
    {
        if(score>highscore) highscore=score;
        if(err==false)              //dacă există fișierul cu scor mare, acesta scrie noul punctaj maxim acolo.
        {
            outp.open("/Program Files/FlappyBird/options.txt");
            outp<<highscore;
            outp.close();
        }
        screen();    //afișează ecranul de sfârșit și revine la int main
        endgame();
        return;
    }
}

void screen()    //func pentru afișarea ecranului
{
    int x,y;
    system("cls");    //golirea conolei
    for(y=0;y<21;y++) //afișează pixelii pe coordonatele lor și scorul dvs
    {
        for(x=0;x<30;x++)
        {
            if(x<29) cout<<c[x][y];
            if(x==29) cout<<c[x][y]<<endl;
        }
    }
    cout<<""<<endl;
    cout<<"Scorul tau este: "<<score;
}

void pipes()  //miscarea peretilor si functia spawn
{
    int x,y,r;
    if(t==10)   //dacă timpul este 10 sau bucla a trecut de 10 ori, generează o nouă conductă
    {
        r=(rand()%11)+5;  //generează un număr aleator, care va fi centrul găurii țevii
        for(y=0;y<20;y++)  // este nevoie doar de coordonatele y
        {
            c[29][y]='|';  //setarea peretilor
            n[29][y]=2;    //n va fi 2, pentru a verifica dacă pasărea a lovit
        }
        c[29][r-1]=' ';  //setam gaurile din pereti
        c[29][r]=' ';
        c[29][r+1]=' ';
        n[29][r-1]=0;
        n[29][r]=0;
        n[29][r+1]=0;
        t=0;
        goto mv; //peretii se misca
    }
    else goto mv;
    mv:                 //miscarea peretiilor
    {
        for(y=0;y<20;y++)  //bucle pentru generarea coordonatelor
        {
            for(x=0;x<30;x++)
            {
                if(c[x][y]=='|')  //toti peretii vor fi mutate la stânga cu 1;
                {
                    if(x>0)
                    {
                        c[x-1][y]='|';
                        n[x-1][y]=2;
                        c[x][y]=' ';
                        n[x][y]=0;
                    }
                    if(x==0)  //dacă ecranul se termină (x=0) conducta va dispărea, pentru a preveni erorile
                    {
                        c[x][y]=' ';
                        n[x][y]=0;
                    }
                }
            }
        }
    }
}

void bird()   //funcția de mișcare a păsărilor!
{
    int x,y;
    if(tuk>0) //dacă tasta este apăsată, pasărea se mișcă în sus
    {
        bt=0;
        for(y=0;y<20;y++)   //bucle pentru găsirea coordonatelor păsărilor
        {
            for(x=0;x<30;x++)
            {
                if(c[x][y]=='*')
                {
                    if(y>0)
                    {
                    c[x][y-1]='*';  //pasărea se mișcă în sus cu 1;
                    c[x][y]=' ';
                    birdx=x;        //stabilește coordonatele pasăre x
                    birdy=y-1;      //stabilirea cordonatei pasarei y
                    return;         //se intoarce la functia jocului
                    }
                }
            }
        }
    }
    else   //daca nici o tasta nu este apasata casa va cadea
    {
        bt++;
        for(y=0;y<20;y++)
        {
            for(x=0;x<30;x++)
            {
                if(c[x][y]=='*')
                {
                    if(y<20)  //daca pasarea nu este la sol
                    {
                        if(bt<3)   //dacă timpul păsării este mai mic de 3, scade cu 1 pixel
                        {
                            c[x][y+1]='*';
                            c[x][y]=' ';
                            birdx=x;
                            birdy=y+1;
                            return;
                        }
                        else if(bt>2 && bt<5)  //a trecut mai mult timp, păsările cade mai repede (accelerare)
                        {
                            c[x][y+2]='*';
                            c[x][y]=' ';
                            birdx=x;
                            birdy=y+2;
                            return;
                        }
                        else if(bt>4)
                        {
                            c[x][y+3]='*';
                            c[x][y]=' ';
                            birdx=x;
                            birdy=y+3;
                            return;
                        }
                    }
                    else
                    {
                        return;  //if bird is already on the ground, function returns to check for game over.
                    }
                }
            }
        }
    }
}
void checkscore()  //verifică dacă pasărea a câștigat punctaj
{

  int y;
    for(y=0;y<20;y++)
    {
        if(c[birdx][y]=='|')  //dacă bird x coord este egal cu coord pipe, obțineți 1 punct
        {
            score++;
            return;
        }
    }
}

bool gameover()  //verifică dacă pasărea a lovit ceva
{
    int x,y,f=0;
    if(birdy>19) //verifică dacă pasărea lovește pământul
    {
        c[birdx][19]='*';  //pune din nou pasărea și pământul, previne erorile
        c[birdx][20]='-';
        f=1;           //f=1, înseamnă că funcția va returna adevărat
        goto quit;
    }
    else
    {     //verifică dacă pasărea a lovit țevi, aici este necesară variabila „n” (coordonatele țevii n este egal cu 2 (mai mult de 0))
        if(n[birdx][birdy]>0 && (c[birdx][birdy]=='|' || c[birdx][birdy]=='*'))
        {
            c[birdx][birdy]='|';
            c[birdx-1][19]='*';
            f=1;
            goto quit;
        }
    }
    quit:
    if(f==1) return true;
    else return false;
}

void endgame() //doar câteva ecrane pentru anumite acțiuni
{
    screen();   //acesta va aparea doar cand jocul se termina
    cout<<""<<endl<<endl;
    cout<<" ------------------------------------------------------------------------- "<<endl;
    cout<<"|    *****      *     *       * ******       ****  *       ****** ****    |"<<endl;
    cout<<"|   *          * *    * *   * * *           *    *  *     * *     *   *   |"<<endl;
    cout<<"|   *  ****   *   *   *  * *  * *****       *    *   *   *  ****  ****    |"<<endl;
    cout<<"|   *  *  *  *******  *   *   * *           *    *    * *   *     * *     |"<<endl;
    cout<<"|    *****  *       * *       * ******       ****      *    ***** *   *   |"<<endl;
    cout<<" ------------------------------------------------------------------------- "<<endl;
    cout<<""<<endl<<endl;
    cout<<"                        Y O U R   S C O R E : "<<score<<endl<<endl;
    cout<<"                        H I G H   S C O R E : "<<highscore<<endl;
    cout<<""<<endl<<endl;
}

void menu()  //afiseaza meniul
{
    system("cls");
    cout<<""<<endl;
    cout<<" --------------------------------------------------------  "<<endl;
    cout<<"|                                                        | "<<endl;
    cout<<"|   **** *    **** **** **** *   *    ***  * ***  ***    | "<<endl;
    cout<<"|   *    *    *  * *  * *  * *   *    *  * * *  * *  *   | "<<endl;
    cout<<"|   ***  *    **** **** **** *****    ***  * ***  *  *   | "<<endl;
    cout<<"|   *    *    *  * *    *      *      *  * * *  * *  *   | "<<endl;
    cout<<"|   *    **** *  * *    *      *      ***  * *  * ***    | "<<endl;
    cout<<"|                                                        | "<<endl;
    cout<<" --------------------------------------------------------  "<<endl;
    cout<<""<<endl<<endl;
    cout<<"                  High Score:  "<<highscore<<endl<<endl;
    cout<<""<<endl<<endl;
    cout<<"                     M E N U:    "<<endl<<endl;
    cout<<"                  1: Incepe jocul  "<<endl<<endl;
    cout<<"                  2: Help     "<<endl<<endl;
    cout<<"                  3: Credite     "<<endl<<endl;
    cout<<"                  4: Exit        "<<endl<<endl;
}

void credits()
{
    char sel;
    system("cls");
    while(true)
    {
    cout<<""<<endl<<endl;
    cout<<"               Designer: Cristi Cudla "<<endl<<endl;
    cout<<"               Testers: Cristi Cudla "<<endl<<endl;
    cout<<"               Version: 1.0 "<<endl<<endl<<endl;
    cout<<"Go back? [y/n]  ";
    cin>>sel;
    if(sel=='y') return;
    else system("cls");
    }
}

void help()
{
    char sel;
    system("cls");
    while(true)
    {
    cout<<""<<endl<<endl;
    cout<<"                   Comenzi: apasati orice tasta pentru a zbura in sus. "<<endl<<endl;
    cout<<"             Scop: Zburati prin gaurile dintre pereti.   "<<endl;
    cout<<"              Cand treci prin gaura, primesti 1 punct."<<endl;
    cout<<"                  Incearca sa treci cat poti de mult.          "<<endl;
    cout<<"            Dar ai grija, nu lovi peretii sau pamantul!"<<endl<<endl;
    cout<<"                         Sunteai gata?Incepem jocul            "<<endl<<endl<<endl;
    cout<<"Go back? [y/n]  ";
    cin>>sel;
    if(sel=='y') return;
    else system("cls");
    }
}
