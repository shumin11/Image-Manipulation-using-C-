#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {

/* YOUR CODE HERE */
    parent.resize(mapImg.height());
    for (int i = 0; i < mapImg.height(); i++) {
        parent[i].resize(mapImg.width());
    }

    Stack<pair<int, int>> stack;  

    pair<int, int> end = findSpot();
    pair<int,int> curr;
    curr = end;
    stack.push(end);
    while(curr != start) {
        stack.push(parent[curr.second][curr.first]);
        curr = parent[curr.second][curr.first];
    }
    
    while (!stack.isEmpty()) {
        pathPts.push_back(stack.pop());
    }
  
}


PNG decoder::renderSolution(){

/* YOUR CODE HERE */ 
       PNG newImage = mapImg;
       for (int i = 0; i < pathPts.size(); i++) {
         int x = pathPts[i].first;
         int y = pathPts[i].second;
         RGBAPixel * pixel = newImage.getPixel(x, y);
         pixel->r = 255;
         pixel->g = 0;
         pixel->b = 0;
       }
       return newImage;
}

PNG decoder::renderMaze(){

/* YOUR CODE HERE */
PNG newMaze = mapImg;

    vector<vector<bool>> visit(mapImg.height(), vector<bool>(mapImg.width()));
    for (int i = 0; i< mapImg.width(); i++) {
        for (int j; j < mapImg.height(); j++) {
            visit[j][i] = false;
        }
    }
    vector<vector<int>> distance(mapImg.height(), vector<int>(mapImg.width()));
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
          if (good(visit, distance, curr, n[i])) {
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

void decoder::setGrey(PNG & im, pair<int,int> loc){

/* YOUR CODE HERE */
   RGBAPixel *locPixel = im.getPixel(loc.first, loc.second);
   locPixel->r = 2* (locPixel->r / 4);
   locPixel->g = 2* (locPixel->g / 4);
   locPixel->b = 2* (locPixel->b / 4);

}

pair<int,int> decoder::findSpot(){

/* YOUR CODE HERE */
  vector<vector<bool>> visit(mapImg.height(), vector<bool>(mapImg.width()));
    for (int i = 0; i< mapImg.width(); i++) {
        for (int j; j < mapImg.height(); j++) {
            visit[j][i] = false;
        }
    }
    vector<vector<int>> distance(mapImg.height(), vector<int>(mapImg.width()));
    Queue<pair<int, int>> q;

    int x = start.first;
    int y = start.second;
    visit[y][x] = true;
    distance[y][x] = 0;
    q.enqueue(start);
    pair<int,int> curr;
    
    while (!q.isEmpty()) {
        curr = q.dequeue();
        vector<pair<int, int>> n = neighbors(curr);
        
        for (int i = 0; i < n.size(); i++) { 
          if (good(visit, distance, curr, n[i])) {
            parent[n[i].second][n[i].first] = curr;
            visit[n[i].second][n[i].first] = true;
            distance[n[i].second][n[i].first] = distance[curr.second][curr.first] + 1;           
            q.enqueue(n[i]);          
         }   
        }        
    }
    return curr;
}

int decoder::pathLength(){

/* YOUR CODE HERE */
    return pathPts.size();

}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){

/* YOUR CODE HERE */
   int x = next.first;
   int y = next.second;
   int a = curr.first;
   int b = curr.second;
   bool withinImageR = x >= 0 && x < mapImg.width() && a >= 0 && a < mapImg.width();  // true
   bool withinImageC = y >= 0 && y < mapImg.height() && b >= 0 && b < mapImg.height();  //true
if (withinImageR == false || withinImageC == false) {
    return false;
} else { 
   bool ifVisited = v[y][x];  // false
   int dcurr = d[curr.second][curr.first];
   RGBAPixel *nextPixel = mapImg.getPixel(next.first, next.second);
   return !ifVisited && compare(*nextPixel, dcurr);   
}

}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {

/* YOUR CODE HERE */
    vector<pair<int, int>> n(4);
    n.push_back(pair<int, int>(curr.first - 1, curr.second)); //left
    n.push_back(pair<int, int>(curr.first, curr.second + 1)); //below
    n.push_back(pair<int, int>(curr.first + 1, curr.second)); //right
    n.push_back(pair<int, int>(curr.first, curr.second - 1)); //above
    return n;
}


bool decoder::compare(RGBAPixel p, int d){

/* YOUR CODE HERE */
    int value = (d + 1) % 64;
    int pvalue = (p.r % 4) * 16 + (p.g % 4) * 4 + (p.b % 4);
    return pvalue == value;

}
