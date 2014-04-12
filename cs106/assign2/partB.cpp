#include "maze.h"
#include "random.h"
#include <set>
#include <queue>
#include <map>

using namespace std;

const int numRows = 10;
const int numCols = 20;

/*
 * 迷宫生成器
 * --------------------
 * 使用Aldous-Broder算法
 * (1)随机选择矩阵中的一个点作为当前点
 * (2)随机选择一个当前点的邻居作为选择点
 * (3)如果如果选择点为"excluded"，则当前点与选择点之间的墙清除，选择点标记为"included"
 * (4)将选择点作为当前点，重复(2)(3)(4)直到所有点都是"included"
 */
class MazeGenerator {
    public:
        /*
         * 构造函数：MazeGenerator
         * 用法：MazeGenerator mg(m);
         * ---------------------
         * 将私有的Maze指针指向m
         */
        MazeGenerator(Maze& m);
        Maze& generateMaze();

    private:
        pointT getRandomNeighborPoint(pointT p);
        Maze* m;
        pointT curp;
        set<pointT> pointSet;
};

class MazeSolver {
    public:
        void solveMaze();
        /*
         * 构造函数：MazeSolver
         * 用法：MazeSolver ms(m);
         * ---------------------
         * 将私有的Maze指针指向m
         * 设定迷宫的起始和结束点
         */
        MazeSolver(Maze& m);

    private:
        enum dirT {North, West, East, South};
        Maze* m;
        pointT startp, endp;
        map<pointT, dirT> pointSet;
        pointT getPointByDir(pointT, dirT);
};

pointT MazeGenerator::getRandomNeighborPoint(pointT p){
    pointT newp{p};

    for(;;){
        int dir = RandomInteger(0, 4);
        switch(dir){
            case 0:     //north
               newp.row--; 
               break;
            case 1:     //south
               newp.row++; 
               break;
            case 2:     //west
               newp.col--;
               break;
            case 3:     //east
               newp.col++;
               break;
        }
        if(m->pointInBounds(newp)){
            return newp;
        }else{
            newp = p;
        }
    }
}

MazeGenerator::MazeGenerator(Maze& m){
    this->m = &m;
}

Maze& MazeGenerator::generateMaze(){
    Randomize();
    curp.row = RandomInteger(0, m->numRows());
    curp.col = RandomInteger(0, m->numCols());
    pointSet.clear();
    pointSet.insert(curp);

    while(pointSet.size() < m->numRows() * m->numCols()){
        pointT newp{ getRandomNeighborPoint(curp) };
        if(pointSet.find(newp) == pointSet.end()){  //excluded
            m->setWall(curp, newp, false);  
            pointSet.insert(newp);
        }
        curp = newp;
    }

    return *m;
}

MazeSolver::MazeSolver(Maze& m){
    this->m = &m;
    this->startp = {0, 0};
    this->endp = {m.numRows() - 1, m.numCols() - 1};
}

pointT MazeSolver::getPointByDir(pointT p, dirT dir){
    pointT newp{p};
    switch(dir){
        case North:     
           newp.row++; 
           break;
        case South:     
           newp.row--; 
           break;
        case East:     
           newp.col++;
           break;
        case West:     
           newp.col--;
           break;
    }
    return newp;
}

/*
 * 成员函数：solveMaze
 * 用法：ms.solveMaze();
 * --------------------
 * 使用广度优先算法遍历迷宫，pointQueue用来保存遍历的点
 * pointSet是一个map，以point为key，方向为value，
 * 作用是记录访问点的路径，用于最后逆向生成路径
 * 方向是一个枚举类型，相反方向之和为3，可用于计算相反方向
 *
 */
void MazeSolver::solveMaze(){
    pointT curp{ startp }; 
    queue<pointT> pointQueue;
    
    pointQueue.push(curp);
    bool isStop = false;
    while(!isStop && !pointQueue.empty()){
        curp = pointQueue.front();
        pointQueue.pop();
        for(int i = 0; i < 4; i++){
            pointT newp = getPointByDir(curp, static_cast<dirT>(i));
            if(m->pointInBounds(newp) && 
               !m->isWall(curp, newp) && 
               pointSet.find(newp) == pointSet.end()){
                pointSet[newp] = static_cast<dirT>(i);
                pointQueue.push(newp);
            }
            if(newp.row == endp.row && newp.col == endp.col){
                isStop = true;
                break;
            }
        }
    }

    curp = endp;
    while(curp.row != startp.row || curp.col != startp.col){
        m->drawMark(curp, "Red");
        dirT reverseDir = static_cast<dirT>(3 - static_cast<int>(pointSet[curp]));
        curp = getPointByDir(curp, reverseDir);
    }
    m->drawMark(curp, "Red");
}

int main(){
    Maze m{numRows, numCols, true};
    MazeGenerator mg{m};
    MazeSolver ms{m};
    
    m.draw();
    mg.generateMaze();
    ms.solveMaze();

    return 0;
}
