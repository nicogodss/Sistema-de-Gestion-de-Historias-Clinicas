#include <iostream> //permite usar cout, cin, etc.
#include <cstring> // permite usar funciones de manejo de cadenas como strcpy, strcmp, etc.
#include <fstream> //perimite usar ifstream, ofstream para manejo de archivos
#include <cstdio> // permite usar sprintf para formatear cadenas

using namespace std;


const char ARCHIVO_PACIENTES[] = "pacientes.dat";
// nombre del archivo para almacenar las consultas, cada registro incluye el codigo del paciente al que pertenece la consulta
const char ARCHIVO_CONSULTAS[] = "consultas.dat";
// nombre del archivo para generar el reporte general de pacientes
const char REPORTE_PACIENTES[] = "reporte_pacientes.txt";
// nombre del archivo para generar el reporte de historial de un paciente, se formateara como "historial_<codigoPaciente>.txt"

// un template es un aforma de hacer codigo generico que permite
// que la misma estructura funciones con diferentes tipos de datos 

// un nodo es un aestructura que gurada un dato y un puntero al siguiente nodo, formando asi una lista enlazada
// una lista enlazada es una estructura de datos dinamica que permite almacenar una cantidad variable de elementos, 
// cada elemento esta conectado al siguiente a traves de un puntero, 
//lo que facilita la insercion y eliminacion de elementos sin necesidad de mover otros elementos como en un arreglo tradicional

template <class T>
struct Nodo
{
    T info;// datos que alamcena el nodo (puede ser paciente o consulta)
    Nodo<T> *sig;// punteroal siguient enodo de la lista 
};


template <class T>
struct Lista
{
    Nodo<T> *cab;// puntero al primer nodo de la lista
};

template <class T>
void inicializarLista(Lista<T> &lista)
{
    lista.cab = NULL;
}

template <class T>
void insertarFinal(Lista<T> &lista, T dato)
//funcion que inserta un nuevo elemento al final de la lista, recibe la lista y el dato a insertar
{
    Nodo<T> *nuevo;// puntero al nodo que se va a crear para almacenar el nuevo dato
    Nodo<T> *aux;// puntero auxiliar para recorrer la lista y encontrar el final

    nuevo = new Nodo<T>;//se crea un nuevo nodo en memoria dinamica
    nuevo->info = dato;//se guarda el dato dentro del nodo
    nuevo->sig = NULL;// se indica que no apunta a nungun nodo(final de la lista)

    if (lista.cab == NULL)
    {
        //si la lista esta vacia, el nuevo nodo se convierte en la cabeza de la lista
        lista.cab = nuevo;
    }
    else
    {
        //si la lista no esta vacia, se recorre hasta el final y se enlaza el nuevo nodo
        aux = lista.cab;
        while (aux->sig != NULL)
        {
            aux = aux->sig;
        }
        aux->sig = nuevo; // el ultimo nodo ahora apunta al nuevo nodo, agregandolo al final de la lista
    }
}


/*Se define una estructura Nodo que guarda un dato y un puntero al siguiente nodo.
Luego se define una Lista que tiene un puntero a la cabeza.
Con template se hace genérico para que funcione con cualquier tipo de dato.
Después se inicializa la lista en NULL para indicar que está vacía.
Finalmente, la función insertarFinal crea un nodo dinámico,
y lo agrega al final recorriendo la lista si es necesario.*/


struct Consulta
{
    int codigoConsulta;
    char fecha[15];
    char motivo[100];
    char diagnostico[100];
    char medico[50];
    int activa;
};
/* esto contiene la estructura de una consulta el codigo de la consulta una fecha un motivo, diagnostico y medico
un diagnostico y el medico y si esta activa o no */ 

struct Paciente
{
    int codigo;
    char nombre[100];
    int edad;
    char genero[20];
    char tipoSangre[10];
    char telefono[30];
    Lista<Consulta> consultas;
};

/* aqui esta el estructura para almacenar informacion de un paciente en el archivo 
de pacientes el codigo del paciente el nombre edad genero su tipo de sangre y telefono ademas de la lista de consultas 
*/

struct RegistroPaciente
{
    int codigo;
    char nombre[100];
    int edad;
    char genero[20];
    char tipoSangre[10];
    char telefono[30];
};

/* aqui esta el estructura para almacenar informacion de una consulta en el archivo de consultas 
cad consulta contiene su codigo respectivo el nombre edad genero y toda la info del paciente */

struct RegistroConsulta
{
    int codigoPaciente;
    int codigoConsulta;
    char fecha[15];
    char motivo[100];
    char diagnostico[100];
    char medico[50];
    int activa;
};
/* esta estructura almacena la informacion de una consulta en el archivo de consultas 
cada consulta conteine su codigo respectivo el nombre edad genero y toda la info del paciente */


void mostrarEncabezado()
{
    cout << "\n==================================================\n";
    cout << "        SISTEMA DE HISTORIAS CLINICAS             \n";
    cout << "==================================================\n";
}
// solo decoracion para que se vea mas bonito el programa

void pausar()
{
    cout << "\nPresione ENTER para continuar...";
    cin.get();
}
// es una funcion para pausar la ejecucion del programa hasta que el usuario presione enter, 
//esto es util para que el usuario pueda leer los mensajes antes de continuar

void leerCadena(const char mensaje[], char cadena[], int tam)
{
    cout << mensaje;
    cin.getline(cadena, tam);
}
// esta funcion muestra un mensaje al usuario y luego lee una cadena de caracteres con espacios
// el mensaje es el texto que se muestra al usuario para indicarle que ingrese la informacion,
// la cadena es el arreglo donde se almacenara la informacion ingresada por el usuario y tam es el tamaño maximo de la cadena para evitar desbordamientos

int leerEntero(const char mensaje[])
{
    //lee un numero entero validando que la entrada sea correcta 
    int valor; // variable donde se guardara el numero ingresado por el usuario
    int valido = 0; // control (0= invalidao, 1= valido) para asegurarse de que el usuario ingrese un numero entero valido

    do
    {
        cout << mensaje; // muestra el mensaje al usuario para indicarle que ingrese un numero entero
        cin >> valor; // intenta leer el numero entero ingresado por el usuario

        if (cin.fail())
        {
            // si el usuario ingresa algo invalido (como texto o un numero con decimales), se limpia el error y se ignoran los caracteres restantes 
            cin.clear(); // limpia el error de flujo de entrada para permitir nuevas entradas
            cin.ignore(1000, '\n'); // descarta la entrada incorrecta hasta el siguiente salto de linea para evitar que afecte futuras lecturas
            cout << "Entrada invalida. Debe ingresar un numero.\n";
        }
        else
        {
            //entrada valida
            cin.ignore(1000, '\n');// limpiar buffer de entrada para evitar que caracteres sobrantes afecten futuras lecturas
            valido = 1; // marca como valido
        }

    } while (valido == 0); // se repite hasta que el usuario ingrese un numero entero valido

    return valor; //retonra el numero ingresado 
}

//aux es un puntero auxiliar que se usa para recorrer la lista sin alterar la estructura original.
Paciente *buscarPacientePorCodigo(Lista<Paciente> &lista, int codigo)
// busca un paciente en la lista por su codigo, recibiendo la lista de pacientes y el codigo a buscar, devuelve un puntero al paciente encontrado o NULL si no se encuentra
{
    Nodo<Paciente> *aux; // puntero auxliar para recorrer la lista de pacientes
    aux = lista.cab; // empieza desde la cabeza de la lista

    while (aux != NULL)
    {
        //recorre la lista de pacientes comparando el codigo de cada paciente con el codigo buscado
        if (aux->info.codigo == codigo)
        {
            //si se encuentra el paciente con el codigo buscado, se devuelve un puntero a ese paciente
            return &(aux->info); // retorna la direccion de memoria del paciente encontrado para que se pueda acceder a su informacion y modificarla si es necesario
        }
        aux = aux->sig; // avanza al siguiente nodo de la lista para seguir buscando
    }

    return NULL;// si no lo encuentra retona null para indicar que no se encontro el paciente con ese codigo
}

Consulta *buscarConsultaPorCodigo(Lista<Consulta> &lista, int codigoConsulta)
// busca una consulta en la lista de consultas por su codigo, recibiendo la lista de consultas y el codigo a buscar
{
    Nodo<Consulta> *aux;// pountero auxiliar para recorrer la lista de consultas
    aux = lista.cab;// empieza desde la cabeza de la lista de consultas

    while (aux != NULL)
    {
        //recorre la lista de consultas comparando el codigo de cada consulta con el codigo buscado
        if (aux->info.codigoConsulta == codigoConsulta)
        {
            // si encuentra la consulta con el codigo buscado, se devuelve un puntero a esa consulta
            return &(aux->info);
        }
        aux = aux->sig;// avanza al siguiente nodo de la lista para seguir buscando
    }

    return NULL;// no econtrado 
}

int existePaciente(Lista<Paciente> lista, int codigo)
// verifica si existe un paciente con el codigo dado en la lista de pacientes, recibiendo la lista y el codigo a verificar, devuelve 1 si existe o 0 si no existe
{
    Nodo<Paciente> *aux; // puntero uxiliar para recorrer la lista de pacientes
    aux = lista.cab; // emepiza desde la cabeza de la lista de pacientes

    while (aux != NULL)
    {
        //recorre toda la lista de pacientes comparando el codigo de cada paciente con el codigo dado
        if (aux->info.codigo == codigo)
        {
            return 1; //reotorna 1 si encuentra un paciente con el codigo dado, indicando que el paciente existe en la lista
        }
        aux = aux->sig;
    }

    return 0;/// retorna 0 si no existe ningun paciente con el codigo dado en la lista, indicando que el paciente no existe
}

int existeConsulta(Lista<Consulta> lista, int codigoConsulta)
//verifica si existe un paciente con ese codigo en la lista de consultas, recibiendo la lista de consultas y el codigo a verificar, devuelve 1 si existe o 0 si no existe
{
    Nodo<Consulta> *aux; // puntero auxiliar para recorrer la lista de consultas
    aux = lista.cab; // empieza desde la cabeza de la lista de consultas

    while (aux != NULL)
    {
        //recorre la lista de consultas comparando el codigo de cada consulta con el codigo dado
        if (aux->info.codigoConsulta == codigoConsulta)
        {
            return 1; // si encuentra al paciente retonra 1 osea si existe
        }
        aux = aux->sig;// siguient enodo
    }

    return 0;// rerorna 0 si no existe ningun paciente con el codigo dado en la lista de consultas
}


void guardarPacienteArchivo(Paciente p)
//// Guarda un paciente en el archivo binario
{
    RegistroPaciente rp; // estructura plana para guardar en archivo
    ofstream archivo; // obejto para escribir en archivo


    //copia los datos del paciente a la estructura de registro para guardar en el archivo
    rp.codigo = p.codigo;
    strcpy(rp.nombre, p.nombre);
    rp.edad = p.edad;
    strcpy(rp.genero, p.genero);
    strcpy(rp.tipoSangre, p.tipoSangre);
    strcpy(rp.telefono, p.telefono);

    archivo.open(ARCHIVO_PACIENTES, ios::binary | ios::app);
    //abre el archivo de pacientes en modo binario y de agregar (app) para no sobrescribir los datos existentes

    if (archivo.fail())
    {
        //verifica su hubo error al abrir
        cout << "Error al abrir archivo de pacientes.\n";
        return;
    }

    archivo.write((char *)&rp, sizeof(rp));
    //convierte la estructura rp a bytes y la guarda en el archivo, escribiendo sizeof(rp) bytes a partir de la direccion de rp
    archivo.close();
}

void guardarConsultaArchivo(int codigoPaciente, Consulta c)
// guarda una consulata en el archivo binario
{
    RegistroConsulta rc; // estructura para guardar en archivo
    ofstream archivo; //archivo de salida para escribir en el archivo

    //copia datos de la consulta al registro de consulta para guardar en el archivo
    rc.codigoPaciente = codigoPaciente;
    rc.codigoConsulta = c.codigoConsulta;
    strcpy(rc.fecha, c.fecha);
    strcpy(rc.motivo, c.motivo);
    strcpy(rc.diagnostico, c.diagnostico);
    strcpy(rc.medico, c.medico);
    rc.activa = c.activa;

    archivo.open(ARCHIVO_CONSULTAS, ios::binary | ios::app);
    // abre archivo en modo binario y agrega datos al final del archivo sin sobrescribir lo existente

    if (archivo.fail())
    {
        cout << "Error al abrir archivo de consultas.\n";
        return;
    }

    archivo.write((char *)&rc, sizeof(rc));
    //guara la estructura con bytes en el archivo, escribiendo sizeof(rc) bytes a partir de la direccion de rc
    archivo.close();
}

void cargarPacientes(Lista<Paciente> &lista)
// Carga pacientes desde archivo binario a la lista en memoria
{
    ifstream archivo;// Archivo de entrada
    RegistroPaciente rp;// Archivo de entrada

    archivo.open(ARCHIVO_PACIENTES, ios::binary);
     // Abre archivo en modo binario

    if (archivo.fail())// Si falla, no hace nada
    {
        return;
    }

    while (archivo.read((char *)&rp, sizeof(rp)))
    {
        // Lee paciente por paciente
        if (existePaciente(lista, rp.codigo) == 0)
        {
            // Evita duplicados

            Paciente p;
            // Copia datos del archivo a estructura en memoria
            p.codigo = rp.codigo;
            strcpy(p.nombre, rp.nombre);
            p.edad = rp.edad;
            strcpy(p.genero, rp.genero);
            strcpy(p.tipoSangre, rp.tipoSangre);
            strcpy(p.telefono, rp.telefono);
            inicializarLista(p.consultas); // Inicializa lista interna de consultas

            insertarFinal(lista, p);// Agrega paciente a la lista
        }
    }

    archivo.close();
}

void cargarConsultas(Lista<Paciente> &lista)
// Carga consultas y las asigna a su paciente correspondiente
{
    ifstream archivo;
    RegistroConsulta rc;

    archivo.open(ARCHIVO_CONSULTAS, ios::binary);

    if (archivo.fail())
    {
        return;
    }

    while (archivo.read((char *)&rc, sizeof(rc)))
    {
        Paciente *p;
        // Busca el paciente al que pertenece la consulta
        p = buscarPacientePorCodigo(lista, rc.codigoPaciente);

        if (p != NULL)
        {
            // Si el paciente existe
            if (existeConsulta(p->consultas, rc.codigoConsulta) == 0)
            {
                 // Evita duplicados
//copia datos de la consulta del archivo a una estructura en memoria y luego la inserta en la lista de consultas del paciente correspondiente
                Consulta c;
                c.codigoConsulta = rc.codigoConsulta;
                strcpy(c.fecha, rc.fecha);
                strcpy(c.motivo, rc.motivo);
                strcpy(c.diagnostico, rc.diagnostico);
                strcpy(c.medico, rc.medico);
                c.activa = rc.activa;

                insertarFinal(p->consultas, c);// Inserta en lista del paciente
            }
        }
    }

    archivo.close();
}

void reescribirArchivoConsultas(Lista<Paciente> listaPacientes)
// Reescribe TODO el archivo de consultas desde la memoria
{
    ofstream archivo;
    Nodo<Paciente> *auxPaciente;
    Nodo<Consulta> *auxConsulta;
    RegistroConsulta rc;

    archivo.open(ARCHIVO_CONSULTAS, ios::binary | ios::trunc);
    // Abre archivo y borra contenido anterior


    if (archivo.fail())
    {
        cout << "Error al reescribir archivo de consultas.\n";
        return;
    }

    auxPaciente = listaPacientes.cab;

    while (auxPaciente != NULL)
    {
        // Recorre pacientes
        auxConsulta = auxPaciente->info.consultas.cab;

        while (auxConsulta != NULL)
        {
            // Recorre consultas de cada paciente
            // Copia datos a estructura de archivo
            rc.codigoPaciente = auxPaciente->info.codigo;
            rc.codigoConsulta = auxConsulta->info.codigoConsulta;
            strcpy(rc.fecha, auxConsulta->info.fecha);
            strcpy(rc.motivo, auxConsulta->info.motivo);
            strcpy(rc.diagnostico, auxConsulta->info.diagnostico);
            strcpy(rc.medico, auxConsulta->info.medico);
            rc.activa = auxConsulta->info.activa;

            archivo.write((char *)&rc, sizeof(rc));
                // Escribe consulta en el archivo
            auxConsulta = auxConsulta->sig;// sigeuinte consulta
        }

        auxPaciente = auxPaciente->sig;// siguiente paciente
    }

    archivo.close();
}

/*Estas funciones cargan y guardan datos entre archivos binarios y memoria.
Primero se leen los pacientes y se almacenan en una lista enlazada.
Luego se leen las consultas y se asignan a su paciente correspondiente.
Finalmente, se puede reescribir el archivo de consultas recorriendo toda la lista en memoria.*/


void registrarPaciente(Lista<Paciente> &lista)
// Registra un nuevo paciente en el sistema
{
    Paciente p;// Variable para almacenar el paciente

    mostrarEncabezado();// Muestra título y decoración
    cout << "REGISTRO DE PACIENTE\n";
    cout << "--------------------------------------------------\n";

    p.codigo = leerEntero("Codigo del paciente: ");// Solicita código

    if (existePaciente(lista, p.codigo) == 1)
    {
         // Verifica si ya existe
        cout << "Ya existe un paciente con ese codigo.\n";
        return;
    }
    // Solicita datos del paciente y los guarda en la estructura
    leerCadena("Nombre del paciente: ", p.nombre, 100);
    p.edad = leerEntero("Edad del paciente: ");
    leerCadena("Genero del paciente: ", p.genero, 20);
    leerCadena("Tipo de sangre: ", p.tipoSangre, 10);
    leerCadena("Telefono: ", p.telefono, 30);

    inicializarLista(p.consultas);// Inicializa lista de consultas

    insertarFinal(lista, p);// Agrega a la lista de pacientes
    guardarPacienteArchivo(p);// Guarda en archivo


    cout << "Paciente registrado correctamente.\n";
}

void listarPacientes(Lista<Paciente> lista)
// Muestra todos los pacientes registrados en el sistema
{
    Nodo<Paciente> *aux;
    aux = lista.cab; // Empieza desde la cabeza de la lista de pacientes

    mostrarEncabezado();
    cout << "LISTADO DE PACIENTES\n";
    cout << "--------------------------------------------------\n";

    if (aux == NULL)
    {
         // Si no hay pacientes registrados, muestra mensaje y retorna
        cout << "No hay pacientes registrados.\n";
        return;
    }

    while (aux != NULL)
    {
         // Recorre la lista de pacientes y muestra su información
        cout << "Codigo        : " << aux->info.codigo << endl;
        cout << "Nombre        : " << aux->info.nombre << endl;
        cout << "Edad          : " << aux->info.edad << endl;
        cout << "Genero        : " << aux->info.genero << endl;
        cout << "Tipo Sangre   : " << aux->info.tipoSangre << endl;
        cout << "Telefono      : " << aux->info.telefono << endl;
        cout << "--------------------------------------------------\n";

        aux = aux->sig;
    }
}


void registrarConsulta(Lista<Paciente> &listaPacientes)
// Registra una consulta médica para un paciente dado su código, solicitando los detalles de la consulta y guardándola tanto en memoria como en el archivo.
{
    int codigoPaciente;
    Paciente *pacienteBuscado;
    Consulta c;

    mostrarEncabezado();
    cout << "REGISTRO DE CONSULTA MEDICA\n";
    cout << "--------------------------------------------------\n";

    codigoPaciente = leerEntero("Codigo del paciente: ");// Pide código del paciente
    pacienteBuscado = buscarPacientePorCodigo(listaPacientes, codigoPaciente); // Busca paciente en la lista

    if (pacienteBuscado == NULL)
    {
            // Si no se encuentra el paciente, muestra mensaje y retorna
        cout << "Paciente no encontrado.\n";
        return;
    }

    c.codigoConsulta = leerEntero("Codigo de la consulta: ");// Pide código de la consulta

    if (existeConsulta(pacienteBuscado->consultas, c.codigoConsulta) == 1)
    {
            // Verifica si ya existe una consulta con ese código para el paciente
        cout << "Ya existe una consulta con ese codigo para este paciente.\n";
        return;
    }
    // Solicita detalles de la consulta y los guarda en la estructura

    leerCadena("Fecha: ", c.fecha, 15);
    leerCadena("Motivo: ", c.motivo, 100);
    leerCadena("Diagnostico: ", c.diagnostico, 100);
    leerCadena("Medico: ", c.medico, 50);
    c.activa = 1; // Marca la consulta como activa

    insertarFinal(pacienteBuscado->consultas, c);
    // Inserta la consulta en la lista de consultas del paciente
    guardarConsultaArchivo(codigoPaciente, c);
    // Guarda la consulta en el archivo de consultas, asociándola al paciente mediante su código

    cout << "Consulta registrada correctamente.\n";
}

/*Estas funciones permiten registrar y mostrar información del sistema.
RegistrarPaciente solicita los datos del paciente, valida duplicados y lo guarda en memoria y archivo.
ListarPacientes recorre la lista y muestra todos los datos.
RegistrarConsulta busca el paciente, valida la consulta y la agrega a su lista y al archivo.*/

void verHistorialPaciente(Lista<Paciente> &listaPacientes)
// Muestra el historial clínico completo de un paciente, incluyendo sus datos personales y todas las consultas registradas, indicando si cada consulta está activa o eliminada lógicamente.
{
    int codigoPaciente; // Variable para almacenar el código del paciente a buscar
    Paciente *pacienteBuscado; // Puntero para almacenar la dirección del paciente encontrado en la lista
    Nodo<Consulta> *auxConsulta; // Puntero auxiliar para recorrer la lista de consultas del paciente encontrado

    mostrarEncabezado();
    cout << "HISTORIAL CLINICO DEL PACIENTE\n";
    cout << "--------------------------------------------------\n";

    codigoPaciente = leerEntero("Codigo del paciente: ");
    // Solicita al usuario el código del paciente cuyo historial se desea ver
    pacienteBuscado = buscarPacientePorCodigo(listaPacientes, codigoPaciente);
    // Busca el paciente en la lista utilizando su código, y almacena la dirección del paciente encontrado en pacienteBuscado

    if (pacienteBuscado == NULL)
    {
        cout << "Paciente no encontrado.\n";
        return;
    }
    // muestra datos personales del paciente encontrado, como su código, nombre, edad, género, tipo de sangre y teléfono
    cout << "\nDATOS DEL PACIENTE\n";
    cout << "Codigo        : " << pacienteBuscado->codigo << endl;
    cout << "Nombre        : " << pacienteBuscado->nombre << endl;
    cout << "Edad          : " << pacienteBuscado->edad << endl;
    cout << "Genero        : " << pacienteBuscado->genero << endl;
    cout << "Tipo Sangre   : " << pacienteBuscado->tipoSangre << endl;
    cout << "Telefono      : " << pacienteBuscado->telefono << endl;
    cout << "--------------------------------------------------\n";

    auxConsulta = pacienteBuscado->consultas.cab;
    // verifica si el paciente tiene consultas registradas, si no tiene muestra un mensaje indicando que no hay consultas y retorna

    if (auxConsulta == NULL)
    {
        cout << "Este paciente no tiene consultas registradas.\n";
        return;
    }

    while (auxConsulta != NULL)
    {
        //recorre los detalles de cada consulta del paciente, incluyendo el código de la consulta, fecha, motivo, diagnóstico, médico y el estado (activa o eliminada lógicamente)
        cout << "Codigo consulta : " << auxConsulta->info.codigoConsulta << endl;
        cout << "Fecha           : " << auxConsulta->info.fecha << endl;
        cout << "Motivo          : " << auxConsulta->info.motivo << endl;
        cout << "Diagnostico     : " << auxConsulta->info.diagnostico << endl;
        cout << "Medico          : " << auxConsulta->info.medico << endl;

        if (auxConsulta->info.activa == 1) // Si la consulta está activa, muestra "Activa", de lo contrario muestra "Eliminada logicamente"
        {
            cout << "Estado          : Activa\n";
        }
        else
        {
            cout << "Estado          : Eliminada logicamente\n";
        }

        cout << "--------------------------------------------------\n";
        auxConsulta = auxConsulta->sig;
    }
}

void modificarDiagnostico(Lista<Paciente> &listaPacientes)
// Modifica el diagnóstico de una consulta existente
{
    int codigoPaciente; // Variable para almacenar el código del paciente a buscar
    int codigoConsulta; // Variable para almacenar el código de la consulta a modificar
    Paciente *pacienteBuscado; // Puntero para almacenar la dirección del paciente encontrado en la lista
    Consulta *consultaBuscada; // Puntero para almacenar la dirección de la consulta encontrada en la lista de consultas del paciente

    mostrarEncabezado();
    cout << "MODIFICAR DIAGNOSTICO\n";
    cout << "--------------------------------------------------\n";

    codigoPaciente = leerEntero("Codigo del paciente: "); 
    // Solicita al usuario el código del paciente cuya consulta se desea modificar
    pacienteBuscado = buscarPacientePorCodigo(listaPacientes, codigoPaciente);
    // Busca el paciente en la lista utilizando su código, y almacena la dirección del paciente encontrado en pacienteBuscado

    if (pacienteBuscado == NULL)
    {
        cout << "Paciente no encontrado.\n";
        return;
    }

    codigoConsulta = leerEntero("Codigo de la consulta: ");
    // Solicita al usuario el código de la consulta a modificar
    consultaBuscada = buscarConsultaPorCodigo(pacienteBuscado->consultas, codigoConsulta);
    //// Modifica el diagnóstico de una consulta existente

    if (consultaBuscada == NULL)
    {
        cout << "Consulta no encontrada.\n";
        return;
    }

    leerCadena("Nuevo diagnostico: ", consultaBuscada->diagnostico, 100);
    // Solicita al usuario el nuevo diagnóstico para la consulta encontrada y lo guarda en la estructura de la consulta

    reescribirArchivoConsultas(listaPacientes);
    // Después de modificar el diagnóstico en memoria, se reescribe todo el archivo de consultas para reflejar el cambio, asegurando que la información en el archivo esté actualizada con los cambios realizados en memoria

    cout << "Diagnostico actualizado correctamente.\n";
}
/*Estas funciones permiten consultar y modificar información clínica.
verHistorialPaciente muestra todos los datos del paciente junto con sus consultas.
modificarDiagnostico permite cambiar el diagnóstico de una consulta específica,
y luego actualiza el archivo para mantener la persistencia.*/

void eliminarConsulta(Lista<Paciente> &listaPacientes)
// Elimina una consulta de forma lógica (sin borrarla físicamente)
{
    int codigoPaciente; // Variable para almacenar el código del paciente a buscar
    int codigoConsulta; // Variable para almacenar el código de la consulta a eliminar
    Paciente *pacienteBuscado; // Puntero para almacenar la dirección del paciente encontrado en la lista
    Consulta *consultaBuscada; // Puntero para almacenar la dirección de la consulta encontrada en la lista de consultas del paciente

    mostrarEncabezado();
    cout << "ELIMINAR CONSULTA (LOGICO)\n";
    cout << "--------------------------------------------------\n";

    codigoPaciente = leerEntero("Codigo del paciente: ");
    // Solicita al usuario el código del paciente cuya consulta se desea eliminar
    pacienteBuscado = buscarPacientePorCodigo(listaPacientes, codigoPaciente);
    // Busca el paciente en la lista utilizando su código, y almacena la dirección del paciente encontrado en pacienteBuscado

    if (pacienteBuscado == NULL)
    {
        cout << "Paciente no encontrado.\n";
        return;
    }

    codigoConsulta = leerEntero("Codigo de la consulta: ");
    // Solicita al usuario el código de la consulta a eliminar
    consultaBuscada = buscarConsultaPorCodigo(pacienteBuscado->consultas, codigoConsulta);
    // Busca la consulta en la lista de consultas del paciente utilizando su código, y almacena la dirección de la consulta encontrada en consultaBuscada

    if (consultaBuscada == NULL)
    {
        cout << "Consulta no encontrada.\n";
        return;
    }

    consultaBuscada->activa = 0;
    // En lugar de eliminar físicamente la consulta, se marca como inactiva (activa = 0) para indicar que ha sido eliminada lógicamente
    reescribirArchivoConsultas(listaPacientes);
    // Después de marcar la consulta como inactiva en memoria, se reescribe todo el archivo de consultas para reflejar el cambio, asegurando que la información en el archivo esté actualizada con los cambios realizados en memoria

    cout << "Consulta eliminada logicamente.\n";
}


void generarReportePacientes(Lista<Paciente> listaPacientes)
// Genera un archivo de texto con todos los pacientes
{
    ofstream archivo; 
    Nodo<Paciente> *auxPaciente; // Puntero auxiliar para recorrer la lista de pacientes

    archivo.open(REPORTE_PACIENTES);// Abre el archivo de texto para escribir el reporte general de pacientes

    if (archivo.fail())
    {
        cout << "Error al generar el reporte de pacientes.\n";
        return;
    }

    archivo << "==================================================\n";
    archivo << "         REPORTE GENERAL DE PACIENTES             \n";
    archivo << "==================================================\n\n";

    auxPaciente = listaPacientes.cab;
    // Verifica si hay pacientes registrados, si no hay muestra un mensaje indicando que no hay pacientes y retorna

    if (auxPaciente == NULL)
    {
        archivo << "No hay pacientes registrados.\n";
    }
    else
    {
        while (auxPaciente != NULL)
        {
                // Recorre la lista de pacientes y escribe su información en el archivo de texto, incluyendo código, nombre, edad, género, tipo de sangre y teléfono
            archivo << "Codigo      : " << auxPaciente->info.codigo << "\n";
            archivo << "Nombre      : " << auxPaciente->info.nombre << "\n";
            archivo << "Edad        : " << auxPaciente->info.edad << "\n";
            archivo << "Genero      : " << auxPaciente->info.genero << "\n";
            archivo << "Tipo Sangre : " << auxPaciente->info.tipoSangre << "\n";
            archivo << "Telefono    : " << auxPaciente->info.telefono << "\n";
            archivo << "--------------------------------------------------\n";

            auxPaciente = auxPaciente->sig;
        }
    }

    archivo.close();

    cout << "Reporte general generado correctamente en " << REPORTE_PACIENTES << endl;
}
/*Estas funciones permiten eliminar consultas y generar reportes.
La eliminación es lógica, marcando la consulta como inactiva sin borrarla.
El reporte recorre la lista de pacientes y genera un archivo de texto con su información.*/

void generarReporteHistorialPaciente(Lista<Paciente> &listaPacientes)
// Genera un archivo de texto con el historial clínico de un paciente específico
{
    int codigoPaciente;// Variable para almacenar el código del paciente a buscar
    Paciente *pacienteBuscado;// Puntero para almacenar la dirección del paciente encontrado en la lista
    Nodo<Consulta> *auxConsulta;// Puntero auxiliar para recorrer la lista de consultas del paciente encontrado
    char nombreArchivo[60];// Variable para almacenar el nombre del archivo de reporte específico para el paciente
    ofstream archivo;

    mostrarEncabezado();
    cout << "REPORTE DE HISTORIAL CLINICO\n";
    cout << "--------------------------------------------------\n";

    codigoPaciente = leerEntero("Codigo del paciente: ");
    // Solicita al usuario el código del paciente cuyo historial se desea generar en un archivo de texto
    pacienteBuscado = buscarPacientePorCodigo(listaPacientes, codigoPaciente);
    // Busca el paciente en la lista utilizando su código, y almacena la dirección del paciente encontrado en pacienteBuscado

    if (pacienteBuscado == NULL)
    {
        cout << "Paciente no encontrado.\n";
        return;
    }

    sprintf(nombreArchivo, "historial_%d.txt", codigoPaciente);
    // Genera un nombre de archivo específico para el paciente utilizando su código, por ejemplo "historial_123.txt" para un paciente con código 123

    archivo.open(nombreArchivo);

    if (archivo.fail())
    {
        cout << "Error al generar el reporte del historial.\n";
        return;
    }

    archivo << "==================================================\n";
    archivo << "        HISTORIAL CLINICO DEL PACIENTE            \n";
    archivo << "==================================================\n\n";

    // Escribe los datos personales del paciente al inicio del archivo de texto, incluyendo código, nombre, edad, género, tipo de sangre y teléfono
    archivo << "Codigo      : " << pacienteBuscado->codigo << "\n";
    archivo << "Nombre      : " << pacienteBuscado->nombre << "\n";
    archivo << "Edad        : " << pacienteBuscado->edad << "\n";
    archivo << "Genero      : " << pacienteBuscado->genero << "\n";
    archivo << "Tipo Sangre : " << pacienteBuscado->tipoSangre << "\n";
    archivo << "Telefono    : " << pacienteBuscado->telefono << "\n";
    archivo << "--------------------------------------------------\n\n";

    auxConsulta = pacienteBuscado->consultas.cab;
    // Verifica si el paciente tiene consultas registradas, si no tiene muestra un mensaje indicando que no hay consultas y retorna

    if (auxConsulta == NULL)
    {
        archivo << "Este paciente no tiene consultas registradas.\n";
    }
    else
    {
        while (auxConsulta != NULL)
        {
                // Recorre la lista de consultas del paciente y escribe los detalles de cada consulta en el archivo de texto, incluyendo código de consulta, fecha, motivo, diagnóstico, médico y el estado (activa o eliminada lógicamente)
            archivo << "Codigo consulta : " << auxConsulta->info.codigoConsulta << "\n";
            archivo << "Fecha           : " << auxConsulta->info.fecha << "\n";
            archivo << "Motivo          : " << auxConsulta->info.motivo << "\n";
            archivo << "Diagnostico     : " << auxConsulta->info.diagnostico << "\n";
            archivo << "Medico          : " << auxConsulta->info.medico << "\n";

            if (auxConsulta->info.activa == 1)
            {
                archivo << "Estado          : Activa\n";
            }
            else
            {
                archivo << "Estado          : Eliminada logicamente\n";
            }

            archivo << "--------------------------------------------------\n";

            auxConsulta = auxConsulta->sig;
        }
    }

    archivo.close();

    cout << "Reporte de historial generado correctamente en " << nombreArchivo << endl;
}


void liberarConsultas(Lista<Consulta> &listaConsultas)
// Libera la memoria de las consultas de un paciente, eliminando cada nodo de la lista de consultas y estableciendo la cabeza de la lista a NULL al finalizar
{
    Nodo<Consulta> *aux;// Puntero auxiliar para recorrer la lista de consultas
    Nodo<Consulta> *borrar;// Puntero para almacenar la dirección del nodo que se va a eliminar

    aux = listaConsultas.cab;
    // Recorre la lista de consultas y elimina cada nodo, liberando la memoria asignada para cada consulta

    while (aux != NULL)
    {
        // Almacena la dirección del nodo actual en borrar, avanza el puntero aux al siguiente nodo y luego elimina el nodo almacenado en borrar para liberar la memoria
        borrar = aux; // Guarda la dirección del nodo actual para eliminarlo después de avanzar el puntero aux
        aux = aux->sig; // Avanza al siguiente nodo antes de eliminar el nodo actual para no perder la referencia a la lista
        delete borrar; // Elimina el nodo actual para liberar la memoria asignada a esa consulta
    }

    listaConsultas.cab = NULL;
    // Después de eliminar todos los nodos, se establece la cabeza de la lista de consultas a NULL para indicar que la lista está vacía y evitar referencias a memoria liberada
}

void liberarPacientes(Lista<Paciente> &listaPacientes)
// Libera la memoria de los pacientes y sus consultas, recorriendo la lista de pacientes, liberando las consultas de cada paciente y luego eliminando cada nodo de la lista de pacientes, estableciendo la cabeza de la lista a NULL al finalizar
{
    Nodo<Paciente> *aux; // Puntero auxiliar para recorrer la lista de pacientes
    Nodo<Paciente> *borrar; // Puntero para almacenar la dirección del nodo que se va a eliminar

    aux = listaPacientes.cab;
        //  Recorre la lista de pacientes y para cada paciente, llama a liberarConsultas para liberar la memoria de sus consultas antes de eliminar el nodo del paciente

    while (aux != NULL)
    {
        // Para cada paciente, se llama a liberarConsultas para liberar la memoria de las consultas asociadas a ese paciente antes de eliminar el nodo del paciente
        liberarConsultas(aux->info.consultas);
        // Después de liberar las consultas del paciente, se almacena la dirección del nodo actual en borrar, se avanza el puntero aux al siguiente nodo y luego se elimina el nodo almacenado en borrar para liberar la memoria del paciente
        borrar = aux;
        // Avanza al siguiente nodo antes de eliminar el nodo actual para no perder la referencia a la lista
        aux = aux->sig;
        // Elimina el nodo actual para liberar la memoria asignada a ese paciente
        delete borrar;
        // Después de eliminar el nodo del paciente, se continúa con el siguiente paciente en la lista hasta que se hayan liberado todos los pacientes y sus consultas
    }

    listaPacientes.cab = NULL;
}


int main()
{
    // Variable para almacenar la lista de pacientes y la opción del menú
    Lista<Paciente> lista;
    int op;

    inicializarLista(lista);// Inicializa la lista de pacientes

    cargarPacientes(lista);// Carga los pacientes desde el archivo binario a la lista en memoria
    cargarConsultas(lista);// Carga las consultas desde el archivo binario y las asigna a su paciente correspondiente en la lista en memoria

    do
    {
        
        mostrarEncabezado();
        cout << "1. Registrar paciente\n";
        cout << "2. Listar pacientes\n";
        cout << "3. Registrar consulta medica\n";
        cout << "4. Ver historial clinico\n";
        cout << "5. Modificar diagnostico\n";
        cout << "6. Eliminar consulta\n";
        cout << "7. Generar reporte general de pacientes\n";
        cout << "8. Generar reporte de historial de un paciente\n";
        cout << "9. Salir\n";
        cout << "--------------------------------------------------\n";

        op = leerEntero("Opcion: ");// Solicita al usuario que ingrese una opción del menú para realizar una acción específica en el sistema de gestión de historia clínica de pacientes

        switch (op)
        {
            case 1:
                registrarPaciente(lista);
                pausar();
                break;

            case 2:
                listarPacientes(lista);
                pausar();
                break;

            case 3:
                registrarConsulta(lista);
                pausar();
                break;

            case 4:
                verHistorialPaciente(lista);
                pausar();
                break;

            case 5:
                modificarDiagnostico(lista);
                pausar();
                break;

            case 6:
                eliminarConsulta(lista);
                pausar();
                break;

            case 7:
                generarReportePacientes(lista);
                pausar();
                break;

            case 8:
                generarReporteHistorialPaciente(lista);
                pausar();
                break;

            case 9:
                cout << "Saliendo...\n";
                break;

            default:
                cout << "Opcion invalida.\n";
                pausar();
                break;
        }

    } while (op != 9);

    liberarPacientes(lista);// Antes de salir, libera la memoria de los pacientes y sus consultas para evitar fugas de memoria

    return 0;
}

/*Es un sistema que usa listas enlazadas y archivos binarios
para gestionar pacientes y consultas médicas,
permitiendo registrar, modificar, eliminar lógicamente y generar reportes.*/
//23/03/2026
