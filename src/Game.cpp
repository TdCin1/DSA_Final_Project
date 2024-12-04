#include "Game.h"
#include "TextureManager.h"

using namespace std;


void Game::init(const char *title, int x, int y, int width, int height, bool fullscreen) {

    timeTaken=0;
    //set Graph
    time_test("loadWordGraph", [&](){
        graph = loadWordGraph("../gen/opted.dict");
    });

    //set our class vars h and w
    windowHeight = height;
    windowWidth = width;

    //offset should start at 0
    offsetX =0;
    offsetY=0;

    int flags = 0;
    if(fullscreen){
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if(SDL_Init(SDL_INIT_EVERYTHING)==0){
        //Creates our window
        window = SDL_CreateWindow(title,x,y,width,height,flags);

        if(window){
            cout<<"Window Done"<<endl;
        }

        //Creates our renderer
        renderer = SDL_CreateRenderer(window,-1,0);
        if(renderer){
            SDL_SetRenderDrawColor(renderer, 255,255,255,255);
            cout<<"Renderer Done"<<endl;
        }

        isRunning=true;
    }
    else{
        isRunning=false;
    }

    //Inits our text manager
    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
    }

    //Load textures
    texture_Title = TextureManager::LoadTexture(renderer,"../Textures/TitleScreen.png");
    texture_Circle = TextureManager::LoadTexture(renderer,"../Textures/circle.png");

    //Title position
    destRect_Title.x = 0;  // Set the x position of the texture
    destRect_Title.y = 0;  // Set the y position of the texture
    destRect_Title.w = 1200;  // Width of the texture
    destRect_Title.h = 550;  // Height of the texture

    srcRect_Title.x = 0;
    srcRect_Title.y =0;
    srcRect_Title.w = 1280;
    srcRect_Title.h = 550;


}

void Game::handleEvents() {
    int mouseX, mouseY;
    int mouseAfterX,mouseAfterY;


    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        //Allows us to quit our game
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }

        //Handles all of the clicking
        if(event.type==SDL_MOUSEBUTTONDOWN){
            int clickX,clickY;
            SDL_GetMouseState(&clickX,&clickY);

            //Check to see if any of the boxes are pressed
            //Checks first input box
            if(detectClick(clickX,clickY, inputBoxOne)){
                boxOnePressed = true;
                boxTwoPressed = false;
            }
            //Selects other box
            else if(detectClick(clickX,clickY, inputBoxTwo)){
                boxOnePressed = false;
                boxTwoPressed = true;
            }
            //Deselects boxes
            else{
                boxOnePressed = false;
                boxTwoPressed = false;
            }

            if(detectClick(clickX,clickY,selectBoxOne)){
                selectOne =true;
                selectTwo = false;
            }
            else if(detectClick(clickX,clickY,selectBoxTwo)){
                selectOne = false;
                selectTwo = true;
            }
            if(detectClick(clickX,clickY,findBox)){

                string strOneLower = strBoxOne;
                string stTwoLower = strBoxTwo;

                std::transform(strOneLower.begin(), strOneLower.end(), strOneLower.begin(), [](unsigned char c) {
                    return std::tolower(c);
                });
                std::transform(stTwoLower.begin(), stTwoLower.end(), stTwoLower.begin(), [](unsigned char c) {
                    return std::tolower(c);
                });


                //Set list to what is selected
                vector<GraphEdgeData<string>> bfs_list;
                if(selectOne){
                    timeTaken = time_test("bfs", [&]() {
                        bfs_list  = graph.bfs(strOneLower,stTwoLower);
                    });
                }
                else{
                    bfs_list  = graph.dfs(strOneLower,stTwoLower);
                    timeTaken = time_test("dfs", [&]() {
                        bfs_list  = graph.dfs(strOneLower,stTwoLower);
                    });
                }

                if(bfs_list.empty()){
                    searchFound = false;
                    cout<<"No Connection"<<endl;
                    continue;
                }

                searchFound = true;

                set<pair<string,string>> tempEdgeList;
                for(const auto& i : bfs_list){
                    tempEdgeList.insert(make_pair(graph.getNode(i.from),graph.getNode(i.to)));
                }

                //If edge list is empty set it
                if(edgeList.empty()){
                    setEdgeList(tempEdgeList);
                }
                else{
                    //delete all entities and create new ones
                    for(const auto& i : nodeMap){
                        delete i.second;
                    }
                    nodeMap.clear();
                    setEdgeList(tempEdgeList);
                }

            }

        }
        //Detect any key press
        if (event.type == SDL_KEYDOWN) {
            //if neither box is pressed we can change offset and zoom
            if(!boxOnePressed && !boxTwoPressed){
                switch (event.key.keysym.sym){
                    //moves our offset accordingly
                    case SDLK_w: offsetY += 15; break;
                    case SDLK_s: offsetY -= 15; break;
                    case SDLK_a: offsetX += 15; break;
                    case SDLK_d: offsetX -= 15; break;

                    //Zoom in
                    case SDLK_UP:
                    {
                        zoomLevel += 0.1f;

                        SDL_GetMouseState(&mouseX, &mouseY);
                        //Get it in world cords
                        ScreenToWorld(mouseX,mouseY,mouseAfterX,mouseAfterY);
                        offsetX += mouseX-mouseAfterX;
                        offsetY += mouseY-mouseAfterY;
                        break;

                    }
                    //Zoom out
                    case SDLK_DOWN:
                    {
                        zoomLevel -= 0.1f;

                        if (zoomLevel < 0.1f) zoomLevel = 0.1f;

                        SDL_GetMouseState(&mouseX, &mouseY);
                        //Get it in world cords
                        ScreenToWorld(mouseX,mouseY,mouseAfterX,mouseAfterY);
                        offsetX += mouseX-mouseAfterX;
                        offsetY += mouseY-mouseAfterY;
                        break;
                    }
                }
            }
            //If we have selected a box we must type to it instead of changing offset
            else if(boxOnePressed){
                //Checks to make sure that we don't inset random chars
                if(isalpha(event.key.keysym.sym)){
                    SDL_Keycode key = event.key.keysym.sym;
                    strBoxOne+= SDL_GetKeyName(key);
                }
                //Make sure that we have something to delete
                if(event.key.keysym.sym == SDLK_BACKSPACE && !strBoxOne.empty()){
                    strBoxOne.erase(strBoxOne.size()-1);
                }
            }
            else if(boxTwoPressed){
                //Checks to make sure that we don't inset random chars
                if(isalpha(event.key.keysym.sym)){
                    SDL_Keycode key = event.key.keysym.sym;
                    strBoxTwo+= SDL_GetKeyName(key);
                }
                //Make sure that we have something to delete
                if(event.key.keysym.sym == SDLK_BACKSPACE && !strBoxTwo.empty()){
                    strBoxTwo.erase(strBoxTwo.size()-1);
                }
            }
        }
    }
}

void Game::update() {
    //Update all circle positions
    updateNodePositions();
}

void Game::render() {
    //clears our renderer every call
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);


    //Draw all circles
    for(auto const& i: nodeMap){
        SDL_RenderCopy(renderer, texture_Circle,nullptr,&i.second->positionScreen);
    }

    //Render fonts
    for(const auto& i : nodeMap){
        SDL_Color color = {255,0,0,255};
        const char* tempStr = i.first.c_str();
        renderText(renderer,tempStr,"../fonts/ARIAL.ttf",i.second->positionScreen.x + (10*zoomLevel),i.second->positionScreen.y +(18*zoomLevel),color,24,true);
    }

    //Draw edges in seperate loop so its over the circles
    for(auto const& i : nodeMap){
        for(auto const& j: i.second->outgoing){
            SDL_Point p0 = {static_cast<int>(i.second->positionScreen.x+(60*zoomLevel)), static_cast<int>(i.second->positionScreen.y+(60*zoomLevel))};
            SDL_Point p1;
            SDL_Point p2 = {static_cast<int>(j->positionScreen.x+(60*zoomLevel)), static_cast<int>(j->positionScreen.y+(00*zoomLevel))};

            //to the right
            if(j->positionScreen.x > i.second->positionScreen.x){
                p1 = {(j->positionScreen.x - i.second->positionScreen.x)+i.second->positionScreen.x, i.second->positionScreen.y};
                //Draw arcs above circles
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer,p0.x,p0.y,p2.x,p2.y);
                //Draw arrow
                drawArrow(renderer, p0.x,p0.y,p2.x,p2.y,90,20);            }
            //to the left
            else if(i.second->positionScreen.x > j->positionScreen.x){
                p1 = {(i.second->positionScreen.x - j->positionScreen.x)+j->positionScreen.x, i.second->positionScreen.y};
                //Draw arcs above circles
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                SDL_RenderDrawLine(renderer,p0.x,p0.y,p2.x,p2.y);
                //Draw arrow
                drawArrow(renderer, p0.x,p0.y,p2.x,p2.y,90,20);
            }
            else{
                //up
                if(j->positionScreen.y > i.second->positionScreen.y){
                    p1 = { i.second->positionScreen.x,(j->positionScreen.y - i.second->positionScreen.y)+i.second->positionScreen.y};
                    //Draw arcs above circles
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawLine(renderer,p0.x,p0.y,p2.x,p2.y);
                    //Draw arrow
                    drawArrow(renderer, p0.x,p0.y,p2.x,p2.y,90,20);
                }
                //down
                else{
                    p1 = { i.second->positionScreen.x,(i.second->positionScreen.y - j->positionScreen.y)+j->positionScreen.y};
                    //Draw arcs above circles
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                    SDL_RenderDrawLine(renderer,p0.x,p0.y,p2.x,p2.y);
                    //Draw arrow
                    drawArrow(renderer, p0.x,p0.y,p2.x,p2.y,90,20);

                }
            }

        }
    }

    //Render Title above all else except the input boxes
    SDL_RenderCopy(renderer, texture_Title,&srcRect_Title,&destRect_Title);

    //input boxes
    const char* boxTextOne = strBoxOne.c_str();
    const char* boxTextTwo = strBoxTwo.c_str();
    SDL_Color color = {255,0,0,255};

    //Renders our input boxes
    renderText(renderer,boxTextOne,"../fonts/ARIAL.ttf",inputBoxOne.x+10,inputBoxOne.y+5,color,40,false);
    renderText(renderer,boxTextTwo,"../fonts/ARIAL.ttf",inputBoxTwo.x-28,inputBoxTwo.y+5,color,40,false);

    //Render time to search and success of search
    if(searchFound){
        renderText(renderer,"YES","../fonts/ARIAL.ttf",380,360,color,40,false);
    }
    else{
        renderText(renderer,"NO","../fonts/ARIAL.ttf",380,360,color,40,false);
    }
    string tempStr  =to_string(timeTaken) += "ms";
    const char* timeDisplay = tempStr.c_str();
    renderText(renderer,timeDisplay,"../fonts/ARIAL.ttf",910,350,color,50,false);


    //END
    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    TTF_Quit();
    cout<<"Game cleaned"<<endl;
}

bool Game::running() const {
    return isRunning;
}

void Game::setEdgeList(const set<pair<string,string>> &inp) {
    edgeList = inp;
    createEntities();
}

void Game::WorldToScreen(int xpos, int ypos, int& screenX, int& screenY) {
    //Puts our cords that were in world position into the actual screen position
    screenX = (xpos - offsetX)*zoomLevel;
    screenY = (ypos - offsetY)*zoomLevel;
}

void Game::ScreenToWorld(int xpos, int ypos,int& worldX, int& worldY) {
    //From our screen to what it should be in world cords
    worldX = (xpos /zoomLevel)+offsetX;
    worldY = (ypos /zoomLevel)+offsetY;
}

void Game::createEntities() {
    //Creates from our edge list all the node and its connections
    int numberOfRows = ceil((sqrt(edgeList.size())));
    int nodesPerRow = ceil(edgeList.size()/numberOfRows);
    int currentRow = 0;
    int currentCol=0;

    for(auto const& i : edgeList){
        bool nodeFound =false;
        //checks to see if that name already exists
        for(auto const&j : nodeMap){
            if(j.second->name == i.first){
                nodeFound=true;
                node* tempNode = nodeMap[i.first];

                //check to see if we need to create the outgoing node or just add it
                if(nodeMap.find(i.second)==nodeMap.end()){
                    tempNode->outgoing.push_back(createNode(numberOfRows,nodesPerRow,currentRow,currentCol,make_pair(i.second,"")));
                }
                else{
                    tempNode->outgoing.push_back(nodeMap[i.second]);
                }
            }
        }
        //If the name does not exist we must create a new one for itself and all its neighbors
        if(!nodeFound){
            createNode(numberOfRows,nodesPerRow,currentRow,currentCol,i);
        }
    }
}

void Game::updateNodePositions() {
    //Correctly shifts our positions according to zoom
    for(auto  const& i: nodeMap){
        WorldToScreen(i.second->positionWorld.x,i.second->positionWorld.y,i.second->positionScreen.x,i.second->positionScreen.y);
        i.second->positionScreen.w = i.second->positionWorld.w * zoomLevel;
        i.second->positionScreen.h = i.second->positionWorld.h * zoomLevel;

    }
}


Game::node* Game::createNode(const int& numberOfRows, const int& nodesPerRow, int& currentRow, int& currentCol, const pair<string,string>& i) {
    //Dynamically creates a new node
    node* node = new Game::node;

    //sets the nodes variables
    node->name = i.first;

    node->positionWorld.w = 120;
    node->positionWorld.h = 60;

    //cannot have more than the amount of nodes in the row
    if(currentCol>=nodesPerRow){
        currentRow+=1;
        currentCol=0;
    }

    node->positionWorld.x = 60 + (180*(currentCol+1));
    node->positionWorld.y = 590 + (100*(currentRow+1));
    currentCol+=1;

    nodeMap[i.first] = node;

    //Do the same for its outgoings recursively
    if(i.second != ""){
        bool isFound= false;
        for(auto const& j : nodeMap){
            if(j.second->name == i.second){
                isFound= true;
            }
        }
        if(!isFound){
            node->outgoing.push_back(createNode(numberOfRows,nodesPerRow,currentRow,currentCol,make_pair(i.second,"")));
            nodeMap[node->name] = node;
        }
        else{
            node->outgoing.push_back(nodeMap[i.second]);
        }
    }


    return node;
}

void Game::drawArrow(SDL_Renderer *renderer, int x1, int y1, int x2, int y2, int arrowAngle, int arrowSize) {

    // Calculate the angle of the arrow line
    float angle = atan2(y2 - y1, x2 - x1);

    // Calculate the arrowhead points
    int arrowLength = arrowSize;

    // Points for the arrowhead
    int x3 = static_cast<int>(x2 - arrowLength * cos(angle - M_PI / 6));
    int y3 = static_cast<int>(y2 - arrowLength * sin(angle - M_PI / 6));

    int x4 = static_cast<int>(x2 - arrowLength * cos(angle + M_PI / 6));
    int y4 = static_cast<int>(y2 - arrowLength * sin(angle + M_PI / 6));

    // Draw the two sides of the arrowhead
    SDL_RenderDrawLine(renderer, x2, y2, x3, y3);
    SDL_RenderDrawLine(renderer, x2, y2, x4, y4);
}

void Game::renderText(SDL_Renderer *renderer, const char *text, const char *fontPath, int x, int y,SDL_Color color, int size, bool zoomActive) {
    //Make sure the text isn't empty with null terminator
    if(text[0]=='\0'){
        return;
    }
    // Scale font size based on the zoom level
    int fontSize = size;

    if(zoomActive){
        fontSize*=zoomLevel;
    }

    TTF_Font* font = TTF_OpenFont(fontPath, fontSize);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return;
    }

    // Create surface with text
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text, color);
    if (!textSurface) {
        std::cerr << "Unable to create text surface! TTF_Error: " << TTF_GetError() << std::endl;
        TTF_CloseFont(font);
        return;
    }

    // Create texture from surface
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        std::cerr << "Unable to create texture! SDL_Error: " << SDL_GetError() << std::endl;
    } else {
        // Render the texture to the screen
        SDL_Rect destRect = {x, y, textSurface->w, textSurface->h};
        SDL_RenderCopy(renderer, textTexture, nullptr, &destRect);
    }

    // Clean up
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);

}

bool Game::detectClick(int mouseX, int mouseY, SDL_Rect rect) {
    //Geometry to detect a click
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w) && (mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

