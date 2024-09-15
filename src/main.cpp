#pragma once

#include<iostream>
#include<fstream>

#include <SFML/Graphics.hpp>
#include "imgui/imgui.h"
#include "imgui/imgui-SFML.h"

int x = 0, y = 0; 

std::vector<std::vector<bool>> grid(x, std::vector<bool>(y, true));

void saveMaze(char* buff)
{
    std::string filename(buff);
    std::cout<< "saving " << filename << "\n";
    filename.append(".txt");

    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    for(const auto& row : grid)
    {
        for(bool cell : row)
            file << ((cell)? "1" : "0");
        file << "\n";
    }
    file.close();
}

int main()
{

    sf::RenderWindow window(sf::VideoMode(1280,720),"Maze maker");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    char buff[100];

    sf::Clock deltaClock;


    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
           
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed)
            {
                switch(event.key.code)
                {
                    case sf::Keyboard::Escape:
                        window.close();
                        break;
                    case sf::Keyboard::C:
                        for(auto& row : grid)
                            std::fill(row.begin(), row.end(), true);
                        break;
                }
            }

                        // Check for mouse clicks
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) 
            {
                // Get mouse position
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                // Calculate rectangle size based on the grid
                float rectWidth = window.getSize().x / static_cast<float>(x);
                float rectHeight = window.getSize().y / static_cast<float>(y);

                // Determine which cell was clicked
                int gridX = mousePos.x / rectWidth;
                int gridY = mousePos.y / rectHeight;

                // Toggle the color state of the clicked cell if within bounds
                if (gridX >= 0 && gridX < x && gridY >= 0 && gridY < y) {
                    grid[gridX][gridY] = !grid[gridX][gridY]; // Toggle color
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::Begin("Size selector");
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::InputInt("x", &x);
        ImGui::InputInt("y", &y);
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("Enter name for maze: ");
        ImGui::InputText("", buff, 100);
        ImGui::SameLine();
        if(ImGui::Button("Save"))
        {
            saveMaze(buff);
        }
        ImGui::End();

                // Resize grid if needed
        if (x > 0 && y > 0) {
            grid.resize(x); // Adjust number of rows
            for (auto& row : grid) {
                row.resize(y, true); // Adjust each row size, initialize new cells to white
            }
        }

        window.clear();

        float recWidth = window.getSize().x / static_cast<float>(x);
        float recHieght = window.getSize().y / static_cast<float>(y);

        if(x!=0 && y!=0)
        {
            for(int i = 0; i < x; ++i)
            {
                for(int j = 0; j < y; ++j)
                {
                    sf::RectangleShape rect(sf::Vector2f(recWidth -1, recHieght -1));
                    rect.setPosition(i*recWidth, j*recHieght);
                    // Set rectangle color based on the grid state (white or black)
                    if (grid[i][j]) {
                        rect.setFillColor(sf::Color::White);
                    } else {
                        rect.setFillColor(sf::Color::Black);
                    }
                    window.draw(rect);
                }
            }
        }


        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}