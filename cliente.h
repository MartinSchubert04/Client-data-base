#ifndef CREDITO_H_INCLUDED
#define CREDITO_H_INCLUDED

struct Cliente { //DEFINICION DE STRUCT
    int orden;
    char nombre[50];
    float importe;
    char tipoCredito[20];
    int dia;
    int mes;
    int anio;
    float numCuotas;
    float importeCuota;
    float iva;
    float totalCuota;
    int activo;
};
int validarNumeros(char numero[]) //SI SE INGRESA UN CARACTER EN VEZ DE UN ENTERO SALE DEL PROGRAMA
{
    int i;
    for(i=0; i<strlen(numero); i++)
    {
        if(!(numero[i]>=48 && numero[i]<=57))
        {
            printf("\nError, se esparaba un numero entero.\n\nSALIENDO DEL PROGRAMA...\n\n");
            exit(1);
        }
    }
    return 1;
}
void datosExtra(struct Cliente* creditos){ //CALCULO DE IMPORTE POR CUOTA, IVA, TOTAL CUOTA

    creditos->importeCuota=creditos->importe/creditos->numCuotas;
    creditos->iva = creditos->importeCuota * 0.21;
    creditos->totalCuota = creditos->importeCuota + creditos->iva;
}




#endif // CREDITO_H_INCLUDED
