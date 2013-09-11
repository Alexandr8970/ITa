#include <stdio.h>
#include <iostream>
#include <fstream>
using namespace std;

char getCryptSymbol(char input)
{
    char res;

    switch(input)
    {
        case '�':
        {
            res = 'a';
            break;
        }
        case '�':
        {
            res = 'b';
            break;
        }
        case '�':
        {
            res = 'c';
            break;
        }
        case '�':
        {
            res = 'd';
            break;
        }
        case '�':
        {
            res = 'e';
            break;
        }
        case '�':
        {
            res = 'f';
            break;
        }
        case '�':
        {
            res = 'g';
            break;
        }
        case '�':
        {
            res = 'k';
            break;
        }
        case '�':
        {
            res = 'l';
            break;
        }
        case '�':
        {
            res = 'm';
            break;
        }
        case '�':
        {
            res = 'n';
            break;
        }
        case '�':
        {
            res = 'o';
            break;
        }
        case '�':
        {
            res = 'p';
            break;
        }
        case '�':
        {
            res = 'r';
            break;
        }
        case '�':
        {
            res = 's';
            break;
        }
        case '�':
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

