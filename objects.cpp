#include "objects.h"
#include <iostream>
#include <string>
using namespace std;


    Disc::Disc(string lol) {
        string a;
        int swtch = 0;
        for (int i = 0; i < lol.length(); i++) {
            if (lol[i] != ';')
                a += lol[i];
            else {
                switch (swtch) {
                case(0): {
                    title = a;
                    a = "";
                    swtch++;
                }
                case(1): {
                    artist = a;
                    a = "";
                    swtch++;
                }
                case(2): {
                    genre = a;
                    a = "";
                    swtch++;
                }
                case(3): {
                    length = a;
                    a = "";
                    swtch++;
                }
                case(4): {
                    break;
                }
                }
            }
        }
    };
