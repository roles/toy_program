#include "graphics.h"
#include "extgraph.h"
#include <iostream>
#include <vector>
#include <string>
#include "random.h"

using namespace std;

struct point{
    double x, y;
};

void test(){
    cout << "width : " << GetWindowWidth() << endl;
    cout << "height : " << GetWindowHeight() << endl;
    cout << "current X : " << GetCurrentX() << endl;
    cout << "current Y : " << GetCurrentY() << endl;
    WaitForMouseDown();
    cout << "mouse X : " << GetMouseX() << endl;
    cout << "mouse Y : " << GetMouseY() << endl;
}

void getInitPoints(vector<point>& pvec){
    for(int i = 0; i < 3; i++){
        WaitForMouseDown();
        pvec[i].x = GetMouseX();
        pvec[i].y = GetMouseY();
        cout << "mouse X : " << pvec[i].x << endl;
        cout << "mouse Y : " << pvec[i].y << endl;
        WaitForMouseUp();
    }
}

void drawTriangle(vector<point>& pvec){
    MovePen(pvec[0].x, pvec[0].y);
    for(int i = 0, j = 0; i < 3; i++){
        j = (i + 1) % 3;
        DrawLine(pvec[j].x - pvec[i].x, pvec[j].y - pvec[i].y);
    }
}

void chaoGame(vector<point>& pvec){
    double curX, curY;
    int i = RandomInteger(0, 3);

    curX = pvec[i].x;
    curY = pvec[i].y;
    MovePen(curX, curY);
    SetPenColor(string{"red"});
    for(;;){
        StartFilledRegion();
        DrawArc(0.02, 0, 360);
        EndFilledRegion();
        if(MouseButtonIsDown()){
            break;
        }
        i = RandomInteger(0, 3);
        curX = (curX + pvec[i].x) / 2.0;
        curY = (curY + pvec[i].y) / 2.0;
        MovePen(curX, curY);
        UpdateDisplay();
    }
}

int main(){
    vector<point> pvec(3);

    InitGraphics();     
    Randomize();

    getInitPoints(pvec);
    drawTriangle(pvec);
    chaoGame(pvec);
    return 0;
}
