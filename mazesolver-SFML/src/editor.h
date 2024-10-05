#include"editor.cpp"

class Editor
{
    int x, y;

    std::vector<std::vector<bool>> grid(x, std::vector<bool>(y, true));    

    void saveMaze(char* buff);

    void readMaze(char* buff);

    void UI(sf::RenderWindow window);
};
