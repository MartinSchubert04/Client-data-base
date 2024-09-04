/*  Autor: Martin Schubert
    Fecha de entrega: 20/6/23

    Librerias propias utilizadas: archivo.h, credito.h

    archivo.h contiene funciones largas

    credito.h tiene definido el struct y contiene funciones auxiliares cortas



*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "archivo.h"
#include "cliente.h"


void mostrarMenu() {
    printf("----------------------------- MENU ------------------------------\n");
    printf("| 1.  Crear nuevo archivo binario (.dat)\t\t\t|\n");
    printf("| 2.  Migrar datos del archivo de texto al archivo binario\t|\n");
    printf("| 3.  Listar archivo de texto (.csv)\t\t\t\t|\n");
    printf("| 4.  Listar archivo binario (.dat)\t\t\t\t|\n");
    printf("| 5.  Alta de nuevo cliente\t\t\t\t\t|\n");
    printf("| 6.  Buscar cliente por numero de orden\t\t\t|\n");
    printf("| 7.  Buscar cliente por apellido\t\t\t\t|\n");
    printf("| 8.  Modificar importe y tipo de credito por numero de orden\t|\n");
    printf("| 9.  Baja logica de cliente por numero de orden\t\t|\n");
    printf("| 10. Baja fisica de clientes inactivos y crear su archivo\t|\n");
    printf("| 11. Listar archivo de clientes dados de baja (archivo.xyz)\t|\n");
    printf("| 0.  Salir\t\t\t\t\t\t\t|\n");
    printf("-----------------------------------------------------------------\n");
}





int listarArchivoTexto() { //FUNCION QUE IMPRIME EL ARCHIVO .csv

    system("cls");
    printf("\t\t\tLISTADO DE ARCHIVO DE TEXTO\n\n");
    FILE* archivo = fopen("prestamos.csv", "r");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }

    char linea[100];
    // Leer las líneas del archivo
    while (!feof(archivo)) {
        fgets(linea, sizeof(linea), archivo);
        printf("%s", linea);
    }
    fclose(archivo);
    return 1;
}

int crearArchivoBinario() {

    system("cls");
    FILE* archivo = fopen("creditos.dat", "wb");
    if (archivo == NULL) {
        printf("Error al crear el archivo.\n");
        return 0;
    }

    printf("Archivo creado con exito.\n");

    fclose(archivo);
    return 1;
}

int validarDiaDelMes(struct Cliente* credito){ // FUNCION QUE VALIDA SI EL DIA DEL MES INGRESADO CORRESPONDE CON LA CANTIDAD DE DIAS DE ESE MES (TIENE EN CUENTA SI EL AÑO ES BISIESTO)

    int diaValido=0;
    while (diaValido==0){
        if (credito->mes==1 || credito->mes==3 || credito->mes==5 || credito->mes==7 || credito->mes==8 || credito->mes==10 || credito->mes==12){
        if(credito->dia<=31 && credito->dia>=1){
            return 0;
        }
    }
    if (credito->mes==4 || credito->mes==6 || credito->mes==9 || credito->mes==11){
        if (credito->dia<=30 && credito->dia>=1){
            return 0;
        }
    }
    if (credito->mes==2){
        time_t tiempoActual = time(NULL);
        struct tm* fechaActual = localtime(&tiempoActual);
        int anioActual = fechaActual->tm_year + 1900;
        if (anioActual % 2 == 1){
            if (credito->dia<=29 && credito->dia>=1){
            return 0;
            }
        }
        else {
            if (credito->dia>28 && credito->dia<1){
            return 0;
            }
        }
    }
    printf("No es valido, el mes %d no tiene esa cantidad de dias. Ingrese dia:", credito->mes);
    scanf("%d", &credito->dia);
    }
return 1;
}


void agregarCredito(struct Cliente* credito) { // AGREGA CLIENTE EN ORDEN Y SI NO HAY PREVIOS DEJA 0

    FILE* archivo = fopen("creditos.dat", "r+b");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    if (credito->orden == 1){
        fseek(archivo,0,SEEK_SET);
        fwrite(credito,sizeof(struct Cliente),1,archivo);}
    else {
        fseek(archivo,(credito->orden-1)*sizeof(struct Cliente),SEEK_SET);
        fwrite(credito,sizeof(struct Cliente),1,archivo);
        }

    fclose(archivo);
}

int migrarDatos() { // MIGRACION DE DATOS DEL ARCHIVO .csv (INCOMPLETO)



    FILE* archivoCSV = fopen("prestamos.csv", "r");
    if (archivoCSV == NULL) {
        printf("Error al abrir el archivo CSV.\n");
        return 0;
    }

    FILE* archivoBinario = fopen("creditos.dat", "r+b");
    if (archivoBinario == NULL) {
        printf("Error al abrir el archivo binario.\n");
        fclose(archivoBinario);
        return 0;
    }

    struct Cliente credito;
    char linea[200];
    int i;

    char *token= strtok(linea, ";");// Usar strtok para dividir la línea en campos separados por punto y coma (;)
    fflush(stdin);
    fgets(linea, 200, archivoCSV);// Leer la primera línea (encabezados) y descartarla

    fseek(archivoBinario,0,SEEK_SET);

    while (!feof(archivoCSV)) {
        credito.activo=1;
        fgets(linea, 200, archivoCSV);

        token=strtok(linea, ";");
        credito.orden = atoi(token);

        strcpy(credito.nombre, strtok(NULL, ";"));

        token = strtok(NULL, ";");
        credito.importe = atof(token);

        strcpy(credito.tipoCredito, strtok(NULL, ";"));

        token = strtok(NULL, ";");
        credito.dia = atoi(token);

        token = strtok(NULL, ";");
        credito.mes = atoi(token);

        token = strtok(NULL, ";");
        credito.anio = atoi(token);

        token = strtok(NULL, ";");
        credito.numCuotas = atof(token);

        token = strtok(NULL, ";");
        credito.importeCuota=atof(token);

        token = strtok(NULL, ";");
        credito.iva = atof(token);

        token = strtok(NULL, ";");
        credito.totalCuota=atof(token);



        if (credito.orden == 1){
        fseek(archivoBinario,0,SEEK_SET);
        fwrite(&credito,sizeof(struct Cliente),1,archivoBinario);}
        else {
        fseek(archivoBinario,(credito.orden-1)*sizeof(struct Cliente),SEEK_SET);
        fwrite(&credito,sizeof(struct Cliente),1,archivoBinario);
        }
    }

    fclose(archivoCSV);
    fclose(archivoBinario);
    system("cls");
    printf("Archivos migrados exitosamente.\n");
    return 1;
}
int listarClientes() { // LISTA LOS CLIENTES CON EL SUB MENU (LA CUARTA OPCION NO FUNCIONA POR ALGUN MOTIVO)
    system("cls");
    printf("\t\t\tMENU DE LISTADO\n\n");
    int op;
    char cad[20], aux[20], *auxMes;
    int existe=0, valido=0;


    FILE* archivo = fopen("creditos.dat", "rb");

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }

    struct Cliente credito;
    struct Cliente creditoAux;
    struct Cliente creditoAux2;

    printf("1. Listar todos (activos e inactivos).\n");
    printf("2. Listar solo los activos.\n");
    printf("3. Listar por tipo de credito.\n");
    printf("4. Listar por rango de tiempo.\n");
    printf("Ingrese una opcion: ");
    scanf("%s", cad);
    valido=validarNumeros(cad);
    op=atoi(cad);


    while (op<1 || op>4){
        printf("Invalido. Ingrese un valor correcto.\n");
        printf("1. Listar todos (activos e inactivos).\n");
        printf("2. Listar solo los activos.\n");
        printf("3. Listar por tipo de credito.\n");
        printf("4. Listar por rango de tiempo.\n");
        printf("Ingrese una opcion: ");
        scanf("%d", &op);
    }
    system("cls");
    switch(op){
    case 1:
        printf("\t\t\t\tLISTADO DE CLIENTES\n\n");
        printf("Orden\tCliente\t\tImporte\t\tFecha\t\tTipo de Credito\t  Nro. Cuotas IVA\tTotal Cuota Estado\n");
        printf("------------------------------------------------------------------------------------------------------------------------\n");
        fread(&credito, sizeof(struct Cliente), 1, archivo);
            while (!feof(archivo)) {
                auxMes=MesDeNumeroATexto(&credito); //CAMBIO EL MES A CADENA

                printf("%-8d%-16s%-16.2f%-2d/%s/%-8d %-17s %-12.0f%-9.2f %-12.2f%d\n",
                credito.orden, credito.nombre, credito.importe, credito.dia, auxMes,
                credito.anio, credito.tipoCredito, credito.numCuotas, credito.iva, credito.totalCuota, credito.activo);
                printf("------------------------------------------------------------------------------------------------------------------------\n");
                fread(&credito, sizeof(struct Cliente), 1, archivo);
            }

        break;
    case 2:
            listaClientesActivos();
        break;

    case 3:
        printf("\t\t\t\tLISTADO DE CLIENTES POR TIPO DE CREDITO\n\n");
        printf("Ingrese tipo de credito(a sola firma / con garantia): ");
        fflush(stdin);
        gets(aux);

        while (strcmp(aux, "a sola firma") != 0 && strcmp(aux, "con garantia") != 0) {
        printf("Tipo de credito no valido. Ingrese tipo de crédito valido (a sola firma / con garantia): ");
        fflush(stdin);
        gets(aux);
        }
        printf("\t\t\t\tLISTADO DE CLIENTES\n\n");
        printf("Orden\tCliente\t\tImporte\t\tFecha\t\tTipo de Credito\t  Nro. Cuotas IVA\tTotal Cuota Estado\n");
        printf("------------------------------------------------------------------------------------------------------------------------\n");
        printf("------------------------------------------------------------------------------------------------------------------------\n");
        fread(&credito, sizeof(struct Cliente), 1, archivo);
        while (!feof(archivo)){
            fread(&credito, sizeof(struct Cliente), 1, archivo);
            if (strcmp(credito.tipoCredito, aux)== 0){
                auxMes=MesDeNumeroATexto(&credito); //CAMBIO EL MES A CADENA

                printf("%-8d%-16s%-16.2f%-2d/%s/%-8d %-17s %-12.0f%-9.2f %-12.2f%d\n",
                credito.orden, credito.nombre, credito.importe, credito.dia, auxMes,
                credito.anio, credito.tipoCredito, credito.numCuotas, credito.iva, credito.totalCuota, credito.activo);
                printf("------------------------------------------------------------------------------------------------------------------------\n");
                fread(&credito, sizeof(struct Cliente), 1, archivo);
                existe=1;
            }
        }
        if (existe=0){
            printf("No se hallo el cliente con ese tipo de credito.\n");
        }
        break;
    case 4:
        printf("\t\t\t\tLISTADO DE CLIENTES POR RANGO DE FECHAS\n\n");
        printf("Desde el a%co:", 164);
        scanf("%d", &creditoAux.anio);
        printf("Desde el mes:");
        scanf("%d", &creditoAux.mes);
        printf("Desde el dia:");
        scanf("%d", &creditoAux.dia);

        int validar=validarFechaNoMayorActual(&creditoAux);
            while (validar==0){
                printf("Desde el a%co:", 164);
                scanf("%d", &creditoAux.anio);
                printf("Desde el mes:");
                scanf("%d", &creditoAux.mes);
                printf("Desde el dia:");
                scanf("%d", &creditoAux.dia);
                validar=validarFechaNoMayorActual(&creditoAux);
            }

        printf("Hasta el a%co:", 164);
        scanf("%d", &creditoAux2.anio);
        printf("Hasta el mes:");
        scanf("%d", &creditoAux2.mes);
        printf("Hasta el dia:");
        scanf("%d", &creditoAux2.dia);

        validar=validarFechaNoMayorActual(&creditoAux2);
            while (validar==0){
                printf("Hasta el a%co:", 164);
                scanf("%d", &creditoAux2.anio);
                printf("Hasta el mes:");
                scanf("%d", &creditoAux2.mes);
                printf("Hasta el dia:");
                scanf("%d", &creditoAux2.dia);;
                validar=validarFechaNoMayorActual(&creditoAux2);
            }
            printf("\t\t\t\tLISTADO DE CLIENTES\n\n");
            printf("Orden\tCliente\t\tImporte\t\tFecha\t\tTipo de Credito\t  Nro. Cuotas IVA\tTotal Cuota Estado\n");
            printf("------------------------------------------------------------------------------------------------------------------------\n");
        fread(&credito, sizeof(struct Cliente), 1, archivo);
        while(!feof(archivo)){
            if (creditoAux.anio<=credito.anio && creditoAux2.anio<=credito.anio){
                if (creditoAux.mes<=credito.mes && creditoAux2.mes>=credito.mes){
                    if (creditoAux.dia<=credito.dia && creditoAux2.dia>=credito.dia)
                        auxMes=MesDeNumeroATexto(&credito); //CAMBIO EL MES A CADENA

                        printf("%-8d%-16s%-16.2f%-2d/%s/%-8d %-17s %-12.0f%-9.2f %-12.2f%d\n",
                        credito.orden, credito.nombre, credito.importe, credito.dia, auxMes,
                        credito.anio, credito.tipoCredito, credito.numCuotas, credito.iva, credito.totalCuota, credito.activo);
                        printf("------------------------------------------------------------------------------------------------------------------------\n");
                        fread(&credito, sizeof(struct Cliente), 1, archivo);
                        existe=1;
                }
            }
        }
        if (existe=0){
            printf("No se encontro registros que coincidan con esas fechas.\n");
        }
        break;
        fclose(archivo);
    }
    printf("\n\nPulse una tecla para regresar al menu principal...\n");
    fflush(stdin);
    getchar();
    system("cls");
    return 1;
}


void altaCliente() { //ALTA DE CLIENTE CON VALIDACIONES Y CONTROL DE ERROR
    FILE* archivo = fopen("creditos.dat", "ab");
    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }
    system("cls");
    printf("\t\t\tALTA DEL CLIENTE\n\n");

    char cad[20];
    int valido=0;
    int ordenExisteONo=0;

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return;
    }
    struct Cliente credito;

    printf("Ingrese los datos del nuevo cliente:\n");
    printf("Numero de orden: ");
    scanf("%s", cad);

    valido=validarNumeros(cad);
    credito.orden=atoi(cad);
    while (credito.orden<0){
        printf("Error el numero ingresado es negativo:\n");
        printf("Numero de orden valido: ");
        scanf("%s", cad);

        valido=validarNumeros(cad);
        credito.orden=atoi(cad);
    }
    ordenExisteONo=validarOrden(&credito);


    while (ordenExisteONo==0){
            printf("Numero de orden: ");
            scanf("%s", cad);

            valido=validarNumeros(cad);
            credito.orden=atoi(cad);
            ordenExisteONo=validarOrden(&credito);

        }
    printf("Ingrese nombre completo del cliente: ");
    fflush(stdin);
    gets(credito.nombre);
    printf("Importe: ");
    scanf("%f", &credito.importe);
    while (credito.importe<=0){
        printf("El importe del cliente debe ser mayor a 0(cero), ingrese un valor correcto.\n");
        printf("Ingrese importe: \n");
        scanf("%f", &credito.importe);
    }
    printf("Numero de cuotas: ");
    scanf("%f", &credito.numCuotas);
    while (credito.numCuotas<=1){
        printf("El numero de cuotas debe ser mayor a 1(uno), ingrese un valor correcto.\n");
        printf("Ingrese numero de cuotas: \n");
        scanf("%f", &credito.numCuotas);
    }
    printf("Tipo de credito (a sola firma / con garatia): ");
    fflush(stdin);
    gets(credito.tipoCredito);
    while (strcmp(credito.tipoCredito, "a sola firma") != 0 && strcmp(credito.tipoCredito, "con garantia") != 0) {
        printf("Tipo de credito no valido. Ingrese tipo de credito valido o asegurese de ingresarlo como esta escrito estre parentesis (a sola firma / con garantia): ");
        fflush(stdin);
        gets(credito.tipoCredito);
    }
    printf("Ingrese a%co:", 164);
    scanf("%d", &credito.anio);
    printf("Ingrese mes (numerico):");
    scanf("%d", &credito.mes);
    while(credito.mes<1 || credito.mes>12){
        printf("Ese mes no existe. ");
        printf("Ingrese mes:");
        scanf("%d", &credito.mes);
        }

    printf("Ingrese dia:");
    scanf("%d", &credito.dia);
    validarDiaDelMes(&credito);

    int validar=validarFechaNoMayorActual(&credito);
    while (validar==0){
        printf("Ingrese a%co:", 164);
        scanf("%d", &credito.anio);
        printf("Ingrese mes:");
        scanf("%d", &credito.mes);
        while(credito.mes<1 || credito.mes>12){
        printf("Ese mes no existe. ");
        printf("Ingrese mes:");
        scanf("%d", &credito.mes);
        }
        printf("Ingrese dia:");
        scanf("%d", &credito.dia);
        validarDiaDelMes(&credito);
        validar=validarFechaNoMayorActual(&credito);
        }

    credito.activo = 1;

    datosExtra(&credito);
    agregarCredito(&credito);

    // Cerrar archivos
    fclose(archivo);
    printf("Cliente agregado con exito.\n");
}




int buscarPorNumeroOrden() { //BUSCA EL NUMERO DE ORDEN CON EL USO DE SU AUXILIAR
    system("cls");
    printf("\t\t\tBUSCAR POR NUMERO DE ORDEN\n\n");
    struct Cliente credito;
    int existe=0;
    int numeroOrden;

    printf("Ingrese numero de orden del cliente a buscar: ");
    scanf("%d", &numeroOrden);

    existe=buscarPorNumeroOrdenAux(&existe, &numeroOrden);

    if (existe==0){
        printf("No se encontro ningun cliente con ese numero de orden.\n");
    }

    return numeroOrden;
}


int bajaLogicaCliente() { //FUNCION QUE HACE QUE UN CLIENTE ACTIVO PASE A ESTAR INACTIVO
    system("cls");
    printf("\t\t\tBAJA LOGICA DE CLIENTE\n\n");
    FILE* archivo=fopen("creditos.dat","r+b");
    int existe=0;
    int numeroOrden;

    char op;
    struct Cliente credito;

    printf("Ingrese numero de orden del cliente a buscar: ");
    scanf("%d", &numeroOrden);
    fseek(archivo,0,SEEK_SET);
    fseek(archivo,(numeroOrden-1)*sizeof(struct Cliente),SEEK_SET);
    fread(&credito,sizeof(struct Cliente),1,archivo);

    existe=buscarPorNumeroOrdenAux(&existe, &numeroOrden);

    if(existe==0){
            system("cls");
            printf("No se encontro ningun cliente con ese numero de orden.\n");
        return 0;
    }

    printf("\n%cEstas seguro que queres dar de baja al cliente %d? (S/N)\n", 168, numeroOrden);
    fflush(stdin);
    scanf("%c", &op);
    while(toupper(op)!='N' && toupper(op)!='S'){
        printf("\nCaracter no valido. Ingrese S o N\n");
    }
    if (toupper(op)=='N'){
        system("cls");
        printf("No sera de baja al cliente. Regresando al menu...\n");
        return 0;
    }

    if (credito.activo==0){
        system("cls");
        printf("El cliente ya estaba dado de baja\n");
        return 0;
    }
    credito.activo=0;
    fseek(archivo,-sizeof(struct Cliente),SEEK_CUR);
    fwrite(&credito,sizeof(struct Cliente),1,archivo);

    fclose(archivo);
    listaClientesActivos();
    printf("\n\nPulse una tecla para regresar al menu principal...\n");
    fflush(stdin);
    getchar();
    system("cls");
    return 1;
}


const char* bajaFisicaClientesInactivos(){ // FUNCION QUE CREA UN ARCHIVO CONTENIENDO A LOS CLIENTES INACTIVOS
    struct Cliente credito;
    char dia[15], mes[5], anio[6];
    int validar=0;


    printf("Ingrese la fecha de hoy\n");
    printf("Ingrese a%co:", 164);
    fflush(stdin);
    gets(anio);
    credito.anio=atoi(anio);
    printf("Ingrese mes (numerico):");
    fflush(stdin);
    gets(mes);
    credito.mes=atoi(mes);
    while(credito.mes<1 || credito.mes>12){
        printf("Ese mes no existe. ");
        printf("Ingrese mes:");
        scanf("%d", &credito.mes);
        }
    printf("Ingrese dia:");
    fflush(stdin);
    gets(dia);
    credito.dia=atoi(dia);
    validarDiaDelMes(&credito);

    validar=validarFechaNoMayorActual(&credito);
    while (validar==0){
        printf("Ingrese la fecha de hoy\n");
        printf("Ingrese a%co:", 164);
        fflush(stdin);
        gets(anio);
        credito.anio=atoi(anio);
        printf("Ingrese mes (numerico):");
        fflush(stdin);
        gets(mes);
        credito.mes=atoi(mes);
            while(credito.mes<1 || credito.mes>12){
            printf("Ese mes no existe. ");
            printf("Ingrese mes:");
            scanf("%d", &credito.mes);
            }
        printf("Ingrese dia:");
        fflush(stdin);
        gets(dia);
        credito.dia=atoi(dia);
        validarDiaDelMes(&credito);
        validar=validarFechaNoMayorActual(&credito);
        }


    strcat(dia,"_");
    strcat(dia,mes);
    strcat(dia,"_");
    strcat(dia,anio);

    char nombreArchivoBajas[40]="clientes_bajas_";

    strcat(nombreArchivoBajas,dia);
    strcat(nombreArchivoBajas,".xyz");


    FILE* archivo= fopen("creditos.dat","rb");
    FILE* archivoBajas= fopen(nombreArchivoBajas,"w+b");
    FILE* temp= fopen("archivo.temp","w+b");

    if (archivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }



    fseek(archivo,0,SEEK_SET);
    while(fread(&credito,sizeof(struct Cliente),1,archivo)==1){
        if(credito.activo == 0){
            fwrite(&credito,sizeof(struct Cliente),1,archivoBajas);
            fread(&credito,sizeof(struct Cliente),1,archivo);
        }
    }


    fseek(archivo,0,SEEK_SET);
    while(fread(&credito,sizeof(struct Cliente),1,archivo)==1){
        if(credito.activo == 1){
            fwrite(&credito,sizeof(struct Cliente),1,temp);
        }
    }

    fclose(temp);
    fclose(archivo);
    fclose(archivoBajas);
    remove("creditos.dat");
    if(rename("archivo.temp","creditos.dat")!=0){
        printf("error\n");
    }
    remove("archivo.temp");


    return nombreArchivoBajas;
}


void listarArchivoClientesBaja(){ // LISTA EL ARCHIVO DE CLIENTES INACTIVOS


    struct Cliente credito;
    char dia[15], mes[5], anio[6];
    int validar=0;

    printf("Ingrese la fecha del dia que se elimino los registros de los clientes\n");
    printf("Ingrese a%co:", 164);
    fflush(stdin);
    gets(anio);
    credito.anio=atoi(anio);
    printf("Ingrese mes (numerico):");
    fflush(stdin);
    gets(mes);
    credito.mes=atoi(mes);
    while(credito.mes<1 || credito.mes>12){
        printf("Ese mes no existe. ");
        printf("Ingrese mes:");
        scanf("%d", &credito.mes);
        }
    printf("Ingrese dia:");
    fflush(stdin);
    gets(dia);
    credito.dia=atoi(dia);
    validarDiaDelMes(&credito);

    validar=validarFechaNoMayorActual(&credito);
    while (validar==0){
        printf("Ingrese la fecha de hoy\n");
        printf("Ingrese a%co:", 164);
        fflush(stdin);
        gets(anio);
        credito.anio=atoi(anio);
        printf("Ingrese mes (numerico):");
        fflush(stdin);
        gets(mes);
        credito.mes=atoi(mes);
            while(credito.mes<1 || credito.mes>12){
            printf("Ese mes no existe. ");
            printf("Ingrese mes:");
            scanf("%d", &credito.mes);
            }
        printf("Ingrese dia:");
        fflush(stdin);
        gets(dia);
        credito.dia=atoi(dia);
        validarDiaDelMes(&credito);
        validar=validarFechaNoMayorActual(&credito);
        }


    strcat(dia,"_");
    strcat(dia,mes);
    strcat(dia,"_");
    strcat(dia,anio);

    char nombreArchivoBajas[40]="clientes_bajas_";

    strcat(nombreArchivoBajas,dia);
    strcat(nombreArchivoBajas,".xyz");

    FILE* archivo= fopen(nombreArchivoBajas,"rb");
    if (archivo== NULL) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }

        printf("\t\t\t\tLISTADO DE CLIENTES ELIMINADOS\n\n");
        printf("Orden\t\tCliente\t\tImporte\t\tFecha\t\tTipo de Credito\t\tNro. Cuotas\t IVA Total Cuota\n");
        printf("------------------------------------------------------------------------------------------------------------------------\n");
        fread(&credito, sizeof(struct Cliente), 1, archivo);
        while (!feof(archivo)){
            if (credito.activo==0){
                printf("%-16d%-16s%-16.2f%d/%d/%-12d%-24s%-12.0f%-8.2f %.2f\n",
                credito.orden, credito.nombre, credito.importe, credito.dia, credito.mes,
                credito.anio, credito.tipoCredito, credito.numCuotas, credito.iva, credito.totalCuota);
                printf("------------------------------------------------------------------------------------------------------------------------\n");
                fread(&credito, sizeof(struct Cliente), 1, archivo);
            }
            fread(&credito, sizeof(struct Cliente), 1, archivo);
        }


    fclose(archivo);
}

int main() {
    int valido=0, opcion;
    char cad[20];
    char *nombreArchivoBajas;
    do {
        mostrarMenu();
        printf("Ingrese una opcion: ");
        scanf("%s", cad);
        valido=validarNumeros(cad);
        opcion=atoi(cad);

        switch (opcion) {
            case 1:
                crearArchivoBinario();
                break;
            case 2:
                migrarDatos();
                break;
            case 3:
                listarArchivoTexto();
                break;
            case 4:
                listarClientes();
                break;
            case 5:
                altaCliente();
                break;
            case 6:
                buscarPorNumeroOrden();
                break;
            case 7:
                buscarPorApellido();
                break;
            case 8:
                modificarCredito();
                break;
            case 9:
                bajaLogicaCliente();
                break;
            case 10:
                *nombreArchivoBajas=bajaFisicaClientesInactivos();
                break;
            case 11:
                listarArchivoClientesBaja(nombreArchivoBajas);
                break;
            case 12:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion invalida. Por favor, ingrese un numero entre 1 y 12.\n");
                break;
        }
     }while (opcion != 12);

    return 0;
}

