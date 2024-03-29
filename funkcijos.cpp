#include "funkcijos.h"
#include "strukturos.h"

// Funkcija, tikrinanti, ar vardas bei pavarde yra sudaryti tik is raidziu
bool tikRaides(string ivedimas)
{
    for(char c : ivedimas) // Keliaujame per kiekviena ivesto zodzio simboli ir tikriname ar tas simbolis yra raide
    {
        if(!isalpha(c))
            return false;
    }
    return true;
}

// Funkcija, pakeicianti visas gauto teksto raides i didziasias
string didziosios(string &tekstas)
{
    for(char &c : tekstas) // Keliaujame per kiekviena ivesto teksto raide ir paverciame ji i didziaja raide
    {
        c = toupper(c);
    }
    return tekstas;
}

// Funkcija, apskaiciuojanti tarpu skaiciu eiluteje
int tarpuSkaicius(string ivedimas)
{
    int kiekis = 0;
    for(char c : ivedimas) // Keliaujame per kiekviena ivedimo simboli ir tikriname ar jis yra tarpas
    {
        if(c == ' ')
            kiekis++;
    }
    return kiekis;
}

// Funkcija, sugeneruojanti atsitiktini skaiciu nuo 1 iki 10 imtinai
int generuotiPazymi()
{
    int skaicius = rand() % 10 + 1;

    return skaicius;
}

// Funkcija, sugeneruojanti studento varda
string generuotiVarda(int i)
{
    string vardas;
    
    vardas = "Vardas" + to_string(i + 1);

    return vardas;
}

// Funkcija, sugeneruojanti studento pavarde
string generuotiPavarde(int i)
{
    string pavarde;
    
    pavarde = "Pavarde" + to_string(i + 1);

    return pavarde;
}

bool palygintiMazejant(const studentas& a, const studentas& b)
{
    return a.galutinis > b.galutinis;
}


bool palygintiDidejant(const studentas& a, const studentas& b)
{
    return a.galutinis < b.galutinis;
}

void generuotiFaila(int studKiekis, int pazKiekis, string failoVardas)
{
    ofstream naujas(failoVardas);
    stringstream eilute;

    eilute << left << setw(20) << "Vardas" << setw(20) << "Pavarde"; // Isvedame antrastine eilute
    for (int i = 0; i < pazKiekis; i++)
    {
        string nd = "ND" + to_string(i + 1);

        eilute << setw(10) << nd;
    }
    eilute << setw(10) << "Egz." << endl;

    for (int i = 0; i < studKiekis; i++)
    {
        string vardas = "Vardas" + to_string(i + 1);
        string pavarde = "Pavarde" + to_string(i + 1);

        eilute << left << setw(20) << vardas << setw(20) << pavarde; // Isvedame kiekvieno studento varda ir pavarde
        for (int j = 0; j <= pazKiekis; j++)
        {
            eilute << setw(10) << generuotiPazymi();
        }
        eilute << endl;

        if ((i + 1) % 1000000 == 0)
        {
            naujas << eilute.str();
            eilute.str("");
        }
    }

    naujas << eilute.str();
    eilute.clear();

    naujas.close();
}

template <typename Cont>
void failoSkaitymas(ifstream &input, Cont &stud)
{
    long count = 0;
    stringstream ss;
    string eilute;

    if constexpr (is_same<Cont, vector<studentas>>::value)
    {
        stringstream::pos_type position = ss.tellg();

        while(!ss.eof())
        {
            getline(ss, eilute);
            count++;
        }
        ss.clear();
        ss.seekg (position, ss.beg);
        stud.reserve(count);
        count = 0;
    }
    
    getline(input, eilute); // Praleidziame pirmaja failo eilute

    int pazymys;

    while (getline(input, eilute)) // Skaitome duomenis is failo kol ju yra
    {
        istringstream iss(eilute); // Padaliname juos i atskirus string'us, atskirtus tarpais
        studentas student;

        iss >> student.vardas >> student.pavarde;

        while(iss >> pazymys)
        {
            student.nd.push_back(pazymys);
        }

        student.egz = student.nd.back();
        student.nd.pop_back();

        stud.push_back(student);
    }
}
template void failoSkaitymas<vector<studentas>>(ifstream&, vector<studentas>&);
template void failoSkaitymas<list<studentas>>(ifstream&, list<studentas>&);
template void failoSkaitymas<deque<studentas>>(ifstream&, deque<studentas>&);

template <typename Cont>
void strategija1(Cont &stud, Cont &vargsiukai, Cont &galvociai)
{
    for (auto &i : stud)
    {
        if (i.galutinis < 5)
        {
            vargsiukai.push_back(i);
        }
        else
        {
            galvociai.push_back(i);
        }
    }
}
template void strategija1<vector<studentas>>(vector<studentas> &stud, vector<studentas> &vargsiukai, vector<studentas> &galvociai);
template void strategija1<list<studentas>>(list<studentas> &stud, list<studentas> &vargsiukai, list<studentas> &galvociai);
template void strategija1<deque<studentas>>(deque<studentas> &stud, deque<studentas> &vargsiukai, deque<studentas> &galvociai);

template <typename Cont>
void strategija2(Cont &stud, Cont &vargsiukai)
{
    typename Cont::iterator it = stud.begin();
    while (it != stud.end())
    {
        if (it->galutinis < 5)
        {
            vargsiukai.push_back(*it);
            it = stud.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
template void strategija2<vector<studentas>>(vector<studentas> &stud, vector<studentas> &vargsiukai);
template void strategija2<list<studentas>>(list<studentas> &stud, list<studentas> &vargsiukai);
template void strategija2<deque<studentas>>(deque<studentas> &stud, deque<studentas> &vargsiukai);

template <typename Cont>
void strategija3(Cont &stud, Cont &vargsiukai)
{
    auto partition_point = stable_partition(stud.begin(), stud.end(), [](const studentas& s) { return s.galutinis < 5; });
    vargsiukai.insert(vargsiukai.end(), stud.begin(), partition_point);
    stud.erase(stud.begin(), partition_point);
}
template void strategija3<vector<studentas>>(vector<studentas> &stud, vector<studentas> &vargsiukai);
template void strategija3<list<studentas>>(list<studentas> &stud, list<studentas> &vargsiukai);
template void strategija3<deque<studentas>>(deque<studentas> &stud, deque<studentas> &vargsiukai);

template <typename Cont>
void rikiuotiDidejant(Cont &container)
{
    if constexpr (is_same<Cont, list<studentas>>::value)
    {
        container.sort(palygintiDidejant);
    }
    else
    {
        sort(container.begin(), container.end(), palygintiDidejant);
    }
}
template void rikiuotiDidejant<vector<studentas>>(vector<studentas> &container);
template void rikiuotiDidejant<list<studentas>>(list<studentas> &container);
template void rikiuotiDidejant<deque<studentas>>(deque<studentas> &container);

template <typename Cont>
void rikiuotiMazejant(Cont &container)
{
    if constexpr (is_same<Cont, list<studentas>>::value)
    {
        container.sort(palygintiMazejant);
    }
    else
    {
        sort(container.begin(), container.end(), palygintiMazejant);
    }
}
template void rikiuotiMazejant<vector<studentas>>(vector<studentas> &container);
template void rikiuotiMazejant<list<studentas>>(list<studentas> &container);
template void rikiuotiMazejant<deque<studentas>>(deque<studentas> &container);