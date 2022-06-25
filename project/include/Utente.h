//
// Created by sento on 10/07/2018.
//

#ifndef PROJECT_UTENTE_H
#define PROJECT_UTENTE_H

#include <iostream>
#include <algorithm>
#include <list>
#include <vector>
#include "Date.h"

using namespace std;


class Utente {
public:
    void addMex(const string &str) const; //Li ordinerei per l'ultimo aggiunto (come data, presa dal file)
    void removeMex(int pos) const;        //NON è VERO!!!!!! USATOOOOOO in removeMessage
    void setMex(int pos,const string & mex,bool theLastOneUsed = false ) const;
    void sortMex() const;
    virtual void addOpinion(list<const Utente *>* ptrToList) const=0;   //Sfruttata in setOpinion
    virtual void deleteOpinion(list<const Utente *>* ptrToList) const=0;
    void removeOpinion(const Utente& follower, bool like=true) const;
    void setOpinion(const Utente& follower, bool like=true) const;        //USATA IN GU
    void setData(const Date &data, int pos=0) const;
    void setInfo(int pos, const string &str) const;
    void newInfo(const string& str,bool back=true) const;         //Assegna una nuova informazione true>> emplaceback false>>front
    void clearInfo(short pos) const;        //USATO IN MANGEMENT per ridurre numero info
    void deleteInfo( vector<string>::const_iterator it1, vector<string>::const_iterator it2) const;
    const string & getMex(int pos) const;
    const string & getID() const;           //idem  in pratica dall'esterno tu richiedi la get, fai le verifiche, fornisi la modifica alla set
    string getData(int pos)const;
    bool setId(string str) const;             //Effettua dei controlli per non introdurre errori
    void addLike(const Utente *utnt, int pos = 0, bool theLastOneUsed = false) const;
    void addDislike(const Utente *utnt, int pos = 0, bool theLastOneUsed=false) const;
    int getNumMex() const;
    void removeLiker(list<const Utente*>::const_iterator it, bool like=true) const;//VA usata SOLO se _mexIt punta all'ultimo mex
    const list<const Utente*>& getLikeListConst(int pos = 0, bool theLastOneUsed = false) const;
    const list<const Utente*>& getDislikeListConst(int pos = 0, bool theLastOneUsed = false) const;
    const vector<string>& getInfo() const;

protected:

    struct mex{
        string messaggio;
        Date dataPubblicazione;
        list<const Utente *> like, dislike;
    };
    mutable string _id;
    mutable vector<string> _informazioni;     //forse meglio vector tanto non hai migliaia di info da memorizzare
    mutable list<mex> _messaggi;
    mutable list<mex>::iterator _mexIt=_messaggi.begin();   //Occhio che prima di fare una una remove ci deve essere una get che lo porti allla giusta pos
};


#endif //PROJECT_UTENTE_H
