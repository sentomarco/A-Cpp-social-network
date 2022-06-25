//
// Created by sento on 25/06/2018.
//

#ifndef PROJECT_LOGGING_H
#define PROJECT_LOGGING_H

#include <iostream>
#include <fstream>
#include <string>
#include "GestioneUtenti.hpp"
#include "Date.h"

using namespace std;

class Logging {
public:
    Logging()= default;
    Logging(const string &a, const string &b, const string &c);
    ~Logging();
    void setFile(const string &a, const string &b, const string &c);
    const string & keyword(int pos) const;
    bool result();
    bool controlParents(const Semplice *partenza,vector<const Semplice *> &fifo,int daesaminare)const;
    bool controlSons(const Semplice *partenza, vector<const Semplice *> &fifo, int daesaminare)const;
    void getNameFile(string &user, string &news, string &relation);
    const GestioneUtenti<Semplice> & getSempl()const;
    const GestioneUtenti<Azienda> & getAz()const;
    const GestioneUtenti<Gruppo> & getGr()const;

//NNBB find_last_of restituisce la posizione della prima occorrenza che trova andando all'indietro, quindi dell'ultima lettera tra " "
private:
    list<const string*> _idlist;
    unsigned short remove_blank(string::iterator first, string::iterator last)const;    //ritorna n° posizioni dopo first
    bool controlUser(string &riga, string &id, int &riesima, const Utente &utente);
    bool verifyInput(ifstream &ifile, string &file)const;        //Lavoro solo con ifstream perchè è solo di controlo
    char identifyFileUser(ifstream &ifile)const;
    bool verifyUser(ifstream &ifile, string &nomeFile);
    bool verifyRelations(ifstream &ifile, string &nomeFile);
    bool verifyNews(ifstream &ifile, string &nomeFile);
    bool verifyCampo(const Utente &utente, int &riesima, string &keyword);
    void close(ifstream &fileStrm);
//Aggiungi controllo per la find dei nominativi
//Aggiungi controllo che uno non può sposare magari se minorenne    (controlli dubbi in quanto non c'è da essere moralisiti)
    bool first_request= true;
    ifstream _file1,_file2,_file3;
    string _nome1, _nome2,_nome3,_riga;
    string _keywords[14]={"amicizia","conoscenza","coniuge","figlio","genitore","consociata","dipendente","appartenente"
                            ,"Nome","Cognome","data di nascita","utente semplice","utente azienda","utente gruppo"};
    GestioneUtenti<Semplice> _semplici;
    GestioneUtenti<Azienda> _aziende;
    GestioneUtenti<Gruppo> _gruppi;

};




#endif //PROJECT_LOGGING_H
