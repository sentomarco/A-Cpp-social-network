//
// Created by sento on 05/09/2018.
//

#ifndef PROJECT_GESTIONEUTENTI_HPP
#define PROJECT_GESTIONEUTENTI_HPP

#include <iostream>
#include <list>
#include <algorithm>
#include "Semplice.h"
#include "Gruppo.h"
#include "Azienda.h"
using namespace std;                    //l'end() è un valore dopo l'ultimo valore valido (tipo sentinella)
template <typename T>
class GestioneUtenti {
public:
    //lascia anche _it puntato sull'utente corrente
    template<typename M>         //per gestire relazioni in modo duplice
    void manageRelation(const T *utnt1,const M *utnt2, void (T::*fnToManage1)(const Utente *) const, void (M::*fnToManage2)(const Utente *)const) const ;
    template<typename M>
    bool manageRelation(const T *utnt1,const M *utnt2, bool (T::*fnToManage1)(const Utente *)const, bool (M::*fnToManage2)(const Utente *) const) const;
    static bool addOpinionGU( const T *poster,  const Semplice *follower, int numMex, bool like = true) ;
    void removeOpinionGU(const T *poster, int numMex, int numOpinion, bool like = true) const;
    void removeMessage(const T *poster, int numMex) const;
    const T* getById(string &id) const;
    const T & operator*()  const;
    const T & getLastUtente() const;
    const T & newUtente() const;
    void addUtente() const;
    void removeUser() const;
    bool findId(string &id) const;        //non sposta _it
    const string & information(int pos) const; //mi restituisce l'informazione n° pos dell'utente a cui punta _it   //INUTILE ma non dannosa
    const string & getId(int userPos=-1) const;
    int getNumUsers() const;
    int getNumberInfo() const;          //INUTILE ma non dannosa
    const typename list<T>::iterator & iteratorBegin() const;
    typename list<T>::iterator iteratorEnd() const;

private:
    mutable typename list<T>::iterator _it=_users.begin();
    mutable list<T> _users;
};

template<typename T>
const T & GestioneUtenti<T>::getLastUtente() const {
    return _users.back();
}

template<typename T>
const T &GestioneUtenti<T>::newUtente() const {
    _users.emplace_back();
    return _users.back();
}

template<typename T>
const typename list<T>::iterator & GestioneUtenti<T>::iteratorBegin() const {   //Non so se meglio per reference così o no, vedro a fine lavoro
    return _it=_users.begin();
}

template<typename T>
typename list<T>::iterator  GestioneUtenti<T>::iteratorEnd() const {
    return _users.end();
}

template<typename T>
void GestioneUtenti<T>::addUtente()  const {
    _users.emplace_back();
    _it=_users.end();
    _it--;
}

template<typename T>
const string & GestioneUtenti<T>::information(int pos)  const{
    return (*_it).getInfo()[pos];
}

template<typename T>
int GestioneUtenti<T>::getNumberInfo() const {
    return (*_it).getInfo().size();       //Probabilmente da problemi per la & (toglila)
}

template<typename T>
bool GestioneUtenti<T>::findId(string &id)  const {
    auto tmp=_users.begin();
    while(tmp!=_users.end()){
        if((*tmp).getID()==id) return true;
        tmp++;
    }
    return false;
}

template<typename T>
const T* GestioneUtenti<T>::getById(string &id) const {
    //_lastPos=0;
    _it=_users.begin();
    while(_it!=_users.end()){
        if((*_it).getID()==id) return &(*_it);
        // _lastPos++;
        _it++;
    }
    return nullptr;
}

template<typename T>
const T &GestioneUtenti<T>::operator*() const {
    return  (*_it);
}

template<typename T>
const string &GestioneUtenti<T>::getId(int userPos) const{
    if(userPos==-1) return (*_it).getID();
    auto it=_users.begin();
    for(int i=0;i<userPos;i++) it++;
    return (*it).getID();
}

template<typename T>
int GestioneUtenti<T>::getNumUsers() const {
    return _users.size();
}

template<typename T>
template<typename M>       //Il controllo viene fatto sul primo utente quindi,tanto quasi tutte duplici ma se si vogliono inserire genitori, andranno passati per primi
bool GestioneUtenti<T>::manageRelation(const T *utnt1,const M *utnt2, bool (T::*fnToManage1)(const Utente *)const, bool (M::*fnToManage2)(const Utente *)const) const {
    if(!((*utnt1).*fnToManage1)(utnt2)) return false;                                     //Il cast da derivata a primitiva viene in automatico
    ((*utnt2).*fnToManage2)(utnt1);
    return true;
}

template<typename T>
template<typename M>
void GestioneUtenti<T>::manageRelation(const T *utnt1,const M *utnt2, void (T::*fnToManage1)(const Utente *)const, void (M::*fnToManage2)(const Utente *)const) const {
    ((*utnt1).*fnToManage1)(utnt2);
    ((*utnt2).*fnToManage2)(utnt1);
}

template<typename T>
bool GestioneUtenti<T>::addOpinionGU( const T *poster, const Semplice *follower, int numMex, bool like)  {    //Forse non ha senso fare la ricerca dopo la distinzione
                                                        //Like/Dislike perche tanto cerca sempre in entrambe le liste, VERIFICA
    const list<const Utente*>* tmp1= nullptr,*tmp2= nullptr;
        tmp1= &(*poster).getLikeListConst(numMex);        //cosi ogni volta non si cerca la lista tra i vari mex
        tmp2=   &(*poster).getDislikeListConst(numMex);

    if (find((*tmp1).begin(), (*tmp1).end(), follower) != (*tmp1).end() ||  //QUANDO HA SENSO STA find_first_of???????
        find((*tmp2).begin(), (*tmp2).end(), follower) != (*tmp2).end() ||
        find_first_of((*tmp1).begin(),(*tmp1).end(), (*tmp2).begin(), (*tmp2).end())!= (*tmp1).end() ){
        cerr<<"L'utente ha gia' espresso un'opinione."<<endl;
        return false;
    }
    if(like) {
        (*poster).addLike(follower,0,true);
        (*poster).setOpinion(*follower,true);
    }
    else {
        (*poster).addDislike(follower,0,true);
        (*poster).setOpinion(*follower,false);      //RIFARLA IN MODO MIGLIORE CON L'ITERATORE COSTANTE COME SOTTO magari come removeliker
    }
    return true;
}


template<typename T>        //Questa toglie i like degli altri al tuo mex
void GestioneUtenti<T>::removeOpinionGU(const T *poster, int numMex, int numOpinion, bool like) const {
    const list<const Utente*>* tmp= nullptr;
    //auto it2=(*poster).getLikeListConst(numMex,true);
    if(like)  tmp= &(*poster).getLikeListConst(numMex);        //cosi ogni volta non si cerca la lista tra i vari mex
    else  tmp = &(*poster).getDislikeListConst(numMex);
    if (numOpinion > (*tmp).size() || numOpinion < 0) {
        cerr << "Valore non valido" << endl;
        return;
    }
    list<const Utente*>::const_iterator it = (*tmp).begin();
    for (int i = 0; i < numOpinion; i++) it++;
    (*poster).removeOpinion(**it,like);
    if(like) (*poster).removeLiker(it);
    else (*poster).removeLiker(it,false);

}

template<typename T>
void GestioneUtenti<T>::removeMessage(const T *poster, int numMex) const {
    auto it=(*poster).getLikeListConst(numMex).begin();     //_mexIt punta al messaggio numMex
    //auto it2=(*poster).getLikeListConst(numMex,true);
    for(int i=0;i<(*poster).getLikeListConst(numMex).size();i++,it++)
        (*poster).removeOpinion(**it,true);      //Remove opinione non la vuole const
    //get likelist la ridà const
    it=(*poster).getDislikeListConst(numMex).begin();
    //it2=(*poster).getDislikeListConst(numMex,true);
    for(int i=0;i<(*poster).getDislikeListConst(numMex).size();i++,it++)
        (*poster).removeOpinion(**it,false);
    (*poster).removeMex(numMex);
    (*poster).sortMex();
}

template<typename T>
void GestioneUtenti<T>::removeUser() const {       //rimuoverlo vuol dire togliere tutti i riferimenti degli altri utenti a lui
    for(int i=0;i<(*_it).getNumMex();) removeMessage(&(*_it),0);
    _users.erase(_it);
    _it=_users.begin();
}


#endif //PROJECT_ANALYZER_HPP


