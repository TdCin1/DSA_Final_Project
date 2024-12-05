#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <iostream>
#include <vector>
#include <cctype>
#include <cmath>
#include <set>

#include "Searching.h"

using namespace std;

class Game {
public:
    struct node{
            string name;
            vector<node*> outgoing;
            SDL_Rect positionWorld;
            SDL_Rect positionScreen;
    };

    Game(){renderer=nullptr;}
    ~Game(){for(auto i:nodeMap){
        delete i.second;
    }}


    void init(const char* title, int x, int y, int width, int height, bool fullscreen);

    void handleEvents();
    void update();
    void render();
    void clean();

    bool running() const;
    void setEdgeList(const set<pair<string,string>>& inp);


private:
    SDL_Window* window;

    set<pair<string,string>> edgeList;
    int windowWidth;
    int windowHeight;

    void WorldToScreen(int xpos, int ypos, int& screenX, int& screenY);
    void ScreenToWorld(int xpos, int ypos,int& worldX, int& worldY);

    void createEntities();
    void updateNodePositions();

    node* createNode(const int& numberOfRows, const int& nodesPerRow, int& currentRow, int& currentCol, const pair<string,string>& i);

    void renderText(SDL_Renderer* renderer, const char* text, const char* fontPath, int x, int y, SDL_Color color, int size,bool zoomActive);

    bool detectClick(int mouseX, int mouseY, SDL_Rect rect);


    SDL_Renderer* renderer;
    static SDL_Event event;
    bool isRunning;
    float zoomLevel=1.0f;
    int offsetX;
    int offsetY;


    //Textures
    SDL_Texture* texture_Title;
    SDL_Texture* texture_Circle;

    SDL_Rect destRect_Title;
    SDL_Rect srcRect_Title;

    SDL_Rect inputBoxOne = {37,209,463,82};
    SDL_Rect inputBoxTwo = {750,210,493,82};
    SDL_Rect selectBoxOne = {320,458,243,82};
    SDL_Rect selectBoxTwo = {564,458,251,82};
    SDL_Rect findBox = {961,458,146,82};

    bool boxOnePressed = false;
    bool boxTwoPressed = false;
    bool selectOne = true;
    bool selectTwo = false;

    string strBoxOne="";
    string strBoxTwo="";

    unordered_map<string,node*> nodeMap;

    void drawArrow(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int arrowAngle,int arrowSize);


    unordered_map<string,SDL_Texture*> textureMap;

    Graph<std::string, std::string> graph;

    int timeTaken;
    bool searchFound=false;
};
