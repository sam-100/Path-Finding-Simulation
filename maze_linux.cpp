#include <iostream>
#include <vector>
#include <queue>
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
#define MODE_SELECT 0
#define MODE_BLOCK 1
#define MODE_FREE 2 


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
void display(vector<vector<int>> const &arr) { 
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

void clear(vector<vector<bool>> &arr, bool val=false) {
    int n=arr.size();
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            arr[i][j]=val;
        }
    }
}

void clear(vector<vector<int>> &arr, int val) {
    int n=arr.size();
    for(int i=0; i<n; i++) {
        for(int j=0; j<n; j++) {
            arr[i][j]=val;
        }
    }
}

int trace_path(vector<vector<char>> const &arr, Coordinate end, vector<vector<bool>> &visited, vector<vector<int>> &path) {
    int n=arr.size();
    int r=end.r, c=end.c;
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

int depthFirstSearch(vector<vector<char>> const &arr, Coordinate curr, vector<vector<bool>> &visited, vector<vector<int>> &total_cost, bool &found, vector<vector<int>> &path) {
    int r=curr.r, c=curr.c;
    if(arr[r][c]==END) {
        found=true;
        return 0;
    }
    if(arr[r][c]==OBSTACLE) 
        return 0;
    
    int cost=0;
    int n=arr.size();
    visited[r][c]=true;

    // Display progress 
        system("clear");
        cout<<"Searching for a path ... "<<endl;
        display_path(arr, visited);
        usleep(50000);
    
    // Updating all neighbours
        // north 
            if(r-1>=0 && found==false && total_cost[r-1][c]>total_cost[r][c]+10) { 
                path[r-1][c]=2;
                total_cost[r-1][c]=total_cost[r][c]+10;
            }
        // north-east
            if(r-1>=0 && c+1<n && found==false && total_cost[r-1][c+1]>total_cost[r][c]+14) {
                path[r-1][c+1]=1;
                total_cost[r-1][c+1]=total_cost[r][c]+14;
            }
        // east
            if(c+1<n && found==false && total_cost[r][c+1]>total_cost[r][c]+10) { 
                path[r][c+1]=4;
                total_cost[r][c+1]=total_cost[r][c]+10;
            }
        // south-east
            if(r+1<n && c+1<n && found==false && total_cost[r+1][c+1]>total_cost[r][c]+14) {
                path[r+1][c+1]=7;
                total_cost[r+1][c+1]=total_cost[r][c]+14;
            }
        // south 
            if(r+1<n && found==false && total_cost[r+1][c]>total_cost[r][c]+10) { 
                path[r+1][c]=8;
                total_cost[r+1][c]=total_cost[r][c]+10;
            }
        // south-west
            if(r+1<n && c-1>=0 && found==false && total_cost[r+1][c-1]>total_cost[r][c]+14) {
                path[r+1][c-1]=9;
                total_cost[r+1][c-1]=total_cost[r][c]+14;
            }
        // west 
            if(c-1>=0 && found==false && total_cost[r][c-1]>total_cost[r][c]+10) { 
                path[r][c-1]=6;
                total_cost[r][c-1]=total_cost[r][c]+10;
            }
        // north-west
            if(r-1>=0 && c-1>=0 && found==false && total_cost[r-1][c-1]>total_cost[r][c]+14) {
                path[r-1][c-1]=3;
                total_cost[r-1][c-1]=total_cost[r][c]+14;
            }

    // Recursively exploring all the paths 
        // north 
            if(r-1>=0 && found==false && visited[r-1][c]==false) {
                cost+=depthFirstSearch(arr, Coordinate(r-1, c), visited, total_cost, found, path);
            }
        // north-east
            if(r-1>=0 && c+1<n && found==false && visited[r-1][c+1]==false) {
                cost+=depthFirstSearch(arr, Coordinate(r-1, c+1), visited, total_cost, found, path);
            }
        // east
            if(c+1<n && found==false && visited[r][c+1]==false) {
                cost+=depthFirstSearch(arr, Coordinate(r, c+1), visited, total_cost, found, path);
            }
        // south-east
            if(r+1<n && c+1<n && found==false && visited[r+1][c+1]==false) {
                cost+=depthFirstSearch(arr, Coordinate(r+1, c+1), visited, total_cost, found, path);
            }
        // south 
            if(r+1<n && found==false && visited[r+1][c]==false) {
                cost+=depthFirstSearch(arr, Coordinate(r+1, c), visited, total_cost, found, path);
            }
        // south-west
            if(r+1<n && c-1>=0 && found==false && visited[r+1][c-1]==false) {
                cost+=depthFirstSearch(arr, Coordinate(r+1, c-1), visited, total_cost, found, path);
            }
        // west 
            if(c-1>=0 && found==false && visited[r][c-1]==false) {
                cost+=depthFirstSearch(arr, Coordinate(r, c-1), visited, total_cost, found, path);
            }
        // north-west
            if(r-1>=0 && c-1>=0 && found==false && visited[r-1][c-1]==false) {
                cost+=depthFirstSearch(arr, Coordinate(r-1, c-1), visited, total_cost, found, path);
            }
    return cost+1;
}

void update_neighbours(vector<vector<char>> const &arr, int const r, int const c, vector<vector<int>> &path, queue<pair<int, int>> &next, vector<vector<int>> &total_cost, bool reverse_path) {
    int n=arr.size();
    // north
    if(r-1>=0 && arr[r-1][c]!=OBSTACLE && total_cost[r-1][c]>total_cost[r][c]+10) {
        next.push({r-1, c});
        total_cost[r-1][c]=total_cost[r][c]+10;
        if(reverse_path==false) 
            path[r-1][c]=2;
        else 
            path[r-1][c]=8;
    }
    // east
    if(c+1<n && arr[r][c+1]!=OBSTACLE && total_cost[r][c+1]>total_cost[r][c]+10) {
        next.push({r, c+1});
        total_cost[r][c+1]=total_cost[r][c]+10;
        if(reverse_path==false) 
            path[r][c+1]=4;
        else 
            path[r][c+1]=6;
    }
    // south
    if(r+1<n && arr[r+1][c]!=OBSTACLE && total_cost[r+1][c]>total_cost[r][c]+10) {
        next.push({r+1,c});
        total_cost[r+1][c]=total_cost[r][c]+10;
        if(reverse_path==false) 
            path[r+1][c]=8;
        else 
            path[r+1][c]=2;
    }
    
    // west
    if(c-1>=0 && arr[r][c-1]!=OBSTACLE && total_cost[r][c-1]>total_cost[r][c]+10) {
        next.push({r, c-1});
        total_cost[r][c-1]=total_cost[r][c]+10;
        if(reverse_path==false) 
            path[r][c-1]=6;
        else 
            path[r][c-1]=4;
    }
    // north-east
    if(r-1>=0 && c+1<n && arr[r-1][c+1]!=OBSTACLE && total_cost[r-1][c+1]>total_cost[r][c]+14) {
        next.push({r-1, c+1});
        total_cost[r-1][c+1]=total_cost[r][c]+14;
        if(reverse_path==false)
            path[r-1][c+1]=1;
        else 
            path[r-1][c+1]=9;
    }        
    // south-east
    if(r+1<n && c+1<n && arr[r+1][c+1]!=OBSTACLE && total_cost[r+1][c+1]>total_cost[r][c]+14) {
        next.push({r+1, c+1});
        total_cost[r+1][c+1]=total_cost[r][c]+14;
        if(reverse_path==false)
            path[r+1][c+1]=7;
        else 
            path[r+1][c+1]=3;
    }
    // south-west
    if(r+1<n && c-1>=0 && arr[r+1][c-1]!=OBSTACLE && total_cost[r+1][c-1]>total_cost[r][c]+14) {
        next.push({r+1, c-1});
        total_cost[r+1][c-1]=total_cost[r][c]+14;
        if(reverse_path==false)
            path[r+1][c-1]=9;
        else 
            path[r+1][c-1]=1;
    }
    // north-west
    if(r-1>=0 && c-1>=0 && arr[r-1][c-1]!=OBSTACLE && total_cost[r-1][c-1]>total_cost[r][c]+14) {
        next.push({r-1, c-1});
        total_cost[r-1][c-1]=total_cost[r][c]+14;
        if(reverse_path==false)
            path[r-1][c-1]=3;
        else 
            path[r-1][c-1]=7;
    }
}

int breadthFirstSearch(vector<vector<char>> const &arr, Coordinate start, vector<vector<int>> &path) {
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
        if(arr[r][c]==END) {
            return cost;
        }
        update_neighbours(arr, r, c, path, next, total_cost, false);
        cost++;
    }
    return -1;
}

int A_starSearch(vector<vector<char>> const &arr, Coordinate start, Coordinate end, vector<vector<int>> &path) {
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
        int mode=MODE_SELECT;
        string  mode_name[3]={ "SELECT", "BLOCK", "FREE"};
        bool found=false;
        vector<vector<bool>> visited(n);
        vector<vector<int>> path(n);
        vector<vector<int>> total_cost(n);
        int difficulty=5;
        string status="NULL - [Chose an algorithm]";
        for(int i=0; i<n; i++) {
            for(int j=0; j<n; j++) {
                visited[i].push_back(false);
                path[i].push_back(5);
                total_cost[i].push_back(INT32_MAX);
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
                                if(mode==MODE_SELECT || curser==src || curser==dst) 
                                    game[curser.r][curser.c]=curr;
                                else if(mode==MODE_BLOCK) 
                                    game[curser.r][curser.c]=OBSTACLE;
                                else if(mode==MODE_FREE)
                                    game[curser.r][curser.c]=BLANK;
                                curser.r--;
                                curr=game[curser.r][curser.c];
                                game[curser.r][curser.c]=SELECTED;
                            }
                            break;
                        case 's':
                        case 'S':
                            if(curser.r!=n-1) {
                                if(mode==MODE_SELECT || curser==src || curser==dst) 
                                    game[curser.r][curser.c]=curr;
                                else if(mode==MODE_BLOCK) 
                                    game[curser.r][curser.c]=OBSTACLE;
                                else if(mode==MODE_FREE)
                                    game[curser.r][curser.c]=BLANK;
                                curser.r++;
                                curr=game[curser.r][curser.c];
                                game[curser.r][curser.c]=SELECTED;
                            }
                            break;
                        case 'a':
                        case 'A':
                            if(curser.c!=0) {
                                if(mode==MODE_SELECT || curser==src || curser==dst) 
                                    game[curser.r][curser.c]=curr;
                                else if(mode==MODE_BLOCK) 
                                    game[curser.r][curser.c]=OBSTACLE;
                                else if(mode==MODE_FREE)
                                    game[curser.r][curser.c]=BLANK;
                                curser.c--;
                                curr=game[curser.r][curser.c];
                                game[curser.r][curser.c]=SELECTED;
                            }
                            break;
                        case 'd':
                        case 'D':
                            if(curser.c!=n-1) {
                                if(mode==MODE_SELECT || curser==src || curser==dst) 
                                    game[curser.r][curser.c]=curr;
                                else if(mode==MODE_BLOCK) 
                                    game[curser.r][curser.c]=OBSTACLE;
                                else if(mode==MODE_FREE)
                                    game[curser.r][curser.c]=BLANK;
                                curser.c++;
                                curr=game[curser.r][curser.c];
                                game[curser.r][curser.c]=SELECTED;
                            }
                            break;
                        case 'o':
                        case 'O':
                            if(mode==MODE_BLOCK)
                                mode=MODE_SELECT;
                            else
                                mode=MODE_BLOCK;
                            break;
                        case 'p':
                        case 'P':
                            if(mode==MODE_FREE)
                                mode=MODE_SELECT;
                            else
                                mode=MODE_FREE;
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
                                clear(path, 5);
                                clear(total_cost, INT32_MAX);
                                found=false;
                                search_cost=depthFirstSearch(game, src, visited, total_cost, found, path);
                                path_cost=trace_path(game, dst, visited, path);
                                break;
                            case 2:
                                clear(visited);
                                clear(path, 5);
                                search_cost=breadthFirstSearch(game, src, path);
                                path_cost=trace_path(game, dst, visited, path);
                                break;
                            case 3:
                                clear(visited);
                                clear(path, 5);
                                search_cost=A_starSearch(game, src, dst, path);
                                path_cost=trace_path(game, dst, visited, path);
                                break;
                            case 4:
                                clear(visited);
                                clear(path, 5);
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
