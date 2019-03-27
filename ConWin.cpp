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
    
    //draws a window to the console.
    //  title: The text displayed at the top of the window
    //  instructions: The text displayed at the top of the window body.
    //  options: an array of char pointers that will be displayed as options. Can be null.
    //  type:   0 to display no return message.
    //          1 to display a "cancel to go back" message.
    //          2 to display a "Press enter to continue". This will take in no input.
    //  showNumbers: Whether or not to number the options.
    void drawWindow(const char *title, const char *instructions, const char **options, int type, bool showNumbers) {
        clear();
        printf("######################################################\n");
        printf("##  %-47s ##\n", title);
        printf("######################################################\n");
        printf("#                                                    #\n");
        printf("#   %-48s #\n", instructions);
        if(options != NULL) {
            for(int i = 0; options[i] != NULL; i++) {
                if(showNumbers == true) {
                    printf("#   %i: %-45s #\n", i+1, options[i]);
                }else {
                    printf("#   %-48s #\n", options[i]);
                }
            }
        }
        printf("#                                                    #\n");
        if(type == 1) {
            printf("#   Type \"cancel\" to return to the main menu         #\n");
        }else if(type == 2) {
            printf("#   Press ENTER to continue                          #\n");
        }
        printf("#                                                    #\n");
        printf("######################################################\n");
        printf("\n");
    }

    //Draws a window that takes no input. 
    void drawDialogWindow(const char *title, const char *instructions, const char **options) {
        drawWindow(title, instructions, options, 2, false);
        std::cin.ignore();
    }

    //Draws a window that will ask the user for a numbered input based on an array of options.
    int getOptionWindow(const char *title, const char *instructions, const char **options, bool type) {
        bool waitingForValidInput = true;
        char *input = (char*)malloc(sizeof(char)*99);
        int input_int;
        bool firstTry = true;

        int option_range;
        for(option_range = 0; options[option_range] != NULL; option_range++);

        while(waitingForValidInput) {
            drawWindow(title, instructions, options, type, true);

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
    int getIntegerWindow(const char *title, const char *instructions, int type) {
        bool waitingForValidInput = true;
        char *input = (char*)malloc(sizeof(char)*99);
        int input_int;
        bool firstTry = true;

        while(waitingForValidInput) {
            drawWindow(title, instructions, NULL, type, true);

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
    char* getStringWindow(const char *title, const char *instruction, int type) {
        //flushBuffer();
        bool waitingForValidInput = true;
        char *input = (char*)malloc(sizeof(char)*99);
        bool firstTry = true;
        while(waitingForValidInput) {

            drawWindow(title, instruction, NULL, type, true);

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
