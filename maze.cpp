#include <iostream>
#include <bits/stdc++.h>
#include <stdio.h>
#include <unistd.h>
using namespace std;

#define START 'S'
#define END 'E'
#define BLANK '.'
#define OBSTACLE '#'
#define SELECTED '+'
#define EXPLORED '@'
#define SIZE 20
#define SELECT 0
#define BLOCK 1
#define FREE 2 


class Coordinate {
    public:
    int r, c;
    Coordinate(int x, int y) {
        r=x;
        c=y;
    }
    bool operator == (Coordinate const &obj) {
        if(r==obj.r && c==obj.c) 
            return true;
        return false;
    }
};

void display(vector<vector<char>> const &arr) { 
    int n=arr.size();
    cout<<"Game:"<<endl;
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            cout<<arr[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

void display_path(vector<vector<char>> const &game, vector<vector<bool>> const &visited) {
    int n=game.size();
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            if(visited[i][j]==true && game[i][j]==BLANK) {
                cout<<EXPLORED<<" ";
            }
            else {
                cout<<game[i][j]<<" ";
            }
        }
        cout<<endl;
    }
}

void editInstructions() {
    cout<<"Key press Instructions: "<<endl;
    cout<<"\t1. "<<SELECTED<<": selected cell in maze."<<endl;
    cout<<"\t2. Navigate the cells by w,s,a,d controls. "<<endl;
    cout<<"\t3. O: toggle mode (block/select)"<<endl;
    cout<<"\t4. P: toggle mode (free/select)"<<endl;
    cout<<"\t5. K: place start point "<<endl; 
    cout<<"\t6. L: place end point "<<endl;
    cout<<"\t7. X: Get a random maze design."<<endl;
    cout<<"\t8. 0: Exit 'Edit mode'"<<endl;
}

void clear(vector<vector<bool>> &arr) {
    int n=arr.size();
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            arr[i][j]=false;
        }
    }
}

void clear(vector<vector<int>> &arr) {
    int n=arr.size();
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            arr[i][j]=5;
        }
    }
}

int trace_path(vector<vector<char>> const &arr, int r, int c, vector<vector<bool>> &visited, vector<vector<int>> &path) {
    int n=arr.size();
    clear(visited);
    int cost=0;
    while(arr[r][c]!=START) {
        system("clear");
        cout<<"Re-tracing path ... "<<endl;
        display_path(arr, visited);
        usleep(200000);
        visited[r][c]=true;
        switch(path[r][c]) 
        {
            case 5: 
                return -1;
                break;
            case 1:
                r++;
                c--;
                break;
            case 2:
                r++;
                break;
            case 3:
                r++;
                c++;
                break;
            case 6:
                c++;
                break;
            case 9:
                r--;
                c++;
                break;
            case 8:
                r--;
                break;
            case 7:
                r--;
                c--;
                break;
            case 4:
                c--;
                break;
        }
        cost++;
    }
    
    return cost;
}

int dfs(vector<vector<char>> const &arr, int r, int c, vector<vector<bool>> &visited, bool &found, vector<vector<int>> &path) {
    if(arr[r][c]==END) {
        found=true;
        return 0;
    }
    if(arr[r][c]==OBSTACLE) 
        return 0;
    int cost=0;
    visited[r][c]=true;
    int n=arr.size();

    // Display progress 
        system("clear");
        cout<<"Searching for a path ... "<<endl;
        display_path(arr, visited);
        usleep(50000);
    
    // north 
        if(r-1>=0 && visited[r-1][c]==false && found==false) { // && arr[r-1][c]==BLANK) {
            cost+=dfs(arr, r-1, c, visited, found, path);
            visited[r-1][c]=true;
            path[r-1][c]=2;
        }
    // north-east
        if(r-1>=0 && c+1<n && visited[r-1][c+1]==false && found==false) {
            cost+=dfs(arr, r-1, c+1, visited, found, path);
            visited[r-1][c+1]=true;
            path[r-1][c+1]=1;
        }
    // east
        if(c+1<n && visited[r][c+1]==false && found==false) { // && arr[r][c+1]==BLANK) {
            cost+=dfs(arr, r, c+1, visited, found, path);
            visited[r][c+1]=true;
            path[r][c+1]=4;
        }
    // south-east
        if(r+1<n && c+1<n && visited[r+1][c+1]==false && found==false) {
            cost+=dfs(arr, r+1, c+1, visited, found, path);
            visited[r+1][c+1]=true;
            path[r+1][c+1]=7;
        }
    // south 
        if(r+1<n && visited[r+1][c]==false && found==false) { // && arr[r+1][c]==BLANK) {
            cost+=dfs(arr, r+1, c, visited, found, path);
            visited[r+1][c]=true;
            path[r+1][c]=8;
        }
    // south-west
        if(r+1<n && c-1>=0 && visited[r+1][c-1]==false && found==false) {
            cost+=dfs(arr, r+1, c-1, visited, found, path);
            visited[r+1][c-1]=true;
            path[r+1][c-1]=9;
        }
    // west 
        if(c-1>=0 && visited[r][c-1]==false && found==false) { // && arr[r][c-1]==BLANK) {
            cost+=dfs(arr, r, c-1, visited, found, path);
            visited[r][c-1]=true;
            path[r][c-1]=6;
        }
    // north-west
        if(r-1>=0 && c-1>=0 && visited[r-1][c-1]==false && found==false) {
            cost+=dfs(arr, r-1, c-1, visited, found, path);
            visited[r-1][c-1]=true;
            path[r-1][c-1]=3;
        }
    return cost+1;
}

int bfs(vector<vector<char>> const &arr, Coordinate start, vector<vector<int>> &path) {
    int n=arr.size();
    int cost=0;
    vector<vector<bool>> visited(n);
    vector<vector<int>> total_cost(n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            visited[i].push_back(false);
            total_cost[i].push_back(INT32_MAX);
        }
    }
    queue<pair<int, int>> next;
    next.push({start.r, start.c});
    total_cost[start.r][start.c]=0;

    while(next.empty()==false) {
        int r=next.front().first, c=next.front().second;
        next.pop();

        // Display progress 
            system("clear");
            cout<<"Searching for a path ... "<<endl;
            display_path(arr, visited);
            usleep(50000);

        visited[r][c]=true;
        if(arr[r][c]==END) 
            return cost;

        // north
        if(r-1>=0 && arr[r-1][c]!=OBSTACLE && total_cost[r-1][c]>total_cost[r][c]+10) {
            next.push({r-1, c});
            path[r-1][c]=2;
            total_cost[r-1][c]=total_cost[r][c]+10;
        }
        // east
        if(c+1<n && arr[r][c+1]!=OBSTACLE && total_cost[r][c+1]>total_cost[r][c]+10) {
            next.push({r, c+1});
            path[r][c+1]=4;
            total_cost[r][c+1]=total_cost[r][c]+10;
        }
        // south
        if(r+1<n && arr[r+1][c]!=OBSTACLE && total_cost[r+1][c]>total_cost[r][c]+10) {
            next.push({r+1,c});
            path[r+1][c]=8;
            total_cost[r+1][c]=total_cost[r][c]+10;
        }
        
        // west
        if(c-1>=0 && arr[r][c-1]!=OBSTACLE && total_cost[r][c-1]>total_cost[r][c]+10) {
            next.push({r, c-1});
            path[r][c-1]=6;
            total_cost[r][c-1]=total_cost[r][c]+10;
        }
        // north-east
        if(r-1>=0 && c+1<n && arr[r-1][c+1]!=OBSTACLE && total_cost[r-1][c+1]>total_cost[r][c]+14) {
            next.push({r-1, c+1});
            total_cost[r-1][c+1]=total_cost[r][c]+14;
            path[r-1][c+1]=1;
        }        
        // south-east
        if(r+1<n && c+1<n && arr[r+1][c+1]!=OBSTACLE && total_cost[r+1][c+1]>total_cost[r][c]+14) {
            next.push({r+1, c+1});
            total_cost[r+1][c+1]=total_cost[r][c]+14;
            path[r+1][c+1]=7;
        }
        // south-west
        if(r+1<n && c-1>=0 && arr[r+1][c-1]!=OBSTACLE && total_cost[r+1][c-1]>total_cost[r][c]+14) {
            next.push({r+1, c-1});
            total_cost[r+1][c-1]=total_cost[r][c]+14;
            path[r+1][c-1]=9;
        }
        // north-west
        if(r-1>=0 && c-1>=0 && arr[r-1][c-1]!=OBSTACLE && total_cost[r-1][c-1]>total_cost[r][c]+14) {
            next.push({r-1, c-1});
            total_cost[r-1][c-1]=total_cost[r][c]+14;
            path[r-1][c-1]=3;
        }
        cost++;
    }
    return -1;
}

int A_star(vector<vector<char>> const &arr, Coordinate start, Coordinate end, vector<vector<int>> &path) {
    int n=arr.size();
    vector<vector<int>> value(n);
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            int x1 = abs(start.c-j);
            int y1 = abs(start.r-i);
            int g = min(x1,y1)*14+abs(x1-y1)*10;
            
            int x2 = abs(end.c-j);
            int y2 = abs(end.r-i);
            int h = min(x2,y2)*14+abs(x2-y2)*10;
            
            value[i].push_back((g+h)*(-1));
        }
    }
    int cost=0;
    vector<vector<bool>>  visited(n);
    vector<vector<int>>  total_cost(n);
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            visited[i].push_back(false);
            total_cost[i].push_back(INT32_MAX);
        }
    }

    priority_queue<pair<int, pair<int, int>>> next;
    next.push({value[start.r][start.c], {start.r, start.c}});
    total_cost[start.r][start.c]=0;

    while(next.empty()==false) {
        int r=next.top().second.first, c=next.top().second.second;
        int v=next.top().first;
        next.pop();

        
        // Display progress 
            system("clear");
            cout<<"Searching for a path ... "<<endl;
            display_path(arr, visited);
            usleep(50000);
        
        visited[r][c]=true;
        if(arr[r][c]==END) 
            return cost;

        if(r+1<n && arr[r+1][c]!=OBSTACLE && total_cost[r+1][c]>total_cost[r][c]+10) {
            next.push({ value[r+1][c], {r+1, c} });
            total_cost[r+1][c]=total_cost[r][c]+10;
            path[r+1][c]=8;
        }
        if(c+1<n && arr[r][c+1]!=OBSTACLE && total_cost[r][c+1]>total_cost[r][c]+10) {
            next.push({ value[r][c+1], {r, c+1} });
            total_cost[r][c+1]=total_cost[r][c]+10;
            path[r][c+1]=4;
        }
        if(r-1>=0 && arr[r-1][c]!=OBSTACLE && total_cost[r-1][c]>total_cost[r][c]+10) {
            next.push({ value[r-1][c], {r-1, c} });
            total_cost[r-1][c]=total_cost[r][c]+10;
            path[r-1][c]=2;
        }
        if(c-1>=0 && arr[r][c-1]!=OBSTACLE && total_cost[r][c-1]>total_cost[r][c]+10) {
            next.push({ value[r][c-1], {r, c-1} });
            total_cost[r][c-1]=total_cost[r][c]+10;
            path[r][c-1]=6;
        }
        if(r-1>=0 && c-1>=0 && arr[r-1][c-1]!=OBSTACLE && total_cost[r-1][c-1]>total_cost[r][c]+14) {
            next.push({ value[r-1][c-1], {r-1, c-1} });
            total_cost[r-1][c-1]=total_cost[r][c]+14;
            path[r-1][c-1]=3;
        }
        if(r-1>=0 && c+1<n && arr[r-1][c+1]!=OBSTACLE && total_cost[r-1][c+1]>total_cost[r][c]+14) {
            next.push({ value[r-1][c+1], {r-1, c+1} });
            total_cost[r-1][c+1]=total_cost[r][c]+14;
            path[r-1][c+1]=1;
        }
        if(r+1<n && c+1<n && arr[r+1][c+1]!=OBSTACLE && total_cost[r+1][c+1]>total_cost[r][c]+14) {
            next.push({ value[r+1][c+1], {r+1, c+1} });
            total_cost[r+1][c+1]=total_cost[r][c]+14;
            path[r+1][c+1]=7;
        }
        if(r+1<n && c-1>=0 && arr[r+1][c-1]!=OBSTACLE && total_cost[r+1][c-1]>total_cost[r][c]+14) {
            next.push({ value[r+1][c-1], {r+1, c-1} });
            total_cost[r+1][c-1]=total_cost[r][c]+14;
            path[r+1][c-1]=9;
        }
        cost++;
    }
    return -1;
}


int main()
{
    // Create board and Initialize
        int n=SIZE;
        vector<vector<char>> game(n);
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                game[i].push_back(BLANK);
            }
        }
            

    // Game variables
        Coordinate src(0,0), dst(n-1,n-1);
        game[src.r][src.c]=START;
        game[dst.r][dst.c]=END;
        int mode=SELECT;
        string  mode_name[3]={ "SELECT", "BLOCK", "FREE"};
        bool found=false;
        vector<vector<bool>> visited(n);
        vector<vector<int>> path(n);
        int difficulty=5;
        string status="NULL - [Chose an algorithm]";
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                visited[i].push_back(false);
                path[i].push_back(5);
            }
        }
        
    int choice, search_cost, path_cost;
    char inp;
    bool editMode=false, pathFindingMode=true;
    Coordinate curser(0,0);
    char curr;
    while(1) {
        display(game);
        cout<<"   <--Menu Options-->  "<<endl;
        cout<<"1. Edit Game"<<endl;
        cout<<"2. Find shortest path"<<endl;
        cout<<"0. Exit"<<endl;
        cout<<"Enter your choice: ";
        cin>>choice;
        switch(choice) 
        {
            case 0:
                cout<<"Exiting game, Thank You."<<endl;
                exit(0);
                break;
            case 1:
                editMode=true;
                curr=game[curser.r][curser.c];
                game[curser.r][curser.c]=SELECTED;
                while(editMode==true) {
                    system("clear");
                    cout<<"\t\t***Edit mode***"<<endl;
                    display(game);
                    cout<<"mode: "<<mode_name[mode]<<endl;
                    editInstructions();
	                system("stty raw");
                    inp=getchar();
	                system("stty cooked");
                    switch(inp) 
                    {
                        case '0':
                            editMode=false;
                            game[curser.r][curser.c]=curr;
                            break;
                        case 'w':
                        case 'W':
                            if(curser.r!=0) {
                                if(mode==SELECT || curser==src || curser==dst) 
                                    game[curser.r][curser.c]=curr;
                                else if(mode==BLOCK) 
                                    game[curser.r][curser.c]=OBSTACLE;
                                else if(mode==FREE)
                                    game[curser.r][curser.c]=BLANK;
                                curser.r--;
                                curr=game[curser.r][curser.c];
                                game[curser.r][curser.c]=SELECTED;
                            }
                            break;
                        case 's':
                        case 'S':
                            if(curser.r!=n-1) {
                                if(mode==SELECT || curser==src || curser==dst) 
                                    game[curser.r][curser.c]=curr;
                                else if(mode==BLOCK) 
                                    game[curser.r][curser.c]=OBSTACLE;
                                else if(mode==FREE)
                                    game[curser.r][curser.c]=BLANK;
                                curser.r++;
                                curr=game[curser.r][curser.c];
                                game[curser.r][curser.c]=SELECTED;
                            }
                            break;
                        case 'a':
                        case 'A':
                            if(curser.c!=0) {
                                if(mode==SELECT || curser==src || curser==dst) 
                                    game[curser.r][curser.c]=curr;
                                else if(mode==BLOCK) 
                                    game[curser.r][curser.c]=OBSTACLE;
                                else if(mode==FREE)
                                    game[curser.r][curser.c]=BLANK;
                                curser.c--;
                                curr=game[curser.r][curser.c];
                                game[curser.r][curser.c]=SELECTED;
                            }
                            break;
                        case 'd':
                        case 'D':
                            if(curser.c!=n-1) {
                                if(mode==SELECT || curser==src || curser==dst) 
                                    game[curser.r][curser.c]=curr;
                                else if(mode==BLOCK) 
                                    game[curser.r][curser.c]=OBSTACLE;
                                else if(mode==FREE)
                                    game[curser.r][curser.c]=BLANK;
                                curser.c++;
                                curr=game[curser.r][curser.c];
                                game[curser.r][curser.c]=SELECTED;
                            }
                            break;
                        case 'o':
                        case 'O':
                            if(mode==BLOCK)
                                mode=SELECT;
                            else
                                mode=BLOCK;
                            break;
                        case 'p':
                        case 'P':
                            if(mode==FREE)
                                mode=SELECT;
                            else
                                mode=FREE;
                            break;
                        case 'k':
                        case 'K':
                            game[src.r][src.c]=BLANK;
                            src=curser;
                            curr=START;
                            break;
                        case 'l':
                        case 'L':
                            game[dst.r][dst.c]=BLANK;
                            dst=curser;
                            curr=END;
                            break;
                        case 'x':
                        case 'X':
                            cout<<"Enter difficulty level of maze in 1(most difficult) to 5(least difficult): ";
                            inp=getchar();
                            difficulty=inp-'1'+1;
                            if(difficulty<1 || difficulty>5) {
                                continue;
                            }
                            difficulty=difficulty+1;
                            for(int i=0; i<n; i++) {
                                for(int j=0; j<n; j++) {
                                    if(game[i][j]!=START && game[i][j]!=END) {
                                        if(rand()%difficulty==0) {
                                            game[i][j]=OBSTACLE;
                                        }
                                        else {
                                            game[i][j]=BLANK;
                                        }
                                    }
                                }
                            }
                            if(curser==src || curser==dst) {
                            	game[curser.r][curser.c]=curr;
                            }

                        default:
                            cout<<"Please Enter a valid key!"<<endl;
                            break;
                    }
                }
                break;
            case 2:
                    search_cost=-1;
                    path_cost=-1;
                    pathFindingMode=true;
                    while(pathFindingMode==true) {
                        system("clear");
                        display_path(game, visited);
                        cout<<"Status: "<<status<<endl;
                        cout<<"search_cost: "<<search_cost<<"\tpath_cost: "<<path_cost<<endl<<endl;
                        cout<<"Algorithms: "<<endl;
                        cout<<"1. DFS"<<endl;
                        cout<<"2. BFS"<<endl;
                        cout<<"3. A star"<<endl;
                        cout<<"4. Clear"<<endl;
                        cout<<"0. Exit"<<endl;
                        cout<<"Enter the algorithm number to find path: ";
                        cin>>choice;
                        switch(choice) 
                        {
                            case 0:
                                pathFindingMode=false;
                                break;
                            case 1:
                                clear(visited);
                                clear(path);
                                found=false;
                                search_cost=dfs(game, src.r, src.c, visited, found, path);
                                path_cost=trace_path(game, dst.r, dst.c, visited, path);
                                break;
                            case 2:
                                clear(visited);
                                clear(path);
                                search_cost=bfs(game, src, path);
                                path_cost=trace_path(game, dst.r, dst.c, visited, path);
                                break;
                            case 3:
                                clear(visited);
                                clear(path);
                                search_cost=A_star(game, src, dst, path);
                                path_cost=trace_path(game, dst.r, dst.c, visited, path);
                                break;
                            case 4:
                                clear(visited);
                                clear(path);
                            default:
                                break;
                        }
                        if(path_cost==-1) {
                            status="Path not Found!";
                        }
                        else {
                            status="path Found!";
                        }
                    }
                break;
            default:
                cout<<"Enter a valid option."<<endl;
                break;
        }
        system("clear");
    }
    return 0;
}















