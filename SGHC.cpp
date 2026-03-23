#include <iostream>
#include <cstring>
#include <fstream>
#include <cstdio>

using namespace std;

// ============================================================
// CONSTANTES DE ARCHIVOS
// ============================================================
const char ARCHIVO_PACIENTES[] = "pacientes.dat";
const char ARCHIVO_CONSULTAS[] = "consultas.dat";
const char REPORTE_PACIENTES[] = "reporte_pacientes.txt";

// ============================================================
// LISTA ENLAZADA TEMPLATE
// ============================================================
template <class T>
struct Nodo
{
    T info;
    Nodo<T> *sig;
};

template <class T>
struct Lista
{
    Nodo<T> *cab;
};

template <class T>
void inicializarLista(Lista<T> &lista)
{
    lista.cab = NULL;
}

template <class T>
void insertarFinal(Lista<T> &lista, T dato)
{
    Nodo<T> *nuevo;
    Nodo<T> *aux;

    nuevo = new Nodo<T>;
    nuevo->info = dato;
    nuevo->sig = NULL;

    if (lista.cab == NULL)
    {
        lista.cab = nuevo;
    }
    else
    {
        aux = lista.cab;
        while (aux->sig != NULL)
        {
            aux = aux->sig;
        }
        aux->sig = nuevo;
    }
}

// ============================================================
// ESTRUCTURAS EN MEMORIA
// ============================================================
struct Consulta
{
    int codigoConsulta;
    char fecha[15];
    char motivo[100];
    char diagnostico[100];
    char medico[50];
    int activa;
};

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

// ============================================================
// ESTRUCTURAS PARA ARCHIVOS BINARIOS
// ============================================================
struct RegistroPaciente
{
    int codigo;
    char nombre[100];
    int edad;
    char genero[20];
    char tipoSangre[10];
    char telefono[30];
};

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

// ============================================================
// FUNCIONES UTILITARIAS
// ============================================================
void mostrarEncabezado()
{
    cout << "\n==================================================\n";
    cout << "        SISTEMA DE HISTORIAS CLINICAS             \n";
    cout << "==================================================\n";
}

void pausar()
{
    cout << "\nPresione ENTER para continuar...";
    cin.get();
}

void leerCadena(const char mensaje[], char cadena[], int tam)
{
    cout << mensaje;
    cin.getline(cadena, tam);
}

int leerEntero(const char mensaje[])
{
    int valor;
    int valido = 0;

    do
    {
        cout << mensaje;
        cin >> valor;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Entrada invalida. Debe ingresar un numero.\n";
        }
        else
        {
            cin.ignore(1000, '\n');
            valido = 1;
        }

    } while (valido == 0);

    return valor;
}

// ============================================================
// FUNCIONES DE BUSQUEDA
// ============================================================
Paciente *buscarPacientePorCodigo(Lista<Paciente> &lista, int codigo)
{
    Nodo<Paciente> *aux;
    aux = lista.cab;

    while (aux != NULL)
    {
        if (aux->info.codigo == codigo)
        {
            return &(aux->info);
        }
        aux = aux->sig;
    }

    return NULL;
}

Consulta *buscarConsultaPorCodigo(Lista<Consulta> &lista, int codigoConsulta)
{
    Nodo<Consulta> *aux;
    aux = lista.cab;

    while (aux != NULL)
    {
        if (aux->info.codigoConsulta == codigoConsulta)
        {
            return &(aux->info);
        }
        aux = aux->sig;
    }

    return NULL;
}

int existePaciente(Lista<Paciente> lista, int codigo)
{
    Nodo<Paciente> *aux;
    aux = lista.cab;

    while (aux != NULL)
    {
        if (aux->info.codigo == codigo)
        {
            return 1;
        }
        aux = aux->sig;
    }

    return 0;
}

int existeConsulta(Lista<Consulta> lista, int codigoConsulta)
{
    Nodo<Consulta> *aux;
    aux = lista.cab;

    while (aux != NULL)
    {
        if (aux->info.codigoConsulta == codigoConsulta)
        {
            return 1;
        }
        aux = aux->sig;
    }

    return 0;
}

// ============================================================
// FUNCIONES DE ARCHIVOS BINARIOS
// ============================================================
void guardarPacienteArchivo(Paciente p)
{
    RegistroPaciente rp;
    ofstream archivo;

    rp.codigo = p.codigo;
    strcpy(rp.nombre, p.nombre);
    rp.edad = p.edad;
    strcpy(rp.genero, p.genero);
    strcpy(rp.tipoSangre, p.tipoSangre);
    strcpy(rp.telefono, p.telefono);

    archivo.open(ARCHIVO_PACIENTES, ios::binary | ios::app);

    if (archivo.fail())
    {
        cout << "Error al abrir archivo de pacientes.\n";
        return;
    }

    archivo.write((char *)&rp, sizeof(rp));
    archivo.close();
}

void guardarConsultaArchivo(int codigoPaciente, Consulta c)
{
    RegistroConsulta rc;
    ofstream archivo;

    rc.codigoPaciente = codigoPaciente;
    rc.codigoConsulta = c.codigoConsulta;
    strcpy(rc.fecha, c.fecha);
    strcpy(rc.motivo, c.motivo);
    strcpy(rc.diagnostico, c.diagnostico);
    strcpy(rc.medico, c.medico);
    rc.activa = c.activa;

    archivo.open(ARCHIVO_CONSULTAS, ios::binary | ios::app);

    if (archivo.fail())
    {
        cout << "Error al abrir archivo de consultas.\n";
        return;
    }

    archivo.write((char *)&rc, sizeof(rc));
    archivo.close();
}

void cargarPacientes(Lista<Paciente> &lista)
{
    ifstream archivo;
    RegistroPaciente rp;

    archivo.open(ARCHIVO_PACIENTES, ios::binary);

    if (archivo.fail())
    {
        return;
    }

    while (archivo.read((char *)&rp, sizeof(rp)))
    {
        if (existePaciente(lista, rp.codigo) == 0)
        {
            Paciente p;
            p.codigo = rp.codigo;
            strcpy(p.nombre, rp.nombre);
            p.edad = rp.edad;
            strcpy(p.genero, rp.genero);
            strcpy(p.tipoSangre, rp.tipoSangre);
            strcpy(p.telefono, rp.telefono);
            inicializarLista(p.consultas);

            insertarFinal(lista, p);
        }
    }

    archivo.close();
}

void cargarConsultas(Lista<Paciente> &lista)
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
        p = buscarPacientePorCodigo(lista, rc.codigoPaciente);

        if (p != NULL)
        {
            if (existeConsulta(p->consultas, rc.codigoConsulta) == 0)
            {
                Consulta c;
                c.codigoConsulta = rc.codigoConsulta;
                strcpy(c.fecha, rc.fecha);
                strcpy(c.motivo, rc.motivo);
                strcpy(c.diagnostico, rc.diagnostico);
                strcpy(c.medico, rc.medico);
                c.activa = rc.activa;

                insertarFinal(p->consultas, c);
            }
        }
    }

    archivo.close();
}

void reescribirArchivoConsultas(Lista<Paciente> listaPacientes)
{
    ofstream archivo;
    Nodo<Paciente> *auxPaciente;
    Nodo<Consulta> *auxConsulta;
    RegistroConsulta rc;

    archivo.open(ARCHIVO_CONSULTAS, ios::binary | ios::trunc);

    if (archivo.fail())
    {
        cout << "Error al reescribir archivo de consultas.\n";
        return;
    }

    auxPaciente = listaPacientes.cab;

    while (auxPaciente != NULL)
    {
        auxConsulta = auxPaciente->info.consultas.cab;

        while (auxConsulta != NULL)
        {
            rc.codigoPaciente = auxPaciente->info.codigo;
            rc.codigoConsulta = auxConsulta->info.codigoConsulta;
            strcpy(rc.fecha, auxConsulta->info.fecha);
            strcpy(rc.motivo, auxConsulta->info.motivo);
            strcpy(rc.diagnostico, auxConsulta->info.diagnostico);
            strcpy(rc.medico, auxConsulta->info.medico);
            rc.activa = auxConsulta->info.activa;

            archivo.write((char *)&rc, sizeof(rc));
            auxConsulta = auxConsulta->sig;
        }

        auxPaciente = auxPaciente->sig;
    }

    archivo.close();
}

// ============================================================
// FUNCIONES DE PACIENTES
// ============================================================
void registrarPaciente(Lista<Paciente> &lista)
{
    Paciente p;

    mostrarEncabezado();
    cout << "REGISTRO DE PACIENTE\n";
    cout << "--------------------------------------------------\n";

    p.codigo = leerEntero("Codigo del paciente: ");

    if (existePaciente(lista, p.codigo) == 1)
    {
        cout << "Ya existe un paciente con ese codigo.\n";
        return;
    }

    leerCadena("Nombre del paciente: ", p.nombre, 100);
    p.edad = leerEntero("Edad del paciente: ");
    leerCadena("Genero del paciente: ", p.genero, 20);
    leerCadena("Tipo de sangre: ", p.tipoSangre, 10);
    leerCadena("Telefono: ", p.telefono, 30);

    inicializarLista(p.consultas);

    insertarFinal(lista, p);
    guardarPacienteArchivo(p);

    cout << "Paciente registrado correctamente.\n";
}

void listarPacientes(Lista<Paciente> lista)
{
    Nodo<Paciente> *aux;
    aux = lista.cab;

    mostrarEncabezado();
    cout << "LISTADO DE PACIENTES\n";
    cout << "--------------------------------------------------\n";

    if (aux == NULL)
    {
        cout << "No hay pacientes registrados.\n";
        return;
    }

    while (aux != NULL)
    {
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

// ============================================================
// FUNCIONES DE CONSULTAS
// ============================================================
void registrarConsulta(Lista<Paciente> &listaPacientes)
{
    int codigoPaciente;
    Paciente *pacienteBuscado;
    Consulta c;

    mostrarEncabezado();
    cout << "REGISTRO DE CONSULTA MEDICA\n";
    cout << "--------------------------------------------------\n";

    codigoPaciente = leerEntero("Codigo del paciente: ");
    pacienteBuscado = buscarPacientePorCodigo(listaPacientes, codigoPaciente);

    if (pacienteBuscado == NULL)
    {
        cout << "Paciente no encontrado.\n";
        return;
    }

    c.codigoConsulta = leerEntero("Codigo de la consulta: ");

    if (existeConsulta(pacienteBuscado->consultas, c.codigoConsulta) == 1)
    {
        cout << "Ya existe una consulta con ese codigo para este paciente.\n";
        return;
    }

    leerCadena("Fecha: ", c.fecha, 15);
    leerCadena("Motivo: ", c.motivo, 100);
    leerCadena("Diagnostico: ", c.diagnostico, 100);
    leerCadena("Medico: ", c.medico, 50);
    c.activa = 1;

    insertarFinal(pacienteBuscado->consultas, c);
    guardarConsultaArchivo(codigoPaciente, c);

    cout << "Consulta registrada correctamente.\n";
}

void verHistorialPaciente(Lista<Paciente> &listaPacientes)
{
    int codigoPaciente;
    Paciente *pacienteBuscado;
    Nodo<Consulta> *auxConsulta;

    mostrarEncabezado();
    cout << "HISTORIAL CLINICO DEL PACIENTE\n";
    cout << "--------------------------------------------------\n";

    codigoPaciente = leerEntero("Codigo del paciente: ");
    pacienteBuscado = buscarPacientePorCodigo(listaPacientes, codigoPaciente);

    if (pacienteBuscado == NULL)
    {
        cout << "Paciente no encontrado.\n";
        return;
    }

    cout << "\nDATOS DEL PACIENTE\n";
    cout << "Codigo        : " << pacienteBuscado->codigo << endl;
    cout << "Nombre        : " << pacienteBuscado->nombre << endl;
    cout << "Edad          : " << pacienteBuscado->edad << endl;
    cout << "Genero        : " << pacienteBuscado->genero << endl;
    cout << "Tipo Sangre   : " << pacienteBuscado->tipoSangre << endl;
    cout << "Telefono      : " << pacienteBuscado->telefono << endl;
    cout << "--------------------------------------------------\n";

    auxConsulta = pacienteBuscado->consultas.cab;

    if (auxConsulta == NULL)
    {
        cout << "Este paciente no tiene consultas registradas.\n";
        return;
    }

    while (auxConsulta != NULL)
    {
        cout << "Codigo consulta : " << auxConsulta->info.codigoConsulta << endl;
        cout << "Fecha           : " << auxConsulta->info.fecha << endl;
        cout << "Motivo          : " << auxConsulta->info.motivo << endl;
        cout << "Diagnostico     : " << auxConsulta->info.diagnostico << endl;
        cout << "Medico          : " << auxConsulta->info.medico << endl;

        if (auxConsulta->info.activa == 1)
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
{
    int codigoPaciente;
    int codigoConsulta;
    Paciente *pacienteBuscado;
    Consulta *consultaBuscada;

    mostrarEncabezado();
    cout << "MODIFICAR DIAGNOSTICO\n";
    cout << "--------------------------------------------------\n";

    codigoPaciente = leerEntero("Codigo del paciente: ");
    pacienteBuscado = buscarPacientePorCodigo(listaPacientes, codigoPaciente);

    if (pacienteBuscado == NULL)
    {
        cout << "Paciente no encontrado.\n";
        return;
    }

    codigoConsulta = leerEntero("Codigo de la consulta: ");
    consultaBuscada = buscarConsultaPorCodigo(pacienteBuscado->consultas, codigoConsulta);

    if (consultaBuscada == NULL)
    {
        cout << "Consulta no encontrada.\n";
        return;
    }

    leerCadena("Nuevo diagnostico: ", consultaBuscada->diagnostico, 100);

    reescribirArchivoConsultas(listaPacientes);

    cout << "Diagnostico actualizado correctamente.\n";
}

void eliminarConsulta(Lista<Paciente> &listaPacientes)
{
    int codigoPaciente;
    int codigoConsulta;
    Paciente *pacienteBuscado;
    Consulta *consultaBuscada;

    mostrarEncabezado();
    cout << "ELIMINAR CONSULTA (LOGICO)\n";
    cout << "--------------------------------------------------\n";

    codigoPaciente = leerEntero("Codigo del paciente: ");
    pacienteBuscado = buscarPacientePorCodigo(listaPacientes, codigoPaciente);

    if (pacienteBuscado == NULL)
    {
        cout << "Paciente no encontrado.\n";
        return;
    }

    codigoConsulta = leerEntero("Codigo de la consulta: ");
    consultaBuscada = buscarConsultaPorCodigo(pacienteBuscado->consultas, codigoConsulta);

    if (consultaBuscada == NULL)
    {
        cout << "Consulta no encontrada.\n";
        return;
    }

    consultaBuscada->activa = 0;
    reescribirArchivoConsultas(listaPacientes);

    cout << "Consulta eliminada logicamente.\n";
}

// ============================================================
// FUNCIONES DE REPORTES
// ============================================================
void generarReportePacientes(Lista<Paciente> listaPacientes)
{
    ofstream archivo;
    Nodo<Paciente> *auxPaciente;

    archivo.open(REPORTE_PACIENTES);

    if (archivo.fail())
    {
        cout << "Error al generar el reporte de pacientes.\n";
        return;
    }

    archivo << "==================================================\n";
    archivo << "         REPORTE GENERAL DE PACIENTES             \n";
    archivo << "==================================================\n\n";

    auxPaciente = listaPacientes.cab;

    if (auxPaciente == NULL)
    {
        archivo << "No hay pacientes registrados.\n";
    }
    else
    {
        while (auxPaciente != NULL)
        {
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

void generarReporteHistorialPaciente(Lista<Paciente> &listaPacientes)
{
    int codigoPaciente;
    Paciente *pacienteBuscado;
    Nodo<Consulta> *auxConsulta;
    char nombreArchivo[60];
    ofstream archivo;

    mostrarEncabezado();
    cout << "REPORTE DE HISTORIAL CLINICO\n";
    cout << "--------------------------------------------------\n";

    codigoPaciente = leerEntero("Codigo del paciente: ");
    pacienteBuscado = buscarPacientePorCodigo(listaPacientes, codigoPaciente);

    if (pacienteBuscado == NULL)
    {
        cout << "Paciente no encontrado.\n";
        return;
    }

    sprintf(nombreArchivo, "historial_%d.txt", codigoPaciente);

    archivo.open(nombreArchivo);

    if (archivo.fail())
    {
        cout << "Error al generar el reporte del historial.\n";
        return;
    }

    archivo << "==================================================\n";
    archivo << "        HISTORIAL CLINICO DEL PACIENTE            \n";
    archivo << "==================================================\n\n";

    archivo << "Codigo      : " << pacienteBuscado->codigo << "\n";
    archivo << "Nombre      : " << pacienteBuscado->nombre << "\n";
    archivo << "Edad        : " << pacienteBuscado->edad << "\n";
    archivo << "Genero      : " << pacienteBuscado->genero << "\n";
    archivo << "Tipo Sangre : " << pacienteBuscado->tipoSangre << "\n";
    archivo << "Telefono    : " << pacienteBuscado->telefono << "\n";
    archivo << "--------------------------------------------------\n\n";

    auxConsulta = pacienteBuscado->consultas.cab;

    if (auxConsulta == NULL)
    {
        archivo << "Este paciente no tiene consultas registradas.\n";
    }
    else
    {
        while (auxConsulta != NULL)
        {
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

// ============================================================
// FUNCIONES DE LIBERACION DE MEMORIA
// ============================================================
void liberarConsultas(Lista<Consulta> &listaConsultas)
{
    Nodo<Consulta> *aux;
    Nodo<Consulta> *borrar;

    aux = listaConsultas.cab;

    while (aux != NULL)
    {
        borrar = aux;
        aux = aux->sig;
        delete borrar;
    }

    listaConsultas.cab = NULL;
}

void liberarPacientes(Lista<Paciente> &listaPacientes)
{
    Nodo<Paciente> *aux;
    Nodo<Paciente> *borrar;

    aux = listaPacientes.cab;

    while (aux != NULL)
    {
        liberarConsultas(aux->info.consultas);
        borrar = aux;
        aux = aux->sig;
        delete borrar;
    }

    listaPacientes.cab = NULL;
}

// ============================================================
// MENU PRINCIPAL
// ============================================================
int main()
{
    Lista<Paciente> lista;
    int op;

    inicializarLista(lista);

    cargarPacientes(lista);
    cargarConsultas(lista);

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

        op = leerEntero("Opcion: ");

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

    liberarPacientes(lista);

    return 0;
}
