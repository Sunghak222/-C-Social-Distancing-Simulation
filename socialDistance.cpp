#include <iostream>
#include <windows.h>
#include <iomanip>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#define INIT_X 20
#define INIT_Y 5

using namespace std;

static int current_contagious = 0;
static int current_vaccinated = 0;

void gotoxy(short x, short y)
{
    COORD pos{x,y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),pos);
}

class People //1:contagious(Red) 2:vaccinated(Blue) 3:Normal(Green)
{
protected:
    int status;
    int x_loc;
    int y_loc;
    int x_velocity = 1; //Velocity is fixed at 1 right now. Will be updated.
    int y_velocity = 1;
    int direction;
public:
    void SetInfo(int X, int R, int V)
    {
        /*Set random information of people*/
        if (current_contagious == R && current_vaccinated == V)
        {
            status = 3;
        }
        else if (current_contagious == R)
            status = (rand()%2)+2;
        else if (current_vaccinated == V)
        {
            status = (rand()%2)+1;
            if (status == 2)
            {
                status++;
            }
        }
        else
            status = (rand()%3)+1;
        current_contagious = (status == 1) ? ++current_contagious : current_contagious;
        current_vaccinated = (status == 2) ? ++current_vaccinated : current_vaccinated;
        x_loc = (rand()%X)+21;
        y_loc = (rand()%X)+6;
        direction = (rand()%8)+1;
    }
    void Infect();
    int GetStatus();
    int GetX_Loc();
    int GetY_Loc();
    void DrawPeople()
    {
        if (status == 1)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
        }
        else if (status == 2)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
        }
        else if (status == 3)
        {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10);
        }
        gotoxy(x_loc,y_loc);
        cout << "o";
    }
    void MovePeople(int X)
    {
        /*Move people according to their direction and velocity, also considering people when colliding with walls.*/
        if (x_loc == INIT_X+1 && y_loc == INIT_Y+1 && (direction != 5 || direction != 7 || direction != 8)) //Top-left corner
        {
            direction = (rand()%3)+5;
            if (direction != 5)
                direction++;
        }
        else if (x_loc == INIT_X+1 && y_loc == INIT_Y+X && (direction != 2 || direction != 3 || direction != 5)) //Down-left corner
        {
            direction = (rand()%3)+2;
            if (direction == 4)
                direction++;
        }
        else if (x_loc == INIT_X+X && y_loc == INIT_Y+1 && (direction != 2 || direction != 6 || direction != 7)) //Top-right corner
        {
            direction = (rand()%3)+4;
            if (direction != 4)
                direction++;
        }
        else if (x_loc == INIT_X+X && y_loc == INIT_Y+X && (direction != 2 || direction != 6 || direction != 7)) //Down-right corner
        {
            direction = (rand()%3)+1;
            if (direction == 3)
                direction++;
        }

        else if (x_loc == INIT_X+1 && (direction == 1 || direction == 4 || direction == 6)) //Left wall
        {
            direction = (rand()%5)+1;
            if (direction < 3)
                direction++;
            else if (direction == 3)
                direction += 2;
            else 
                direction += 3;
        }
        else if (x_loc == INIT_X+X && (direction == 3 || direction == 5 || direction == 8)) //Right wall
        {
            direction = (rand()%5)+1;
            if (direction == 3)
                direction++;
            else if (direction > 3)
                direction += 2; 
        }
        else if (y_loc == INIT_Y+1 && (direction < 4)) //Top wall
        {
            direction = (rand()%5)+4;
        }
        else if (y_loc == INIT_Y+X && (direction > 5)) //Down wall
        {
            direction = (rand()%5)+1;
        }

        /*Move people*/
        if (direction == 1) // 1:topleft
        {
            x_loc -= x_velocity;
            y_loc -= y_velocity;
        }
        else if (direction == 2) //2:top
        {
            y_loc -= y_velocity;
        }
        else if (direction == 3) //3:topright
        {
            x_loc += x_velocity;
            y_loc -= y_velocity;
        }
        else if (direction == 4) //4:left
        {
            x_loc -= x_velocity;
        } 
        else if (direction == 5) //5:right 
        {
            x_loc += x_velocity;
        }
        else if (direction == 6) //6:downleft
        {
            x_loc -= x_velocity;
            y_loc += y_velocity;
        }
        else if (direction == 7) //7:down
        {
            y_loc += y_velocity;
        }
        else if (direction == 8) //8:downright
        {
            x_loc += x_velocity;
            y_loc += y_velocity;
        }
    }
};

class Simulation : public People
{
private:
    int N;
    int X;
    int C;
    int R;
    int V;
public:
    int Iteration = 0;
    Simulation();
    Simulation(int _N, int _X, int _C, int _R, int _V)
    {
        N = _N;
        X = _X;
        C = _C;
        R = _R;
        V = _V;
    }
    void PrintInfo()
    {
        cout << N << " " << X << " " << C << " " << R << " " << V << endl;
    }
    void DrawSquare() //x: 20~20+X+2 y:5~5+X+2
    {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
        for (int i = 0; i < X+2; i++)
        {
            gotoxy(INIT_X+i,5);
            cout << "#";
            gotoxy(INIT_X+i,INIT_Y+X+1);
            cout << "#";
        }
        for (int i = 0; i < X+2; i++)
        {
            gotoxy(INIT_X,INIT_Y+i);
            cout << "#";
            gotoxy(INIT_X+X+1,INIT_Y+i);
            cout << "#";
        }
    }
    void ShowResult()
    {
        gotoxy(INIT_X,INIT_Y+X+2);
        cout << "AFTER " << Iteration << " SECONDS,";
        gotoxy(INIT_X,INIT_Y+X+3);
        cout << "NUMBER OF CONTAGIOUS PEOPLE: " << current_contagious;
        gotoxy(INIT_X,INIT_Y+X+4);
        cout << "NUMBER OF UNCONTAGIOUS PEOPLE: " << N-current_contagious;
    }
};

void People::Infect()
{
    status = 1;
}
inline int People::GetStatus()
{
    return status;
}
inline int People::GetX_Loc()
{
    return x_loc;
}
inline int People::GetY_Loc()
{
    return y_loc;
}

void MainMenu()
{
    system("cls");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
    gotoxy(5,12);
    cout << "Welcome to Social Distancing Simulation!";
    gotoxy(14,14);
    cout << "Tap any key to start!";
    for (int i = 0; i < 50; i++)
    {
        gotoxy(i,5);
        cout << "+";
        gotoxy(i,19);
        cout << "+";
    }
    for (int i = 0; i < 15; i++)
    {
        gotoxy(0,5+i);
        cout << "+";
        gotoxy(49,5+i);
        cout << "+";
    }
    while (true)
    {
        if (_kbhit())
        {
            _getch();
            system("cls");
            break;
        }
    }
}
void CheckDistanceAndInfect(People people[], int size, int C)
{
    /*Check whether two people are in distance of C meter and infect normal people*/
    double distance;
    int status1,status2,i,j;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            distance = sqrt(pow((people[i].GetX_Loc()-people[j].GetX_Loc()),2) + pow((people[i].GetY_Loc()-people[j].GetY_Loc()),2));
            if (distance <= C)
            {
                status1 = people[i].GetStatus();
                status2 = people[j].GetStatus();
                if (status1 != status2 && status1 != 2 && status2 != 2)
                {
                    people[i].Infect();
                    people[j].Infect();
                    current_contagious++;
                    i = 0;
                    j = 0;
                }
            }
        }
    }
}
int main()
{
    srand(time(NULL));
    int get;
    int NumOfPeople,SideOfSquare,MinDist,NumOfContagious,NumOfVaccin; //N,X,C,R,V

    People *people = new People[NumOfPeople];
    MainMenu();
    cout << "To finish simulation, press ESC.\n";
    cout << "Please input the total number of people.\n";
    cin >> NumOfPeople;
    cout << "\nPlease input the side of square.\n";
    cout << "Tip: Not too large and small!\n";
    cin >> SideOfSquare ;
    cout << "\nPlease input the distance between two people that can infect each other.\n";
    cout << "Tip: Don't put too big number!\n";
    cin >> MinDist;
    cout << "\nPlease input the number of contagious people.\n";
    cout << "Tip: Number of Contagious People <= Total number of People - Number of Vaccinated People\n";
    cin >> NumOfContagious;
    cout << "\nPlease input the number of vaccinated people.\n";
    cout << "Tip: Number of Vaccinated People <= Total number of People - Number of Contagious People\n";
    cin  >> NumOfVaccin;

    Simulation sm(NumOfPeople,SideOfSquare,MinDist,NumOfContagious,NumOfVaccin);

    system("cls");

    for (int i = 0; i < NumOfPeople; i++)
    {
        people[i].SetInfo(SideOfSquare,NumOfContagious,NumOfVaccin);
        //people[i].PrintInfo();
    }

    sm.DrawSquare();

    for (int i = 0; i < NumOfPeople; i++)
    {
        people[i].DrawPeople();
    }
    
    while (true)
    {
        if (_kbhit())
        {
            get = _getch();
            if (get == 27)
            {
                break;
            }
        }
        system("cls");
        sm.DrawSquare();
        sm.ShowResult();
        for (int i = 0; i < NumOfPeople; i++)
        {
            people[i].DrawPeople();
            CheckDistanceAndInfect(people,NumOfPeople,MinDist);
            people[i].MovePeople(SideOfSquare);
        }
        sm.Iteration++;
        Sleep(1000);
    }
    system("cls");

    delete[] people;

    return 0;
}
