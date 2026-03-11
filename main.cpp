#include <iostream>

#include "ChessDesk.h"
#include "Input.h"

int main()
{
    try
    {
        ChessDesk desk;
        Input input;
        int choice = 0;

        std::cout << "1. Add figures from console\n";
        std::cout << "2. Add figures from file (figures.txt)\n";
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1)
        {
            while (input.addFromConsole(desk)) {}
        }
        else if (choice == 2)
        {
            std::string filename = "figures.txt";
            if (input.addFromFile(desk, filename))
            {
                
            }
            else
            {
                std::cout << "Failed to load file.\n";
                return 1;
            }
        }
        else
        {
            std::cout << "Invalid choice\n";
            return 1;
        }


        std::cout << "\n--- FINAL DESK ---\n";
        desk.print();

        std::cout << "\n--- RESULT of ANALYSE ---\n";
        desk.analyze();
    }
    catch (const std::out_of_range& ex)
    {
        std::cout << ex.what() << '\n';
        return 1;
    }
    catch (const std::exception& e)
    {
        std::cout << e.what() << '\n';
        return 1;
    }
    catch (...)
    {
        std::cout << "ERROR!!!\n";
        return 1;
    }
    
    return 0;
}