#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

char getCryptSymbol(char input)
{
    char res;

    switch(input)
    {
        case 'ï':
        {
            res = 'a';
            break;
        }
        case 'ð':
        {
            res = 'b';
            break;
        }
        case 'è':
        {
            res = 'c';
            break;
        }
        case 'â':
        {
            res = 'd';
            break;
        }
        case 'å':
        {
            res = 'e';
            break;
        }
        case 'ò':
        {
            res = 'f';
            break;
        }
        case 'ý':
        {
            res = 'g';
            break;
        }
        case 'î':
        {
            res = 'k';
            break;
        }
        case 'ô':
        {
            res = 'l';
            break;
        }
        case 'à':
        {
            res = 'm';
            break;
        }
        case 'é':
        {
            res = 'n';
            break;
        }
        case 'ë':
        {
            res = 'o';
            break;
        }
        case 'ä':
        {
            res = 'p';
            break;
        }
        case 'æ':
        {
            res = 'r';
            break;
        }
        case 'ø':
        {
            res = 's';
            break;
        }
        case 'ê':
        {
            res = 't';
            break;
        }

        default:
        {
            res = input;
            break;
        }
    }

    return res;
}


int main(int arg, char *args[])
{
    ifstream in_file;
    in_file.open ("C:\\IOleshko\\tmp\\in.data", ios::in | ios::app | ios::binary); 

    ofstream out_file;
    out_file.open ("C:\\IOleshko\\tmp\\out.data", ios::in | ios::binary); 

    char read_ch;
    char crypt_ch;
    while(!in_file.eof())
    {
        read_ch = in_file.get();
        crypt_ch = getCryptSymbol(read_ch);
        if(crypt_ch != -1)
            out_file << crypt_ch;
    }

    in_file.close();
    out_file.close();

    return 0;
}

