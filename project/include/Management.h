//
// Created by sento on 29/06/2018.
//

#ifndef PROJECT_MANAGEMENT_H
#define PROJECT_MANAGEMENT_H

#include "Logging.h"
#include <fstream>
#include "Date.h"
#include <cctype>
#include <cstdio>
#include <utility>      //per la pair

using namespace std;

class Management {      //Metodi altamente specifici
public:
    Management()= default;
    bool controls(const string &a,const string &b,const string &c); //Qui mi fa tutte le verifiche:TUTTE
    void execution();
private:
    bool verifyChoose()const;
    void exit();
    bool manageUser();
    bool getAllInfo();
    bool manageRelations();
    bool verifyInfo(const string &dato)const;
    bool genealogicalTree(const Semplice* utnu_partena);
    bool findParents(const Semplice *partenza, vector<const Semplice *> &fifo, int &daesaminare);
    bool findSons(const Semplice *partenza, vector<const Semplice *> &fifo, int &daesaminare);
    bool reserch();
    bool statistics();
    int stop(int iterazione, int numMax, string oggetti="utenti");
    void removeSemplice(const Semplice& darimuovere,bool remove_all=true) const;
    void removeAzienda(const Azienda & darimuovere)const;
    void removeGruppo(const Gruppo & darimuovere)const;
    int parentTree( list<const Semplice*>::iterator &partenza, list<list<const Semplice*>>::iterator &lista_attuale,
                     list<list<const Semplice*>>::iterator &daesaminare,int maxUser=0);
    int sonsTree( list<const Semplice*>::iterator &partenza, list<list<const Semplice*>>::iterator &lista_attuale,
                   list<list<const Semplice*>>::iterator &daesaminare,int maxUser=0);
    void printTree(const Semplice* utnu_partena);
    template<typename T>
    bool removeInfo(const GestioneUtenti<T> &gestUtnt,int pos1, int pos2 = -1)const;
    template<typename T>
    bool getAllInfoPlus(const GestioneUtenti<T> &getUtnt,const Utente &(T::*fnGetUtnt1)(int pos)const,
                        int(T::*fnGetNum1)()const, string relazione1, string relazione2= "none",
                                const Utente &(T::*fnGetUtnt2)(int pos)const= nullptr, int(T::*fnGetNum2)()const= nullptr);
    template<typename T>
    void refresh(const GestioneUtenti<T> &gestUtnt,string &userFile,string &newsFile,string tipo_utente);
    template <typename T>
    void countOpinions(const GestioneUtenti<T> &gestUtnt, vector<pair<string, string>>& result,
                      const list<const Utente *> &(Utente::*fnGetList)(int posMex, bool likeOrDis)const , int& biggerNum)const;
    template<typename T>
    void countProperties(const GestioneUtenti<T> &gestUtnt, string opzione, int(T::*fnGetNum)() const) const;
    template <typename T,typename M>
    bool iniRelations(int choose, string relazione,const GestioneUtenti<T> &utnt,const M *caller, int(M::*fnNumUser)() const,
                      const Utente &(M::*fnGetId)(int pos) const);
    template <typename T>
    bool addUser(const GestioneUtenti<T> &gestUtnt);
    template <typename T>
    void usersList(const GestioneUtenti<T> &gestUtnt, string tipo)const;
    template <typename T>
    void addInfo(const GestioneUtenti<T> & gestUtnt);
    template <typename T>
    bool modifyUser(const GestioneUtenti<T> &gestUtnt, short choose);       //con getline non può essere const
    template <typename T>
    bool manageMessages(const GestioneUtenti<T> & gestUtnt,const T* utnt,string &dato, short choose);
    void countRelations(short tipoUtnt);
    int _incremento=3,_i=0,_choose=0;
    list<list<const Semplice*>> _graph;
    ofstream _oStream;
    string _dato;
    bool _modifyUser=false,_modifyNews=false,_modifyRel=false;
    char _yorn='y';
    Logging log;
    Date _ctrldate;
    const Semplice *_semplice= nullptr;
    const Azienda *_azienda= nullptr;
    const Gruppo *_gruppo= nullptr;
    list<string> _sheet;
};


template<typename T>
bool Management::addUser(const GestioneUtenti<T> &gestUtnt) {
    gestUtnt.addUtente();
    do{
        cout << "\nInserire id utente:\nENTER per annullare."<<endl;
        getline(cin,_dato);
        if(_dato.empty()){
            gestUtnt.removeUser();
            return false;
        }
        if(_dato.find_first_not_of(" \t\v\n\r")==-1){
            cerr<<"ID non valido.";
            _dato="}}";
        }
        if(_dato=="}}" || log.getSempl().findId(_dato) || log.getGr().findId(_dato) || log.getAz().findId(_dato)){
            cerr<<"L'id e' gia' in uso, sceglierne un altro"<<endl;
            _dato="}}";
        }
    } while(_dato=="}}" || !gestUtnt.getLastUtente().setId(_dato) );
    do{
        cout<<"\nInserire nome utente:\nENTER per annullare."<<endl;
        getline(cin,_dato);
        if(_dato.empty()) {
            gestUtnt.removeUser();
            return false;
        }
    }while(!verifyInfo(_dato));
    (*log.getSempl()).newInfo(_dato,false);
    (*log.getSempl()).newInfo(log.keyword(8),false);
    return true;
}

template<typename T>
void Management::addInfo(const GestioneUtenti<T> &gestUtnt) {
    cout<<"\nIt is possible to insert additional fields in the form: ’information' ’value'\n"
          "Press ENTER if there is no further information.\nInsert information:" << endl;
    getline(cin,_dato);
    while(!_dato.empty()){
        if(verifyInfo(_dato)) {
            (*gestUtnt).newInfo(_dato);
            cout << "Inserire il valore:" << endl;
            getline(cin, _dato);        //Se si provano ad inserire anomalie nel sistema non si ha l'inserimento dell'informazione
            if(!verifyInfo(_dato)) removeInfo(gestUtnt,gestUtnt.getNumberInfo() - 1);
            else (*gestUtnt).newInfo(_dato);
        }
        cout << "Inserire l'informazione:" << endl;
        getline(cin, _dato);
    }
}

template<typename T>
bool Management::modifyUser(const GestioneUtenti<T> &gestUtnt, short choose) {      //ritorna falso se è terminato per via di una eliminazione o comando errato
    short k=2, campo=1;
    switch (choose) {
        while (campo != 0) {
            k=2;
            case 2:
            cout << "\nE' possibile scegliere uno dei seguenti campi da modificare o rimuovere :\n1) id: " << gestUtnt.getId()
                 << "\t";
            for (int i = 0; i < gestUtnt.getNumberInfo(); i++) cout << k++ << ") " << gestUtnt.information(i) << ": " << gestUtnt.information(++i) << "\t";
            cout << endl;
            cout << "Seguito da una delle seguenti opzioni:\n1) Modifica valore\n2) Elimina informazione\n"
                       "[n1] [n2] - [Inserire 0 per terminare]" << endl;
            cin>>campo;
            cin>>choose;
            verifyChoose();
            switch (campo) {
                case 0:
                    break;
                case 1:
                    if(choose==2){
                        cout<<"Non e' possibile eliminare l'id utente."<<endl;
                        break;
                    }
                    do {
                        cout << "Inserire id utente:" << endl;
                        getline(cin, _dato);
                        if (log.getSempl().findId(_dato) || log.getGr().findId(_dato) || log.getAz().findId(_dato)) {
                            cerr << "L'id e' gia' in uso, sceglierne un altro." << endl;
                            _dato = "}}";
                        }
                    } while (_dato == "}}" || !verifyInfo(_dato));
                    (*gestUtnt).setId(_dato);
                    break;
                default:    //Se lo si elimina cambierà la numerazione dei campi
                    campo = campo + (campo - (short)4);
                    if (choose==2 && campo<= gestUtnt.getNumberInfo()) {
                        if (removeInfo(gestUtnt,campo, campo + 2))      //erase elimina da first a last, last escluso
                            cout << "Eliminato." << endl;       //elimina entrambi "informazione" e "valore"
                        return false;
                    } else if (choose!=1 || campo> gestUtnt.getNumberInfo()) return false;
                    cout << "Inserire il nuovo valore.\n" << gestUtnt.information(campo) << " :" << endl;
                    getline(cin, _dato);
                    if(verifyInfo(_dato)) (*gestUtnt).setInfo(campo + 1, _dato);   //Qua se non è valido, alla peggio, non sovvrascrive il valore
                    campo = -1;
                    break;
            }
        }
        break;
    case 3:
        if((*gestUtnt).getInfo()[0]==log.keyword(8)) (*gestUtnt).clearInfo(2);
        else if((*gestUtnt).getInfo()[4]==log.keyword(8)) (*gestUtnt).clearInfo(6);
        cout<<"\nL'utente ora possiede unicamente le seguenti informazioni:\n\t";
        for (int i = 0; i < gestUtnt.getNumberInfo(); i++) cout << k++ << ") " << gestUtnt.information(i) << ": " << gestUtnt.information(++i) << "\t";
        cout << endl;
        break;
    case 1:
        addInfo(gestUtnt);
        break;
    default:
        cerr << "Non sono previste altre opzioni";
        break;
    }
    return true;
}

template<typename T>
void Management::usersList(const GestioneUtenti<T> &gestUtnt, string tipo)const {
    int pos=0;
    if(gestUtnt.getNumUsers()==0){
        cout<<"Non sono presenti utenti "<<tipo<<"."<<endl;
        return;
    }
    cout<<"Sono presenti i seguenti utenti "<<tipo<<":"<<endl;
    for(;pos<gestUtnt.getNumUsers();pos++) cout<<gestUtnt.getId(pos)<<"\t";
    cout<<endl;
}


template <typename T,typename M>
bool Management::iniRelations(int choose, string relazione,const  GestioneUtenti<T> &utnt,const M *caller, int(M::*fnNumUser)() const,
                              const Utente &(M::*fnGetId)(int pos) const){
        if(choose==1){
            cout<<"\nInserire l'id dell'utente "<< relazione<<" da aggiungere:\nENTER per annullare."<<endl;
            getline(cin,_dato);
            if(_dato.empty()) return false;
            if(utnt.getById(_dato)== nullptr) {
                cerr<<"Utente non trovato"<<endl;
                return false;
            }
        }else if(choose==2) {
            if (((*caller).*fnNumUser)() == 0) {
                cout << "Non vi sono utenti da eliminare" << endl;
                return false;
            }
            cout << "\nE' possibile eliminare un utente " << relazione << " tra i seguenti: " << endl;
            for (_i = 0; _i < ((*caller).*fnNumUser)(); _i++) {
                cout << _i + 1 << ") " << (*(caller).*fnGetId)(_i).getID() << "\t";
                if (_i + 1 % 5 == 0) cout << endl;
            }
            cout << endl;
            cout << "\nScegliere il numero dell'id da rimuovere:\nENTER per annullare."<<endl;
            getline(cin,_dato);
            if(_dato.empty()) return false;
            if (atoi(_dato.c_str()) > ((*caller).*fnNumUser)() || atoi(_dato.c_str())< 1) {        //sostituibile con funzioncina
                cerr << "Attenzione: il valore inserito non e' valido" << endl;
                return false;
            }
            cout<< "Utente rimosso."<<endl;
            _modifyRel=true;
        }
    return true;
}

template<typename T>
bool Management::manageMessages(const GestioneUtenti<T> & gestUtnt,const T* utnt,string &dato, short choose) {
    string testo;
    int i=0,numMex=0;
    list<const Utente *>::const_iterator itlike,itdisl;
    switch (choose) {
        case 0:
            break;
        case 1:
            cout<<"\nInserire il testo del messaggio:\nENTER per annullare."<<endl;
            getline(cin,testo);
            if(testo.empty()) return true;
            if(testo.find_first_not_of(" \t\v\n\r")==-1){
                cerr<<"Il testo non puo' essere vuoto.";
                return false;
            }
            _modifyNews= true;
            utnt->addMex(testo);
            cout<<"\nIt is possible to add reactions to the message."
                  "\nChoose an option from the following:"
                  "\n1) To aggingere like\n2) To aggingere dislike\n"
                  "Followed by the id of the user expressing their opinion:\n[op] [id] – [insert 0 to cancel]\n";
            cin >> choose;
            if(choose==0) {
                verifyChoose();
                return true;
            }
            cin.ignore();
            getline(cin,_dato);
            verifyChoose();
            cout<< endl;
            if(log.getSempl().getById(dato) != nullptr) {
                switch (choose) {
                    case 1:
                        gestUtnt.addOpinionGU(utnt, &(*log.getSempl()), 0);
                        break;
                    case 2:
                        gestUtnt.addOpinionGU(utnt, &(*log.getSempl()), 0, false);
                        break;
                    default:
                        cerr << "Non sono previsiti ulteriori comandi." << endl;
                        break;      //Non va bene return false qua o non aggiungeresti più al messaggio attuale e saresti costretto a metterne uno nuovo
                }
            } else cerr<<"L'id inserito non e' valido."<<endl;
            return true;
        case 2:
            cout << "\nL'utente ha pubblicato i seguenti messaggi:" << endl;
            for (i = 0; i < utnt->getNumMex(); i++) cout << i + 1 << ") " << utnt->getMex(i) << endl;
            if(i==0) cout<<"Non vi sono post"<<endl;
            cout << "Scegliere il numero dell'messaggio da gestire, premere 0 per tornare alla selezione precedente:" << endl;
            cin >> i;
            verifyChoose();
            if(i==0) return true;
            if (i > utnt->getNumMex() || i < 1) {       //sostituibile con funzioncina
                cerr << "Attenzione: il valore inserito non e' valido" << endl;
                return false;
            }
            _modifyNews= true;
            gestUtnt.removeMessage(utnt,i-1);
            cout << "Messaggio rimosso." << endl;
            return true;
        case 3:
            cout << "\nL'utente ha pubblicato i seguenti messaggi:" << endl;
            for (i = 0; i < utnt->getNumMex(); i++) cout << i + 1 << ") " << utnt->getMex(i) <<endl;
            if(i==0) {
                cout<<"Non vi sono post"<<endl;
                return true;
            }
            else {
                cout << "\nChoose one of the following options:\n\t"
                        "1) To amend the text.\t2) To add like.\t"
                        "3) To add dislike.\n\t4) To remove like.\t"
                        "5) To remove dislike.\t6) To change the publication date.\n"
                        "Followed by the message number to handle:\n\t[n1] [n2] – [Insert 0 to end]"<<endl;
                cin >> choose;
                if(choose==0) {
                    verifyChoose();
                    return true;
                }
                cin >> i;
                verifyChoose();
                cout << endl;
                if (i > utnt->getNumMex() || (i < 1 && choose!=0)) {        //sostituibile con funzioncina
                    cerr << "\nAttenzione: il valore inserito non e' valido" << endl;
                    return true;
                }
                switch (choose) {
                    case 1:
                        cout << "L'attuale messaggio e':\n" << utnt->getMex(i - 1)
                             << "\nDigitare il nuovo testo:\nENTER per annullare." << endl;
                        getline(cin,_dato);
                        if(_dato.empty()) return false;
                        if(_dato.find_first_not_of(" \t\v\n\r")==-1){
                            cerr<<"Il testo non puo' essere vuoto.";
                            return false;
                        }
                        _modifyNews= true;
                        (*utnt).setMex(i - 1,_dato);
                        break;   //é andato a buon fine ma nel caso volesse compiere ulteriori modifiche sullo stesso..... magari con una ricorsione
                    case 2:
                        cout << "\nInserire l'id del utente che esprime gradimento:\nENTER per annullare." << endl;
                        getline(cin,_dato);
                        if(_dato.empty()) return false;
                        _modifyNews= true;
                        if (log.getSempl().getById(_dato) != nullptr)
                            gestUtnt.addOpinionGU(utnt, &(*log.getSempl()), i - 1);
                        else cerr << "L'id inserito non e' valido" << endl;
                        break;
                    case 3:
                        cout << "\nInserire l'id del utente che esprime disappunto:\nENTER per annullare." << endl;
                        getline(cin,_dato);
                        if(_dato.empty()) return false;
                        _modifyNews= true;
                        if (log.getSempl().getById(_dato) != nullptr)
                            gestUtnt.addOpinionGU(utnt, &(*log.getSempl()), i - 1, false);
                        else cerr << "L'id inserito non e' valido" << endl;
                        break;
                    case 4:
                        itlike = utnt->getLikeListConst(i - 1).begin();
                        cout << "\nIl messaggio piace a:" << endl;
                        if (utnt->getLikeListConst(i - 1).empty()) cout << "Nessuno" << endl;
                        else
                            for (int a = 0; a < utnt->getLikeListConst(i - 1).size(); a++, itlike++) {
                                cout << a + 1 << ") " << (*itlike)->getID() << "\t";
                                if (a + 1 % 5 == 0) cout << endl;
                            }
                        cout << endl;
                        cout << "Selezionare il numero del follower da rimuovere:\nENTER per annullare." << endl;
                        getline(cin,_dato);
                        if(_dato.empty()) return false;
                        _modifyNews= true;
            //funzioncina
                        gestUtnt.removeOpinionGU(utnt, i - 1, atoi(_dato.c_str()) - 1, true);
                        break;
                    case 5:
                        itdisl = utnt->getDislikeListConst(i - 1).begin();
                        cout << "\nIl messaggio non piace a:" << endl;
                        for (int a = 0; a < utnt->getDislikeListConst(i - 1).size(); a++, itdisl++) {
                            cout << a + 1 << ") " << (*itdisl)->getID() << "\t";
                            if (a + 1 % 5 == 0) cout << endl;
                        }
                        cout << endl;
                        cout << "\nSelezionare il numero del follower da rimuovere:\nENTER per annullare." << endl;
                        getline(cin,_dato);
                        if(_dato.empty()) return false;
                        _modifyNews= true;
            //funzioncina
                        gestUtnt.removeOpinionGU(utnt, i - 1, atoi(_dato.c_str()) - 1, false);
                        break;
                    case 6: {
                        Date ctrldata;
                        cout << "L'attuale data e':\n" << utnt->getData(i - 1) << "\nInserire la nuova data:\nENTER per annullare." << endl;
                        getline(cin, _dato);
                        if(_dato.empty()) return false;
                        _modifyNews= true;
                        if(ctrldata.isDate(_dato)) utnt->setData(ctrldata,i-1);
                        utnt->sortMex();
                        break;
                    }
                    default:
                        cerr << "\nNon sono previsiti ulteriori comandi" << endl;
                        return false;
                }
            }
            return true;
        case 4:
            for (i = 0; i < utnt->getNumMex(); i++) {
                int a = 0;
                cout <<"\n" <<i + 1 << ") " << utnt->getMex(i) << endl;
                if(utnt->getLikeListConst(i).empty()) cout<<"Non ci sono like."<<endl;
                else {
                    cout << "Piace a " << utnt->getLikeListConst(i).size() << " utenti, tra cui: ";
                    itlike = utnt->getLikeListConst(i).begin();
                    for (a = 0; a < utnt->getLikeListConst(i).size()-1 && a < 10; a++, itlike++)
                        cout<< (*itlike)->getID() << ",\t";
                    cout <<(*itlike)->getID()<< endl;
                }
                if(utnt->getDislikeListConst(i).empty()) cout<<"Non ci sono dislike."<<endl;
                else {
                    cout << "Non piace a " << utnt->getDislikeListConst(i).size() << " utenti, tra cui: ";
                    itdisl = utnt->getDislikeListConst(i).begin();
                    for (a = 0; a < utnt->getDislikeListConst(i).size()-1 && a < 5; a++, itdisl++)
                        cout << (*itdisl)->getID() << ",\t";
                    cout <<(*itdisl)->getID() << endl;
                }
                if((i=stop(i,utnt->getNumMex(),"messaggi"))==-1) return false;
            }
            _incremento=3;
            do {
                cout << "\nPremere 0 per tornare alla selezione precedente altrimenti inserire il numero di un messaggio "
                        "per visualizzare ulteriori like e dislike." << endl;
                cin >> numMex;
                verifyChoose();
                if (numMex == 0) return true;
                cout << "Like:\t\tDislike:" << endl;
                itdisl = utnt->getDislikeListConst(numMex - 1).begin();
                itlike = utnt->getLikeListConst(numMex - 1).begin();
                if (utnt->getDislikeListConst(numMex - 1).size() <=
                    utnt->getLikeListConst(numMex - 1).size()) {      //se like>dislike
                    for (i = 0; i < utnt->getDislikeListConst(numMex - 1).size(); i++, itlike++, itdisl++)
                        cout << (*itlike)->getID() << "\t\t" << (*itdisl)->getID() << endl;
                    for (; i < utnt->getLikeListConst(numMex - 1).size(); i++, itlike++) cout << (*itlike)->getID() << endl;
                } else {
                    for (i = 0; i < utnt->getLikeListConst(numMex - 1).size(); i++, itlike++, itdisl++)
                        cout << (*itlike)->getID() << "\t\t" << (*itdisl)->getID() << endl;
                    for (; i < utnt->getDislikeListConst(numMex - 1).size(); i++, itdisl++)
                        cout << "\t\t" << (*itdisl)->getID() << endl;
                }
            }while(numMex!=0);
          return true;
        default:
            cerr << "\nNon sono previsiti ulteriori comandi" << endl;
            return true;
    }
    return true;
}

template<typename T>
void Management::countOpinions(const GestioneUtenti<T> &gestUtnt, vector<pair<string, string>>& result,
                               const list<const Utente *> &(Utente::*fnGetList)(int posMex, bool likeOrDis) const, int& biggerNum)const {
    int i = 0 ;
    auto it = gestUtnt.iteratorBegin();
    for (i = 0; i < gestUtnt.getNumUsers(); i++, it++) {
        for (int k = 0; k < (*it).getNumMex(); k++) {
            if (((*it).*fnGetList)(k, false).size() > biggerNum) {
                biggerNum = ((*it).*fnGetList)(k,false).size();
                result.clear();
                result.emplace_back((*it).getID(),(*it).getMex(k));
            } else if (((*it).*fnGetList)(k,false).size() == biggerNum) result.emplace_back((*it).getID(),(*it).getMex(k));
        }
    }
}

template<typename T>
void Management::countProperties(const GestioneUtenti<T> &gestUtnt, string opzione, int (T::*fnGetNum)() const) const {
    int i=0, biggerNum=0;
    auto it=gestUtnt.iteratorBegin();
    vector<string> result;
    for(i=0;i<gestUtnt.getNumUsers();i++,it++) if (((*it).*fnGetNum)() > biggerNum) {
            biggerNum = ((*it).*fnGetNum)();
            result.clear();
            result.emplace_back((*it).getID());
        } else if (((*it).*fnGetNum)() == biggerNum) result.emplace_back((*it).getID());
    if(biggerNum==0) {
        cout << "Non vi sono utenti con " << opzione << endl;
        return;
    }
    else if (result.size() == 1) cout << "L'utente con il maggior numero di "<< opzione <<" (" << biggerNum << ") e':" << endl;
    else cout << "Gli utenti con il maggior numero di "<< opzione <<" (" << biggerNum << ") sono:" << endl;
    for (i = 0; i < result.size(); i++) cout<<i+1<<") id: "<< result[i] << endl;
}

template<typename T>
void Management::refresh(const GestioneUtenti<T> &gestUtnt,string &userFile,string &newsFile,string tipo_utente) {
    int k=0;
    list<const Utente*>::const_iterator it2;
    auto it=gestUtnt.iteratorBegin();
    if(gestUtnt.getNumUsers()!=0) {         //Altrimenti apre e chiude e cancella tutto
       if(_modifyUser) {
           _oStream.open(userFile,ios::app);
           if(!_oStream.is_open()){
               cerr<<"\nNon e' stato possibile aggiornare il file "<<userFile<<endl;
               return;
           }
           for (; it != gestUtnt.iteratorEnd(); it++) {
               _oStream << (*it).getID() << ","<<tipo_utente<<",{";
               for (k = 0; (*it).getInfo().size()>2 && k < (*it).getInfo().size() - 3; k++) {
                   _oStream << (*it).getInfo()[k] << ":" << (*it).getInfo()[++k] << ",";
               }
               _oStream << (*it).getInfo()[k] << ":" << (*it).getInfo()[++k];
               _oStream << "}"<<endl;
           }
           _oStream.close();
       }if(_modifyNews) {
            _oStream.open(newsFile,ios::app);
            if(!_oStream.is_open()){
                cerr<<"\nNon e' stato possibile aggiornare il file "<<newsFile<<endl;
                return;
            }
            for (it = gestUtnt.iteratorBegin(); it != gestUtnt.iteratorEnd(); it++) {
                for (k = 0; k < (*it).getNumMex(); k++) {
                    _oStream << (*it).getID() << "," << (*it).getMex(k) << "," << (*it).getData(k) << ",like:{";
                    for (it2 = (*it).getLikeListConst(k).begin();!(*it).getLikeListConst(k).empty() && it2 != --(*it).getLikeListConst(k).end(); it2++)
                        _oStream << (*it2)->getID() << ",";
                    if(!(*it).getLikeListConst(k).empty()) _oStream << (*it2)->getID();
                    _oStream<< "},dislike:{";
                    for (it2 = (*it).getDislikeListConst(k).begin();!(*it).getDislikeListConst(k).empty() && it2 != --(*it).getDislikeListConst(k).end(); it2++)
                        _oStream << (*it2)->getID() << ",";
                    if(!(*it).getDislikeListConst(k).empty()) _oStream << (*it2)->getID();
                    _oStream<< "}" << endl;
                }
            }
            _oStream.close();
        }
    }
}

template<typename T>
bool Management::removeInfo(const GestioneUtenti<T> &gestUtnt, int pos1, int pos2)const {
    if(pos2==-1) pos2=pos1+1;    //+1 fa si che venga eliminato il solo elemento pos1 (fosse pos2=pos1 non ne eliminerebbe)
    if((*gestUtnt).getInfo()[pos1]==log.keyword(8) || (*gestUtnt).getInfo()[pos1]==log.keyword(9)
       || (*gestUtnt).getInfo()[pos1]==log.keyword(10)){
        cerr<<"\nNon e' possibile rimuovere l'informazione scelta"<<endl;
        return false;
    }
    auto it= (*gestUtnt).getInfo().begin();
    (*gestUtnt).deleteInfo(it+pos1,it+pos2);
    return true;
}

template<typename T>        //RITORNA FALSO SE HA RICEVUTO INPUT ERRATI
bool Management::getAllInfoPlus(const GestioneUtenti<T> &getUtnt,const Utente &(T::*fnGetUtnt1)(int pos) const,
                  int(T::*fnGetNum1)() const, string relazione1, string relazione2,
                  const Utente &(T::*fnGetUtnt2)(int pos) const, int(T::*fnGetNum2)() const) {
    int choose=0;
    const Utente* utntptr;
    int last=1,k=0;
    _i=0;        //last serve per ricordarsi la scelta precedente
    cout << "\nSelezionare il numero di un utente seguito da:\n1) visualizza ulteriori informazioni\n"
            "2) visualizza "<< relazione1;
    if(relazione2!= "none")cout<<"\n3) visualizza "<< relazione2;
    cout<<"\n[n1] [n2] - [Inserire 0 per annullare]" << endl;
    cin >> k;
    if(k>getUtnt.getNumUsers()){
        cerr<<"L'utente selezionato non e' valido."<<endl;
        verifyChoose();
        return false;
    }
    if (k == 0)
        return verifyChoose();
    cin >> choose;   //choose ora è l'opzione 1 o 2 o 3 mentre k è l'utente o 0 nel caso
    verifyChoose();
    auto it = getUtnt.iteratorBegin();
    if(last!=k) for (last=1; last < k; last++, it++);
    switch (choose) {                              //Nell'if cosi se mi richiede lo stesso non itera
        case 0:
            break;
        case 1:
            cout<<"\nL'utente possiede le seguenti informazioni:"<<endl;
            for (_i = 0; _i < (*it).getInfo().size(); _i++) cout <<(*it).getInfo()[_i] << ": " <<(*it).getInfo()[++_i] << ", ";
            cout<<endl;
            break;
        case 2:
            if (((*it).*fnGetNum1)() != 0) cout << "L'utente ha i seguenti "<< relazione1<<":" << endl;
            else cout << " Non sono presenti "<< relazione1<<"." << endl;
            for (_i = 0; _i < ((*it).*fnGetNum1)(); _i++) {
                utntptr= &((*it).*fnGetUtnt1)(_i);
                cout << (*utntptr).getInfo()[5] << " " << (*utntptr).getInfo()[3] << " [id "
                     << (*utntptr).getID() << "]." << endl;
                if((_i=stop(_i,((*it).*fnGetNum1)()))==-1) return false;
            }
            break;
        case 3:
            if(relazione2!= "none") {
                if (((*it).*fnGetNum2)()  != 0) cout << "L'utente ha i seguenti "<< relazione2<<":" << endl;
                else cout << " Non sono presenti "<< relazione2<<"." << endl;
                for (_i = 0; _i < ((*it).*fnGetNum2)(); _i++) {
                    utntptr = &((*it).*fnGetUtnt2)(_i);
                    cout << (*utntptr).getInfo()[5] << " " << (*utntptr).getInfo()[3] << " [id "
                         << (*utntptr).getID() << "]." << endl;
                    if ((_i = stop(_i, ((*it).*fnGetNum2)())) == -1) return false;
                }
            }else cerr << "Non sono previste altre scelte." << endl;
            break;
        default:
            cerr << "Non sono previste altre scelte." << endl;
            break;
    }
    _incremento = 3;
    return false;
}


#endif //PROJECT_MANAGEMENT_H
