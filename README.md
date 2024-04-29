
** Maze Generation and Path Discovery Program **                               


 This C++ program demonstrates the generation of mazes using a  backtracking algorithm
and subsequently discovering a path from the entry to the exit points within the generated mazes.

The program is using a stacks to manage maze cells and their attributes. Each cell
in the maze is represented by a struct named 'cell_of_maze', containing information such as the
presence of walls in four directions (up, down, right, left), coordinates (x, y), and whether thecell has been visited or not.

 The maze generation process is controlled by the 'maze_generator' function, which uses backtracking to create  paths. The generated mazes are then stored in files, labeled with unique maze IDs. To find a path within a specific maze, the 'path_discovery' function is used It uses the same maze
generation logic, backtracking, and a stack and a matrix to discover a path from the entry to the exit points, marking the visited cells in a separate matrix.

 The program allows the user to specify the number of mazes to generate, the dimensions of each maze,
 and the entry and exit points for path discovery
