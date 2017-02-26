#include <iostream>
#include <fstream>
using namespace std;

int count_char_vect[256];
int chars[256];
string codificare[256];

void count_char(string s) //numara aparitiile fiecarui caracter
{
    for(int i=0; i<s.length(); i++)
        count_char_vect[s[i]]++;
}
void sort_count_char() //sorteaza caracterele in functie de numarul de aparitii
{
    int da, i, j, aux;
    do
    {
        da=0;
        for(i=0;i<255;i++)
            if(count_char_vect[i]<count_char_vect[i+1])
            {
                aux=count_char_vect[i];
                count_char_vect[i]=count_char_vect[i+1];
                count_char_vect[i+1]=aux;

                aux=chars[i];
                chars[i]=chars[i+1];
                chars[i+1]=aux;

                da=1;
            }
    }
    while(da);
}

int nr_char() //numara cate caractere distincte apar in text
{
    for(int i=0; i<256; i++)
        if(count_char_vect[i]==0)
            return i;
}
void huffman()
{

}
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
int getPos(char x)
{
    for(int i=0;i<256;i++)
        if(chars[i]==(int)x)
            return i;
}
int main()
{
    float oldsize, newsize;
    //fisiere in/out
    ifstream fin("date_in.txt");
    //ofstream fout("date.out");

    //citire tot fisierul date.in in stringul s
    string s((istreambuf_iterator<char>(fin)), istreambuf_iterator<char>());
    //string s;
    //getline(fin, s);
    int n;

    for(int i=0; i<256;i++) //vectorul care contine fiecare caracter
     {
         chars[i]=i;
         codificare[i]="";
     }

    count_char(s);
    sort_count_char();
    n=nr_char();

    makeTree(0, n-1);
    fstream fout ("date_out.bin", ios::out |  ios::binary);


    for(int i=0; i<n;i++)
        cout<<(char)chars[i]<<" "<<count_char_vect[i]<<" "<<codificare[i]<<endl;

    string arh="";
    unsigned char chunk;
    for(int i=0; i<s.length(); i++){
        if(codificare[getPos(s[i])]=="")
            codificare[getPos(s[i])]="0";
        arh+=codificare[getPos(s[i])];
    }

    int lungime_arhivat=arh.length();

    fout.write((char*) &n, 1);
    fout.write((char*) &lungime_arhivat, 2);
    for(int i=0;i<n;i++)
        fout.write((char*) &chars[i], 1);
    for(int i=0;i<n;i++)
        fout.write((char*) &count_char_vect[i], 2);


    int nrchunks=0;
    int j;
    for(int i=0; i<arh.length();i=i+8)
    {
        chunk=0;
        for(j=i;j<i+8;j++)
        {
            chunk=chunk<<1;
            if(arh[j]=='1')
                chunk=chunk+1;
        }

            while(j%8!=0)
            {
                //chunk=chunk>>1;
                j=j+1;
            }/*
        for(int i=7;i>=0;i--)
            cout<<((chunk >> i) & 1);
        cout<<endl;*/
        fout.write((char*) &chunk, 1);
        nrchunks++;
    }

    oldsize=s.length();
    for(int i=0; i<s.length(); i++)
        if(s[i]==10)
            oldsize++;
    newsize=nrchunks+n*3+3;
    cout<<"Dimensiune veche: "<<oldsize<<" bytes"<<endl;
    cout<<"Dimensiune noua: "<<newsize<<" bytes"<<endl;
    cout<<"Rata de compresie: "<<100- (newsize/oldsize*1.0*100)<<"%"<<endl;
    fout.close();
    fin.close();
    cin>>n;
}
