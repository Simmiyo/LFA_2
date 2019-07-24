#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;

struct tranzitie{int s1;int s2;string eticheta;};

class automat
{
    int card_Q;
    int q0;
    int card_F;
    int nr_tranzitii;
    vector <int> F;
    vector <tranzitie> T;
    public:
    friend istream & operator>>(istream &,automat &);
    friend ostream & operator<<(ostream &,const automat &);
    void transfomare_AFE();
    void adaugare_lambda_cicluri();
    void stari_init_fin();
    void eliminare_stari(int st);
    string cauta_eticheta(int,int);
    void eliminare_tranzitie(int,int);
    friend string REX(automat);
};

istream & operator>>(istream & intrare,automat & aut)
{
    int i,x;
    tranzitie t;
    intrare>>aut.card_Q;
    intrare>>aut.q0;
    intrare>>aut.card_F;
    for(i=0;i<aut.card_F;i++)
    {
        intrare>>x;
        aut.F.push_back(x);
    }
    intrare>>aut.nr_tranzitii;
    for(i=0;i<aut.nr_tranzitii;i++)
    {
        intrare>>t.s1>>t.s2>>t.eticheta;
        aut.T.push_back(t);
    }
    return intrare;
}

ostream & operator<<(ostream & iesire,const automat & aut)
{
    int i;
    iesire<<"Nr stari: "<<aut.card_Q<<endl;
    iesire<<"Stare initiala: "<<aut.q0<<endl;
    iesire<<"Nr stari finale: "<<aut.card_F<<endl;
    iesire<<"Stari finale: ";
    for(i=0;i<aut.card_F;i++)
    {
        iesire<<aut.F[i]<<" ";
    }
    iesire<<endl<<"Nr. tranzitii: "<<aut.nr_tranzitii<<endl<<"Tranzitii: "<<endl;
    for(i=0;i<aut.nr_tranzitii;i++)
    {
        iesire<<aut.T[i].s1<<" "<<aut.T[i].s2<<" "<<aut.T[i].eticheta<<endl;
    }
    return iesire;
}

void automat::transfomare_AFE()
{
    int i,j,l;
    for(i=0;i<nr_tranzitii;i++)
    {
        if(T[i].eticheta.find(','))
        {
            l = T[i].eticheta.size();
            for(j=0;j<l;j++)
                if(T[i].eticheta[j] == ',')
                    T[i].eticheta[j] = '+';
        }
    }
}

void automat::adaugare_lambda_cicluri()
{
    int i;

    bool v[card_Q];
    tranzitie t;
    for(i=1;i<=card_Q;i++)
        v[i] = 0;
    for(i=0;i<=nr_tranzitii;i++)
    {
        if(T[i].s1 == T[i].s2)
            v[T[i].s1] = 1;
    }
    v[q0] = v[F[0]] = 1;
    for(i=1;i<=card_Q;i++)
    {
        if(v[i] == 0)
        {
            nr_tranzitii++;
            t.s1 = i;
            t.s2 = i;
            t.eticheta = "@";
            T.push_back(t);
        }
    }
}

void automat::stari_init_fin()
{
    int i, k = 0;
    tranzitie t;
    for(i=0;i<nr_tranzitii && !k;i++)
    {
        if(T[i].s2 == q0)
        {
            k = 1;
            card_Q++;
            nr_tranzitii++;
            t.s1 = card_Q;
            t.s2 = q0;
            t.eticheta = "@";
            q0 = card_Q;
            T.push_back(t);
        }
    }
    k = 0;
    if(card_F > 1)
        k = 1;
    else
    {
        for(i=0;i<nr_tranzitii && !k;i++)
            if(T[i].s1 == F[0])
                k = 1;
    }
    if(k == 1)
    {
        card_Q++;
        for(i=0;i<card_F;i++)
        {
            nr_tranzitii++;
            t.s1 = F[i];
            t.s2 = card_Q;
            t.eticheta = "@";
            T.push_back(t);
        }
        card_F = 1;
        F.clear();
        F.push_back(card_Q);
    }
}

string automat::cauta_eticheta(int q1,int q2)
{
    int i;
    for(i=0;i<nr_tranzitii;i++)
    {
        if(T[i].s1 == q1 && T[i].s2 == q2)
            return T[i].eticheta;
    }
    return "";
}

void automat::eliminare_tranzitie(int q1,int q2)
{
    int i;
    for(i=0;i<nr_tranzitii;i++)
    {
        if(T[i].s1 == q1 && T[i].s2 == q2)
        {
            T.erase(T.begin()+i);
            nr_tranzitii--;
            break;
        }
    }
}

void automat::eliminare_stari(int q)
{
    int i,j;
    tranzitie t;
    string bucla_q = this->cauta_eticheta(q,q),et1,et2;
    if(bucla_q.find("+") != string::npos)
            bucla_q = "(" + bucla_q + ")";
    this->eliminare_tranzitie(q,q);
    for(i=0;i<nr_tranzitii;i++)
    {
        if(T[i].s2 == q)
        {
            et1 = T[i].eticheta;
            for(j=0;j<nr_tranzitii;j++)
            {
                if(T[j].s1 == q)
                {
                     et2 = T[j].eticheta;
                     t.s1 = T[i].s1;
                     t.s2 = T[j].s2;
                     t.eticheta = (*this).cauta_eticheta(t.s1,t.s2);
                     if(t.eticheta.size()>=1) t.eticheta = t.eticheta + "+";
                     this->eliminare_tranzitie(t.s1,t.s2);
                     if( et1.find("+") != string::npos )
                        et1 = "(" + et1 + ")";
                     if( et2.find("+") != string::npos )
                        et2 = "(" + et2 + ")";
                     t.eticheta = t.eticheta + et1 + bucla_q + "*" + et2;
                     T.push_back(t);
                     nr_tranzitii++;
                }
            }
        }
    }
    i=0;
    while(i<T.size())
    {
        if(T[i].s1 == q || T[i].s2 == q)
        {
            T.erase(T.begin()+i);
            nr_tranzitii--;
        }
        else i++;
    }
    card_Q--;
}

string REX(automat A)
{
    int i,s;
    A.transfomare_AFE();
    A.stari_init_fin();
    A.adaugare_lambda_cicluri();
    s = A.card_Q;
    for(i=1;i<=s;i++)
    {
        if(i != A.q0 && i != A.F[0])
           A.eliminare_stari(i);
    }
    return A.T[0].eticheta;
}

int main()
{
    ifstream f;
    ofstream g;
    f.open("automat.in");
    g.open("REX.out");
    automat A;
    f>>A;
    g<<REX(A);
    f.close();
    g.close();
    return 0;
}
