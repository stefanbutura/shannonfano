#include <iostream>
#include <fstream>
using namespace std;

int count_char_vect[256];
int chars[256];
int nr_char, text_length;
string codificare[256];
string text;

void makeTree(int i_begin, int i_end)
{
    int sum_l=0, sum_r=0, i, dif, middle;
    if(i_begin==i_end)
    {
        //codificare[i_begin]+='0';
        return;
    }

    else if(i_begin==i_end-1)
    {
        codificare[i_begin]+='0';
        codificare[i_end]+='1';
    }
    else
    {
        for(i=i_begin; i<=i_end;i++)
            sum_r+=count_char_vect[i];

        i=i_begin;
        while(sum_l < sum_r && i<i_end)
        {
            dif=sum_r-sum_l;
            sum_l+=count_char_vect[i];
            sum_r-=count_char_vect[i];
            i=i+1;
        }
        if(dif <= sum_l-sum_r)
            middle=i-2;
        else middle=i-1;

        for(i=i_begin; i<=middle; i++)
            codificare[i]+='0';
        for(i=middle+1; i<=i_end; i++)
            codificare[i]+='1';
    /*
         for(int i=i_begin;i<=middle;i++)
            cout<<(char)chars[i]<<" ";
        cout<<"|";
        for(int i=middle+1;i<=i_end;i++)
            cout<<(char)chars[i]<<" ";
        cout<<endl;
*/
        makeTree(i_begin, middle);
        makeTree(middle+1, i_end);

    }
}
int find_cod(string c)
{
    int i;
    for(i=0; i<nr_char; i++)
    {

        if(codificare[i]==c)
        {
            //cout<<codificare[i]<<" "<<c<<endl;
            return i;
        }
    }
    return nr_char;
}
int main()
{
    int i, j, k=0;
    unsigned char chunk;
    fstream fin("date_out.bin", ios::in |  ios::binary);
    fin.read((char*)&nr_char, 1);
    fin.read((char*)&text_length, 2);
    for(i=0;i<nr_char;i++)
        fin.read((char*)&chars[i], 1);
    for(i=0;i<nr_char;i++)
        fin.read((char*)&count_char_vect[i], 2);
    int nrbytes=text_length/8;
    if(!text_length%8)
        nrbytes++;
    for(i=0; i<nrbytes+1; i++)
    {
        fin.read((char*)&chunk, 1);
        for(j=7; j>=0 && k<text_length; j--)
        {
            if((chunk >> j) & 1)
                text+='1';
            else text+='0';
            k++;
        }
    }

    string c;
    string text_init;
    makeTree(0, nr_char-1);
    for(i=0; i< text.length(); i++)
    {
        c+=text[i];

        if((k=find_cod(c))!=nr_char)
        {
            text_init+=(char)chars[k];
            c="";
        }

    }
    ofstream fout("date_out.txt");
    cout<<text.length();
    fout<<text_init;
    fout.close();
}
