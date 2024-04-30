// *****************************************************************************
// ** Maze Generation and Path Discovery Program                                 **
// *****************************************************************************

// This C++ program demonstrates the generation of mazes using a  backtracking algorithm
// and subsequently discovering a path from the entry to the exit points within the generated mazes.
//
// The program is using a stacks to manage maze cells and their attributes. Each cell
// in the maze is represented by a struct named 'cell_of_maze', containing information such as the
// presence of walls in four directions (up, down, right, left), coordinates (x, y), and whether the
// cell has been visited or not.

// The maze generation process is controlled by the 'maze_generator' function, which uses backtracking to create  paths.
// The generated mazes are then stored in files, labeled with unique maze IDs.
// To find a path within a specific maze, the 'path_discovery' function is used It uses the same maze
// generation logic, backtracking, and a stack and a mattrix to discover a path from the entry to the exit points,
// marking the visited cells in a separate matrix.

// The program allows the user to specify the number of mazes to generate, the dimensions of each maze,
// and the entry and exit points for path discovery

// *****************************************************************************
// ** Author: İpek Uzun                                                    **
// ** Date: 09.11.2023                                                     **
// *****************************************************************************

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include "Stack.h"
#include "randgen.h"

using namespace std;

struct cell_of_maze {

    // Indicates whether there is an opening to the up/down/left/right from this cell
    bool up;
    bool down;
    bool right;
    bool left;
    int x,y;  // The coordinates of the cell in the maze
    bool is_visited; // Indicates whether this cell has been visited

    // Constructor with parameters
    cell_of_maze(): up(true), down(true), right(true), left(true), x(), y(), is_visited(false) {}

    // Constructor with custom values
    cell_of_maze(bool upValue, bool downValue, bool rightValue, bool leftValue, int xValue, int yValue, bool isVisitedValue)

            : up(upValue), down(downValue), right(rightValue), left(leftValue), x(xValue), y(yValue), is_visited(isVisitedValue) {}
    
    //Copy constructor
    cell_of_maze(const cell_of_maze &other)
            : up(other.up), down(other.down), right(other.right), left(other.left), x(other.x), y(other.y), is_visited(other.is_visited) {}
};

// Read a maze file and populate a stack with cell information
void read_maze_file(Stack<cell_of_maze> & maze_stack, int mazeID) {
    
    int row, column;
    string filename = "maze_" + to_string(mazeID) + ".txt";
    ifstream file(filename);
    string x, y, left, right, up, down;
    
    // Read the row and column information from the first line of the file
    string line;
    getline(file, line);
    istringstream ss(line);
    ss >> row >> column;
    
    // Read each line of cell information from the file and push it onto the maze_stack
    while(getline(file, line)) {
        istringstream ss(line);
        // Extract and store cell attributes
        cell_of_maze cell;
        ss>>x >> y>>left>>right>>up>>down;
        cell.x = stoi(x.substr(2));
        cell.y= stoi(y.substr(2));
        cell.left = stoi(left.substr(2));
        cell.right = stoi(right.substr(2));
        cell.up = stoi(up.substr(2));
        cell.down = stoi(down.substr(2));
        // Push the cell onto the maze_stack
        maze_stack.push(cell);
    }
    
}

// Function to write maze data to an output file
void writing_output_file(Stack<cell_of_maze> &stack1, int rows, int columns, int mazeID,  Stack<cell_of_maze> & stack2) {
    
    string filename = "maze_" + to_string(mazeID) + ".txt";
    // Open the output file
    ofstream outFile(filename);
    
    // Write maze size (rows anf columns) to the file
    outFile << rows << " " << columns << endl;
    
    // Create a 2D vector to represent the maze
    vector<vector<cell_of_maze>> output_maze(columns, vector<cell_of_maze>(rows));

    // Pop cells from stack1 and populate the output_maze
    while(!stack1.isEmpty()) {
        cell_of_maze cell2 = stack1.top();
        stack1.pop();
        output_maze[cell2.x][cell2.y]=cell2;
    }
    // Write the cells in the desired order and push them to stack2
    for (int x = 0; x < columns; x++) {
        for (int y = 0; y < rows; y++) {
            cell_of_maze cell = output_maze[x][y];
            outFile << "x=" << cell.x << " y=" << cell.y << " l=" << cell.left << " r=" << cell.right<< " u=" << cell.up << " d=" << cell.down << endl;
            stack2.push(cell);
        }
    }
    
    // Close the file
    outFile.close();

}

// Write the path information from a stack to a file for a specific maze
void writing_path_file(Stack<cell_of_maze>& stack1, int rows, int columns, int mazeID, int x_entry, int y_entry,int x_exit,int y_exit) {
    
    
    // Generate the path file's filename based on mazeID and entry/exit coordinates
    string filename = "maze_" + to_string(mazeID) +"_path_"+to_string(x_entry)+ "_"+to_string(y_entry)+ "_"+ to_string(x_exit) + "_"+to_string(y_exit)+".txt";
    Stack<cell_of_maze> stack2;

    // Open the output file for writing
    ofstream outFile(filename);

    // Copy the cell information from stack1 to stack2
    while(!stack1.isEmpty()) {
        cell_of_maze cell2 = stack1.top();
        stack2.push(cell2);
        stack1.pop();
    }
    
    // Write the coordinates of the path cells to the output file
    while (!stack2.isEmpty()) {
        cell_of_maze cell = stack2.top();
        outFile << cell.x << " "<<cell.y << endl;
        stack2.pop();
        
    }
    // Close the file
    outFile.close();
}

// Check if a cell is within the maze boundaries
bool is_in_boundaries(int x, int y, int rows, int columns) {
    bool check;
    check= (x >= 0 && x < columns && y >= 0 && y < rows);
    return check;
}



// Check if a cell is unvisited
bool unvisited_check(vector<vector<cell_of_maze>> & isvisitedvector, int j, int k) {
    bool check =true;
    // Check if j and k are within valid bounds
    if ( (j >= 0 && j < isvisitedvector.size()) && (k >= 0 && k < isvisitedvector[0].size()) ) {
        check = isvisitedvector[j][k].is_visited;
        return check;
    }
    return check;

}

// Check if all cells in the maze have been visited
bool isAllVisited(vector<vector<cell_of_maze>>& isvisitedvector) {

    for (int i=0; i<isvisitedvector.size(); i++) {
        for(int j=0; j<isvisitedvector[i].size(); j++)
            if(isvisitedvector[i][j].is_visited == false) {
                return false;
            }
    }
    return true;

}

// Check if the current cell is a dead end, considering if there are walls in each direction
bool isdeadEnd(cell_of_maze Currentcell, vector<vector<cell_of_maze>>& isvisitedvector,int rows,int columns, bool path ) {

    // Check all four directions (up, down, right, left) if they are visited or not and if they are in boundaries
    bool isRightWall = false, isLeftWall = false, isUpWall = false, isDownWall = false, check;

    bool isUpVisited= ((is_in_boundaries(Currentcell.x, (Currentcell.y+1),  rows, columns) && isvisitedvector[Currentcell.x][(Currentcell.y)+1].is_visited) || !is_in_boundaries(Currentcell.x, Currentcell.y+1, rows, columns));

    bool isDownVisited= ((is_in_boundaries(Currentcell.x, (Currentcell.y-1),  rows, columns) && isvisitedvector[Currentcell.x][(Currentcell.y)-1].is_visited) || !is_in_boundaries(Currentcell.x, Currentcell.y-1, rows, columns));

    bool isRightVisited = ((is_in_boundaries((Currentcell.x + 1), Currentcell.y, rows, columns) && isvisitedvector[Currentcell.x + 1][Currentcell.y].is_visited) || !is_in_boundaries(Currentcell.x + 1, Currentcell.y, rows, columns));

    bool isLeftVisited = ((is_in_boundaries((Currentcell.x - 1), Currentcell.y, rows, columns) && isvisitedvector[Currentcell.x - 1][Currentcell.y].is_visited) || !is_in_boundaries(Currentcell.x - 1, Currentcell.y, rows, columns));

    
    //Checks whether there is a wall in each direction for if we are looking for path
    if(path){
        if(Currentcell.left) {
            isLeftWall = true;
        }  if (Currentcell.right) {
            isRightWall = true;
        }  if(Currentcell.up) {
            isUpWall = true;
        } if (Currentcell.down) {
            isDownWall = true;
        }

    }
    check = isUpVisited && isDownVisited && isRightVisited && isLeftVisited;
    
    // If 'path' is true, consider walls
    if(path) {
        check = (isLeftVisited || isLeftWall) && (isRightWall || isRightVisited) && (isUpWall || isUpVisited) && (isDownWall || isDownVisited);
    }
    return check;

}

//Function to generate a maze
void maze_generator(Stack<cell_of_maze> stack1,  vector<vector<cell_of_maze>> isvisitedvector, int row, int column, Stack<cell_of_maze> stack2, int mazeID) {

    cell_of_maze cell1;
    RandGen(rnd);
    bool deadEnd;
    
    // Initialize the starting cell(0,0) and push it
    cell1.x=0;
    cell1.y=0;
    cell1.is_visited=true;
    stack1.push(cell1);
    
    // Create a copy of the starting cell
    cell_of_maze cell2 =cell1;
    // Mark the starting cell as visited in the isvisitedvector
    isvisitedvector[0][0].is_visited=true;

    while(!stack1.isEmpty()){
        if (isAllVisited(isvisitedvector)) {
            break; // Stop tracing back if all cells have been visited
        }
        bool found = false; // A flag to track if a valid cell has been found in any direction
        cell_of_maze currentCell = stack1.top();
        // Check if the current cell is a dead end
        deadEnd = isdeadEnd(currentCell, isvisitedvector, row, column, false);
        if(!deadEnd) {
            while(!found) {
                // Generate a random number to choose a direction (1 up, 2 down, 3 right, 4 left)
                int way_of_the_cell = rnd.RandInt(1, 4);
                
                //Check if the cell is visited or not and in if in boundries while looking for direction
                //Moving right
                if(way_of_the_cell==2 && unvisited_check(isvisitedvector, (currentCell.x)+1, ((currentCell.y)))== false && is_in_boundaries((currentCell.x)+1, ((currentCell.y)), row, column) == true) {
                    
                    stack1.setTop(2);
                    cell_of_maze cell2;
                    cell2.x=(currentCell.x)+1;
                    cell2.y=((currentCell.y));
                    cell2.is_visited=true;
                    cell2.left=false;
                    isvisitedvector[cell2.x][cell2.y].is_visited=true;
                    stack1.push(cell2);
                    found=true;
                }
                //Moving left
                else if(way_of_the_cell==1 && unvisited_check(isvisitedvector, (currentCell.x)-1, ((currentCell.y)))==false && is_in_boundaries((currentCell.x)-1, ((currentCell.y)), row, column)==true) {

                    stack1.setTop(1);
                    cell_of_maze cell2;
                    cell2.x=(currentCell.x)-1;
                    cell2.y=((currentCell.y));
                    cell2.is_visited=true;
                    cell2.right=false;
                    isvisitedvector[cell2.x][cell2.y].is_visited=true;
                    stack1.push(cell2);
                    found=true;
                }
                
                //Moving up
                else if(way_of_the_cell==3 && unvisited_check(isvisitedvector, ((currentCell.x)), ((currentCell.y)+1))== false && is_in_boundaries(((currentCell.x)), ((currentCell.y)+1), row, column)==true) {
                    
                    stack1.setTop(3);
                    cell_of_maze cell2;
                    cell2.x=((currentCell.x));
                    cell2.y=((currentCell.y)+1);
                    cell2.is_visited=true;
                    cell2.down=false;
                    isvisitedvector[cell2.x][cell2.y].is_visited=true;
                    stack1.push(cell2);
                    found=true;

                }
                //Moving down
                else if(way_of_the_cell==4 && unvisited_check(isvisitedvector, ((currentCell.x)), ((currentCell.y)-1))== false &&is_in_boundaries(((currentCell.x)), ((currentCell.y)-1), row, column)==true ) {

                    stack1.setTop(4);
                    cell_of_maze cell2;
                    cell2.x=((currentCell.x));
                    cell2.y=((currentCell.y)-1);
                    cell2.is_visited=true;
                    cell2.up=false;
                    isvisitedvector[cell2.x][cell2.y].is_visited=true;
                    stack1.push(cell2);
                    found=true;
                }
                break;
            }
        }
        else {
            // The current cell is a dead end, backtrack by popping it from stack1 and pushing it to stack2
            cell_of_maze x = stack1.top();
            stack1.pop();
            stack2.push(x);
        }
    }
    // After generating the maze, push the remaining cells in stack1 to stack2 for writing to a file
    while(!stack1.isEmpty()) {
        cell_of_maze x = stack1.top();
        stack1.pop(); // stack2 ye push et
        stack2.push(x);
    }
    // Call the function to write the maze to a file
    writing_output_file(stack2, row, column, mazeID, stack1);
}

// Discover a path through the maze
void path_discovery(Stack<cell_of_maze> & stack2, int x_entry, int y_entry,int x_exit,int y_exit, Stack<cell_of_maze> & stack_for_solving, vector<vector<cell_of_maze>> & path_matrix, int row, int column, int mazeID) {

    // Read the maze data from the maze file associated with the mazeID
    read_maze_file(stack2, mazeID);
    RandGen(rnd);
    bool deadEnd;

    // Copy cell information from stack2 to the path_matrix
    while(!stack2.isEmpty()) {
        cell_of_maze cell = stack2.top();
        path_matrix[cell.x][cell.y]= cell;
        path_matrix[cell.x][cell.y].is_visited=false;
        stack2.pop();
    }
    
    // Mark the entry cell as visited and push it to stack_for_solving
    path_matrix[x_entry][y_entry].is_visited = true;
    stack_for_solving.push(path_matrix[x_entry][y_entry]);

    while(!stack_for_solving.isEmpty()){
        bool found = false;// A flag to track if a valid cell has been found in any direction
        cell_of_maze currentCell = stack_for_solving.top();
        // Exit condition: The exit cell has been reached
        
        if(currentCell.x==x_exit && currentCell.y==y_exit) {
            break;
        }
        // Check if the current cell is a dead end
        deadEnd = isdeadEnd(currentCell, path_matrix, row, column, true);
        if(!deadEnd) {
            while(!found) {
                // Generate a random number to choose a direction (1 up, 2 down, 3 right, 4 left)
                int way_of_the_cell = rnd.RandInt(1, 4);
                
                //Check if the cell is visited or not and in if in boundries while looking for direction
                if(way_of_the_cell==1 && !unvisited_check(path_matrix, (currentCell.x+1) , ((currentCell.y))) &&
                   is_in_boundaries((currentCell.x+1) , ((currentCell.y)), row, column) && !path_matrix[(currentCell.x+1) ][currentCell.y].left) { // moving right
                    cell_of_maze cell2;
                    cell2.x=(currentCell.x)+1;
                    cell2.y=((currentCell.y));
                    path_matrix[cell2.x][cell2.y].is_visited=true;
                    stack_for_solving.push(path_matrix[cell2.x][cell2.y]);
                    found=true;
                }
                
                else if(way_of_the_cell==2 && unvisited_check(path_matrix, (currentCell.x)-1, ((currentCell.y)))==false && is_in_boundaries((currentCell.x)-1, ((currentCell.y)), row, column)==true && path_matrix[(currentCell.x)-1][currentCell.y].right == false) { // moving left
                    cell_of_maze cell2;
                    cell2.x=(currentCell.x)-1;
                    cell2.y=((currentCell.y));
                    path_matrix[cell2.x][cell2.y].is_visited=true;
                    stack_for_solving.push(path_matrix[cell2.x][cell2.y]);
                    found=true;
                }
                else if(way_of_the_cell==3 && unvisited_check(path_matrix, ((currentCell.x)), ((currentCell.y)+1))== false && is_in_boundaries(((currentCell.x)), ((currentCell.y)+1), row, column)==true && path_matrix[(currentCell.x)][(currentCell.y)+1].down == false) { // moving up
                    cell_of_maze cell2;
                    cell2.x=((currentCell.x));
                    cell2.y=((currentCell.y)+1);
                    path_matrix[cell2.x][cell2.y].is_visited=true;
                    stack_for_solving.push(path_matrix[cell2.x][cell2.y]);
                    found=true;
                }

                else if(way_of_the_cell==4 && unvisited_check(path_matrix, ((currentCell.x)), ((currentCell.y)-1))== false &&is_in_boundaries(((currentCell.x)), ((currentCell.y)-1), row, column)==true && path_matrix[(currentCell.x)][(currentCell.y)-1].up == false) { // moving down
                    cell_of_maze cell2;
                    cell2.x=((currentCell.x));
                    cell2.y=((currentCell.y)-1);

                    path_matrix[cell2.x][cell2.y].is_visited=true;
                    stack_for_solving.push(path_matrix[cell2.x][cell2.y]);

                    found=true;
                }
                break;
            }
        }
        else {
            // The current cell is a dead end, backtrack by popping it from stack_for_solving
            stack_for_solving.pop();
        }
    }
    
    // Call the function to write the path to a file
    writing_path_file(stack_for_solving, row, column, mazeID,  x_entry,  y_entry, x_exit, y_exit);

}
int main() {
    int number_of_mazes, rows, columns, mazeID, x_entry, y_entry, x_exit, y_exit;
    
    // Initialize stacks and vectors to store maze data and path information
    Stack<cell_of_maze> stack1;
    Stack<cell_of_maze> stack2;
    Stack<cell_of_maze> stack_for_solving;
    Stack<cell_of_maze>  stack_for_output;
    Stack<cell_of_maze>  maze_stack;

    vector<vector<cell_of_maze>>  path_stack;
    // Prompt the user for the number of mazes, rows, columns to generate
    cout << "Enter the number of mazes: " << endl;
    cin>> number_of_mazes;

    cout << "Enter the number of rows and columns (M and N): " << endl;
    cin>> rows >> columns;
    
    // Initialize vectors to represent visited cells and paths
    vector<vector<cell_of_maze>>isvisitedvector(columns, vector<cell_of_maze>(rows));
    vector<vector<cell_of_maze>>  path_matrix(columns, vector<cell_of_maze>(rows));
    
    // Generate the specified number of mazes and display a message when done
    for(int i=1; i<=number_of_mazes; i++) {
        maze_generator(stack1, isvisitedvector, rows, columns, stack2, i);
    }
    cout << "All mazes are generated."<<endl;
    
    // Prompt the user for the maze ID, entry, and exit points
    cout << "Enter a maze ID between 1 to " <<  number_of_mazes << " inclusive to find a path: " << endl;
    cin >> mazeID;

    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): " << endl;
    cin>> x_entry >> y_entry;

    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): " << endl;
    cin>> x_exit >> y_exit;
    
    // Discover and write the path for the specified maze
    path_discovery(maze_stack, x_entry,  y_entry, x_exit, y_exit,  stack_for_solving,  path_matrix,  rows,  columns, mazeID);

    return 0;
}
