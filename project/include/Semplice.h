//
// Created by sento on 29/06/2018.
//

#ifndef PROJECT_SEMPLICE_H
#define PROJECT_SEMPLICE_H

#include "Utente.h"
#include <algorithm>
                                //A sostituire i metodi con mere get di tutto il vector o lista per reference cambiava poco
                                //perchè remove e get andavano comunque filtrate in quanto non potevo gestirle con metodi unici, essendo di liste e vector
class Semplice: public Utente {
public:
    bool addConoscenze(const Utente *utente) const;
    bool addAmici(const Utente *utente) const;
    bool addFigli(const Utente *utente) const;
    bool addConiuge(const Utente *utente) const;
    bool addGenitori(const Utente *utente) const;
    bool addAppartenenze(const Utente *gruppo) const;
    bool addLavori(const Utente *azienda) const;
    const Utente & getConoscente(int pos) const;
    const Utente & getGenitori(int pos) const;
    const Utente & getAmici(int pos) const;
    const Utente & getFiglio(int pos) const;
    const Utente & getLavoro(int pos) const;
    const Utente & getGruppo(int pos) const;
    const Utente & getConiuge()  const;
    virtual void addOpinion(list<const Utente *>* ptrToList) const;
    virtual void deleteOpinion(list<const Utente *>* da_rimuovere) const;
    int getOpinionSize() const;
    int getNumAmici() const;
    int getNumConoscenti() const;
    int getNumFigli() const;
    int getNumLavori() const;
    int getNumGruppi() const;
    int getNumGenitori() const;
    bool single() const;
    void removeAllOpinions() const;
    void removeGenitore(const Utente *darimuovere) const;
    void removeAmico(const Utente *darimuovere) const;
    void removeConiuge(const Utente *darimuovere) const;
    void removeConoscente(const Utente *darimuovere) const;
    void removeFiglio(const Utente *darimuovere) const;
    void removeLavoro(const Utente * darimuovere) const;
    void removeGruppo(const Utente * darimuovere) const;
    void setVisitato(const char& visit) const;
    void setStampato(const bool& visit) const;
    char getVisitato() const;
    bool getStampato() const;

private:
    mutable char _visitato='b';
    mutable bool _stampato=false;
    mutable list<const Utente*> _amicizie,_conoscenza;
    mutable list<list<const Utente *>*> _opinione;
    mutable vector<const Utente*> _figli;
    mutable const Utente* _coniuge= nullptr;
    mutable const Utente* _genitori[2]={nullptr, nullptr};
    mutable vector<const Utente*> _gruppi;
    mutable vector<const Utente*> _aziende;
};


#endif //PROJECT_SEMPLICE_H
