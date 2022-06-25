//
// Created by sento on 25/06/2018.
//

#include "Logging.h"                //NB end() è la sentinella dopo l'ultimo valore valido

Logging::Logging(const string &a, const string &b, const string &c){
    _file1.open(_nome1=a.c_str());
    _file2.open(_nome2=b.c_str());
    _file3.open(_nome3=c.c_str());
}

Logging::~Logging() {
    if(_file1.is_open()) close(_file1);
    if(_file2.is_open()) close(_file2);
    if(_file3.is_open()) close(_file3);
}

bool Logging::verifyInput(ifstream &ifile, string &file)const {
    ifile.open(file);
    if(!ifile.is_open() ) {
        cerr<<"Error opening file " << file<<endl;  //Probabilmente in lettura se non esiste non ne crea uno nuovo e da errore yeeeeee
        return false;
    }
    //_File.close();       //chiuderlo subito pulisce il file
    return true;
}

void Logging::close(ifstream &fileStrm) {
    if(fileStrm.is_open()){
        fileStrm.close();
        fileStrm.clear();
    }
    //if(_oFile.is_open()) _oFile.close();
}

char Logging::identifyFileUser(ifstream & ifile)const {      //Analizzo _riga per _riga e la "passo" a una struttura per la memorizzazione
    string riga;
    getline(ifile,riga);
   //FIND E FIND_LAST PARTONO DA 0 a n-1 CARATTERI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    if(riga.rfind("},dislike:"+9)==riga.rfind('{')-1) return 'n';        //find last mi da la posizione dei ':'
    auto pos= remove_blank(riga.begin(), riga.end()); //così non ho rotture !!!!!!!!!!!!!!!!!!!!!!!!!!!
    if(count(riga.begin(),riga.begin()+pos,',')==2 && riga.find_first_of("{}:")==-1 ) return 'r';
    else if(pos==(riga.find('}')+1) && count(riga.begin(),find(riga.begin(),riga.begin()+pos,'{'),',')==2) return 'u';
    else if(riga.find_first_not_of(" \n\r\t\v")==-1){
        getline(ifile,riga);
        if(ifile.eof()) return 'v';
    }
    else return 'z';
}

bool Logging::verifyUser(ifstream &ifile, string &nomeFile) {     //NB devi dire che non ci siano spazi ai lati dell'id
    Date ctrldata;
    ifile.seekg(0);
    string id;
    int riesima=1;
    while(!ifile.eof() && ifile.peek()!=ifstream::traits_type::eof()) {
        getline(ifile, _riga);
        auto pos = _riga.find('}');
        if (_riga.find_first_not_of(" \t\v\n\r") != -1) {             // (\n e \r mandano a capo) Verifica automaticamente anche che non sia vuota
            id.assign(_riga, 0, _riga.find(','));
            if (id.find_first_of(":{}") != -1 || id.find_first_not_of(" \t\v\n\r")==-1) {
                cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: The ID cannot be void or"
                       "use the following characters:\t,{}:\nThe formatting is therefore incorrect." << endl;
                return false;
            }
            if (id.size() != _riga.find(",utente semplice,{") && id.size() != _riga.find(",utente azienda,{") &&
                id.size() != _riga.find(",utente gruppo,{")) {
                cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: could not be identified "
                       "the type of user.\nUsers are identified by the following types: \n simple user "
                       "\n company user \n group user " << endl;
                return false;
                //INSERIRE KEYWORD ALTERNATIVE PER I TIPI, SE ERANO GIUSTE ALLORA ERA UN ERRORE DI FORMATTAZIONE
            }
            if (id.find_first_not_of(" \t\v\n\r") == -1) {
                cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: ID missing." << endl;
                return false;
            }
            if (pos != pos + remove_blank(_riga.begin() + 1 + pos, _riga.end()) ||
                //pos parte da 0 begin è la sentinella prima di 0
                _riga.find('{', _riga.find('{') + 1) != string::npos) {
                cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: wrong format."
                     << " Remember: staples are reserved characters." << endl;
                return false;
            }
            if (_riga.find("utente semplice") != -1) {
                if (!controlUser(_riga, id, riesima, _semplici.newUtente()) ||
                    !verifyCampo(_semplici.getLastUtente(), riesima, _keywords[9]) ||
                    !verifyCampo(_semplici.getLastUtente(), riesima, _keywords[10])) {
                    cerr<<"ERROR[ file "<<nomeFile<<"]"<<endl;
                    return false;
                }
                string tmp = _semplici.getLastUtente().getInfo()[1];
                if (!ctrldata.isDate(tmp)) {
                    cerr << "ERROR[ file "<< nomeFile<<" riga " << riesima << "]: date of birth is not valid" << endl;
                    return false;
                }
            } else if ((_riga.find("utente azienda") != -1 && !controlUser(_riga, id, riesima, _aziende.newUtente())) ||
                       (_riga.find("utente gruppo") != -1 && !controlUser(_riga, id, riesima, _gruppi.newUtente()))) {
                cerr<<"ERROR[ file "<<nomeFile<<"]"<<endl;
                return false;
            }
        } else {
            getline(ifile, _riga);
            if (!ifile.eof()) {
                cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: empty line" << endl;
                return false;
            }
        }
        riesima++;
    }
    for(auto it=_idlist.begin();it!=--_idlist.end();it++){
        for(auto iesimo=it;iesimo!=--_idlist.end();){     //non metto l'iterazione ++ tanto lo fa già dopo
            if (**it==**(++iesimo)) {
                cerr << "ERROR[ file "<< nomeFile<<" id " << **it << "]: duplicate ID" << endl; //un * entra in it il secondo * entra nel contenuto del puntatore in *it
                return false;
            }
        }
    }
    _idlist.clear();
    return true;
}       //no spazi: , utente semplice ,

bool Logging::controlUser(string &riga, string &id, int &riesima,const Utente &utente) {
                                            //last_not_of(INIZIO(sarebbe la fine perchè itera da POS all'indietro),POS(carattere da cui partire))
    unsigned int i=riga.find('{');
    string nuovoDato;
    if(riga[riga.find_first_not_of(" \t\f",i+1)]!='}')
    {
        unsigned int virgola,duep,_campo=1,pos, boundary;
        boundary=(unsigned int)string::npos; //It is needed if there an assignment of npos to a unsigned int
        utente.setId(id);
        int tmp=0;
        while ( (virgola=riga.find(',', i+1)) != boundary) {
            //cout<<virgola<<endl;
            duep=riga.find(':', i+1 );
            pos=riga.find_first_not_of(" \t\f",i+1);
            if (virgola < duep || duep == boundary || riga.find(':', duep+1 )<virgola )
            {

                //cout<<"errore 1"<<endl;
                cerr << "ERROR[ line " << riesima
                     << "]: verificare formattazione successiva al campo "<< _campo<<endl;
                cerr << "The following characters are reserved \n: , { }" << endl;
                return false;
            }
            if (pos==duep || riga[i+1]==':' ){
                cerr << "ERROR[ line " << riesima << "]: field "<< _campo<< " void" << endl;
                return false;
            }
            utente.newInfo(nuovoDato.assign(riga,pos,riga.find_last_not_of(" \t\f", duep-1)-pos+1 ));
            pos=riga.find_first_not_of(" \t\f",duep+1);
            if (pos==virgola || riga[duep+1]==','){
                cerr << "ERRORE[ line " << riesima << "]: field "<<_campo<<" void" << endl;
                return false;
            }
            utente.newInfo(nuovoDato.assign(riga, pos, riga.find_last_not_of(" \t\f",virgola-1)- pos+1)); //forse ci va -pos+1 perche prende n caratteri da 1 a n
            //cout<< nuovoDato.assign(riga, pos, riga.find_last_not_of(" \t\f",virgola-1)- pos+1)<<endl;
            i = virgola;
            //cout<<"Virgola: "<<i<<endl;
            _campo+=2;
        }
        duep = riga.find(':', i);
        pos=riga.find_first_not_of(" \t\f",i+1);
        if(duep==-1 || riga.find(':',duep + 1)!=string::npos){

            // cout<<"errore 2"<<endl;
            cerr << "ERROR[ line " << riesima
                 << "]: check formatting following field "<< _campo<<endl;
            cerr << "The following characters are reserved \n: , { }" << endl;
            return false;
        }
        if (pos==duep || riga[i+1]==':' ){
            cerr << "ERROR[ line " << riesima << "]: field "<< _campo<< " void" << endl;
            return false;
        }
        utente.newInfo(nuovoDato.assign(riga, pos, riga.find_last_not_of(" \t\f",duep-1)-pos+1));  //forse ci va -pos-1 perche prende n caratteri da 0 a n-1
        pos=riga.find_first_not_of(" \t\f",duep+1);
        if (pos==riga.rfind('}') || riga[duep+1]=='}' ){
            cerr << "ERROR[ line " << riesima << "]: field "<< ++_campo<< " void" << endl;
            return false;
        }
        utente.newInfo(nuovoDato.assign(riga, pos, riga.find_last_not_of(" \t\f",riga.rfind('}')-1)-pos+1));
    }else {
       cerr << "ERROR[ line " << riesima << "]: a user must make explicit mandatory information "
                                             "such as a call sign and,\in the case of simple users, a date of birth." << endl;
        return false;
    }
    for(i=0;i< utente.getInfo().size(); i+=2)
        for(int k=i+2;k< utente.getInfo().size(); k+=2) if(utente.getInfo()[i] == utente.getInfo()[k]){
        cerr<<" ERROR[ line "<< riesima << "]: field "<< i+1 << " duplicated" << endl;
                return false;
    }
    _idlist.emplace_front(&utente.getID());     //con & salvo nella lista l'indirizzo dell'id
    return verifyCampo(utente,riesima,_keywords[8]);
}

bool Logging::verifyCampo( const Utente &utente, int &riesima, string &keyword) {
    //Date ctrldata;
    string tmp;
    int i=0;
    for(;i< utente.getInfo().size() && utente.getInfo()[i] != keyword; i++);
        if(i== utente.getInfo().size() && first_request){
            cout<<"WARNING: missing parameter '"<< keyword<<"'"<<endl;
            cout<<"'"<<keyword<<"' is a mandatory parameter for the user of the line "<<riesima<<endl;
            cout<<"Insert an alternative keyword to '"<<keyword<<"'; enter 0 if it is not present."<<endl;
            first_request= false;
        getline(cin,tmp);
        if(tmp=="0") return false;
           keyword=tmp;
            return verifyCampo(utente, riesima, keyword);
        }
        if(i== utente.getInfo().size()){     //dovrebbe accadere solo se non è la prima volta e non ha trovato la keyword
            cerr << "ERROR[ line " << riesima << "]: parameter '"<< keyword<<"' not found."<<endl;
            return false;
        }
    first_request= true;       //preferirei chiedesse al + solo una volta per nome, una per cognome e una per data e bom
    if(i>1) {
        string info,val;
        info = utente.getInfo()[i];
        val = utente.getInfo()[i + 1];
        for (int k = i; k > 0; k -= 2) {
            utente.setInfo(k,utente.getInfo()[k - 2]);
            utente.setInfo(k + 1, utente.getInfo()[k - 1]);
        }
        utente.setInfo(0, info);
        utente.setInfo(1, val);
    }
    return true;
}

bool Logging::verifyRelations(ifstream &ifile, string &nomeFile) {
    string id1,id2,relazione;
    int riesima=1,j=0;
    short i=0;
    vector<const Semplice*> parentsResult,sonsResult;
    bool failed=false,newKey= false;
    ifile.seekg(0);
    const Semplice* semptr1,*semptr2;     //COSA SONO 5 PUNTATORI RISPETTO A, MAGARI, MIGLIAIA E MIGLIAIA DI ITERAZIONI
    const Azienda* azptr1,*azptr2;         // per ritrovare ogni volta gli id corrispondenti????
    const Gruppo* grptr;
    while(!ifile.eof() && ifile.peek()!=ifstream::traits_type::eof()){
        getline(ifile,_riga);
        if (_riga.find_first_not_of(" \t\v\n\r")!=-1){          //Verifica automaticamente anche che non sia vuota
            auto virgola=_riga.find(','),virgola2=_riga.find(',',virgola+1);  //così ti becchi gli id1
            id1.assign(_riga,0,virgola);
            id2.assign(_riga,virgola+1,virgola2-virgola-1);               //anche mi capitasse tipo id1-id2,, non toccherei oltre _riga.end()
            if(_riga.find(',',virgola2+1)==-1 &&
               id1.find_first_not_of(" \t\v\n\r")!=-1 && id2.find_first_not_of(" \t\v\n\r")!=-1){
            relazione.assign(_riga,virgola2+1,remove_blank(_riga.begin()+_riga.rfind(','),_riga.end())); //così becchi il tipo di relazione
            if(id1==id2){
                cerr<<"ERROR[ file "<< nomeFile<<" line " << riesima << "]: same id"<<endl;
                return false;
            }
            // in questo modo poi ti basta verificare 1: keywords giuste 2: id1 validi e tutti i controlli di formattazione vengono da se "implicitamente"
        for(i=0;i<9 && _keywords[i]!=relazione;i++); //se è arrivato a 8 so che non ha trovato la keyword
        newKey=false;
        for(int k=0;k<1;k++){
            switch(i) {      //c'è un ordine, il primo id è quello da cui parte la relazione
                case 0:
                    if ((semptr1 = _semplici.getById(id1)) != nullptr &&
                        (semptr2 = _semplici.getById(id2)) != nullptr) {
                        failed = !_semplici.manageRelation(semptr1, semptr2, semptr1->addAmici, semptr2->addAmici);
                    } else failed = true;
                    break;
                case 1:
                    if ((semptr1 = _semplici.getById(id1)) != nullptr &&
                        (semptr2 = _semplici.getById(id2)) != nullptr) {
                        failed = !(*semptr1).addConoscenze(semptr2);
                    } else failed = true;
                    break;
                case 2:
                    if ((semptr1 = _semplici.getById(id1)) != nullptr &&
                        (semptr2 = _semplici.getById(id2)) != nullptr || !semptr1->single() || !semptr2->single()) {
                        cerr << "ERROR[ file " << nomeFile << " line " << riesima
                             << "]: there can’t be a conjugal relationship between the two users" << endl;
                        return false;
                    } else _semplici.manageRelation(semptr1, semptr2, semptr1->addConiuge, semptr2->addConiuge);
                    break;
                case 3:
                    if ((semptr1 = _semplici.getById(id1)) != nullptr &&
                        (semptr2 = _semplici.getById(id2)) != nullptr && (*semptr1).getNumGenitori() < 2) {
                        failed != _semplici.manageRelation(semptr1, semptr2, semptr1->addGenitori, semptr2->addFigli);
                        parentsResult.push_back(semptr2);
                        sonsResult.push_back(semptr1);
                        controlParents(semptr2, parentsResult, i = 0);
                        controlSons(semptr1,sonsResult,i = 0);
                        if (find_first_of(parentsResult.begin(),parentsResult.end(),sonsResult.begin(),sonsResult.end())
                            !=parentsResult.end()) {
                            cerr << "ERROR[ file " << nomeFile << " line " << riesima
                                 << "]: there can’t be a parent-child relationship between the two users" << endl;
                            return false;
                        }
                        for (j = 0; j < parentsResult.size(); j++) parentsResult[j]->setVisitato('b');
                        for (j = 0; j < sonsResult.size(); j++) sonsResult[j]->setVisitato('b');
                        parentsResult.clear();
                        sonsResult.clear();
                        break;
                    } else {
                        cerr << "ERROR[ file " << nomeFile << " line " << riesima
                             << "]: between the two users there can’t be a parent-child relationship" << endl;
                        return false;
                    }
                case 4:
                    if ((semptr1 = _semplici.getById(id1)) != nullptr &&
                        (semptr2 = _semplici.getById(id2)) != nullptr && (*semptr2).getNumGenitori() < 2) {
                        failed = !_semplici.manageRelation(semptr2, semptr1, semptr2->addGenitori, semptr1->addFigli);
                        parentsResult.push_back(semptr1);
                        sonsResult.push_back(semptr2);
                        controlParents(semptr1, parentsResult, i = 0);
                        controlSons(semptr2,sonsResult,i = 0);
                        if (find_first_of(parentsResult.begin(),parentsResult.end(),sonsResult.begin(),sonsResult.end())
                            !=parentsResult.end()) {
                            cerr << "ERROR[ line " << riesima
                                 << "]: between the two users there can’t be a parent-child relationship" << endl;
                            return false;
                        }
                        for (j = 0; j < parentsResult.size(); j++) parentsResult[j]->setVisitato('b');
                        for (j = 0; j < sonsResult.size(); j++) sonsResult[j]->setVisitato('b');
                        parentsResult.clear();
                        sonsResult.clear();
                        break;
                    } else {
                        cerr << "ERROR[ file " << nomeFile << " line " << riesima
                             << "]: between the two users there can’t be a parent-child relationship" << endl;
                        return false;
                    }
                case 5:
                    if ((azptr1 = _aziende.getById(id1)) != nullptr && (azptr2 = _aziende.getById(id2)) != nullptr) {
                        failed = !(*azptr1).addConsociate(azptr2);
                    } else failed = true;
                    break;
                case 6:
                    if ((azptr1 = _aziende.getById(id1)) != nullptr && (semptr2 = _semplici.getById(id2)) != nullptr) {
                        failed = !_aziende.manageRelation(azptr1, semptr2, azptr1->addDipendenti, semptr2->addLavori);
                    } else failed = true;
                    break;
                case 7:
                    if ((grptr = _gruppi.getById(id1)) != nullptr && (semptr2 = _semplici.getById(id2)) != nullptr) {
                        failed = !_gruppi.manageRelation(grptr, semptr2, grptr->addMembri, semptr2->addAppartenenze);
                    } else failed = true;
                    break;
                default:                                //include il caso 8
                    if(newKey){
                        cerr << "ERROR[ file " << nomeFile << " line " << riesima << "]: invalid keyword or wrong format" << endl;
                        return false;
                    }
                    cout << "WARNING: no parameter has been found '" << relazione << "' in the line "
                         << riesima << "." << endl;
                    cout << "The following keywords are provided:" << endl;
                    for (i = 0; i < 8; i++) cout << i + 1 << ") " << _keywords[i] << " ";
                    cout << endl;
                    cout << "You can choose the number of a keyword to replace with the relation found.\n"
                            "WARNING: replacing a keyword in an irrelevant way results in an incorrect analysis of the system."
                            "\nEnter 0 to cancel." << endl;
                    cin >> i;
                    if (i == 0) {
                        cerr << "ERROR[ file " << nomeFile << " line " << riesima << "]: invalid keyword or wrong format." << endl;
                        return false;
                    }
                    _keywords[--i] = relazione;
                    newKey = true;
                    k=0;
                    break;
            }
        }

        if(failed) {
                cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: invalid IDs." << endl;
                return false;
        }
        } else {
                cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: wrong format."<<endl;
                return false;
        }
        }else{
            getline(ifile, _riga);
            if(!ifile.eof()){
                cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: empty line" << endl;
                return false;
            }
        }
        riesima++;
    }                                       //ritorno vero solo se ho finito i controlli su tutto il file
    return true;
}  //Devi dire o quali sono le keywords accettate o dare la possibilità di modificarle e
                                                    // che non vuoi spazzi nella formattazione (gli id possono contenerne)
bool Logging::verifyNews(ifstream &ifile, string &nomeFile) {
    string str_tmp,error_word="like";
    int riesima=1, idiesimo=1;
    int i=0;
    const Utente *multiuso= nullptr;
    const Semplice *opinions= nullptr;
    Date datanews;
    ifile.seekg(0);
    while(ifile.peek()!=ifstream::traits_type::eof() && !ifile.eof()){
        i=0;//CAZZOOOOOO find_last_of come tutte le '_of' basta che trovi uno dei caratteri secificati, non l'intera frase
        getline(ifile,_riga);
        auto pos=_riga.rfind('}'),lastlike=_riga.rfind('}',pos-1);
        if (_riga.find_first_not_of(" \t\v\n\r")!=-1) {                   //Occhio che la remove_blank ti sposta gli spazi prima dell'iteratore "last"
            if(pos!=-1 && pos==pos+remove_blank(_riga.begin() + pos+1, _riga.end()) &&
                    lastlike!=-1 && lastlike==_riga.rfind("},dislike:{") &&
                    _riga.rfind("},dislike:{")+10==(pos=_riga.rfind('{')) && pos!=-1 &&   //pos!=-1 controlla che non siano uguali ma xke enrambi nulli
                    _riga.rfind('{',pos-1)==(pos=_riga.rfind(",like:{")+6) && pos!=-1 &&
                    _riga.find('}',pos)==_riga.rfind("},dislike:{") && count(_riga.begin(),_riga.begin()+pos,',')>=3){       //pos ora punta alla { di like:{
                str_tmp.assign(_riga,_riga.rfind(',',pos-7)+1,10);         //i 10 caratteri li ho garantiti dai controlli precedenti (altrimenti sigseg)
                if(!datanews.isDate(str_tmp)){
                    cerr<<"ERROR[ file "<< nomeFile<<" line " << riesima << "]: date of publication invalid news."<<endl;
                    return false;
                }
               //Assign ritorna reference a stringa, substr ritorna un oggetto stringa
                str_tmp.assign(_riga,0,_riga.find(','));  //tanto non si salvasse in una temporanea qua, lo farei in getbyid in quanto non avrei una reference
                if(!(str_tmp.find_first_not_of(" \t\v\n\r")!=-1 && (((multiuso= _semplici.getById(str_tmp))!= nullptr ||
                        (multiuso=  _aziende.getById(str_tmp))!= nullptr) || (multiuso=  _gruppi.getById(str_tmp))!= nullptr))){
                    cerr<<"ERROR[ file "<< nomeFile<<" line " << riesima << "]: first field ID not found."<<endl;
                    return false;
                }
                str_tmp.assign(_riga,_riga.find(',')+1,_riga.rfind(",like:{")-_riga.find(',')-1-11);
                if(str_tmp.find_first_not_of(" \t\v\n\r")==-1){
                    cerr<<"ERROR[ file "<< nomeFile<<" line " << riesima << "]: mex field void."<<endl;
                    return false;
                }
                multiuso->addMex(str_tmp);
                multiuso->setData(datanews, 0);
                i=0;
                bool likeordis= true;
                if(_riga[_riga.find_first_not_of(" \t\v\n\r",pos+1)]=='}') {        //per verificare se è vuoto
                    i++;
                    pos=_riga.rfind('{');
                    likeordis= false;
                    error_word="dislike";
                    lastlike=_riga.rfind('}');
                }
                for(;i<2;i++) {
                    bool singolo=true;  //Per verificare che non vi siano id duplicati
                    idiesimo=1;
                    while (_riga.find(',', pos+1) < lastlike && singolo) { //fin che c'è ancora un like itero
                        str_tmp.assign(_riga,pos+1, _riga.find(',', pos+1) - 1 - pos);      //gia qua itero pos(pos=virgola successiva)
                        pos=_riga.find(',', pos +1);
                        if (str_tmp.find_first_not_of(" \t\v\n\r")!=-1 && (opinions = _semplici.getById(str_tmp)) != nullptr)
                            singolo=GestioneUtenti<Utente>::addOpinionGU(multiuso, opinions, 0, likeordis);
                        else {
                            cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: associated ID "<< error_word<<" number " << idiesimo
                                    << "not found. Remember: only simple users can express opinions." << endl;
                            return false;
                        }
                        idiesimo++;
                    }
                    str_tmp.assign(_riga,pos + 1, _riga.find('}', pos) - 1  - pos);     //Arrivato all'ultimo like/dislike
                    if (str_tmp.find_first_not_of(" \t\v\n\r")!=-1 && (opinions = _semplici.getById(str_tmp)) != nullptr && singolo)
                        singolo=GestioneUtenti<Utente>::addOpinionGU(multiuso, opinions, 0, likeordis);
                    else {
                        if(!singolo) {
                            cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: associated ID "<< error_word<<" number " << idiesimo
                                 << " Repeated. Remember: Simple users can only express one opinion per message." << endl;
                            return false;
                        }
                        cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: associated ID "<< error_word<<" number " << idiesimo
                             << "not found. Remember: only simple users can express opinions." << endl;
                        return false;
                    }
                    pos=_riga.rfind('{');
                    lastlike=_riga.rfind('}');
                    if(_riga[_riga.find_first_not_of(" \t\v\n\r",pos+1)]=='}') i++;
                    likeordis= false;
                    error_word="dislike";
                }
                bool duplicato=false;  //A quanto pare va tutto senza controllo ..List.empty()perche actual like e Dis sono NULL
                auto actualDis = multiuso->getDislikeListConst().begin();
                auto actualLike = multiuso->getLikeListConst().begin();
                if(multiuso->getLikeListConst().size()>1) {
                    for (i = 0; i < multiuso->getLikeListConst().size() - 1 && !duplicato; i++) {
                        if (find(++actualLike, multiuso->getLikeListConst().end(), *(--actualLike)) !=
                                multiuso->getLikeListConst().end()) duplicato = true;
                        actualLike++;
                    }
                }
                if(!duplicato && multiuso->getDislikeListConst().size()>1) {
                    for (i = 0; i < multiuso->getDislikeListConst().size() - 1 && !duplicato; i++) {
                        if (find(++actualDis, multiuso->getDislikeListConst().end(), *(--actualDis)) !=
                                multiuso->getDislikeListConst().end()) duplicato = true;
                        actualDis++;
                    }
                }
                if(!duplicato && find_first_of(multiuso->getLikeListConst().begin(), multiuso->getLikeListConst().end(),
                                               multiuso->getDislikeListConst().begin(), multiuso->getDislikeListConst().end())!=
                                         multiuso->getLikeListConst().end()) duplicato= true;
                if(duplicato) {
                   cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: a user may not express more than one opinion." << endl;
                   return false;
               }
            }else{
                cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: invalid format." << endl;
                return false;
            }
        }else{
            getline(ifile, _riga);
            if(!ifile.eof()){
                cerr << "ERROR[ file "<< nomeFile<<" line " << riesima << "]: empty line" << endl;
                return false;
            }
        }
        riesima++;
    }
    if(riesima>1)multiuso->sortMex();
    return true;
}   //Devi ancora dire che non vuoi spazi tra gli id e tra data e altre parole (gli id possono contenerne)

bool Logging::result() {
    if(!verifyInput(_file1, _nome1) || !verifyInput(_file2, _nome2) || !verifyInput(_file3, _nome3)) return false;
    short k=0;
    ifstream *vectStream[3]={&_file1,&_file2,&_file3};     //La & prima del nome li prende per reference perche non posso copiarli
    string vectName[3]={_nome1,_nome2,_nome3};
    for(short i=0;i<3;i++) {
        if (identifyFileUser(*vectStream[i]) == 'u') {
            if(!verifyUser(*vectStream[i], vectName[i])) return false;
            for (; k < 3; k++)
                if (k != i) {
                    switch(identifyFileUser(*vectStream[k])) {
                        case 'n':
                        if (!verifyNews(*vectStream[k], vectName[k]) ||
                            !verifyRelations(*vectStream[3 - (k + i)], vectName[3 - (k + i)])) return false;
                        _nome2 = vectName[k];
                        _nome3 = vectName[3 - (k + i)];       //Metto nome1=User, nome2=news, nome3=Relations
                        k = 4;
                            break;
                        case 'r':
                        if (!verifyRelations(*vectStream[k], vectName[k])
                            || !verifyNews(*vectStream[3 - (k + i)], vectName[3 - (k + i)])) return false;
                        _nome2 = vectName[3 - (k + i)];
                        _nome3 = vectName[k];                 //Metto nome1=User, nome2=news, nome3=Relations
                        k = 4;
                            break;
                        case 'z':
                            cerr<<"ERROR [file "<<vectName[k]<<"]: wrong format."<<endl;
                            return false;
                    }  //Lo è per forza, alla peggio vuoto ed è come se fossimo a fine file
            } //Se alla fine non è ne uno ne laltro è perche hai due vuoti per il quale devi chiede i nomi (controlla che non siano 'z')
            if (k != 5) {
                cout << "Couldn’t make out the files. Enter the names of the files on which  "
                        "the information will be printed.\nFile news:" << endl;
                getline(cin, vectName[--k]);
                cout << "File relations:" << endl;
                getline(cin, vectName[3 - (k + i)]);
                close(*vectStream[k]);
                close(*vectStream[3 - (k + i)]);
                if (!verifyInput(*vectStream[k], vectName[k]) || !verifyInput(*vectStream[3 - (k + i)], vectName[3 - (k + i)]))
                    return false;
                if (!verifyNews(*vectStream[k], vectName[k]) || !verifyRelations(*vectStream[3 - (k + i)], vectName[3 - (k + i)])) return false;
                _nome2 = vectName[k];
                _nome3 = vectName[3 - (k + i)];
                k=5;
            }
            _nome1 = vectName[i];
            i = 5;
        }
    }
    if(k!= 5 && (identifyFileUser(*vectStream[0])!='v'
                 || identifyFileUser(*vectStream[1])!='v' || identifyFileUser(*vectStream[2])!='v')) {
        cerr<<"ERROR:There are no users or file formatting is incorrect, "
              "it is therefore impossible to initialize the information."<<endl;
        return false;
    }
    else if(k!= 5) {
        cout << "Couldn’t make out the files. Enter the names of the files on which  "
                        "the information will be printed.\nFile users:" << endl;
        getline(cin,_nome1);
        cout<<"File news:"<<endl;
        getline(cin,_nome2);
        cout<<"File relations:"<<endl;
        getline(cin,_nome3);
        close(_file1);
        close(_file2);
        close(_file3);
        if(!verifyInput(_file1,_nome1) || !verifyInput(_file2,_nome2) || !verifyInput(_file3,_nome3)) return false;
        if(!verifyUser(_file1, _nome1) || !verifyNews(_file2, _nome2) || !verifyRelations(_file3, _nome3)) return false;
    }

    close(_file1);          //La chiusura avviene anche nel distruttore
    close(_file2);
    close(_file3);
   /* _semplici.setKeywords(_keywords);
    _aziende.setKeywords(_keywords);
    _gruppi.setKeywords(_keywords);*/
    return true;
}

unsigned short Logging::remove_blank(string::iterator first, string::iterator last)const {
    string::iterator result = first;
    unsigned short posizioni=0;
    while (first!=last) {
        if (!isblank(*first)) {
            *result = *first;
            ++result;
            ++posizioni;
        }
        ++first;
    }
    return posizioni;
}

const GestioneUtenti<Semplice> &Logging::getSempl() const{
   return _semplici;
}

const GestioneUtenti<Azienda> &Logging::getAz() const{
    return _aziende;
}

const GestioneUtenti<Gruppo> &Logging::getGr() const{
    return _gruppi;
}

void Logging::setFile(const string &a, const string &b, const string &c) {
    _nome1=a.c_str();
    _nome2=b.c_str();
    _nome3=c.c_str();
}

const string &Logging::keyword(int pos) const {
    return _keywords[pos];
}

bool Logging::controlParents( const Semplice *partenza,vector< const Semplice *> &fifo,int daesaminare)const {
    const Semplice* semptr;
       for(int i=0;i<partenza->getNumGenitori();i++){
        if((semptr=static_cast<const Semplice*>(&partenza->getGenitori(i)))->getVisitato()!='g'){
            semptr->setVisitato('g');
            fifo.emplace_back(semptr);
        }

       }
    if(fifo.size()>++daesaminare) return controlParents(fifo[daesaminare],fifo,daesaminare);
    else return true;

}

bool Logging::controlSons( const Semplice *partenza, vector< const Semplice *> &fifo, int daesaminare)const {
    const Semplice* semptr;
    for(int _i=0;_i<partenza->getNumFigli();_i++){
        if((semptr=static_cast<const Semplice*>(&partenza->getFiglio(_i)))->getVisitato()=='b'){
            semptr->setVisitato('g');
            fifo.emplace_back(semptr);
        }
    }
    if(fifo.size()>++daesaminare) return controlSons(fifo[daesaminare],fifo,daesaminare);
    else return true;
}


void Logging::getNameFile(string &user, string &news, string &relation) {
    user=_nome1;
    news=_nome2;
    relation=_nome3;
}





