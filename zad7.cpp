#include<iostream>
#include<fstream>
#include<string>
#include<list>

using namespace std;

/* Cvor koji sadrzi rijec i niz brojeva koji oznacavaju poziciju te rijeci
 * te pokazivace na lijevi i desni cvor stabla ukoliko postoje, ukoliko ne 
 * postoje imaju vrijednost NULL.
 * Ukoliko pri dodavanju rijeci se desi da ta rijec vec postoji u stablu
 * samo updejtamo vrijednost list<unsigned int> pozicije sa metodom .push_back()
 */

struct cvor {
    string	rijec;
    list<unsigned int> pozicije;
    struct cvor *lijevo;
    struct cvor *desno;
};
typedef struct cvor Cvor;

class BST {
    public:
	void dodaj( string rijec, unsigned int pozicija );
	void ispisi_stablo( Cvor *korijen );  			// Ispisuje stablo od zadanog cvora.
	void ispisi();						// Ispisuje cijelo stablo.
	void trazi( string rijec);
	int velicina();
        int velicina( Cvor *stablo);
	BST(){ };
	~BST();
	void izbrisi_stablo( Cvor *stablo );			// Brise stablo od zadanog korijena.
    private:
	Cvor	*korijen;
};

int BST::velicina()
{
    return velicina( korijen );
}

int BST::velicina( Cvor *stablo)
{
    if( !stablo )
	return 0;
    return velicina(stablo->lijevo) + velicina(stablo->desno) + 1;
}
    


/* metod izbrisi_stablo ide rekurzivno od zadanog cvora te ukoliko taj cvor ima
 * lijevi ili desni clan poziva sam sebe nad tim clanovima.
 * Ukoliko nema dealocira taj cvor i ide do cvora roditelja te tako sve do 
 * korijena stabla
 */ 
void BST::izbrisi_stablo( Cvor *stablo)
{
    if(!stablo)
	return;
    if( stablo->lijevo )
	izbrisi_stablo( stablo->lijevo );
    if( stablo->desno )
	izbrisi_stablo( stablo->desno );

    delete stablo;
    return;
}

BST::~BST()
{
    izbrisi_stablo( korijen );
    return;
}

/* Metod dodaj radi na principu:
 * 	- Ukoliko je stablo prazno napravi novi cvor "tmp" i dodijeli vrijednost tmp 
 * 		korijenu stabla.
 *
 * 	- Ukoliko stablo nije prazno i vrijednost korjenog cvora je veca od vrijednosti
 * 		rijeci koje dodajemo, dodajemo tu rijec lijevo od cvora samo ako taj cvor
 * 		nema djece, ako ima silazimo do tog cvora i ponavljamo proceduru.
 *
 * 	- Ukoliko stablo nije prazno i vrijednost korjenog cvora je manja od vrijednosti
 * 		rijeci koje dodajemo, dodajemo tu rijec desno od cvora samo ako taj cvor
 * 		nema djece, ako ima silazimo do tog cvora i ponavljamo proceduru.
 *
 * 	- Ukoliko stablo nije prazno i vec sadrzi rijec koju dodajemo u nekom cvoru
 * 		samo cemo dodati jos jednu poziciju u listu pozicija cvora.
 */

void BST::dodaj( string rijec, unsigned int pozicija )
{
    Cvor *tmp = new Cvor;
    tmp->rijec = rijec;
    tmp->lijevo = NULL;
    tmp->desno = NULL;
    (tmp->pozicije).push_back( pozicija );

    if( !korijen ) {
	korijen = tmp;
    }

    Cvor *trenutni = korijen;
    while( trenutni ) {

    if( trenutni->rijec < rijec ) {
	if ( trenutni->desno ) {
	    trenutni = trenutni->desno;
	}else {
	    trenutni->desno = tmp;
	    trenutni = NULL;
	}
	continue;
    }

    if( trenutni->rijec > rijec ) {
	if ( trenutni->lijevo ) {
	    trenutni = trenutni->lijevo;
	}else {
	    trenutni->lijevo = tmp;
	    trenutni = NULL;
	}
	continue;
    }

    if( trenutni->rijec == rijec ) {
	(trenutni->pozicije).push_back( pozicija );
    }

    trenutni = NULL;
 }
}
/*
 * Rekurzivno ispisujemo stablo
 * prvo lijevi cvor ako postoji, zatim trenutni, zatim desni
 * tako dobijamo abecedno poredane rijeci
 */
void BST::ispisi_stablo( Cvor *stablo )
{
    if( !stablo )
	return;

    if ( stablo->lijevo )
	ispisi_stablo( stablo->lijevo );

    cout << stablo->rijec << " ";

    list<unsigned int>::iterator it = (stablo->pozicije).begin();
    while( it != (stablo->pozicije).end() ) {
	cout << *it << " ";
	it++;
    }
    cout << endl;

    if ( stablo->desno )
	ispisi_stablo( stablo->desno );

    return;
}

void BST::ispisi()
{
    ispisi_stablo( korijen );
    return;
}



/* 
 * jednostavna iterativna pretraga rijeci
 */
void BST::trazi( string rijec )
{
    Cvor *trenutni = korijen;

    while( trenutni && trenutni->rijec != rijec ) {
	if( trenutni->rijec > rijec )
	{
	    if( trenutni->lijevo ) {
		trenutni=trenutni->lijevo;
		continue;
	    }
	}else if( trenutni->rijec < rijec ) {
	    if( trenutni->desno ) {
		trenutni=trenutni->desno;
		continue;
	    }
	}
	trenutni = NULL;
	cout << "Rijec '" << rijec << "' nije pronadena." << endl;
    }

    if( trenutni ) {
	cout << "Rijec '" << rijec << "' se ponavlja " << (trenutni->pozicije).size() 
	    << " puta na pozicijama: ";
	list<unsigned int>::iterator it = (trenutni->pozicije).begin();
	while( it != (trenutni->pozicije).end() ) {
	    cout << *it << ", ";
	    it++;
	}
	cout << endl;
    }
    return;
}


/* 
 * funkcija koja se poziva u mainu za ucitavanje rijeci iz fajla
 * vidi primjer u zadaci
 */
bool ucitaj_rijeci( BST& kontejner, string ime_fajla)
 {
     ifstream fp( ime_fajla.c_str() );
     if( fp == NULL ) {
	 cout << "File ne postoji." << endl;
	 return false;
     }
     string rijec;

     unsigned int i = 0;
     while( fp >> rijec ) { 
         kontejner.dodaj( rijec, i );
	 i++;
     }   
     cout << "Ucitano je " << i << " rijeci " << kontejner.velicina()
	 << " razlicitih iz fajla " << ime_fajla << endl;

     fp.close();
     return true;
 }

int main()
{
    BST r;
    string tmp;


    cout << "> ";
    cin.clear();

    while( cin >> tmp ) {
	if( tmp == "F") {
	    cin >> tmp;
	    ucitaj_rijeci( r, tmp );
	} else if( tmp == "R" ) {
	    cin >> tmp;
	    r.trazi( tmp );
	} else if( tmp == "S" ) {
	    r.ispisi();
	} else if( tmp == "K" ) {
	    return 0;
	} else { 
	    cout << "  Komanda ne postoji." << endl;
	}

	cin.clear();
	cout << "> ";
	
    }
    return 0;
}

