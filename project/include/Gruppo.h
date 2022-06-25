//
// Created by sento on 29/06/2018.
//

#ifndef PROJECT_GRUPPO_H
#define PROJECT_GRUPPO_H

#include "Utente.h"

class Gruppo: public Utente  {
public:
    bool addMembri(const Utente *membro) const;
    void removeMembro(const Utente* darimuovere) const;
    int getNumMembri() const;
    const Utente & getMembri(int pos) const;
private:
    virtual void addOpinion(list<const Utente *>* ptrToList) const;
    virtual void deleteOpinion(list<const Utente *>* da_rimuovere) const;
    mutable list<const Utente*> _membri;
};


#endif //PROJECT_GRUPPO_H
