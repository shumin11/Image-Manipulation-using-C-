#include "treasureMap.h"
#include "queue.h"
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{

/* YOUR CODE HERE */
    base = baseim;
    maze = mazeim;
    start = s;
}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){

/* YOUR CODE HERE */
    RGBAPixel *locPixel = im.getPixel(loc.first, loc.second);
    locPixel->r = 2* (locPixel->r / 4);
    locPixel->g = 2* (locPixel->g / 4);
    locPixel->b = 2* (locPixel->b / 4);
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){

/* YOUR CODE HERE */
     unsigned char newd = d % 64;
     RGBAPixel *locPixel = im.getPixel(loc.first, loc.second);
     unsigned char r = locPixel-> r & 0b11111100;
     unsigned char g = locPixel-> g & 0b11111100;
     unsigned char b = locPixel-> b & 0b11111100;
     locPixel->r = r | (newd >> 4);
     locPixel->g = g | (newd >> 2 & 0b00000011);
     locPixel->b = b | (newd & 0b00000011);
}

PNG treasureMap::renderMap(){

/* YOUR CODE HERE */
    PNG newImage = base;
    vector<vector<bool>> visit(base.height(), vector<bool>(base.width()));
    for (int i = 0; i< base.width(); i++) {
        for (int j; j < base.height(); j++) {
            visit[j][i] = false;
        }
    }
    vector<vector<int>> distance(base.height(), vector<int>(base.width()));
    Queue<pair<int, int>> q;

    int x = start.first;
    int y = start.second;
    visit[y][x] = true;
    distance[y][x] = 0;
    setLOB(newImage, start, 0); 
    q.enqueue(start);
   
    while (!q.isEmpty()) {
        pair<int,int> curr = q.dequeue();
        vector<pair<int, int>> n = neighbors(curr);

        for (int i = 0; i < n.size(); i++) {   
          if (good(visit, curr, n[i])) {
            visit[n[i].second][n[i].first] = true;
            distance[n[i].second][n[i].first] = distance[curr.second][curr.first] + 1;
            setLOB(newImage, n[i], distance[n[i].second][n[i].first]);
            q.enqueue(n[i]);          
         }   
        }        
    }
    return newImage;

}


PNG treasureMap::renderMaze(){

/* YOUR CODE HERE */
    PNG newMaze = base;

    vector<vector<bool>> visit(base.height(), vector<bool>(base.width()));
    for (int i = 0; i< base.width(); i++) {
        for (int j; j < base.height(); j++) {
            visit[j][i] = false;
        }
    }
    vector<vector<int>> distance(base.height(), vector<int>(base.width()));
    Queue<pair<int, int>> q;
    
    int x = start.first;
    int y = start.second;
    visit[y][x] = true;
    distance[y][x] = 0;
    setGrey(newMaze, start);
    q.enqueue(start);

    while (!q.isEmpty()) {
       pair<int,int> curr = q.dequeue();
       vector<pair<int, int>> n = neighbors(curr);
       for (int i = 0; i < n.size(); i++) {      
          if (good(visit, curr, n[i])) {
            visit[n[i].second][n[i].first] = true;
            distance[n[i].second][n[i].first] = distance[curr.second][curr.first] + 1;
            setGrey(newMaze, n[i]);         
            q.enqueue(n[i]);
          }   
        } 
    }
    
    for (int i = x - 3; i <= x + 3; i++) {
        for (int j = y - 3; j <= y + 3; j++) {
            if (i >= 0 && i < newMaze.width() && j >= 0 && j < newMaze.height()) {
             RGBAPixel * pixel = newMaze.getPixel(i, j);
             pixel->r = 255;
             pixel->g = 0;
             pixel->b = 0;
            }
        }
    }

    return newMaze;    

}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next) {

/* YOUR CODE HERE */
int x = next.first;
int y = next.second;
int a = curr.first;
int b = curr.second;
bool withinImageR = x >= 0 && x < maze.width() && a >= 0 && a < maze.width();  // true
bool withinImageC = y >= 0 && y < maze.height() && b >= 0 && b < maze.height();  //true
if (withinImageR == false || withinImageC == false) {
    return false;
} else { 
    bool ifVisited = v[y][x];  // false
    RGBAPixel *currPixel = maze.getPixel(curr.first, curr.second);
    RGBAPixel *nextPixel = maze.getPixel(next.first, next.second);
    return (*nextPixel == *currPixel) && !ifVisited;   
}
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {

/* YOUR CODE HERE */
    vector<pair<int, int>> n(4);
    n.push_back(pair<int, int>(curr.first - 1, curr.second)); //left
    n.push_back(pair<int, int>(curr.first, curr.second + 1)); //below
    n.push_back(pair<int, int>(curr.first + 1, curr.second)); //right
    n.push_back(pair<int, int>(curr.first, curr.second - 1)); //above
    return n;
}

