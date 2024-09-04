#ifndef ARCHIVO_H_INCLUDED
#define ARCHIVO_H_INCLUDED

#include "cliente.h"

int validarFechaNoMayorActual(struct Cliente* credito) { //COMPARA LA FECHA INGRESADA Y SI ES MAYOR A LA ACTUAL DE VUELVE FALSO


    // Obtener fecha actual
    time_t tiempoActual = time(NULL);
    struct tm* fechaActual = localtime(&tiempoActual);
    int diaActual = fechaActual->tm_mday;
    int mesActual = fechaActual->tm_mon + 1;  // Se suma 1 ya que en struct tm los meses van de 0 a 11
    int anioActual = fechaActual->tm_year + 1900;  // Se suma 1900 ya que en struct tm el año se cuenta desde 1900


    // Comparar fecha ingresada con fecha actual
    if (credito->anio > anioActual) {
        printf("La fecha es mayor a la actual. Ingrese una fecha valida\n");
        return 0;  // La fecha ingresada es mayor al año actual
    } else if (credito->anio == anioActual && credito->mes > mesActual) {
        printf("La fecha es mayor a la actual. Ingrese una fecha valida\n");
        return 0;  // La fecha ingresada es mayor al mes actual
    } else if (credito->anio== anioActual && credito->mes == mesActual && credito->dia > diaActual) {
        printf("La fecha es mayor a la actual. Ingrese una fecha valida\n");
        return 0;  // La fecha ingresada es menor al día actual
    }
    return 1;  // La fecha ingresada es válida
}

char* MesDeNumeroATexto(struct Cliente* credito){ //RECIBE EL MES EL FORMA DE ENTERO Y DEVUELVE CADENA



    switch (credito->mes){
        case 1: return "ene";
            break;
        case 2: return "feb";
            break;
        case 3: return "mar";
            break;
        case 4: return "abr";
            break;
        case 5: return "may";
            break;
        case 6: return "jun";
            break;
        case 7: return "jul";
            break;
        case 8: return "ago";
            break;
        case 9: return "sep";
            break;
        case 10: return "oct";
            break;
        case 11: return "nov";
            break;
        case 12: return "dic";
            break;


    }

return 0;
}
int listaClientesActivos() { //FUNCION AUXILIAR QUE LISTA LOS CLIENTES ACTIVOS




    FILE* archivo = fopen("creditos.dat", "rb");
    int existe=0;
    char *auxMes;
    struct Cliente credito;
        printf("\t\t\t\tLISTADO DE CLIENTES\n\n");
        printf("Orden\tCliente\t\tImporte\t\tFecha\t\tTipo de Credito\t  Nro. Cuotas IVA\tTotal Cuota Estado\n");
        printf("------------------------------------------------------------------------------------------------------------------------\n");
        fread(&credito, sizeof(struct Cliente), 1, archivo);
        while (!feof(archivo)){

            if (credito.activo==1){
                auxMes=MesDeNumeroATexto(&credito); //CAMBIO EL MES A CADENA

                printf("%-8d%-16s%-16.2f%-2d/%s/%-8d %-17s %-12.0f%-9.2f %-12.2f%d\n",
                credito.orden, credito.nombre, credito.importe, credito.dia, auxMes,
                credito.anio, credito.tipoCredito, credito.numCuotas, credito.iva, credito.totalCuota, credito.activo);
                printf("------------------------------------------------------------------------------------------------------------------------\n");
                fread(&credito, sizeof(struct Cliente), 1, archivo);
                existe=1;
            }
            fread(&credito, sizeof(struct Cliente), 1, archivo);
        }
        if (existe==0){
            printf("No se hallaron clientes activos.\n");
        }

    fclose(archivo);
return 0;
}
int validarOrden(struct Cliente* nuevoCredito){
    int aux=nuevoCredito->orden;
    struct Cliente viejoCredito;
FILE* nombreArchivo=fopen("creditos.dat","rb");
    if (nombreArchivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }
    fread(&viejoCredito, sizeof(struct Cliente),1,nombreArchivo);
        while(!feof(nombreArchivo)){

         if(aux == viejoCredito.orden){
            printf("Error este numero de orden ya existe.\n");
            return 0;
         }

         fread(&viejoCredito, sizeof(struct Cliente),1,nombreArchivo);
    }
    fclose(nombreArchivo);
    return 1;



}


int buscarPorNumeroOrdenAux(int* existe, int* numeroOrden){ // FUNCION AUXILIAR PARA SER USADA EN BUSCAR POR ORDEN Y PARA BAJA LOGICA
    FILE* nombreArchivo=fopen("creditos.dat","rb");
    if (nombreArchivo == NULL) {
        printf("Error al abrir el archivo.\n");
        return 0;
    }
    struct Cliente credito;
    fread(&credito, sizeof(struct Cliente),1,nombreArchivo);
        while(!feof(nombreArchivo)){

         if(*numeroOrden == credito.orden){
            printf("**** CLIENTE ENCONTRADO ****\n");
            printf("Orden: %d\n", credito.orden);
            printf("Cliente: %s\n", credito.nombre);
            printf("Importe: %.2f\n", credito.importe);
            printf("Tipo de credito: %s\n", credito.tipoCredito);
            printf("Fecha: %02d/%d/%d\n", credito.dia, credito.mes, credito.anio);
            printf("Numero de cuotas: %f\n", credito.numCuotas);
            printf("Importe de cada cuota: %.2f\n", credito.importeCuota);
            printf("IVA: %.2f\n", credito.iva);
            printf("Total de cada cuota (incluido IVA): %.2f\n", credito.totalCuota);
            *existe=1;
         }

         fread(&credito, sizeof(struct Cliente),1,nombreArchivo);
    }
    fclose(nombreArchivo);
    return *existe;
}


int modificarCredito() {
    struct Cliente credito;
    int numeroOrden;
    int existe=0;
    char op;
    printf("Ingrese numero de orden del cliente a modificar: ");
    scanf("%d", &numeroOrden);
    FILE* nombreArchivo = fopen("creditos.dat", "r+b");


    fread(&credito, sizeof(struct Cliente), 1, nombreArchivo);
    while (!feof(nombreArchivo)) {

        if (numeroOrden==credito.orden) {
        printf("**** CLIENTE ENCONTRADO ****\n");
        printf("Orden: %d\n", credito.orden);
        printf("Cliente: %s\n", credito.nombre);
        printf("Importe: %.2f\n", credito.importe);
        printf("Tipo de credito: %s\n", credito.tipoCredito);
        printf("Fecha: %02d/%d/%d\n", credito.dia, credito.mes, credito.anio);
        printf("Numero de cuotas: %.0f\n", credito.numCuotas);
        printf("Importe de cada cuota: %.2f\n", credito.importeCuota);
        printf("IVA: %.2f\n", credito.iva);
        printf("Total de cada cuota (incluido IVA): %.2f\n", credito.totalCuota);

        printf("%cEsta seguro que quiere modificar los datos del cliente %d? (S/N)\n", 168, numeroOrden);
        fflush(stdin);
        scanf("%c", &op);
        while(toupper(op)!='N' && toupper(op)!='S'){
            printf("\nCaracter no valido. Ingrese S o N\n");
        }
        if (toupper(op)=='N'){
            system("cls");
            return 0;
        }

        printf("Ingrese el nuevo importe: ");
        scanf("%f", &credito.importe);

        printf("Ingrese el nuevo tipo de crédito: ");
        fflush(stdin);
        gets(credito.tipoCredito);

        int pos=ftell(nombreArchivo)-sizeof(struct Cliente);
        fseek(nombreArchivo,pos,SEEK_SET);
        fwrite(&credito, sizeof(struct Cliente), 1, nombreArchivo);
        existe=1;

        printf("El cliente ha sido modificado exitosamente.\n");
        }

    fread(&credito, sizeof(struct Cliente), 1, nombreArchivo);
    }
    if (existe==0) {
        printf("No se encontro ningun cliente con ese numero de orden.\n");
    }

    return 1;
}


void buscarPorApellido(){
    system("cls");
    printf("\t\t\tBUSCAR POR APELLIDO\n\n");
    struct Cliente credito;
    char apellido[50];
    int existe=0;

    printf("Ingrese apellido del cliente a buscar: ");
    fflush(stdin);
    gets(apellido);
    FILE* nombreArchivo=fopen("creditos.dat","rb");

    fread(&credito, sizeof(struct Cliente),1,nombreArchivo);
    while(!feof(nombreArchivo)){

         if(strstr(credito.nombre,apellido)){
            printf("**** CLIENTE ENCONTRADO ****\n");
            printf("Orden: %d\n", credito.orden);
            printf("Cliente: %s\n", credito.nombre);
            printf("Importe: %.2f\n", credito.importe);
            printf("Tipo de credito: %s\n", credito.tipoCredito);
            printf("Fecha: %02d/%d/%d\n", credito.dia, credito.mes, credito.anio);
            printf("Numero de cuotas: %f\n", credito.numCuotas);
            printf("Importe de cada cuota: %.2f\n", credito.importeCuota);
            printf("IVA: %.2f\n", credito.iva);
            printf("Total de cada cuota (incluido IVA): %.2f\n", credito.totalCuota);
            existe=1;
         }

         fread(&credito, sizeof(struct Cliente),1,nombreArchivo);
    }
    if (existe==0){
        printf("No se encontro ningun cliente con ese numero de orden.\n");
    }
    fclose(nombreArchivo);

}

#endif // ARCHIVO_H_INCLUDED
