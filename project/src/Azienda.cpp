//
// Created by sento on 29/06/2018.
//

#include "Azienda.h"

bool Azienda::addConsociate(const Utente *conso)const {
    if(find(_consociate.begin(),_consociate.end(),conso)!=_consociate.end()){
        cerr<<"ERROR: user already present among the affiliates.."<<endl;
        return false;
    }
    _consociate.emplace_back(conso);
    return true;
}

bool Azienda::addDipendenti(const Utente *dipe)const {
    if(find(_dipendenti.begin(),_dipendenti.end(),dipe)!=_dipendenti.end()){
        cerr<<"ERROR: user already listed as an employee."<<endl;
        return false;
    }
    _dipendenti.emplace_back((dipe));
    return true;
}

void Azienda::removeConsociata(const Utente *darimuovere)const {
    int i=0;
    for(;i<_consociate.size();i++) if(_consociate[i]==darimuovere){
            _consociate.erase(_consociate.begin()+i);
            return;
        }
}

void Azienda::removeDipendente(const Utente *darimuovere)const {
    auto it=_dipendenti.begin();
    while(it!=_dipendenti.end() && (*it)!=darimuovere) it++;
    if(it!=_dipendenti.end()) _dipendenti.erase(it);
}

const Utente &Azienda::getDipendente(int pos) const {
    auto it=_dipendenti.begin();
    for(int i=0;i<pos;i++) it++;
    return **it;
}

const Utente &Azienda::getConsociata(int pos) const {
    return *_consociate[pos];
}

int Azienda::getNumDipendenti() const {
    return _dipendenti.size();
}

int Azienda::getNumConsociate() const{
    return _consociate.size();
}

void Azienda::addOpinion(list<const Utente *> *ptrToList) const {

}

void Azienda::deleteOpinion(list<const Utente *> *da_rimuovere) const {

}
