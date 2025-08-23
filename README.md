# HopfieldNetwork
In questa sezione verrà tratttato come è stato costruito la cartella HopfieldSimulator al cui interno si trovano diverse classi.
## math/MathDimension.hpp
Inanzitutto parliamo di una classe che ho dovuto implementare per conoscere la dimensione matematica di un oggetto. Un intero, un double un float, sono contenuti nell'insieme R che è unidimensionale; i numeri complessi sono isomorfi a uno spazio bidimensionali, i quaternioni hanno 4 dimensioni ecc. Dato che le reti di Hopfield possono essere matematicamente generalizzate in cui vengono usati i vertici di un iperbcubo con un lato di lunghezza L, math/MathDimension.hpp permette di ricavare inanzitutto la dimensione di un tipo informatico e allo stesso modo può restituire proprio i vertici di quel tipo.
## math/Cast.hpp
Dato che nelle reti di hopfield si lavora sia con i double per la costruzione delle matrici sia con numeri interi per rappresentare lo stato di un singolo pixel: era necessario introdurre una funzione che trasformasse gli std::complex<int> in std::complex<double> e viceversa. Quindi questa classe fa un overload di static_cast della standard library aggiungendo questa conversione.

## math/RandomUtils.hpp
È il generatore dei numeri causuali

## Training Pattern 
Questa classe rappresenta il pattern originale con cui verrà addestrata la rete. L'input può essere sia una immagine che si trova in un percorso del proprio PC oppure può essere anche generato un regrid. Dato che l'utente può decidere se caricare una immagine oppure generare una immagine casuale, nel caso in cui l'utente ha inserito una immagine la funzione regrid manterrà i pixel della immagine originale mentre nel caso in cui l'utente voglia un pattern casuale la chiamata regrid genererà un pattern casuale nuovo da zero, in modo da evitare un appesantimento del codice in cui si tenta di ridimensionare un pattern casuale.

## Noisy Pattern
Noisy Pattern è un pattern che prende in input una immagine e si può aggiungere del rumore.

## Evolving Pattern
Evolving Pattern è il pattern che può essere modificato in modo dinamico per ottenere quindi la sua evoluzione.

## Hopfield Network
Questa classe contiene una matrice ch può essere modificata inserendo i pattern che si vuole e si può risolvere un pattern corrotto. Inoltre questa classe può calcolare l'energia intrinseca di un qualsiasi tipo di pattern, originale, corrotto oppure evoluto. Hopfield Network ha due rappresentazioni matriciali: una con Eigen, efficinte per le operazioni matematiche, e l'altra vettoriale, necessaria per importare o esportare i dati di questa matrice, infatti si ha un aggiornamento delle matrici solo nel momento di trascrizione o lettura dei dati.
In questa classe viene usata la libreria di Eigen che ottimizza le operazioni matriciali:
quindi a favore della mia funzione che opera solo a numeri reali:
        // metodo alternativo per fare la stessa cosa ma funzionante solo per numeri reali e non per numeri complessi 
        //     W_ij = Eigen::Matrix<matrix_type, Eigen::Dynamic, Eigen::Dynamic>::Zero(numberNeurons, numberNeurons);

        // for (int i = 0; i < numberNeurons; ++i) {
        //     for (int j = i; j < numberNeurons; ++j) {
                
        //         if(status!=nullptr){
        //         if(*status<0){
        //           return;
        //         }
        //            count++;
        //            *status = static_cast<float>(count) / totalIteration;
        //         }
                
               
        //         if(i==j){continue;}
        //           matrix_type product_p_ij = static_cast<matrix_type>(p[i] * p[j])* norm_factor;
        //     W_ij(i,j) += product_p_ij ;
        //     W_ij(j,i) += product_p_ij ;
            
        //     }
        // }
    Questa è stata sostituita con una versione generale che accettasse anche numeri complessi

## Coherence Set Pattern
CoherenceSetPattern contiene tre pattern; il pattern originale (TrainingPattern),  il pattern corrotto (Noisy Pattern) e il pattern dinamico (Evolving Pattern). Hopfield Network  può essere addestrato (trainNetworkWith..) con un qualsiasi tipo di pattern, l'importante è dare in input la funzione per estrarre i vettori. ResolvePattern prende in input un vettore e con la matrice dei pesi addestrata in grado di rimuovere il rumore presente nelle immagini corrotte. Esiste anche il resolvePattern che prende in input un CPS e modifica EvolvingPattern. 

## Hopfield Simulator
Hopfield Simulator  è la classe finale che contiene la classe HopfieldNetork e un array di CPS. Quindi questa classe è in grado di gestire in modo sicuro che il tutto sia facilemente gestibile ed evitare il più possibile comportamenti inaspettati. L'uso di un std::mutex era necessario per evitare che un utente chiami contemporaneamente funzioni che richiedono tanto calcolo computazionale. Ho dovuto separare il regrid in due funzioni: una privata e una pubblica, che attivasse il lock. Questo era necessario perchè all'interno di una funzione pubblica, chiamo regrid ma uscendo dalla funzione il lock si disattiva e quindi era necessario che rimanessi ancora all'interno del blocco