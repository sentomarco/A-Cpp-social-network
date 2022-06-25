//
// Created by sento on 29/06/2018.
//

#ifndef PROJECT_AZIENDA_H
#define PROJECT_AZIENDA_H

#include "Utente.h"

class Azienda: public Utente  {
public:
    bool addConsociate(const Utente *conso) const;
    bool addDipendenti(const Utente *dipe) const;
    const Utente & getDipendente(int pos) const;
    const Utente & getConsociata(int pos) const;
    int getNumDipendenti() const;
    int getNumConsociate() const;
    void removeConsociata(const Utente *darimuovere) const;
    void removeDipendente(const Utente *darimuovere) const;
    //list<Utente*> &getDipeConsociata(int pos);
private:
    virtual void addOpinion(list<const Utente *>* ptrToList) const;
    virtual void deleteOpinion(list<const Utente *>* da_rimuovere) const;
    mutable vector<const Utente*> _consociate;
    mutable list<const Utente*> _dipendenti;
};


#endif //PROJECT_AZIENDA_H
