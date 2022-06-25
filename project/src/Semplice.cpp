//
// Created by sento on 29/06/2018.
//

#include "Semplice.h"


bool Semplice::addConiuge(const Utente *utente) const {
    if(_coniuge!= nullptr) {
        cerr<<"ERROR: user already married."<<endl;
        return false;
    }
    _coniuge=utente;
    return true;
}

bool Semplice::addGenitori(const Utente *utente) const {
    if(_genitori[0]==utente || _genitori[1]==utente){
        cerr<<"ERROR: user already insert between parent."<<endl;
        return false;
    }
    if(_genitori[0]== nullptr) _genitori[0]=utente;
    else if(_genitori[1]== nullptr) _genitori[1]=utente;
    else{
        cerr << "ERROR: not possible to have more then 2 parents" << endl;
        return false;
    }
    return true;
}

bool Semplice::addAmici(const Utente *utente) const {
    if(find(_amicizie.begin(),_amicizie.end(),utente)!=_amicizie.end()){
        cerr<<"ERROR: user already insert in his friends."<<endl;
        return false;
    }
    _amicizie.emplace_front(utente);
    return true;
}

bool Semplice::addFigli(const Utente *utente) const {
    if(find(_figli.begin(),_figli.end(),utente)!=_figli.end()){
        cerr<<"ERROR: user already insert in his sons."<<endl;
        return false;
    }
    _figli.emplace_back(utente);
    return true;
}

bool Semplice::addAppartenenze(const Utente *gruppo) const {
    if(find(_gruppi.begin(),_gruppi.end(),gruppo)!=_gruppi.end()){
        cerr<<"ERROR: user already insert in the group."<<endl;
        return false;
    }
    _gruppi.emplace_back(gruppo);
    return true;
}

bool Semplice::addLavori(const Utente *azienda) const {
    if(find(_aziende.begin(),_aziende.end(),azienda)!=_aziende.end()){
        cerr<<"ERROR: user already insert between employees."<<endl;
        return false;
    }
    _aziende.emplace_back(azienda);
    return true;
}

bool Semplice::addConoscenze(const Utente *utente) const {
    if(find(_conoscenza.begin(),_conoscenza.end(),utente)!=_conoscenza.end()){
        cerr<<"ERROR: user already insert between privity ."<<endl;
        return false;
    }
    _conoscenza.emplace_back(utente);
    return true;
}

void Semplice::removeLavoro(const Utente *darimuovere) const{
    int i=0;
    for(;i<_aziende.size();i++)
        if(_aziende[i]==darimuovere) _aziende.erase(_aziende.begin()+i);
}

void Semplice::removeGenitore(const Utente *darimuovere) const {
    if(_genitori[1]==darimuovere) _genitori[1]= nullptr;
    else if(_genitori[0]==darimuovere) {
        _genitori[0]=_genitori[1];      //Questo perchè nell'aggiornare il file in chiusura si fa for k<numGenitori
        _genitori[1]= nullptr;
    }

}

void Semplice::removeAmico(const Utente *darimuovere) const {
   _amicizie.remove(darimuovere);
}

void Semplice::removeConiuge(const Utente *darimuovere) const {
    if(_coniuge==darimuovere) _coniuge= nullptr;
}

void Semplice::removeFiglio(const Utente *darimuovere) const {
    int i=0;
    for(;i<_figli.size();i++) if(_figli[i]==darimuovere){
            _figli.erase(_figli.begin()+i);
            return;
    }
}

void Semplice::removeGruppo(const Utente *darimuovere) const {
    int i=0;
    for(;i<_gruppi.size();i++) if(_gruppi[i]==darimuovere){
            _gruppi.erase(_gruppi.begin()+i);
            return;
        }
}

int Semplice::getNumAmici() const {
    return _amicizie.size();
}

const Utente &Semplice::getConiuge() const {
    return *_coniuge;
}

const Utente &Semplice::getConoscente(int pos) const {
    auto it=_conoscenza.begin();
    for(int i=0;i<pos;i++) it++;
    return **it;
}

const Utente &Semplice::getGenitori(int pos) const {
    return *_genitori[pos];
}

const Utente &Semplice::getAmici(int pos) const {
    auto it=_amicizie.begin();
    for(int i=0;i<pos;i++) it++;
    return **it;
}

const Utente & Semplice::getFiglio(int pos) const {
    return *_figli[pos];
}

const Utente &Semplice::getLavoro(int pos) const {
    return *_aziende[pos];
}

const Utente &Semplice::getGruppo(int pos) const {
    return *_gruppi[pos];
}

int Semplice::getNumConoscenti() const {
    return _conoscenza.size();
}

int Semplice::getNumFigli() const {
    return _figli.size();
}

int Semplice::getNumLavori() const {
    return _aziende.size();
}

int Semplice::getNumGruppi() const{
    return _gruppi.size();
}

int Semplice::getNumGenitori() const{
   if(_genitori[1]!= nullptr && _genitori[0]!= nullptr) return 2;
   else if(_genitori[0]!= nullptr || _genitori[1]!= nullptr) return 1;
   return 0;
}

bool Semplice::single() const{
    return _coniuge == nullptr;
}

void Semplice::removeConoscente(const Utente *darimuovere) const {
    _conoscenza.remove(darimuovere);
}

void Semplice::removeAllOpinions() const{
    auto it=_opinione.begin();
    while(it!=_opinione.end()) {
        (*it)->remove((Utente*)this);
       it++;
    }
}

char Semplice::getVisitato() const {
    return _visitato;
}

void Semplice::setVisitato(const char &visit)const {
    _visitato=visit;
}

bool Semplice::getStampato() const {
    return _stampato;
}

void Semplice::setStampato(const bool &stamp)const {
    _stampato=stamp;
}

int Semplice::getOpinionSize() const {
    return _opinione.size();
}

void Semplice::deleteOpinion(list<const Utente *> *da_rimuovere) const {
    _opinione.remove(da_rimuovere);
}

void Semplice::addOpinion(list<const Utente *> *ptrToList) const {
    _opinione.emplace_back(ptrToList);
}

