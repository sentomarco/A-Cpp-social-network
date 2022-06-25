//
// Created by sento on 10/07/2018.
//

#include "Utente.h"


bool Utente::setId(string str) const {
    _id=str;
    return true;
}

const string & Utente::getID() const {
    return _id;
}

void Utente::newInfo(const string& str,bool back) const {
    if(back) _informazioni.emplace_back(str);
    else _informazioni.emplace(_informazioni.begin(),str);
}

void Utente::addDislike(const Utente *utnt, int pos, bool theLastOneUsed)const {
    if(theLastOneUsed) {
        (*_mexIt).dislike.emplace_front(utnt);
        return;
    }
    if(pos>_messaggi.size()) return;
    if(_messaggi.size()/2>=pos) {
        _mexIt=_messaggi.begin();
        for (int i = 0; i < pos; i++) _mexIt++;
    }
    else if(_messaggi.size()>=pos) {
        _mexIt=_messaggi.end()--;
        for (int i = _messaggi.size(); i > pos; i--) _mexIt--;
        }
    (*_mexIt).dislike.emplace_front(utnt);
}

void Utente::addLike(const Utente *utnt, int pos, bool theLastOneUsed )const {
    if(theLastOneUsed) {
        (*_mexIt).like.emplace_front(utnt);
        return;
    }
    if(pos>_messaggi.size()) return;
    if(_messaggi.size()/2>=pos) {
        _mexIt=_messaggi.begin();
        for (int i = 0; i < pos; i++) _mexIt++;
    }
    else if(_messaggi.size()>=pos) {
        _mexIt=_messaggi.end()--;
        for (int i = _messaggi.size(); i > pos; i--) _mexIt--;
    }
    (*_mexIt).like.emplace_front(utnt);
}

void Utente::addMex(const string &str) const {
    _messaggi.emplace_front();
    _messaggi.front().messaggio=str;
    _mexIt=_messaggi.begin();
}

const list<const Utente *> &Utente::getLikeListConst(int pos, bool theLastOneUsed) const {  //Occhio che sia pos<=_messaggi.size()
    if(theLastOneUsed) return (*_mexIt).like;
    if(_messaggi.size()/2>=pos) {
        _mexIt=_messaggi.begin();
        for (int i = 0; i < pos; i++) _mexIt++;
    }
    else if(_messaggi.size()>=pos) {
        _mexIt=_messaggi.end()--;
        for (int i = _messaggi.size(); i > pos; i--) _mexIt--;
    }return (*_mexIt).like;
}

const list<const Utente *> &Utente::getDislikeListConst(int pos, bool theLastOneUsed) const {
    if(theLastOneUsed) return (*_mexIt).dislike;
    if(_messaggi.size()/2>=pos) {
        _mexIt=_messaggi.begin();
        for (int i = 0; i < pos; i++) _mexIt++;
    }
    else if(_messaggi.size()>=pos) {
        _mexIt=_messaggi.end()--;
        for (int i = _messaggi.size(); i > pos; i--) _mexIt--;
    }return (*_mexIt).dislike;
}


void Utente::setInfo(int pos,const string &str) const {
    _informazioni[pos]=str;
}

const vector<string> &Utente::getInfo() const {
    return _informazioni;
}

int Utente::getNumMex() const{
    return _messaggi.size();
}

const string & Utente::getMex(int pos)const {
    if(_messaggi.size()/2>=pos) {
        _mexIt=_messaggi.begin();
        for (int i = 0; i < pos; i++) _mexIt++;
    }
    else if(_messaggi.size()>=pos) {
        _mexIt=_messaggi.end()--;
        for (int i = _messaggi.size(); i > pos; i--) _mexIt--;
    }return (*_mexIt).messaggio;
}

void Utente::removeMex(int pos) const {
    if(_messaggi.size()/2>=pos) {
        _mexIt=_messaggi.begin();
        for (int i = 0; i < pos; i++) _mexIt++;
    }
    else if(_messaggi.size()>=pos) {
        _mexIt=_messaggi.end()--;
        for (int i = _messaggi.size(); i > pos; i--) _mexIt--;
    }_messaggi.erase(_mexIt);
    _mexIt=_messaggi.begin();
}

void Utente::setData(const Date &data, int pos) const {
    if(pos==0){
        _messaggi.front().dataPubblicazione=data;
        return;
    }
    if(_messaggi.size()/2>=pos) {
        _mexIt=_messaggi.begin();
        for (int i = 0; i < pos; i++) _mexIt++;
    }
    else if(_messaggi.size()>=pos) {
        _mexIt=_messaggi.end()--;
        for (int i = _messaggi.size(); i > pos; i--) _mexIt--;
    } (*_mexIt).dataPubblicazione=data;
}

void Utente::sortMex() const {
    _messaggi.sort([&](mex m1,mex m2){
        return !((m1).dataPubblicazione<(m2).dataPubblicazione);
    });
}

string Utente::getData(int pos)const  {
    if(_messaggi.size()/2>=pos) {
        _mexIt=_messaggi.begin();
        for (int i = 0; i < pos; i++) _mexIt++;
    }
    else if(_messaggi.size()>=pos) {
        _mexIt=_messaggi.end()--;
        for (int i = _messaggi.size(); i > pos; i--) _mexIt--;
    }return (*_mexIt).dataPubblicazione.str();
}

void Utente::deleteInfo( vector<string>::const_iterator it1,  vector<string>::const_iterator it2) const {
    _informazioni.erase(it1,it2);       //NB ERASE ELIMINA TUTTO CIò CHE è TRA FIRST E LAST, LAST EsCLUsO!!!!!!!!!!!!!!
}

void Utente::clearInfo(short pos) const {
    _informazioni.resize(pos);
}

void Utente::setMex(int pos, const string &mex, bool theLastOneUsed) const {
    if(theLastOneUsed) {
        _mexIt->messaggio=mex;
        return ;
    }
    if(_messaggi.size()/2>=pos) {
        _mexIt=_messaggi.begin();
        for (int i = 0; i < pos; i++) _mexIt++;
    }
    else if(_messaggi.size()>=pos) {
        _mexIt=_messaggi.end()--;
        for (int i = _messaggi.size(); i > pos; i--) _mexIt--;
    }_mexIt->messaggio=mex;
}

void Utente::removeLiker(list<const Utente*>::const_iterator it, bool like) const {
    if(like) (*_mexIt).like.erase(it);
    else (*_mexIt).dislike.erase(it);
}

void Utente::setOpinion(const Utente &follower, bool like) const {
    if(like) follower.addOpinion(&(*_mexIt).like);
    else  follower.addOpinion(&(*_mexIt).dislike);
}

void Utente::removeOpinion(const Utente &follower, bool like) const {
    if(like) follower.deleteOpinion(&(*_mexIt).like);
    else  follower.deleteOpinion(&(*_mexIt).dislike);
}
