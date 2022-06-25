//
// Created by sento on 29/06/2018.
//

#include "Management.h"

bool Management::controls(const string &a, const string &b, const string &c){
    log.setFile(a,b,c);
    return log.result();
}

void Management::execution() {
    short choose=0;     //Dovrai gestire input errati come 'a' oppure 'a_ssd\acc'
    cout<<"\nWelcome to the management of the social network.\nWhat do you want to do?\nFollows the list of commands:"<<endl;
    do {
        cout<<"Press 1 to display the id of all users.\nPress 2 for detailed information about each user.\n"
              "Press 3 to manage individual users.\nPress 4 to manage user relationships.\n"
              "Press 5 to manage messages and opinions.\nPress 6 for statistical functionality.\n"
              "Press 7 for search functionality.\nPress 0 to finish."<<endl;
        cin >> choose;
        verifyChoose();
        switch (choose) {
            case 0:
                break;
            case 1:     //Potrei mostrare anche le informazioni base
                usersList(log.getSempl(),"semplici");
                usersList(log.getAz(),"azienda");
                usersList(log.getGr(),"gruppo");
                break;
            case 2:
                while (!getAllInfo());
                break;
            case 3:
               while(!manageUser());
                break;
            case 4:
                while(!manageRelations());
                break;
            case 5:        //dopo i casi 1 2 3 se sono state fatte modifiche bisogna aggiornare i file, rieseguire i controlli e poi passare ai casi 4 e 5
                do {
                    cout <<"\nChoose one of the following options:\n"
                        "1) add a message.\n2) delete a message.\n3) modify a message and its reactions.\n"
                        "4) display their messages and reactions.\n"
                        "Followed by the id of the user whose messages you want to handle:\n[op] [id] – [insert 0 to undo]"<<endl;
                    cin >> choose;
                    cin.ignore();
                    if(choose!=0) {         //Se input errato è come fosse 0
                        getline(cin,_dato);
                        verifyChoose();
                        if ((_semplice = log.getSempl().getById(_dato)) != nullptr) {
                            while (!manageMessages(log.getSempl(), _semplice, _dato, choose));
                        } else if ((_gruppo = log.getGr().getById(_dato)) != nullptr) {
                            while (!manageMessages(log.getGr(), _gruppo, _dato, choose));
                        } else if ((_azienda = log.getAz().getById(_dato)) != nullptr) {
                            while (!manageMessages(log.getAz(), _azienda, _dato, choose));
                        } else cerr << "\nUtente non trovato" << endl;
                    }
                    verifyChoose();
                }while(choose!=0);
                choose=5;
                break;
            case 6:
                while(!statistics());
                break;
            case 7:
                while (!reserch());
                break;
            default:
                cerr << "\nNon sono previsiti ulteriori comandi" << endl;
                break;
        }
    }while(choose!=0);
    if(_modifyNews || _modifyUser || _modifyRel) cout<<"Aggiornamento modifiche..."<<endl;
    exit();
    cout << "Fine esecuzione" << endl;
}

bool Management::getAllInfo() {                             // NB ottimo associare a utntptr così ogni volta
    int i=1,k=0,continua=0;                                            //non itera per riprendere sempre lo stesso utente
    const Utente * utntptr;
    cout<<"\nVisualizzare gli utenti semplici? Y/N"<<endl;
    cin >> _yorn;
    verifyChoose();
    if ((char) tolower(_yorn) == 'y') {
        cout<<"Sono presenti i seguenti utenti semplici:"<<endl;
        auto it = log.getSempl().iteratorBegin();
        while (i <= log.getSempl().getNumUsers()) {
            int like = 0, dislike = 0;
            for (k = 0; k < (*it).getNumMex(); k++) {
                like += (*it).getLikeListConst(k).size();
                dislike += (*it).getDislikeListConst(k, true).size();
            }
            cout << "\n"<< i << " ) [id " << (*it).getID() << "] " << (*it).getInfo()[4] << ": "
                 << (*it).getInfo()[5] << ", " << (*it).getInfo()[2] << ": " << (*it).getInfo()[3] << ", "
                 << (*it).getInfo()[0] << ": " << (*it).getInfo()[1] << "\n L'utente ha pubblicato "
                 << (*it).getNumMex() << " messaggi, ricevendo un totale di " << like << " like e "
                 << dislike << " dislike,\n mentre ha espresso " << (*it).getOpinionSize()
                 << " opinioni: like e dislike.\n L'utente ha " << (*it).getNumAmici() << " amici e "
                 << (*it).getNumConoscenti() << " conoscenti." << endl;
            if ((*it).single()) cout << " E' single." << endl;
            else
                cout << " E' sposato con: \n" << (*it).getConiuge().getInfo()[5] << " "
                     << (*it).getConiuge().getInfo()[3] << " [id " << (*it).getConiuge().getID() << "]."
                     << endl;
            if ((k = (*it).getNumGenitori()) != 0) cout << " Ha " << k << " genitori:" << endl;
            else cout << " Non sono presenti genitori." << endl;
            for (k = 0; k < (*it).getNumGenitori(); k++) {
                utntptr = &(*it).getGenitori(k);
                cout << (*utntptr).getInfo()[5] << " " << (*utntptr).getInfo()[3] << " [id "
                     << (*utntptr).getID() << "]." << endl;
            }
            if ((k = (*it).getNumFigli()) != 0) cout << " Ha " << k << " figli:" << endl;
            else cout << " Non sono presenti figli." << endl;
            for (k = 0; k < (*it).getNumFigli(); k++) {
                utntptr = &(*it).getFiglio(k);
                cout << (*utntptr).getInfo()[5] << " " << (*utntptr).getInfo()[3] << " [id "
                     << (*utntptr).getID() << "]." << endl;
            }
            if ((k = (*it).getNumLavori()) != 0) cout << " Lavora per " << k << " aziende:" << endl;
            else cout << " Risulta disoccupato." << endl;
            for (k = 0; k < (*it).getNumLavori(); k++) {
                utntptr = &(*it).getLavoro(k);
                cout << (*utntptr).getInfo()[1] << " [id " << (*utntptr).getID() << "]." << endl;
            }
            if ((k = (*it).getNumGruppi()) != 0) cout << " Ha aderito a " << k << " gruppi:" << endl;
            else cout << " Non ha aderito ad alcun gruppo." << endl;
            for (k = 0; k < (*it).getNumGruppi(); k++) {
                utntptr = &(*it).getGruppo(k);
                cout << (*utntptr).getInfo()[1] << " [id " << (*utntptr).getID() << "]." << endl;
            }
            while((continua = stop(i, log.getSempl().getNumUsers())) == -1);   //iL PROBLEMA è CHE RIMETTE I=-1 :/
            i=++continua;
            it++;
        }
        if (i != 1) {
           while (!getAllInfoPlus(log.getSempl(),_semplice->getAmici,_semplice->getNumAmici, "amici","conoscenti",
                    _semplice->getConoscente,_semplice->getNumConoscenti));
        }else cout << "Non sono presenti utenti semplici." << endl;
    }
    else if(tolower(_yorn)!='n') {
        cerr<<"Input non valido"<<endl;
        return false;
    }
    cout<<"\nVisualizzare le aziende? Y/N"<<endl;
    cin >> _yorn;
    verifyChoose();
    i=1;
    if ((char) tolower(_yorn) == 'y') {
        cout<<"Sono presenti le seguenti aziende:"<<endl;
        auto it = log.getAz().iteratorBegin();
        while (i <= log.getAz().getNumUsers()) {
            int like = 0, dislike = 0;
            for (k = 0; k < (*it).getNumMex(); k++) {
                like += (*it).getLikeListConst(k).size();
                dislike += (*it).getDislikeListConst(k, true).size();
            }
            cout << "\n" << i << " ) [id " << (*it).getID() << "] " << (*it).getInfo()[0] << ": "
                 << (*it).getInfo()[1] <<  "\n L'azienda ha pubblicato "
                 << (*it).getNumMex() << " messaggi, ricevendo un totale di " << like << " like e "
                 << dislike << " dislike.\n L'utente ha " << (*it).getNumDipendenti() << " dipendenti e "
                 << (*it).getNumConsociate() << " consociate." << endl;
            while((continua = stop(i, log.getSempl().getNumUsers())) == -1);
            i=++continua;
            it++;
        }if (i != 1) {
            while (!getAllInfoPlus(log.getAz(),_azienda->getDipendente,_azienda->getNumDipendenti, "dipendenti","utenti consociati",
                                   _azienda->getConsociata,_azienda->getNumConsociate));
        }else cout << "Non sono presenti aziende." << endl;
    }
    else if(tolower(_yorn)!='n') {
        cerr<<"Input non valido"<<endl;
        return false;
    }
    cout<<"\nVisualizzare i gruppi? Y/N"<<endl;
    cin >> _yorn;
    verifyChoose();
    i=1;
    if ((char) tolower(_yorn) == 'y') {
        cout<<"Sono presenti i seguenti gruppi:"<<endl;
        auto it = log.getGr().iteratorBegin();
        while (i <= log.getGr().getNumUsers()) {
            int like = 0, dislike = 0;
            for (k = 0; k < (*it).getNumMex(); k++) {
                like += (*it).getLikeListConst(k).size();
                dislike += (*it).getDislikeListConst(k, true).size();
            }
            cout <<"\n" << i << " ) [id " << (*it).getID() << "] " << (*it).getInfo()[0] << ": "
                 << (*it).getInfo()[1] <<  "\n Il gruppo ha pubblicato "
                 << (*it).getNumMex() << " messaggi, ricevendo un totale di " << like << " like e "
                 << dislike << " dislike.\n L'utente ha " << (*it).getNumMembri() << " membri."<< endl;
            while ((continua = stop(i, log.getSempl().getNumUsers())) == -1);
            //Alla fine selezionare il numero di un utente per visualizzare ulteriori informazioni o tutti i membri
            i=++continua;
            it++;
        }if (i != 1) {
            while (!getAllInfoPlus(log.getGr(),_gruppo->getMembri,_gruppo->getNumMembri, "membri"));
        }else cout << "Non sono presenti gruppi." << endl;
    }
    else if(tolower(_yorn)!='n') {
        cerr<<"Input non valido"<<endl;
        return false;
    }
    _incremento=3;
    return true;
}

bool Management::manageUser() {
    Date ctrldata;
    short choose=0;
    do {
        cout << "\nPress 1 to add one.\nPress 2 to delete one.\nPress 3 to change one.\n"
                "Press 0 to return to previous selection"<< endl;
        cin >> choose;
        verifyChoose();
        switch (choose) {
            case 0:
                break;
            case 1:
                cout<< "\n| Utente Semplice | Utente Azienda | Utente Gruppo | Annullare |"<< endl;
                cout<< "|       1         |       2        |       3       |     0     |"<< endl;
                cin >> choose;
                verifyChoose();
                if(choose==0) return false;
                switch (choose) {
                    case 1:
                        if(addUser(log.getSempl())) {
                            do {
                                cout << "\nInserire cognome utente semplice:\nENTER per annullare." << endl;
                                getline(cin, _dato);
                                if (_dato.empty()){
                                    log.getSempl().removeUser();
                                    return false;
                                }
                            } while (!verifyInfo(_dato));
                            (*log.getSempl()).newInfo(_dato, false);
                            (*log.getSempl()).newInfo(log.keyword(9), false);
                            do {
                                cout<< "\nInserire data di nascita utente semplice nel formato dd/mm/yyyy:"
                                           "\nENTER per annullare." << endl;
                                getline(cin, _dato);
                                if (_dato.empty()){
                                    log.getSempl().removeUser();
                                    return false;
                                }
                                if (!ctrldata.isDate(_dato)) {
                                    cerr << "La data non e' valida." << endl;
                                    _dato = "}}";
                                }
                            } while (!verifyInfo(_dato));
                            (*log.getSempl()).newInfo(_dato,false);
                            (*log.getSempl()).newInfo(log.keyword(10),false);
                            addInfo(log.getSempl());
                        }
                        _modifyUser= true;
                        break;
                    case 2:
                        if(addUser(log.getAz())) addInfo(log.getAz());
                        _modifyUser= true;
                        break;
                    case 3:
                        if(addUser(log.getGr())) addInfo(log.getGr());
                        _modifyUser= true;
                        break;
                    default:
                        cerr << "\nNon sono previsite altre tipologie di utente." << endl;
                        break;
                }
                choose=1;
                break;
            case 2:
                cout<<"\nInserire l'id dell'utente da eliminare:\nENTER per annullare." << endl;      //carino se implementi altre modalità
                getline(cin, _dato);
                if (_dato.empty()) return false;
                if(log.getSempl().getById(_dato)!= nullptr) removeSemplice(*log.getSempl());
                else if(log.getGr().getById(_dato)!= nullptr) removeGruppo(*log.getGr());
                else if(log.getAz().getById(_dato)!= nullptr) removeAzienda(*log.getAz());
                else{
                    cout<<"Utente non trovato."<<endl;
                    break;
                }
                cout<< "Utente "<< _dato<< " rimosso."<<endl;
                remove((_dato+".csv").c_str());
                _modifyUser= true;
                _modifyNews= true;
                _modifyRel= true;
                break;
            case 3:
                cout << "\nChoose one of the following options:\n1) Add information\n"
                        "2) Edit information\n3) Delete all unnecessary info\nFollowed by id "
                        "of the user to modify:\n[op] [id] – [insert 0 to cancel]" << endl;
                cin>>choose;
                cin.ignore();
                if (choose==0) {
                    verifyChoose();
                    return false;
                }
                getline(cin, _dato);
                verifyChoose();
                if((_semplice=log.getSempl().getById(_dato)) != nullptr){
                    while(!modifyUser(log.getSempl(), choose));    //tanto ora la getbyid mi pone li anche l'iteratore interno
                }
                else if((_gruppo=log.getGr().getById(_dato)) != nullptr){
                    while(!modifyUser(log.getGr(), choose));
                }
                else if((_azienda=log.getAz().getById(_dato)) != nullptr) {
                    while(!modifyUser(log.getAz(), choose));
                }
                else{
                    cout<<"Utente non trovato"<<endl;
                    break;
                }
                _modifyUser= true;
                choose=3;
                break;
            default:
                cerr << "Non sono previsiti ulteriori comandi" << endl;
                break;
        }
    }while(choose!=0);
    return true;
}

bool Management::statistics() {     //Bigger e i dopo ogni scelta sono reinizializzati al fondo a 0
    short choose;
    int i=0,bigger=0;
    do {
        cout << "\n\nYou can choose between:\n1) Total number of users, number of simple users, company and group.\n"
                "2) Number of friends and relatives for each simple user.\n3) Number of users born after a specific date of birth.\n"
                "4) Number of employees and number of subsidiaries for each holding.\n5) Number of users for each group.\n"
                "6) Company with the largest number of employees.\n7) News with the highest number of ratings.\n"
                "8) News with the greatest number of indignation.\n9) User with the most friends.\n"
                "10) User with the most knowledge.\n11) Average age of simple users.\n"
                "Press 0 to go back to the previous selection." << endl;
        cin >> choose;
        verifyChoose();
        switch (choose) {
            case 0:
                break;
            case 1:
                cout<<"\nSono presenti "
                      <<log.getSempl().getNumUsers()+log.getGr().getNumUsers()+log.getAz().getNumUsers()<<" utenti totali di cui:"<<endl;
                cout<<log.getSempl().getNumUsers()<<" utenti semplici, "<< log.getGr().getNumUsers()<<
                    " utenti gruppo, "<<log.getAz().getNumUsers()<<" utenti azienda"<<endl;
                break;
            case 3: {
                cout << "\nInserire data di nascita nella forma dd/mm/yyyy:" << endl;
                cin >> _dato;
                if(_ctrldate.isDate(_dato)) {
                    auto it = log.getSempl().iteratorBegin();
                    while(it!=log.getSempl().iteratorEnd()) {
                        if(_ctrldate<Date::convert((*it).getInfo()[1])|| Date::convert((*it).getInfo()[1])==_ctrldate) bigger++;
                        it++;
                    }
                    cout<<"\nNumero utenti nati dopo il "<< _dato<< ": "<< bigger<<endl;
                }else cout<<"Data non valida"<<endl;
            }
                break;
            case 2:
            case 4:
            case 5:
                countRelations(choose);
                break;
            case 6: {
                countProperties(log.getAz(), "dipendenti", _azienda->getNumDipendenti);
                auto it = log.getAz().iteratorBegin();
                vector<string> result;
                for (i = 0; i < log.getAz().getNumUsers(); i++, it++) {
                    for (int k = 0; k < (*it).getNumConsociate(); k++) {
                        _azienda = (static_cast<const Azienda *>(&(*it).getConsociata(k)));
                        if ((*_azienda).getNumDipendenti() > bigger) {
                            bigger = (*_azienda).getNumDipendenti();
                            result.clear();
                            result.emplace_back((*it).getID());
                        } else if ((*_azienda).getNumDipendenti() == bigger) result.emplace_back((*it).getID());
                    }
                }
                cout << "\nConsiderando i dipendenti relativi alla aziende consociate:" << endl;
                if(bigger==0) cout << "Non vi sono utenti con dipendanti" << endl;
                else if (result.size() == 1) cout << "L'utente con il maggior numero di dipendenti (" << bigger << ") e':" << endl;
                else cout << "Gli utenti con il maggior numero di dipendenti (" << bigger << ") sono:" << endl;
                for (i = 0; i < result.size(); i++) cout<<i+1<<") id: "<< result[i] << endl;
            }
                break;
            case 7: {
                bigger=0;
                vector<pair<string,string>> maggioriLike;
                countOpinions(log.getSempl(), maggioriLike, (*log.getSempl()).getLikeListConst, bigger);
                countOpinions(log.getAz(), maggioriLike, (*log.getAz()).getLikeListConst, bigger);
                countOpinions(log.getGr(), maggioriLike, (*log.getGr()).getLikeListConst, bigger);
                if(maggioriLike.empty()) cout<<"\nNon sono presenti notizie:"<<endl;
                else if(maggioriLike.size()==1) cout<<"\nLa notizia con il maggior numero di like ("<< bigger <<") e':"<<endl;
                else cout<<"\nLe notizie con il maggior numero di like ("<< bigger <<") sono:"<<endl;
                for(i=0;i<maggioriLike.size();i++) cout<<get<0>(maggioriLike[i])<<" di: "<< get<1>(maggioriLike[i])<<endl;
                }
                break;
            case 8: {
                bigger=0;
                vector<pair<string, string>> maggioriDisLike;
                countOpinions(log.getSempl(), maggioriDisLike, (*log.getSempl()).getDislikeListConst, bigger);
                countOpinions(log.getAz(), maggioriDisLike, (*log.getAz()).getDislikeListConst, bigger);
                countOpinions(log.getGr(), maggioriDisLike, (*log.getGr()).getDislikeListConst, bigger);
                if (maggioriDisLike.empty()) cout << "Non sono presenti notizie:" << endl;
                else if (maggioriDisLike.size() == 1) cout << "La notizia con il maggior numero di dislike (" << bigger << ") e':" << endl;
                else cout << "\nLe notizie con il maggior numero di dislike (" << bigger << ") sono:" << endl;
                for (i = 0; i < maggioriDisLike.size(); i++)
                    cout << i+1<<")"<< get<0>(maggioriDisLike[i]) << " di: " << get<1>(maggioriDisLike[i]) << endl;
            }
                break;
            case 9:
                countProperties(log.getSempl(), "amici", _semplice->getNumAmici);
                break;
            case 10:
                countProperties(log.getSempl(), "conoscenti", _semplice->getNumConoscenti);
                break;
            case 11: {
                bigger = 0;
                auto it = log.getSempl().iteratorBegin();
                while(it!=log.getSempl().iteratorEnd()) {
                    bigger+=_ctrldate.yearsFrom((*it).getInfo()[1]);
                    it++;
                }
                cout<<"\nL'eta' media di tutti gli utenti e' di "<<bigger/log.getSempl().getNumUsers()<< " anni"<<endl;
            }
                break;
            default:
                cerr << "\nNon sono previsiti ulteriori comandi" << endl;
                break;
        }
        i=0;bigger=0;
    }while(choose!=0);
    return true;
}

bool Management::reserch() {
    short choose;
    do {
        cout << "\nPress 1 to get a user’s family tree.\nPress 2 to get family trees of all users\n"
                "Press 3 to search for ’lone wolves', simple users who do not actively participate in social life.\n"
                "Press 4 to sort companies for best ratio between positive and negative reactions to their news.\n"
                "Press 5 to sort companies for worst ratio between positive and negative reactions to their news.\n"
                "Press 0 to return to previous selection" << endl;
        cin >> choose;
        verifyChoose();
        switch (choose) {
            case 0:
                break;
            case 1:
                cout << "\nInserire l'id dell'utente di cui si vuole ottenere l'albero genealogico, ENTER per annullare." << endl;
                getline(cin,_dato);
                if(_dato.empty()) break;
                if((_semplice=log.getSempl().getById(_dato))!= nullptr){
                    if(_semplice->getStampato()) {
                    cerr<<"\nE' gia' presente l'albero genealogico dell'utente scelto."<<endl;
                    break;
                }
                if(!genealogicalTree(_semplice)) cout<<"Non vi sono i presupposti per la stampa dell'albero genealogico."<<endl;
                }
                break;
            case 2:
                for(auto it=log.getSempl().iteratorBegin();it!=log.getSempl().iteratorEnd();it++)
                    genealogicalTree(&(*it));
                break;
            case 3: {
                int  amici=log.getSempl().getNumUsers(),cono=amici,figli=amici,genit=2,gruppi=log.getGr().getNumUsers(),
                        lavori=log.getAz().getNumUsers(),like=-1;
                short lonly= 2;
                vector<Utente*> result;
                auto it=log.getSempl().iteratorBegin();
                cout << "\nIt is possible to define a ’Lone Wolf' according to the following parameters."
                        "\n1) Number of friends:\n2) Number of acquaintances:\n3) Number of children:\n4) Number of parents:"
                        "\n5) Number of groups:\n6) Number of jobs:\n7) Number of likes & dislikes expressed:\n8) Single?  (Y/N) "
                        "\nPress 0 to cancel, ENTER to process." << endl;
                do {
                    getline(cin,_dato);
                    switch (atoi(_dato.c_str())) {
                        case 0:
                            break;
                        case 1:
                            cout << "Numero amici:" << endl;
                            cin >> amici;
                            verifyChoose();
                            break;
                        case 2:
                            cout << "Numero conoscenti:" << endl;
                            cin >> cono;
                            verifyChoose();
                            break;
                        case 3:
                            cout << "Numero figli:" << endl;
                            cin >> figli;
                            verifyChoose();
                            break;
                        case 4:
                            cout << "Numero genitori:" << endl;
                            cin >> genit;
                            verifyChoose();
                            break;
                        case 5:
                            cout << "Numero gruppi:" << endl;
                            cin >> gruppi;
                            verifyChoose();
                            break;
                        case 6:
                            cout << "Numero lavori:" << endl;
                            cin >> lavori;
                            verifyChoose();
                            break;
                        case 7:
                            cout << "Numero di like & dislike espressi:" << endl;
                            cin>>like;
                            verifyChoose();
                            break;
                        case 8:
                            cout << "Single? (Y/N)" << endl;
                            cin>>_yorn;
                            verifyChoose();
                            if ((char) tolower(_yorn) == 'n') lonly = 0;
                            else if ((char) tolower(_yorn) == 'y') lonly = 1;
                            else cerr<<"Input non valido, non verra' considerato."<<endl;
                            break;
                        default:
                            cerr << "Non sono previste altre opzioni" << endl;
                            break;
                    }
                    if(atoi(_dato.c_str())!=0) cout<<"\nScegliere un ulteriore parametro da settare altrimenti "
                                                     "confermare (ENTER) o annullare (0)."<<endl;
                }while(atoi(_dato.c_str())!=0);
                //cin.ignore();
                if(_dato.empty()) {
                    while (it != log.getSempl().iteratorEnd()) {
                        if ((*it).getNumAmici() <= (amici) && (*it).getNumConoscenti() <= (cono) &&
                            (*it).getNumGruppi() <= (gruppi) && (*it).getNumGenitori() <= (genit) &&
                            (*it).getNumLavori() <= (lavori) && (*it).getNumFigli() <= (figli) &&
                            (like==-1 || static_cast<Semplice>((*it)).getOpinionSize()<=like)
                            && (lonly==2 || (*it).single()==lonly ))
                            result.emplace_back(&(*it));
                        it++;
                    }
                    cout << "Vi sono " << result.size()
                         << " utenti che soddisfano i parametri inseriti." << endl;
                    for(int i=0;i<result.size();i++) cout<< result[i]->getInfo()[5]<< " " << result[i]->getInfo()[3]
                            << " id: " <<result[i]->getID()<<endl;
                }
            }
                break;
            case 4:
            case 5:{
                int like,dislike,totRatio=0,totMex=0;
                bool cons= true;
                vector<pair<int,Utente*>> result;
                cout << "\nInserire il rapporto like:dislike tra reazioni positive e negative, premere 0 per annullare.\nLike:"<<endl;
                cin >>like;
                verifyChoose();
                if(like==0) break;
                cout<<"Dislike:"<<endl;
                cin>>dislike;
                verifyChoose();
                if(dislike==0){
                    cerr<<"Non puo' essere 0"<<endl;
                    return false;
                }
                cout<<"\nConsiderare sulla ratio le news delle consociate? (Y/N)"<<endl;
                cin>>_yorn;
                verifyChoose();
                if ((char) tolower(_yorn) == 'n') cons = false;
                else if ((char) tolower(_yorn) != 'y') return false;
                auto it=log.getAz().iteratorBegin();
                while (it!=log.getAz().iteratorEnd()){
                    totRatio=0,totMex=0;
                    for(int i=0;i<(*it).getNumMex();i++)
                        (*it).getDislikeListConst(i).size()==0 ? totRatio+= (*it).getLikeListConst(i).size() :
                                totRatio+=((*it).getLikeListConst(i).size() / (*it).getDislikeListConst(i).size());
                    if(cons) {
                        for(int k=0;k<(*it).getNumConsociate();k++) {
                            for (int m = 0; m < (*it).getConsociata(k).getNumMex(); m++)
                                (*it).getDislikeListConst(m).size()==0 ? totRatio+= (*it).getLikeListConst(m).size():
                                      totRatio += ((*it).getLikeListConst(m).size() / (*it).getDislikeListConst(m).size());
                            totMex+=(*it).getConsociata(k).getNumMex();
                        }
                    }
                    totMex+=(*it).getNumMex();
                    if(totMex!=0) {
                        if (choose == 3 && totRatio >= like / dislike) result.emplace_back(totRatio, &(*it));
                        else if (choose == 4 && totRatio <= like / dislike) result.emplace_back(totRatio, &(*it));
                    }
                    it++;
                }
                sort(result.begin(),result.end());  //Dovrebbe andare
                if(result.empty()) cout<<"Non vi sono aziende che soddisfano la ratio."<<endl;
                else cout << "Vi sono " << result.size() << " aziende che soddisfano la ratio, in ordine dal peggiore al migliore:" << endl;
                for(int i=0;i<result.size();i++) cout<< get<1>(result[i])->getInfo()[1]<<" like:dislike "<< get<0>(result[i])
                        <<" id: " <<get<1>(result[i])->getID()<<endl;
                break;
            }
            default:
                cerr << "Non sono previsiti ulteriori comandi" << endl;
                break;
        }
    }while(choose!=0);
    return true;
}

int Management::stop(int iterazione, int numMax, string oggetti) {      //dopo stop va posto _incremento al valore di partenza
    if ((iterazione+20) / 10 ==_incremento) {
        cout << "\nVisualizzare ulteriori "<< oggetti <<"? Y/N" << endl;
        cin >> _yorn;
        verifyChoose();
        if ((char) tolower(_yorn) == 'n'){
            _incremento=3;
            return numMax;
        }
        else if ((char) tolower(_yorn) != 'y') {
            cerr<<"Input non valido."<<endl;
            _incremento=3;
            return -1;
        }
        _incremento+=_incremento;
    }
    return iterazione;
}

bool Management::manageRelations() {
    int choose = 0, relation = 0,i=0;
    const Semplice *semptr;
    const Azienda *azptr;
    const Gruppo *gruptr;
    string id_utente;
    vector<const Semplice *> parentsResult,sonsResult;
    cout << "\nInserire l'id dell'utente da gestire.\nENTER per tornare alla selezione precedente." << endl;
    getline(cin, id_utente);
    if (id_utente.empty()) return true;
    do{
    if ((_semplice = log.getSempl().getById(id_utente)) != nullptr) {
        cout << "\nScegliere una delle seguenti opzioni per l'utente "<< id_utente<<":"
                "\n\t1) Aggiungi\t2) Rimuovi\nSeguita dal tipo di relazione desiderata:" << endl;
        cout << "\t1) Amici\t2) Conoscenti\t3) Genitori\t4) Coniuge\t5) Figli\t6) Lavori\t7) Gruppi\n[n1] [n2] - [Inserire 0 per annullare]" << endl;
        cin >> choose;
        if (choose == 0) {
            verifyChoose();
            return false;
        }
        cin >> relation;
        verifyChoose();
        switch (relation) {
            case 1:
                while (iniRelations<Semplice>(choose, "amico", log.getSempl(), _semplice, _semplice->getNumAmici, _semplice->getAmici)) {
                    if (choose == 1 && !_dato.empty() &&
                        log.getSempl().manageRelation(_semplice, &*log.getSempl(), _semplice->addAmici, (*log.getSempl()).addAmici)) {
                        cout << "Utente\t " << _dato << "\t aggiunto." << endl;
                        _modifyRel = true;
                    } else if (choose == 2 && !_dato.empty()) {
                        semptr = static_cast<const Semplice *>(&_semplice->getAmici(atoi(_dato.c_str()) - 1));
                        log.getSempl().manageRelation(_semplice, semptr, _semplice->removeAmico, semptr->removeAmico);
                    }
                }
                break;
            case 2:
                while (iniRelations<Semplice>(choose, "conoscente", log.getSempl(), _semplice,
                                              _semplice->getNumConoscenti, _semplice->getConoscente)) {
                    if (choose == 1 && !_dato.empty() && _semplice->addConoscenze(&*log.getSempl())) {
                        cout << "Utente\t " << _dato << "\t aggiunto." << endl;
                        _modifyRel = true;
                    } else if (choose == 2 && !_dato.empty()) {
                        semptr = static_cast<const Semplice *>(&_semplice->getConoscente(atoi(_dato.c_str()) - 1));
                        _semplice->removeConoscente(semptr);
                    }
                }
                break;
            case 3:
                while (iniRelations<Semplice>(choose, "genitore", log.getSempl(), _semplice, _semplice->getNumGenitori,
                                              _semplice->getGenitori)) {
                    if (choose == 1 && !_dato.empty() &&
                        log.getSempl().manageRelation(_semplice, &*log.getSempl(), _semplice->addGenitori,
                                                      (*log.getSempl()).addFigli)) {
                        parentsResult.push_back(&(*log.getSempl()));
                        sonsResult.push_back(_semplice);
                        log.controlParents(&(*log.getSempl()), parentsResult, i = 0);
                        log.controlSons(_semplice,sonsResult, i = 0);
                        if (find_first_of(parentsResult.begin(),parentsResult.end(),sonsResult.begin(),sonsResult.end())
                            !=parentsResult.end()) {
                            cerr << "ERRORE: tra i due utenti non vi puo' essere una relazione genitore-figlio."
                                 << endl;
                            log.getSempl().manageRelation(_semplice, &*log.getSempl(), _semplice->removeGenitore,
                                                          (*log.getSempl()).removeFiglio);
                            break;
                        }
                        for (int k = 0; k < parentsResult.size(); k++) parentsResult[k]->setVisitato('b');
                        for (int k = 0; k < sonsResult.size(); k++) sonsResult[k]->setVisitato('b');
                        parentsResult.clear();
                        sonsResult.clear();
                        cout << "Utente\t " << _dato << "\t aggiunto." << endl;
                        _modifyRel = true;
                    } else if (choose == 2 && !_dato.empty()) {
                        semptr = static_cast<const Semplice *>(&_semplice->getGenitori(atoi(_dato.c_str()) - 1));
                        log.getSempl().manageRelation(_semplice, semptr, _semplice->removeGenitore,
                                                      semptr->removeFiglio);
                    }
                }
                break;
            case 4:
                if (choose == 1) {
                    if (!_semplice->single()) {
                        cerr << "L'utente e' gia' coniugato" << endl;
                        return false;
                    }
                    do {
                        cout << "Inserire l'id del coniuge da aggiungere:\nENTER per annullare." << endl;
                        getline(cin, _dato);
                        if (_dato.empty()) return false;
                        if (log.getSempl().getById(_dato) != nullptr)
                            log.getSempl().manageRelation(_semplice, &*log.getSempl(), _semplice->addConiuge,
                                                          (*log.getSempl()).addConiuge);
                        else cerr << "Utente non trovato" << endl;
                    } while (&*log.getSempl() != nullptr);
                    _modifyRel = true;
                    cout << "Utente " << _dato << " aggiunto.";
                } else if (choose == 2) {
                    if (_semplice->single()) {
                        cout << "Non vi sono utenti da eliminare" << endl;
                        return false;
                    }
                    cout << "L'utente " << _dato << " e' sposato con: " << endl;
                    cout << _semplice->getConiuge().getID() << endl;
                    _semplice->removeConiuge(&_semplice->getConiuge());
                    cout << "Coniuge rimosso." << endl;
                    _modifyRel = true;
                }
                break;
            case 5:
                while (iniRelations<Semplice>(choose, "figlio", log.getSempl(), _semplice, _semplice->getNumFigli,
                                              _semplice->getFiglio)) {
                    if (choose == 1 && !_dato.empty() &&
                        log.getSempl().manageRelation(&*log.getSempl(), _semplice, (*log.getSempl()).addGenitori,
                                                      _semplice->addFigli)) {
                        parentsResult.push_back(_semplice);
                        sonsResult.push_back(&(*log.getSempl()));
                        log.controlParents(_semplice, parentsResult, i = 0);
                        log.controlSons(&(*log.getSempl()),sonsResult, i = 0);
                        if (find_first_of(parentsResult.begin(),parentsResult.end(),sonsResult.begin(),sonsResult.end())
                            !=parentsResult.end()) {
                            cerr << "ERRORE: tra i due utenti non vi puo' essere una relazione genitore-figlio."
                                 << endl;
                            log.getSempl().manageRelation(&(*log.getSempl()), _semplice,
                                                          (*log.getSempl()).removeGenitore, _semplice->removeFiglio);
                            break;
                        }
                        for (int k = 0; k < parentsResult.size(); k++) parentsResult[k]->setVisitato('b');
                        for (int k = 0; k < sonsResult.size(); k++) sonsResult[k]->setVisitato('b');
                        parentsResult.clear();
                        sonsResult.clear();
                        cout << "Utente\t " << _dato << "\t aggiunto." << endl;
                        _modifyRel = true;
                    } else if (choose == 2 && !_dato.empty()) {
                        semptr = static_cast<const Semplice *>(&_semplice->getFiglio(atoi(_dato.c_str()) - 1));
                        log.getSempl().manageRelation(_semplice, semptr, _semplice->removeFiglio,
                                                      semptr->removeGenitore);
                    }
                }
                break;
            case 6:
                while (iniRelations<Azienda>(choose, "azienda", log.getAz(), _semplice, _semplice->getNumLavori,
                                             _semplice->getLavoro)) {
                    if (choose == 1 && !_dato.empty() &&
                        log.getAz().manageRelation(&*log.getAz(), _semplice, (*log.getAz()).addDipendenti,
                                                   _semplice->addLavori)) {
                        cout << "Utente\t " << _dato << "\t aggiunto." << endl;
                        _modifyRel = true;
                    } else if (choose == 2 && !_dato.empty()) {
                        azptr = static_cast<const Azienda *>(&_semplice->getLavoro(atoi(_dato.c_str()) - 1));
                        log.getAz().manageRelation(azptr, _semplice, azptr->removeDipendente, _semplice->removeLavoro);
                    }
                }
                break;
            case 7:
                while (iniRelations<Gruppo>(choose, "gruppo", log.getGr(), _semplice, _semplice->getNumGruppi,
                                            _semplice->getGruppo)) {
                    if (choose == 1 && !_dato.empty() &&
                        log.getGr().manageRelation(&*log.getGr(), _semplice, (*log.getGr()).addMembri,
                                                   _semplice->addAppartenenze)) {
                        cout << "Utente\t " << _dato << "\t aggiunto." << endl;
                        _modifyRel = true;
                    } else if (choose == 2 && !_dato.empty()) {
                        gruptr = static_cast<const Gruppo *>(&_semplice->getGruppo(atoi(_dato.c_str()) - 1));
                        log.getGr().manageRelation(gruptr, _semplice, gruptr->removeMembro, _semplice->removeGruppo);
                    }
                }
                break;
            default:
                cerr << "Non sono previste altre scelte" << endl;
                return false;
        }
    } else if ((_azienda = log.getAz().getById(id_utente)) != nullptr) {
        cout << "\nScegliere una delle seguenti opzioni per l'utente "<< id_utente<<":"
              "\n\t1) Aggiungi\t2) Rimuovi\nSeguita dal tipo di relazione desiderata:" << endl;
        cout << "\t1) Consociate\t2) Dipendenti\n[n1] [n2] - [Inserire 0 per annullare]" << endl;
        cin >> choose;
        if (choose == 0) {
            verifyChoose();
            return false;
        }
        cin >> relation;
        verifyChoose();
        switch (choose) {
            case 1:
                while (iniRelations<Azienda>(choose, "consociata", log.getAz(), _azienda, _azienda->getNumConsociate,
                                             _azienda->getConsociata)) {
                    if (choose == 1 && !_dato.empty() && _azienda->addConsociate(&*log.getAz())) {
                        cout << "Utente\t " << _dato << "\t aggiunto." << endl;
                        _modifyRel = true;
                    } else if (choose == 2 && !_dato.empty()) {
                        azptr = static_cast<const Azienda *>(&_azienda->getConsociata(atoi(_dato.c_str()) - 1));
                        _azienda->removeConsociata(azptr);
                    }
                }
                break;
            case 2:
                while (iniRelations<Semplice>(choose, "dipendente", log.getSempl(), _azienda,
                                              _azienda->getNumDipendenti, _azienda->getDipendente)) {
                    if (choose == 1 && !_dato.empty() &&
                        log.getSempl().manageRelation(&*log.getSempl(), _azienda, (*log.getSempl()).addLavori,
                                                      _azienda->addDipendenti)) {
                        cout << "Utente\t " << _dato << "\t aggiunto." << endl;
                        _modifyRel = true;
                    } else if (choose == 2 && !_dato.empty()) {
                        semptr = static_cast<const Semplice *>(&_azienda->getDipendente(atoi(_dato.c_str()) - 1));
                        log.getSempl().manageRelation(semptr, _azienda, semptr->removeLavoro,
                                                      _azienda->removeDipendente);
                    }
                }
                break;
            default:
                cerr << "Non sono previste altre scelte" << endl;
                return false;
        }
    } else if ((_gruppo = log.getGr().getById(id_utente)) != nullptr) {
        cout << "\nScegliere una delle seguenti opzioni per l'utente "<< id_utente<<":"
              "\n\t1) Aggiungi utente al gruppo\t2) Rimuovi utente dal gruppo\n0) Annulla" << endl;
        cin >> choose;
        verifyChoose();
        if (choose == 0) {
            verifyChoose();
            return false;
        }
        while (iniRelations<Semplice>(choose, "appartenente al gruppo", log.getSempl(), _gruppo, _gruppo->getNumMembri,
                                      _gruppo->getMembri)) {
            if (choose == 1 && !_dato.empty() &&
                log.getSempl().manageRelation(&*log.getSempl(), _gruppo, (*log.getSempl()).addAppartenenze,
                                              _gruppo->addMembri)) {
                cout << "Utente\t " << _dato << "\t aggiunto." << endl;
                _modifyRel = true;
            } else if (choose == 2 && !_dato.empty()) {
                semptr = static_cast<const Semplice *>(&_gruppo->getMembri(atoi(_dato.c_str()) - 1));
                log.getSempl().manageRelation(semptr, _gruppo, semptr->removeGruppo, _gruppo->removeMembro);
            }
        }
    } else {
        cerr << "\nUtente non trovato o input errato." << endl;
        return false;
    }
    }while(!id_utente.empty());
    return true;
}

void Management::removeSemplice(const Semplice &darimuovere, bool remove_all) const {
    int i=0;
    for(;i<darimuovere.getNumFigli();i++) static_cast<const Semplice*>(&darimuovere.getFiglio(i))->removeGenitore(&darimuovere);
    for(i=0;i<darimuovere.getNumAmici();i++)  static_cast<const Semplice *>(&darimuovere.getAmici(i))->removeAmico(&darimuovere);
    for(i=0;i<darimuovere.getNumGenitori();i++) static_cast<const Semplice *>(&darimuovere.getGenitori(i))->removeFiglio(&darimuovere);
    if(!darimuovere.single()) static_cast<const Semplice *>(&darimuovere.getConiuge())->removeConiuge(&darimuovere);
    for(i=0;i<darimuovere.getNumLavori();i++) static_cast<const Azienda *>(&darimuovere.getLavoro(i))->removeDipendente(&darimuovere);
    for(i=0;i<darimuovere.getNumGruppi();i++) static_cast<const Gruppo *>(&darimuovere.getGruppo(i))->removeMembro(&darimuovere);
   /* if(remove_all)*/ darimuovere.removeAllOpinions();        //Iterazione molto esosa se si puo da non fare
    log.getSempl().removeUser();
}

void Management::removeAzienda(const Azienda &darimuovere)const {
    int i=0;
    for(;i<darimuovere.getNumDipendenti();i++) static_cast<const Semplice *>(&darimuovere.getDipendente(i))->removeLavoro( &darimuovere);
    log.getAz().removeUser();
}

void Management::removeGruppo(const Gruppo &darimuovere)const {
    int i=0;
    for(;i<darimuovere.getNumMembri();i++) static_cast<const Semplice *>(&darimuovere.getMembri(i))->removeGruppo( &darimuovere);
    log.getGr().removeUser();
}

bool Management::findSons(const Semplice *partenza, vector<const Semplice *> &fifo, int &daesaminare) {
    for(_i=0;_i<partenza->getNumFigli();_i++){
        if((_semplice=static_cast<const Semplice*>(&partenza->getFiglio(_i)))->getVisitato()!='g'){
            _semplice->setVisitato('g');
            fifo.emplace_back(_semplice);
        }
    }
    if(fifo.size()>++daesaminare) return findSons(fifo[daesaminare],fifo,daesaminare);
    else return true;
}

bool Management::findParents(const Semplice *partenza, vector<const Semplice *> &fifo, int &daesaminare) {
    for(_i=0;_i<partenza->getNumGenitori();_i++){
        if((_semplice=static_cast<const Semplice*>(&partenza->getGenitori(_i)))->getVisitato()=='b'){
            _semplice->setVisitato('g');
            fifo.emplace_back(_semplice);
        }
    }
    if(fifo.size()>++daesaminare) return findParents(fifo[daesaminare],fifo,daesaminare);
    else return true;
}

bool Management::genealogicalTree(const Semplice* utnu_partena) {
    short k=-2;
    if(utnu_partena->getNumGenitori()==0) {
        for (k = 0; k < utnu_partena->getNumFigli() && k != -1; k++)
            if (static_cast<const Semplice *>(&utnu_partena->getFiglio(k))->getNumFigli() != 0) k = -2;
    }
    else if(utnu_partena->getNumFigli()==0) {
                for (k = 0; k < utnu_partena->getNumGenitori() && k != -1; k++)
                    if (static_cast<const Semplice *>(&utnu_partena->getGenitori(k))->getNumGenitori() != 0) k = -2;
            }
    if(k>-1) {
        remove((utnu_partena->getID()+".csv").c_str());
        return false;
    }

    _oStream.open(utnu_partena->getID()+".csv",ios::trunc);
    if(_oStream.is_open()) {
        printTree(utnu_partena);
        utnu_partena->setStampato(true);
        _sheet.clear();
        _graph.clear();
    }else cerr<<"Non e' stato possibile creare il file di output "<<utnu_partena->getID()<<".csv"<<endl;
    return true;
}

void Management::exit() {
    string userFile,newsFile,relationFile;
    log.getNameFile(userFile,newsFile,relationFile);
    int k=0;
    if(_modifyUser){
        _oStream.open(userFile);
        _oStream.close();
    }
    if(_modifyNews){
        _oStream.open(newsFile);
        _oStream.close();
    }
    refresh(log.getSempl(),userFile,newsFile,"utente semplice");
    refresh(log.getAz(),userFile,newsFile,"utente azienda");
    refresh(log.getGr(),userFile,newsFile,"utente gruppo");
    if(_modifyRel) {
        _oStream.open(relationFile,ios::app);
        _oStream.close();
        _oStream.open(relationFile.c_str());
        if(!_oStream.is_open()){
            cerr<<"Non e' stato possibile aggiornare il file "<<relationFile<<endl;
            return;
        }
        for (auto it = log.getSempl().iteratorBegin(); it != log.getSempl().iteratorEnd();it= log.getSempl().iteratorBegin()) {
            for (k = 0; k < (*it).getNumFigli(); k++)
                _oStream << (*it).getID() << "," << (*it).getFiglio(k).getID() << ",genitore" << endl;
            for (k = 0; k < (*it).getNumGenitori(); k++)
                _oStream << (*it).getID() << "," << (*it).getGenitori(k).getID() << ",figlio" << endl;
            for (k = 0; k < (*it).getNumLavori(); k++)
                _oStream << (*it).getLavoro(k).getID() << "," << (*it).getID() << ",dipendente" << endl;
            for (k = 0; k < (*it).getNumGruppi(); k++)
                _oStream << (*it).getGruppo(k).getID() << "," << (*it).getID() << ",appartenente" << endl;
            for (k = 0; k < (*it).getNumConoscenti(); k++)
                _oStream << (*it).getID() << "," << (*it).getConoscente(k).getID() << ",conoscente" << endl;
            for (k = 0; k < (*it).getNumAmici(); k++)
                _oStream << (*it).getID() << "," << (*it).getAmici(k).getID() << ",amicizia" << endl;
            if (!(*it).single()) _oStream << (*it).getID() << "," << (*it).getConiuge().getID() << ",coniuge" << endl;
            removeSemplice((*it), false);                       //Non avrebbe alcun senso andare ad aggiornare magari migliaia di liste e puntatori ora, perche
                            //  l'utente viene rimosso solo per non avere duplicati nelle relazioni
        }
        for (auto it = log.getAz().iteratorBegin(); it != log.getAz().iteratorEnd();it = log.getAz().iteratorBegin()) {
            for (k = 0; k < (*it).getNumConsociate(); k++)
                _oStream << (*it).getID() << "," << (*it).getConsociata(k).getID() << ",consociata" << endl;
            for (k = 0; k < (*it).getNumDipendenti(); k++)
                _oStream << (*it).getID() << "," << (*it).getDipendente(k).getID() << ",dipendente" << endl;
            removeAzienda((*it));

        }
        for (auto it = log.getGr().iteratorBegin(); it != log.getGr().iteratorEnd(); it = log.getGr().iteratorBegin() ) {
            for (k = 0; k < (*it).getNumMembri(); k++)
                _oStream << (*it).getID() << "," << (*it).getMembri(k).getID() << ",appartenente" << endl;
            removeGruppo((*it));

        }
        _oStream.close();
    }

}

bool Management::verifyChoose()const {
    if(cin.fail()) {
        cin.clear();
        cin.sync();
        cerr<<"Input non valido."<<endl;
        return false ;
    }
    cin.sync();
    return true;
}

void Management::countRelations(short tipoUtnt) {
    short choose;
    cout<<"\nPremere 1 per scegliere l'utente, 2 per visualizzarli tutti, premere 0 per tornare alla selezione precedente."<<endl;
    cin >> choose;
    verifyChoose();
    switch(choose){
        case 0:
            return;
        case 1:
            cout<<"Inserire id utente:"<<endl;
            getline(cin,_dato);
            if(tipoUtnt==2 && (_semplice=log.getSempl().getById(_dato))!= nullptr) {
                vector<const Semplice*> fifo;
                int partenza=0;
                fifo.push_back(_semplice);
                findParents(_semplice, fifo, partenza);
                fifo.push_back(fifo[0]);
                findSons(fifo[0], fifo, partenza);
                cout << "L'utente possiede " << (*log.getSempl()).getNumAmici() << " amici e ha "<<
                     (*log.getSempl()).getNumFigli()<<" figli, "<<(*log.getSempl()).getNumGenitori()<<" genitori,";
                if((*log.getSempl()).single()) cout<<"e' sposato,";
                else cout<<"non e' sposato,";
                cout<<" per un totale di: "<< partenza-2+(int)(*log.getSempl()).single()<<" parenti"<<endl;
            }else if(tipoUtnt==4 && log.getAz().getById(_dato)!= nullptr)
                cout<<"L'azienda possiede "<<(*log.getAz()).getNumDipendenti() <<" dipendenti e "<<
                    (*log.getAz()).getNumConsociate()<<" aziende consociate"<<endl;
            else if(tipoUtnt==5 && log.getGr().getById(_dato)!= nullptr)
                cout<<"Il gruppo possiede "<< (*log.getGr()).getNumMembri()<<" membri"<<endl;
            else cerr<<"Utente non trovato"<<endl;
            break;
        case 2:
            if(tipoUtnt==2) {
                vector<const Semplice*> fifo;
                int partenza=0;
                auto it = log.getSempl().iteratorBegin();
                for (int i = 0; i < log.getSempl().getNumUsers(); i++, it++) {
                    fifo.push_back(&*it);
                    findParents(&*it, fifo, partenza);
                    fifo.push_back(fifo[0]);
                    findSons(fifo[0], fifo, partenza);
                    cout << "L'utente "<<(*it).getID() << " possiede " << (*it).getNumAmici() << " amici e ha "<<
                          (*it).getNumFigli()<<" figli, "<<(*it).getNumGenitori()<<" genitori,";
                    if((*it).single()) cout<<" e' sposato,";
                    else cout<<" non e' sposato,";
                    cout<<" per un totale di: "<< partenza-2+(int)(*it).single()<<" parenti."<<endl;
                    if ((i=stop(i, log.getSempl().getNumUsers()))== -1) return;
                    fifo.clear();
                    partenza=0;
                }
                if (it == log.getSempl().iteratorBegin()) cout << "Non vi sono utenti da mostrare" << endl;
            }else if(tipoUtnt==4){
                auto it = log.getAz().iteratorBegin();
                for (int i = 0; i < log.getAz().getNumUsers(); i++, it++) {
                    cout<<"L'azienda "<<(*it).getID() << " possiede "<<(*log.getAz()).getNumDipendenti() <<" dipendenti e "<<
                        (*log.getAz()).getNumConsociate()<<" aziende consociate."<<endl;
                    if ((i=stop(i, log.getAz().getNumUsers())) == -1) return;
                }
                if (it == log.getAz().iteratorBegin()) cout << "Non vi sono utenti da mostrare." << endl;
            }else if(tipoUtnt==5) {
                auto it = log.getGr().iteratorBegin();
                for (int i = 0; i < log.getGr().getNumUsers(); i++, it++) {
                    cout<<"Il gruppo "<<(*it).getID() << " possiede "<< (*log.getGr()).getNumMembri()<<" membri."<<endl;
                    if ((i=stop(i, log.getGr().getNumUsers())) == -1) return;
                }
                if (it == log.getGr().iteratorBegin()) cout << "Non vi sono utenti da mostrare." << endl;

            }
            break;
        default:
            cerr << "Non sono previsiti ulteriori comandi." << endl;
            break;
    }
}

int Management::parentTree( list<const Semplice*>::iterator &partenza, list<list<const Semplice*>>::iterator &lista_attuale,
                             list<list<const Semplice*>>::iterator &daesaminare,int maxUser) {
    if(partenza!=(*lista_attuale).end()) {
        (*partenza)->setVisitato('n');
        for (int i = 0; i < (*partenza)->getNumGenitori(); i++) {
            if ((_semplice = static_cast<const Semplice *>(&(*partenza)->getGenitori(i)))->getVisitato() == 'b') {
                _semplice->setVisitato('g');
                _graph.emplace_back();
                (*daesaminare).emplace_back(_semplice);
            }
        }
        if((*lista_attuale).end()!=++partenza) return parentTree(partenza,lista_attuale,daesaminare,maxUser);
    }
    if((*lista_attuale).size()>maxUser) maxUser=(*lista_attuale).size();
    if(_graph.end()!=++daesaminare) return parentTree(partenza=(*++lista_attuale).begin(),lista_attuale,daesaminare,maxUser);
    else return maxUser;

}

int Management::sonsTree( list<const Semplice*>::iterator &partenza, list<list<const Semplice*>>::iterator &lista_attuale,
                           list<list<const Semplice*>>::iterator &daesaminare,int maxUser) {
    if(partenza!=(*lista_attuale).end()) {
       // (*partenza)->visitato = 'n';
        for (int i = 0; i < (*partenza)->getNumFigli(); i++) {
            if ((_semplice = static_cast<const Semplice *>(&(*partenza)->getFiglio(i)))->getVisitato() == 'b') {
                _semplice->setVisitato('g');
                _graph.emplace_back();
                (*daesaminare).emplace_back(_semplice);
                }
        }
        if((*lista_attuale).end()!=++partenza) return sonsTree(partenza,lista_attuale,daesaminare,maxUser);
    }
    if((*lista_attuale).size()>maxUser) maxUser=(*lista_attuale).size();
    if(_graph.end()!=++daesaminare) return sonsTree(partenza=(*++lista_attuale).begin(),lista_attuale,daesaminare,maxUser);
    else return maxUser;
}

void Management::printTree(const Semplice* utnu_partena) {
    int maxPerRiga=0;
    _graph.emplace_back();
    (*(_graph.begin())).emplace_back(utnu_partena);
    auto lista_attuale = _graph.begin(), daesaminare = _graph.begin();
    auto partenza = (*_graph.begin()).begin();
    _graph.emplace_back();
    maxPerRiga=sonsTree(partenza, lista_attuale, ++daesaminare);
    //_graph.reverse();
    auto it = _graph.rbegin();
    short k = 0;
    _sheet.emplace_front();
    _sheet.emplace_front();
    auto it2 = _sheet.rbegin();
    while((*it).empty()) it++;
    auto it3 = (*it).begin();
    for(_i=0;_i<2;_i++) {
        while (it != _graph.rend() && !(*it).empty()) {
            if((((maxPerRiga*3+(maxPerRiga-1))-((*it).size()*3+((*it).size()-1)))/2)>0) {
                for (k = 0; k < ((maxPerRiga*3+(maxPerRiga-1))-((*it).size()*3+((*it).size()-1))) / 2; k++)
                    (*it2) += ";";
                it2++;
                for (k = 0;
                     k < ((maxPerRiga*3+(maxPerRiga-1))-((*it).size()*3+((*it).size()-1))) / 2; k++)
                    (*it2) += ";";
                it2--;
            }
            for (; it3 != (*it).end(); it3++, it2--) {
                (*it3)->setVisitato('b');
                (*(it2)) += ";genitori:;";
                for (k = 0; k < (*it3)->getNumGenitori(); k++) (*it2) += (*it3)->getGenitori(k).getID() + " ";
                for (; k < 2; k++) (*it2) += "???? ";
                (*it2) += ";;";
                (*++it2) += "id: " + (*it3)->getID() + ";" + (*it3)->getInfo()[3] + ";" + (*it3)->getInfo()[5] + ";;";
            }
            _sheet.emplace_front();
            _sheet.emplace_front();
            it2++,it2++,it2++;
            _sheet.emplace_front();
            //_sheet.emplace_back();
            it++;
            it3 = (*it).begin();
        }

        _graph.clear();
        if(_i==0) {
            short nuser;
            _graph.emplace_back();
            (*(_graph.begin())).emplace_back(utnu_partena);
            lista_attuale = _graph.begin(), daesaminare = _graph.begin(), partenza = (*_graph.begin()).begin();
            _graph.emplace_back();
            if((nuser=(short)parentTree(partenza, lista_attuale, ++daesaminare))>maxPerRiga) maxPerRiga=nuser;
            _graph.reverse();
            it = _graph.rbegin();
            (*(*it).begin())->setVisitato('b');
            it++;
            it3 = (*it).begin();
            _sheet.emplace_front();
            _sheet.emplace_front();
        }
    }

    _oStream<<"Stirpe di:;"<<utnu_partena->getInfo()[5]<<";"<<utnu_partena->getInfo()[3]
            <<";(id: "<<utnu_partena->getID()<<")"<<endl;
    _oStream<<";\n"<<endl;
    auto it1=_sheet.begin();
    while((*it1).empty()) it1++;
    while(it1!=_sheet.end()) {
    _oStream<<";;"<<(*it1)<<endl;
    it1++;
    }

    _oStream.close();
    _oStream.clear();

}


bool Management::verifyInfo(const string &dato)const {
    if(dato.find_first_of(":{},")!=-1 || dato.find_first_not_of(" \t\v\n\r")==-1 )  {
        cerr<<"\nI campi non possono essere vuoti o contenere i seguenti caratteri:\t :{},"<<endl;
        return false;
    }
    return true;
}


