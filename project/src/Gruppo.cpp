//
// Created by sento on 29/06/2018.
//

#include "Gruppo.h"

bool Gruppo::addMembri(const Utente *membro) const {
    if(find(_membri.begin(),_membri.end(),membro)!=_membri.end()){
        cerr<<"ERROR: user already present among members."<<endl;
        return false;
    }
    _membri.emplace_front(membro);
    return true;
}

void Gruppo::removeMembro(const Utente *darimuovere) const {
    auto it=_membri.begin();
    while(it!=_membri.end() && (*it)!=darimuovere) it++;
    _membri.erase(it);
}

const Utente &Gruppo::getMembri(int pos) const {
    auto it=_membri.begin();
    for(int i=0;i<pos;i++) it++;
    return **it;
}

int Gruppo::getNumMembri() const {
    return _membri.size();
}

void Gruppo::addOpinion(list<const Utente *> *ptrToList) const {

}

void Gruppo::deleteOpinion(list<const Utente *> *da_rimuovere) const {

}
