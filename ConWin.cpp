/***************************************
 * CONWIN: The CONsole WINdowing system
 * 
 * Useful for making basic visual interfaces 
 * within limited console windows.
 * 
 * Created March 2019, Jeremy Clark
 * ****************************************/

namespace ConWin {

    //Clears the console screen and sets the cursor to the top left.
    void clear() {
        std::cout << "\x1B[2J\x1B[H";
    }
    void wait() {
        std::cin.ignore();
    }
    //draws a window to the console.
    //  title: The text displayed at the top of the window
    //  instructions: The text displayed at the top of the window body.
    //  options: an array of char pointers that will be displayed as options. Can be null.
    //  type:   0 to display no return message.
    //          1 to display a "cancel to go back" message.
    //          2 to display a "Press enter to continue". This will take in no input.
    //  showNumbers: Whether or not to number the options.
    void drawHeader(std::string title, std::string instructions) {
        clear();
        printf("######################################################\n");
        printf("##  %-47s ##\n", title.c_str());
        printf("######################################################\n");
        printf("#                                                    #\n");
        printf("#   %-48s #\n", instructions.c_str());
        printf("#                                                    #\n");
    }

    void drawOptions(std::vector<std::string> options, bool showNumbers) {
        if(options.size() != 0) {
            for(int i = 0; i < static_cast<int>(options.size()); i++) {
                if(showNumbers == true) {
                    printf("#   %i: %-45s #\n", i+1, options[i].c_str());
                }else {
                    printf("#   %-48s #\n", options[i].c_str());
                }
            }
        }
        printf("#                                                    #\n");
    }

    void drawWindowControls(int type) {
        if(type == 1) {
            printf("#   Type \"cancel\" to return to the main menu         #\n");
        }else if(type == 2) {
            printf("#   Press ENTER to continue                          #\n");
        }
        printf("#                                                    #\n");
    }

    void drawFooter() {
        printf("######################################################\n");
        printf("\n");
    }

    //Draws a window that takes no input. 
    void drawDialogWindow(std::string title, std::string instructions, std::vector<std::string> options) {
        drawHeader(title, instructions);
        drawOptions(options, false);
        drawWindowControls(2);
        drawFooter();
        std::cin.ignore();
    }

    void drawPartWindow(Part *part, std::string title, std::string instructions) {
        char *lineOne = (char*)malloc(sizeof(char) * 99);
        char *lineTwo = (char*)malloc(sizeof(char) * 99);
        char *lineThree = (char*)malloc(sizeof(char) * 99);
        char *lineFour = (char*)malloc(sizeof(char) * 99);
        sprintf(lineOne, "Part Name: %s", part->getName().c_str());
        sprintf(lineTwo, "Part Cost: %d", part->getPrice());
        sprintf(lineThree, "Part Type: %d", part->getType());
        sprintf(lineFour, "Part Stock: %d", part->getStock());
        std::vector<std::string> lines = {lineOne, lineTwo, lineThree, lineFour};
        ConWin::drawDialogWindow(title, instructions, lines);
        free(lineOne);
        free(lineTwo);
        free(lineThree);
        free(lineFour);
    }

    //Draws a window that will ask the user for a numbered input based on an array of options.
    int getOptionWindow(std::string title, std::string instructions, std::vector<std::string> options, bool type) {
        bool waitingForValidInput = true;
        char *input = (char*)malloc(sizeof(char)*99);
        int input_int;
        bool firstTry = true;

        int option_range;
        for(option_range = 0; option_range < options.size(); option_range++);

        while(waitingForValidInput) {
            drawHeader(title, instructions);
            drawOptions(options, true);
            drawWindowControls(type);
            drawFooter();

            if(firstTry == false) {
                printf("Invalid input. Try again.\n");
            }

            std::cin.getline(input, 100);
            if(strcmp(input, "") != 0) {
                try{
                    input_int = std::stoi(input);
                    if(input_int > 0 && input_int <= option_range) {
                        waitingForValidInput = false;
                    }
                }catch(std::exception const & e) {
                    if(strcmp(input, "cancel") == 0) {
                        return -69;
                    }
                }
            }
            firstTry = false;
        }
        return input_int;
    }

    //draws a window that will ask the user for a positive number of any range.
    int getIntegerWindow(std::string title, std::string instructions, int type) {
        bool waitingForValidInput = true;
        char *input = (char*)malloc(sizeof(char)*99);
        int input_int;
        bool firstTry = true;

        while(waitingForValidInput) {
            drawHeader(title, instructions);
            drawWindowControls(type);
            drawFooter();

            if(firstTry == false) {
                printf("Invalid input. Try again.\n");
            }

            std::cin.getline(input, 100);
            if(strcmp(input, "") != 0) {
                try{
                    input_int = std::stoi(input);
                    waitingForValidInput = false;
                }catch(std::exception const & e) {
                    if(strcmp(input, "cancel") == 0) {
                        return -69;
                    }
                }
                
            }
            firstTry = false;
        }
        return input_int;
    }

    //Draws a window that asks a user for a string.
    char* getStringWindow(std::string title, std::string instructions, int type) {
        //flushBuffer();
        bool waitingForValidInput = true;
        char *input = (char*)malloc(sizeof(char)*99);
        bool firstTry = true;
        while(waitingForValidInput) {
            drawHeader(title, instructions);
            drawWindowControls(type);
            drawFooter();

            if(firstTry == false) {
                printf("Invalid input: %s. Try again.\n", input);
            }
            std::cin.getline(input, 100);
            if(strcmp(input, "") != 0) {
                waitingForValidInput = false;
                if(strcmp(input, "cancel") == 0){
                    return NULL;
                }
            }
            firstTry = false;
        }
        return input;
    }
}
